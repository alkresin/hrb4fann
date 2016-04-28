/*
 *  Converted to Harbour from the original xor_train.c example program
 *  of FANN distribution.
 */

#include "fann2hrb.ch"

Function Main

   LOCAL pAnn, pData, i, iLen, aInput, aOutput
   LOCAL calc_out
   LOCAL num_input := 2, num_output := 1
   LOCAL num_layers := 3, num_neurons_hidden := 3
   LOCAL desired_error := 0
   LOCAL max_epochs := 1000
   LOCAL epochs_between_reports := 10

   SET DECIMALS TO 6

   ? "Creating network."
   pAnn := fann_create_standard( num_layers, { num_input, num_neurons_hidden, num_output } );

   pData := fann_read_train_from_file( "xor.data" )

   fann_set_activation_steepness_hidden( pAnn, 1 )
   fann_set_activation_steepness_output( pAnn, 1 )

   fann_set_activation_function_hidden( pAnn, FANN_SIGMOID_SYMMETRIC )
   fann_set_activation_function_output( pAnn, FANN_SIGMOID_SYMMETRIC )

   fann_set_train_stop_function( pAnn, FANN_STOPFUNC_BIT )
   fann_set_bit_fail_limit( pAnn, 0.01 )

   fann_set_training_algorithm( pAnn, FANN_TRAIN_RPROP )

   fann_init_weights( pAnn, pData )

   ? "Training network."
   fann_set_callback( pAnn, "CB" )
   fann_train_on_data( pAnn, pData, max_epochs, epochs_between_reports, desired_error )

   ? "Testing network. " + Ltrim(Str( fann_test_data( pAnn, pData) ))

   iLen := fann_length_train_data( pData )
   FOR i := 1 TO iLen
      aInput := fann_get_input_train_data( pData, i )
      aOutput := fann_get_output_train_data( pData, i )
      calc_out := fann_run( pAnn, aInput )
      ? "XOR test (" + Ltrim(Str(aInput[1])) + "," + Ltrim(Str(aInput[2])) + ;
         ") -> " + Ltrim(Str(calc_out[1])) + ", should be " + ;
         Ltrim(Str(aOutput[1])) + ", difference=" + Ltrim(Str(Abs(calc_out[1]-aOutput[1])))
   NEXT

   ? "Saving network."

   fann_save( pAnn, "xor_float.net" )

   ? "Cleaning up."
   fann_destroy_train( pData )
   fann_destroy( pAnn )

   RETURN Nil

Function CB( pAnn, pData, max_epochs, epochs_between_reports, desired_error, epochs )

      ? "Epochs     " + Ltrim(Str(epochs)) + ". MSE: " + ;
         Ltrim(Str(fann_get_MSE(pAnn))) + ". Desired-MSE: " + Ltrim(Str(desired_error))
   RETURN Nil	
