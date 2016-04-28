/*
 *  Converted to Harbour from the original simple_test.c example program
 *  of FANN distribution.
 */

Function Main

   LOCAL pAnn, aOut

   SET DECIMALS TO 6

   pAnn := fann_create_from_file( "xor_float.net" )

   aOut = fann_run( pAnn, { -1,1 } )

   ? "xor test ( -1, 1 ) => " + Str( aOut[1] )
   ?

   fann_destroy( pAnn )

   Return Nil