/*
 *  Converted to Harbour from the original simple_train.c example program
 *  of FANN distribution.
 */

#include "fann2hrb.ch"

Function Main

   LOCAL pAnn, i, arr

   SET DECIMALS TO 6

   pAnn := fann_create_standard( 3, {2, 3, 1} )

   fann_set_activation_function_hidden( pAnn, FANN_SIGMOID_SYMMETRIC )
   fann_set_activation_function_output( pAnn, FANN_SIGMOID_SYMMETRIC )

   //fann_set_callback( pAnn )
   fann_train_on_file( pAnn, "xor.data", 500000, 1000, 0.001 );

   arr := fann_get_connection_array( pAnn )
   FOR i := 1 TO Len( arr )
      ? arr[i,1], arr[i,2], arr[i,3]
   NEXT

   fann_save( pAnn, "xor_float.net");

   fann_destroy( pAnn )

   Return Nil