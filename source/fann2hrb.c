/*
 */

#include "doublefann.h"

#include "hbvm.h"
#include "hbstack.h"
#include "hbapiitm.h"

#include "fann2hrb.h"

static char * sz_Callback = NULL;
static PHB_DYNS s_pSymTest = NULL;

void fann_writelog( const char * sFile, const char * sTraceMsg, ... )
{
   FILE *hFile;

   if( sFile == NULL )
   {
      hFile = fopen( "ac.log", "a" );
   }
   else
   {
      hFile = fopen( sFile, "a" );
   }

   if( hFile )
   {
      va_list ap;

      va_start( ap, sTraceMsg );
      vfprintf( hFile, sTraceMsg, ap );
      va_end( ap );

      fclose( hFile );
   }

}

int FANN_API dummy_callback( struct fann *ann, struct fann_train_data *train,
	unsigned int max_epochs, unsigned int epochs_between_reports, 
	float desired_error, unsigned int epochs )
{
   //printf("Epochs     %8d. MSE: %.5f. Desired-MSE: %.5f\n", epochs, fann_get_MSE(ann), desired_error);
   return 0;
}

int FANN_API fn_callback( struct fann *ann, struct fann_train_data *train,
	unsigned int max_epochs, unsigned int epochs_between_reports,
	float desired_error, unsigned int epochs )
{

   if( s_pSymTest == NULL )
      s_pSymTest = hb_dynsymGetCase( sz_Callback );

   if( hb_dynsymIsFunction( s_pSymTest ) )
   {
      hb_vmPushDynSym( s_pSymTest );
      hb_vmPushNil();   /* places NIL at self */

      HB_PUSHITEM( train );
      hb_vmPushLong( ( long int ) max_epochs );
      hb_vmPushLong( ( long int ) epochs_between_reports );
      hb_vmPushDouble( ( long int ) desired_error, 6 );
      hb_vmPushLong( ( long int ) epochs );
      hb_vmDo( 5 );     /* where iArgCount is the number of pushed parameters */
   }
   return 0;
}

/*
 * *ann = fann_create_standard( num_layers, { num_input, ..., num_output } )
 */

HB_FUNC( FANN_CREATE_STANDARD )
{

   struct fann *ann;
   PHB_ITEM pArr = hb_param( 2, HB_IT_ARRAY );
   unsigned int iLayers = hb_parni(1), ui, * pLayers;

   if( iLayers != hb_arrayLen( pArr ) )
   {
      hb_ret();
      return;
   }
   pLayers = (unsigned int *) malloc( sizeof(unsigned int) * iLayers );
   for( ui = 0; ui < iLayers; ui++ )
      *( pLayers+ui ) = hb_arrayGetNI( pArr, ui+1 );
   
   ann = fann_create_standard_array( iLayers, pLayers );

   free( pLayers );
   HB_RETHANDLE( ann );
}

/*
 * *ann = fann_create_sparse( connection_rate, num_layers, { num_input, ..., num_output } )
 */

HB_FUNC( FANN_CREATE_SPARSE )
{

   struct fann *ann;
   PHB_ITEM pArr = hb_param( 3, HB_IT_ARRAY );
   unsigned int iLayers = hb_parni(2), ui, * pLayers;

   if( iLayers != hb_arrayLen( pArr ) )
   {
      hb_ret();
      return;
   }
   pLayers = (unsigned int *) malloc( sizeof(unsigned int) * iLayers );
   for( ui = 0; ui < iLayers; ui++ )
      *( pLayers+ui ) = hb_arrayGetNI( pArr, ui+1 );
   
   ann = fann_create_sparse_array( hb_parnd(1), iLayers, pLayers );

   free( pLayers );
   HB_RETHANDLE( ann );
}

/*
 * *ann = fann_copy( *ann )
 */

HB_FUNC( FANN_COPY )
{

   struct fann *ann = fann_copy( (struct fann *) HB_PARHANDLE(1) );

   HB_RETHANDLE( ann );
}


/*
 * fann_destroy( *ann )
 */

HB_FUNC( FANN_DESTROY )
{

   fann_destroy( (struct fann *) HB_PARHANDLE(1) );
}

/*
 * *output = fann_run( *ann, *input )
 */

HB_FUNC( FANN_RUN )
{

   struct fann * ann = (struct fann *) HB_PARHANDLE(1);
   fann_type *output, *input;
   PHB_ITEM pArr = hb_param( 2, HB_IT_ARRAY );
   unsigned long ulLen = hb_arrayLen( pArr ), ul;
   unsigned int uiLen, ui;
   PHB_ITEM aMetr, temp;

   input = (fann_type *) malloc( sizeof(fann_type) * ulLen );
   for( ul = 0; ul < ulLen; ul++ )
      *( input+ul ) = hb_arrayGetND( pArr, ul+1 );

   output = fann_run( ann, input );

   free( input );

   uiLen = fann_get_num_output( ann );
   aMetr = hb_itemArrayNew( uiLen );
   temp = hb_itemNew( NULL );
   for( ui = 1; ui <= uiLen; ui++ )
   {
      hb_itemPutND( temp, *(output+ui-1) );
      hb_itemArrayPut( aMetr, ui, temp );
   }
   hb_itemRelease( temp );

   hb_itemReturn( aMetr );
   hb_itemRelease( aMetr );

}

/*
 * fann_randomize_weights( *ann )
 */

HB_FUNC( FANN_RANDOMIZE_WEIGHTS )
{

   fann_randomize_weights( (struct fann *) HB_PARHANDLE(1), hb_parnd(2), hb_parnd(3) );
}


/*
 * *ann = fann_create_from_file( *ann );
 */

HB_FUNC( FANN_CREATE_FROM_FILE )
{

   struct fann *ann = fann_create_from_file( hb_parc(1) );
   HB_RETHANDLE( ann );
}

/*
 * fann_save( *ann, cFileName )
 */

HB_FUNC( FANN_SAVE )
{

   fann_save( (struct fann *) HB_PARHANDLE(1), hb_parc(2) );
}

/*
 * fann_get_num_input( *ann )
 */

HB_FUNC( FANN_GET_NUM_INPUT )
{

   hb_retni( fann_get_num_input( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_get_num_output( *ann )
 */

HB_FUNC( FANN_GET_NUM_OUTPUT )
{

   hb_retni( fann_get_num_output( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_get_total_neurons( *ann )
 */

HB_FUNC( FANN_GET_TOTAL_NEURONS )
{

   hb_retni( fann_get_total_neurons( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_get_total_connections( *ann )
 */

HB_FUNC( FANN_GET_TOTAL_CONNECTIONS )
{

   hb_retni( fann_get_total_connections( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_get_network_type( *ann )
 */

HB_FUNC( FANN_GET_NETWORK_TYPE )
{

   hb_retni( fann_get_network_type( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_get_connection_rate( *ann )
 */

HB_FUNC( FANN_GET_CONNECTION_RATE )
{

   hb_retnd( fann_get_connection_rate( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_get_num_layers( *ann )
 */

HB_FUNC( FANN_GET_NUM_LAYERS )
{

   hb_retni( fann_get_num_layers( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_get_layer_array( *ann )
 */

HB_FUNC( FANN_GET_LAYER_ARRAY )
{

   struct fann *ann = (struct fann *) HB_PARHANDLE(1);
   unsigned int iLayers = fann_get_num_layers( ann ), ui, * pLayers;
   PHB_ITEM aMetr = hb_itemArrayNew( iLayers ), temp;

   pLayers = (unsigned int*) malloc( sizeof(unsigned int) * iLayers );
   fann_get_layer_array( ann, pLayers );

   for( ui = 1; ui <= iLayers; ui++ )
   {
      temp = hb_itemPutNL( NULL, *(pLayers+ui) );
      hb_itemArrayPut( aMetr, ui, temp );
      hb_itemRelease( temp );
   }
   free( pLayers );

   hb_itemReturn( aMetr );
   hb_itemRelease( aMetr );

}

/*
 * fann_get_bias_array( *ann )
 */

HB_FUNC( FANN_GET_BIAS_ARRAY )
{

   struct fann *ann = (struct fann *) HB_PARHANDLE(1);
   unsigned int iLayers = fann_get_num_layers( ann ), ui, * pBias;
   PHB_ITEM aMetr = hb_itemArrayNew( iLayers ), temp;

   pBias = (unsigned int*) malloc( sizeof(unsigned int) * iLayers );
   fann_get_bias_array( ann, pBias );

   for( ui = 1; ui <= iLayers; ui++ )
   {
      temp = hb_itemPutNL( NULL, *(pBias+ui) );
      hb_itemArrayPut( aMetr, ui, temp );
      hb_itemRelease( temp );
   }
   free( pBias );

   hb_itemReturn( aMetr );
   hb_itemRelease( aMetr );

}

/*
 * fann_get_connection_array( *ann )
 */

HB_FUNC( FANN_GET_CONNECTION_ARRAY )
{

   struct fann *ann = (struct fann *) HB_PARHANDLE(1);
   unsigned int iConns = fann_get_total_connections( ann ), ui;
   struct fann_connection * pConns;
   PHB_ITEM aMetr = hb_itemArrayNew( iConns ), temp, tarr;

   pConns = (struct fann_connection*) malloc( sizeof(struct fann_connection) * iConns );
   fann_get_connection_array( ann, pConns );

   temp = hb_itemNew( NULL );
   for( ui = 1; ui <= iConns; ui++ )
   {
      //fann_writelog( NULL, "from: %u %u %f\r\n ",(pConns+ui)->from_neuron, (pConns+ui)->to_neuron, (pConns+ui)->weight );
      tarr = hb_itemArrayNew( 3 );
      hb_itemPutNL( temp, pConns[ui].from_neuron );
      hb_itemArrayPut( tarr, 1, temp );
      hb_itemPutNL( temp, pConns[ui].to_neuron );
      hb_itemArrayPut( tarr, 2, temp );
      hb_itemPutND( temp, pConns[ui].weight );
      hb_itemArrayPut( tarr, 3, temp );

      hb_itemArrayPut( aMetr, ui, tarr );
      hb_itemRelease( tarr );
   }
   hb_itemRelease( temp );

   free( pConns );

   hb_itemReturn( aMetr );
   hb_itemRelease( aMetr );

}

/*
 * fann_set_weight( *ann )
 */

HB_FUNC( FANN_SET_WEIGHT )
{

   fann_set_weight( (struct fann *) HB_PARHANDLE(1), hb_parnl(1), hb_parnl(2), hb_parnd(3) );
}

// ------------- Training -------------


/*
 * fann_train( *ann, *inputs, *outputs )
 */

HB_FUNC( FANN_TRAIN )
{

   struct fann *ann = (struct fann *) HB_PARHANDLE(1);
   fann_type *output, *input;
   PHB_ITEM pArrIn = hb_param( 2, HB_IT_ARRAY );
   PHB_ITEM pArrOut = hb_param( 3, HB_IT_ARRAY );
   unsigned int uiInputs = (unsigned int) hb_arrayLen( pArrIn ), ui;
   unsigned int uiOutputs = (unsigned int) hb_arrayLen( pArrOut );

   if( uiInputs != fann_get_num_input( ann ) || uiOutputs != fann_get_num_output( ann ) )
   {
      hb_ret();
      return;
   }

   input = (fann_type *) malloc( sizeof(fann_type) * uiInputs );
   for( ui = 0; ui < uiInputs; ui++ )
      *( input+ui ) = hb_arrayGetND( pArrIn, ui+1 );

   output = (fann_type *) malloc( sizeof(fann_type) * uiOutputs );
   for( ui = 0; ui < uiOutputs; ui++ )
      *( output+ui ) = hb_arrayGetND( pArrOut, ui+1 );


   fann_train( ann, input, output );
   free( input );
   free( output );
}

/*
 * fann_train_on_file( *ann, cFileName, max_epochs, epochs_between_reports, desired_error )
 */

HB_FUNC( FANN_TRAIN_ON_FILE )
{

   fann_train_on_file( (struct fann *) HB_PARHANDLE(1), hb_parc(2), (const unsigned int) hb_parni(3), (const unsigned int) hb_parni(4), (const float) hb_parnd(5) );
}

/*
 * fann_read_train_from_file( cFileName )
 */

HB_FUNC( FANN_READ_TRAIN_FROM_FILE )
{

   struct fann_train_data * anndata = fann_read_train_from_file( hb_parc(1) );

   HB_RETHANDLE( anndata );
}

/*
 * fann_get_training_algorithm( *ann )
 */

HB_FUNC( FANN_GET_TRAINING_ALGORITHM )
{

   hb_retni( fann_get_training_algorithm( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_training_algorithm( *ann )
 */

HB_FUNC( FANN_SET_TRAINING_ALGORITHM )
{

   fann_set_training_algorithm( (struct fann *) HB_PARHANDLE(1), hb_parni(2) );
}

/*
 * fann_get_learning_rate( *ann )
 */

HB_FUNC( FANN_GET_LEARNING_RATE )
{

   hb_retnd( fann_get_learning_rate( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_learning_rate( *ann )
 */

HB_FUNC( FANN_SET_LEARNING_RATE )
{

   fann_set_learning_rate( (struct fann *) HB_PARHANDLE(1), (float) hb_parnd(2) );
}

/*
 * fann_get_learning_momentum( *ann )
 */

HB_FUNC( FANN_GET_LEARNING_MOMENTUM )
{

   hb_retnd( fann_get_learning_momentum( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_learning_momentum( *ann )
 */

HB_FUNC( FANN_SET_LEARNING_MOMENTUM )
{

   fann_set_learning_momentum( (struct fann *) HB_PARHANDLE(1), (float) hb_parnd(2) );
}


/*
 * fann_get_activation_function( *ann, ilayer, iNeuron )
 */

HB_FUNC( FANN_GET_ACTIVATION_FUNCTION )
{

   hb_retni( fann_get_activation_function( (struct fann *) HB_PARHANDLE(1),
         hb_parni(3), hb_parni(4) ) );
}

/*
 * fann_set_activation_function( *ann, iType, ilayer, iNeuron )
 */

HB_FUNC( FANN_SET_ACTIVATION_FUNCTION )
{

   fann_set_activation_function( (struct fann *) HB_PARHANDLE(1), hb_parni(2),
         hb_parni(3), hb_parni(4) );
}

/*
 * fann_set_activation_function_layer( *ann, iType, ilayer )
 */

HB_FUNC( FANN_SET_ACTIVATION_FUNCTION_LAYER )
{

   fann_set_activation_function_layer( (struct fann *) HB_PARHANDLE(1), hb_parni(2),
         hb_parni(3) );
}

/*
 * fann_set_activation_function_hidden( *ann, iType )
 */

HB_FUNC( FANN_SET_ACTIVATION_FUNCTION_HIDDEN )
{

   fann_set_activation_function_hidden( (struct fann *) HB_PARHANDLE(1), hb_parni(2) );
}

/*
 * fann_set_activation_function_output( *ann, iType )
 */

HB_FUNC( FANN_SET_ACTIVATION_FUNCTION_OUTPUT )
{

   fann_set_activation_function_output( (struct fann *) HB_PARHANDLE(1), hb_parni(2) );
}

/*
 * fann_get_activation_steepness( *ann, ilayer, iNeuron )
 */

HB_FUNC( FANN_GET_ACTIVATION_STEEPNESS )
{

   hb_retnd( fann_get_activation_steepness( (struct fann *) HB_PARHANDLE(1),
         hb_parni(3), hb_parni(4) ) );
}

/*
 * fann_set_activation_steepness( *ann, dSteepness, ilayer, iNeuron )
 */

HB_FUNC( FANN_SET_ACTIVATION_STEEPNESS )
{

   fann_set_activation_steepness( (struct fann *) HB_PARHANDLE(1), hb_parnd(2),
         hb_parni(3), hb_parni(4) );
}

/*
 * fann_set_activation_steepness_layer( *ann, dSteepness, ilayer )
 */

HB_FUNC( FANN_SET_ACTIVATION_STEEPNESS_LAYER )
{

   fann_set_activation_steepness_layer( (struct fann *) HB_PARHANDLE(1), hb_parnd(2),
         hb_parni(3) );
}

/*
 * fann_set_activation_steepness_hidden( *ann, dSteepness )
 */

HB_FUNC( FANN_SET_ACTIVATION_STEEPNESS_HIDDEN )
{

   fann_set_activation_steepness_hidden( (struct fann *) HB_PARHANDLE(1), hb_parnd(2) );
}

/*
 * fann_set_activation_steepness_output( *ann, dSteepness )
 */

HB_FUNC( FANN_SET_ACTIVATION_STEEPNESS_OUTPUT )
{

   fann_set_activation_steepness_output( (struct fann *) HB_PARHANDLE(1), hb_parnd(2) );
}

/*
 * fann_get_train_error_function( *ann )
 */

HB_FUNC( FANN_GET_TRAIN_ERROR_FUNCTION )
{

   hb_retni( fann_get_train_error_function( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_train_error_function( *ann, iType )
 */

HB_FUNC( FANN_SET_TRAIN_ERROR_FUNCTION )
{

   fann_set_train_error_function( (struct fann *) HB_PARHANDLE(1), hb_parni(2) );
}

/*
 * fann_get_train_stop_function( *ann )
 */

HB_FUNC( FANN_GET_TRAIN_STOP_FUNCTION )
{

   hb_retni( fann_get_train_stop_function( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_train_stop_function( *ann, iType )
 */

HB_FUNC( FANN_SET_TRAIN_STOP_FUNCTION )
{

   fann_set_train_stop_function( (struct fann *) HB_PARHANDLE(1), hb_parni(2) );
}

/*
 * fann_get_bit_fail_limit( *ann )
 */

HB_FUNC( FANN_GET_BIT_FAIL_LIMIT )
{

   hb_retnd( fann_get_bit_fail_limit( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_bit_fail_limit( *ann, dLimit )
 */

HB_FUNC( FANN_SET_BIT_FAIL_LIMIT )
{

   fann_set_bit_fail_limit( (struct fann *) HB_PARHANDLE(1), (float) hb_parnd(2) );
}

/*
 * fann_set_callback( *ann, cFuncName )
 */

HB_FUNC( FANN_SET_CALLBACK )
{

   if( HB_ISCHAR(2) )
   {
      sz_Callback = ( char* ) hb_parc(2);
      s_pSymTest = NULL;
      fann_set_callback( (struct fann *) HB_PARHANDLE(1), fn_callback );
   }
   else
      fann_set_callback( (struct fann *) HB_PARHANDLE(1), dummy_callback );
}

/*
 * fann_get_quickprop_decay( *ann )
 */

HB_FUNC( FANN_GET_QUICKPROP_DECAY )
{

   hb_retnd( fann_get_quickprop_decay( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_quickprop_decay( *ann, dDecay )
 */

HB_FUNC( FANN_SET_QUICKPROP_DECAY )
{

   fann_set_quickprop_decay( (struct fann *) HB_PARHANDLE(1), (float) hb_parnd(2) );
}

/*
 * fann_get_quickprop_mu( *ann )
 */

HB_FUNC( FANN_GET_QUICKPROP_MU )
{

   hb_retnd( fann_get_quickprop_mu( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_quickprop_mu( *ann, dMU )
 */

HB_FUNC( FANN_SET_QUICKPROP_MU )
{

   fann_set_quickprop_mu( (struct fann *) HB_PARHANDLE(1), (float) hb_parnd(2) );
}

/*
 * fann_get_rprop_increase_factor( *ann )
 */

HB_FUNC( FANN_GET_RPROP_INCREASE_FACTOR )
{

   hb_retnd( fann_get_rprop_increase_factor( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_rprop_increase_factor( *ann, dFactor )
 */

HB_FUNC( FANN_SET_RPROP_INCREASE_FACTOR )
{

   fann_set_rprop_increase_factor( (struct fann *) HB_PARHANDLE(1), (float) hb_parnd(2) );
}

/*
 * fann_get_rprop_decrease_factor( *ann )
 */

HB_FUNC( FANN_GET_RPROP_DECREASE_FACTOR )
{

   hb_retnd( fann_get_rprop_decrease_factor( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_rprop_decrease_factor( *ann, dFactor )
 */

HB_FUNC( FANN_SET_RPROP_DECREASE_FACTOR )
{

   fann_set_rprop_decrease_factor( (struct fann *) HB_PARHANDLE(1), (float) hb_parnd(2) );
}

/*
 * fann_get_rprop_delta_min( *ann )
 */

HB_FUNC( FANN_GET_RPROP_DELTA_MIN )
{

   hb_retnd( fann_get_rprop_delta_min( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_rprop_delta_min( *ann, ddelta_min )
 */

HB_FUNC( FANN_SET_RPROP_DELTA_MIN )
{

   fann_set_rprop_delta_min( (struct fann *) HB_PARHANDLE(1), (float) hb_parnd(2) );
}

/*
 * fann_get_rprop_delta_max( *ann )
 */

HB_FUNC( FANN_GET_RPROP_DELTA_MAX )
{

   hb_retnd( fann_get_rprop_delta_max( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_rprop_delta_max( *ann, ddelta_max )
 */

HB_FUNC( FANN_SET_RPROP_DELTA_MAX )
{

   fann_set_rprop_delta_max( (struct fann *) HB_PARHANDLE(1), (float) hb_parnd(2) );
}

/*
 * fann_get_rprop_delta_zero( *ann )
 */

HB_FUNC( FANN_GET_RPROP_DELTA_ZERO )
{

   hb_retnd( fann_get_rprop_delta_zero( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_rprop_delta_zero( *ann, ddelta_zero )
 */

HB_FUNC( FANN_SET_RPROP_DELTA_ZERO )
{

   fann_set_rprop_delta_zero( (struct fann *) HB_PARHANDLE(1), (float) hb_parnd(2) );
}

/*
 * fann_get_sarprop_weight_decay_shift( *ann )
 */

HB_FUNC( FANN_GET_SARPROP_WEIGHT_DECAY_SHIFT )
{

   hb_retnd( fann_get_sarprop_weight_decay_shift( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_sarprop_weight_decay_shift( *ann, dShift )
 */

HB_FUNC( FANN_SET_SARPROP_WEIGHT_DECAY_SHIFT )
{

   fann_set_sarprop_weight_decay_shift( (struct fann *) HB_PARHANDLE(1), (float) hb_parnd(2) );
}

/*
 * fann_get_sarprop_step_error_threshold_factor( *ann )
 */

HB_FUNC( FANN_GET_SARPROP_STEP_ERROR_THRESHOLD_FACTOR )
{

   hb_retnd( fann_get_sarprop_step_error_threshold_factor( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_sarprop_step_error_threshold_factor( *ann, dFactor )
 */

HB_FUNC( FANN_SET_SARPROP_STEP_ERROR_THRESHOLD_FACTOR )
{

   fann_set_sarprop_step_error_threshold_factor( (struct fann *) HB_PARHANDLE(1), (float) hb_parnd(2) );
}


/*
 * fann_get_sarprop_step_error_shift( *ann )
 */

HB_FUNC( FANN_GET_SARPROP_STEP_ERROR_SHIFT )
{

   hb_retnd( fann_get_sarprop_step_error_shift( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_sarprop_step_error_shift( *ann, dShift )
 */

HB_FUNC( FANN_SET_SARPROP_STEP_ERROR_SHIFT )
{

   fann_set_sarprop_step_error_shift( (struct fann *) HB_PARHANDLE(1), (float) hb_parnd(2) );
}

/*
 * fann_get_sarprop_temperature( *ann )
 */

HB_FUNC( FANN_GET_SARPROP_TEMPERATURE )
{

   hb_retnd( fann_get_sarprop_temperature( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_set_sarprop_temperature( *ann, dTemp )
 */

HB_FUNC( FANN_SET_SARPROP_TEMPERATURE )
{

   fann_set_sarprop_temperature( (struct fann *) HB_PARHANDLE(1), (float) hb_parnd(2) );
}

