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
   HB_SYMBOL_UNUSED( ann );
   HB_SYMBOL_UNUSED( train );
   HB_SYMBOL_UNUSED( max_epochs );
   HB_SYMBOL_UNUSED( epochs_between_reports );
   HB_SYMBOL_UNUSED( desired_error );
   HB_SYMBOL_UNUSED( epochs );
   //printf("Epochs     %8d. MSE: %.5f. Desired-MSE: %.5f\n", epochs, fann_get_MSE(ann), desired_error);
   return 0;
}

int FANN_API fn_callback( struct fann *ann, struct fann_train_data *train,
	unsigned int max_epochs, unsigned int epochs_between_reports,
	float desired_error, unsigned int epochs )
{

   long int res = 0;
   if( s_pSymTest == NULL )
      s_pSymTest = hb_dynsymGetCase( sz_Callback );

   if( hb_dynsymIsFunction( s_pSymTest ) )
   {
      hb_vmPushDynSym( s_pSymTest );
      hb_vmPushNil();   /* places NIL at self */

      HB_PUSHITEM( ann );
      HB_PUSHITEM( train );
      hb_vmPushLong( ( long int ) max_epochs );
      hb_vmPushLong( ( long int ) epochs_between_reports );
      hb_vmPushDouble( ( double ) desired_error, 6 );
      hb_vmPushLong( ( long int ) epochs );
      hb_vmDo( 6 );     /* where iArgCount is the number of pushed parameters */
      res = hb_parnl( -1 );
   }
   return res;
}

/*
 * *ann = fann_create_standard( { num_input, ..., num_output } )
 */

HB_FUNC( FANN_CREATE_STANDARD )
{

   struct fann *ann;
   PHB_ITEM pArr = hb_param( 1, HB_IT_ARRAY );
   unsigned int iLayers = hb_arrayLen( pArr ), ui, * pLayers;

   pLayers = (unsigned int *) malloc( sizeof(unsigned int) * iLayers );
   for( ui = 0; ui < iLayers; ui++ )
      *( pLayers+ui ) = hb_arrayGetNI( pArr, ui+1 );
   
   ann = fann_create_standard_array( iLayers, pLayers );

   free( pLayers );
   HB_RETHANDLE( ann );
}

/*
 * *ann = fann_create_sparse( connection_rate, { num_input, ..., num_output } )
 */

HB_FUNC( FANN_CREATE_SPARSE )
{

   struct fann *ann;
   PHB_ITEM pArr = hb_param( 2, HB_IT_ARRAY );
   unsigned int iLayers = hb_arrayLen( pArr ), ui, * pLayers;

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
 * fann_randomize_weights( *ann, min_weight, max_weight )
 */

HB_FUNC( FANN_RANDOMIZE_WEIGHTS )
{

   fann_randomize_weights( (struct fann *) HB_PARHANDLE(1), hb_parnd(2), hb_parnd(3) );
}

/*
 * fann_init_weights( *ann, pdata )
 */

HB_FUNC( FANN_INIT_WEIGHTS )
{

   fann_init_weights( (struct fann *) HB_PARHANDLE(1),
         (struct fann_train_data *) HB_PARHANDLE(2) );
}


/*
 * *ann = fann_create_from_file( *ann );
 */

HB_FUNC( FANN_CREATE_FROM_FILE )
{

   struct fann *ann = fann_create_from_file( hb_parc(1) );
   if( ann )
      HB_RETHANDLE( ann );
   else
      hb_ret();
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
      temp = hb_itemPutNL( NULL, *(pLayers+ui-1) );
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
      temp = hb_itemPutNL( NULL, *(pBias+ui-1) );
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
   for( ui = 0; ui < iConns; ui++ )
   {
      //fann_writelog( NULL, "from: %u %u %f\r\n ",(pConns+ui)->from_neuron, (pConns+ui)->to_neuron, (pConns+ui)->weight );
      tarr = hb_itemArrayNew( 3 );
      hb_itemPutNL( temp, pConns[ui].from_neuron );
      hb_itemArrayPut( tarr, 1, temp );
      hb_itemPutNL( temp, pConns[ui].to_neuron );
      hb_itemArrayPut( tarr, 2, temp );
      hb_itemPutND( temp, pConns[ui].weight );
      hb_itemArrayPut( tarr, 3, temp );

      hb_itemArrayPut( aMetr, ui+1, tarr );
      hb_itemRelease( tarr );
   }
   hb_itemRelease( temp );

   free( pConns );

   hb_itemReturn( aMetr );
   hb_itemRelease( aMetr );

}

/*
 * fann_set_weight( *ann, from_neuron, to_neuron, weight )
 */

HB_FUNC( FANN_SET_WEIGHT )
{

   fann_set_weight( (struct fann *) HB_PARHANDLE(1), hb_parnl(1), hb_parnl(2), hb_parnd(3) );
}

// ------------- Training -------------


/*
 * fann_train( *ann, *inputs, *desired_outputs )
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
 * fann_test( *ann, *inputs, *desired_outputs )
 */

HB_FUNC( FANN_TEST )
{

   struct fann *ann = (struct fann *) HB_PARHANDLE(1);
   fann_type *output, *input, *calc_out;
   PHB_ITEM pArrIn = hb_param( 2, HB_IT_ARRAY );
   PHB_ITEM pArrOut = hb_param( 3, HB_IT_ARRAY );
   unsigned int uiInputs = (unsigned int) hb_arrayLen( pArrIn ), ui;
   unsigned int uiOutputs = (unsigned int) hb_arrayLen( pArrOut );
   PHB_ITEM aMetr, temp;

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


   calc_out = fann_test( ann, input, output );
   free( input );
   free( output );

   aMetr = hb_itemArrayNew( uiOutputs );
   temp = hb_itemNew( NULL );
   for( ui = 1; ui <= uiOutputs; ui++ )
   {
      hb_itemPutND( temp, *(calc_out+ui-1) );
      hb_itemArrayPut( aMetr, ui, temp );
   }
   hb_itemRelease( temp );

   hb_itemReturn( aMetr );
   hb_itemRelease( aMetr );

}

/*
 * fann_get_MSE( *ann )
 */

HB_FUNC( FANN_GET_MSE )
{

   hb_retnd( fann_get_MSE( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_get_bit_fail( *ann )
 */

HB_FUNC( FANN_GET_BIT_FAIL )
{

   hb_retnl( fann_get_bit_fail( (struct fann *) HB_PARHANDLE(1) ) );
}

/*
 * fann_reset_MSE( *ann )
 */

HB_FUNC( FANN_RESET_MSE )
{

   fann_reset_MSE( (struct fann *) HB_PARHANDLE(1) );
}

/*
 * fann_train_on_data( *ann, *pdata, max_epochs, epochs_between_reports, desired_error )
 */

HB_FUNC( FANN_TRAIN_ON_DATA )
{

   fann_train_on_data( (struct fann *) HB_PARHANDLE(1),
      (struct fann_train_data *) HB_PARHANDLE(2), (const unsigned int) hb_parni(3),
      (const unsigned int) hb_parni(4), (const float) hb_parnd(5) );
}

/*
 * fann_train_on_file( *ann, cFileName, max_epochs, epochs_between_reports, desired_error )
 */

HB_FUNC( FANN_TRAIN_ON_FILE )
{

   fann_train_on_file( (struct fann *) HB_PARHANDLE(1), hb_parc(2), (const unsigned int) hb_parni(3), (const unsigned int) hb_parni(4), (const float) hb_parnd(5) );
}

/*
 * fann_train_epoch( *ann, *pdata )
 */

HB_FUNC( FANN_TRAIN_EPOCH )
{

   hb_retnd( fann_train_epoch( (struct fann *) HB_PARHANDLE(1),
      (struct fann_train_data *) HB_PARHANDLE(2) ) );
}

/*
 * fann_test_data( *ann, *pdata )
 */

HB_FUNC( FANN_TEST_DATA )
{

   hb_retnd( fann_test_data( (struct fann *) HB_PARHANDLE(1),
      (struct fann_train_data *) HB_PARHANDLE(2) ) );
}

/*
 * fann_read_train_from_file( cFileName )
 */

HB_FUNC( FANN_READ_TRAIN_FROM_FILE )
{

   struct fann_train_data * anndata = fann_read_train_from_file( hb_parc(1) );

   if( anndata )
      HB_RETHANDLE( anndata );
   else
      hb_ret();
}

/*
 * fann_create_train( num_data, num_input, num_output )
 */

HB_FUNC( FANN_CREATE_TRAIN )
{

   struct fann_train_data * anndata = fann_create_train( (unsigned int) hb_parni(1),
         (unsigned int) hb_parni(2), (unsigned int) hb_parni(3) );

   HB_RETHANDLE( anndata );
}

/*
 * fann_destroy_train( *pdata )
 */

HB_FUNC( FANN_DESTROY_TRAIN )
{

   fann_destroy_train( (struct fann_train_data *) HB_PARHANDLE(1) );
}

/*
 * fann_get_input_train_data( *pdata, num_input )
 */

HB_FUNC( FANN_GET_INPUT_TRAIN_DATA )
{

   struct fann_train_data *pdata = (struct fann_train_data *) HB_PARHANDLE(1);
   unsigned int num_input, ui, iInput = hb_parni(2) - 1;
   PHB_ITEM aMetr, temp;

   num_input = pdata->num_input;
   aMetr = hb_itemArrayNew( num_input );
   temp = hb_itemNew( NULL );
   for( ui = 0; ui < num_input; ui++ )
   {
      hb_itemPutND( temp, (*(pdata->input+iInput))[ui] );
      hb_itemArrayPut( aMetr, ui+1, temp );
   }
   hb_itemRelease( temp );

   hb_itemReturn( aMetr );
   hb_itemRelease( aMetr );

}

/*
 * fann_get_output_train_data( *pdata, num_input )
 */

HB_FUNC( FANN_GET_OUTPUT_TRAIN_DATA )
{

   struct fann_train_data *pdata = (struct fann_train_data *) HB_PARHANDLE(1);
   unsigned int num_output, ui, iOutput = hb_parni(2) - 1;
   PHB_ITEM aMetr, temp;

   num_output = pdata->num_output;
   aMetr = hb_itemArrayNew( num_output );
   temp = hb_itemNew( NULL );
   for( ui = 0; ui < num_output; ui++ )
   {
      hb_itemPutND( temp, (*(pdata->output+iOutput))[ui] );
      hb_itemArrayPut( aMetr, ui+1, temp );
   }
   hb_itemRelease( temp );

   hb_itemReturn( aMetr );
   hb_itemRelease( aMetr );

}

/*
 * fann_set_train_data( *pdata, num, pInput, pOutput )
 */

HB_FUNC( FANN_SET_TRAIN_DATA )
{
   struct fann_train_data *pdata = (struct fann_train_data *) HB_PARHANDLE(1);
   unsigned int uiData = hb_parni(2), ui;
   PHB_ITEM pArrIn = hb_param( 3, HB_IT_ARRAY ), pArr;
   PHB_ITEM pArrOut = hb_param( 4, HB_IT_ARRAY );

   if( uiData )
   {
      uiData --;
      if( pArrIn )
      {
         for( ui = 0; ui < pdata->num_input; ui++ )
            (*(pdata->input+uiData))[ui] = hb_arrayGetND( pArrIn, ui+1 );
      }
      if( pArrOut )
      {
         for( ui = 0; ui < pdata->num_output; ui++ )
            (*(pdata->output+uiData))[ui] = hb_arrayGetND( pArrOut, ui+1 );
      }
   }
   else
   {
      for( uiData = 0; uiData < pdata->num_data; uiData++ )
      {
         if( pArrIn )
         {
            pArr = hb_arrayGetItemPtr( pArrIn, uiData+1 );
            for( ui = 0; ui < pdata->num_input; ui++ )
               (*(pdata->input+uiData))[ui] = hb_arrayGetND( pArr, ui+1 );
         }
         if( pArrOut )
         {
            pArr = hb_arrayGetItemPtr( pArrOut, uiData+1 );
            for( ui = 0; ui < pdata->num_output; ui++ )
               (*(pdata->output+uiData))[ui] = hb_arrayGetND( pArr, ui+1 );
         }
      }
   }
}

/*
 * fann_length_train_data( *pdata )
 */

HB_FUNC( FANN_LENGTH_TRAIN_DATA )
{

   hb_retni( fann_length_train_data( (struct fann_train_data *) HB_PARHANDLE(1) ) );
}

/*
 * fann_num_input_train_data( *pdata )
 */

HB_FUNC( FANN_NUM_INPUT_TRAIN_DATA )
{

   hb_retni( fann_num_input_train_data( (struct fann_train_data *) HB_PARHANDLE(1) ) );
}

/*
 * fann_num_output_train_data( *pdata )
 */

HB_FUNC( FANN_NUM_OUTPUT_TRAIN_DATA )
{

   hb_retni( fann_num_output_train_data( (struct fann_train_data *) HB_PARHANDLE(1) ) );
}

/*
 * fann_save_train( *pdata, cFileName )
 */

HB_FUNC( FANN_SAVE_TRAIN )
{

   int iRes = fann_save_train( (struct fann_train_data *) HB_PARHANDLE(1), hb_parc(2) );
   hb_retl( (iRes==0)? 1 : 0 );
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

