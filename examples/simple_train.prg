

Function Main

   LOCAL pAnn, i, j, arr

   pAnn := fann_create_standard( 3, {2, 3, 1} )

   fann_set_activation_function_hidden( pAnn, 5 )
   fann_set_activation_function_output( pAnn, 5 )

   //fann_set_callback( pAnn )
   fann_train_on_file( pAnn, "xor.data", 500000, 1000, 0.001 );

   fann_save( pAnn, "xor_float.net");

   fann_destroy( pAnn )

   Return Nil