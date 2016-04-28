/*
 *  Converted to Harbour from the original xor_test.c example program
 *  of FANN distribution.
 */

#include "fann2hrb.ch"

Function Main

   LOCAL calc_out
   LOCAL i, iLen, iRet := 0
   LOCAL pAnn, pData, aInput, aOutput

   SET DECIMALS TO 6

   ? "Creating network."

   IF ( pAnn := fann_create_from_file( "xor_float.net" ) ) == Nil
      ? "Error creating ann --- ABORTING."
      QUIT
   ENDIF

   ? "Testing network."

   pData := fann_read_train_from_file( "xor.data" )

   iLen := fann_length_train_data( pData )
   FOR i := 1 TO iLen
      fann_reset_MSE( pAnn )
      aInput := fann_get_input_train_data( pData, i )
      aOutput := fann_get_output_train_data( pData, i )
      calc_out := fann_test( pAnn, aInput, aOutput )
      ? "XOR test (" + Ltrim(Str(aInput[1])) + ", " + Ltrim(Str(aInput[2])) + ;
            ") -> " + Ltrim(Str(calc_out[1])) + ", should be " + ;
            Ltrim(Str(aOutput[1])) + ", difference=" + Ltrim(Str(Abs(calc_out[1]-aOutput[1])))
   NEXT

   ? "Cleaning up."
   fann_destroy_train( pData )
   fann_destroy( pAnn )

   RETURN Nil
