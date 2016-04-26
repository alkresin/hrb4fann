
Function Main

   LOCAL pAnn, aOut

   SET DECIMALS TO 10

   pAnn := fann_create_from_file( "xor_float.net" )

   aOut = fann_run( pAnn, { -1,1 } )

   ? "xor test ( -1, 1 ) => " + Str( aOut[1] )
   ?

   fann_destroy( pAnn )

   Return Nil