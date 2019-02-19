/*
 *  Converted to Harbour from the original robot.c example program
 *  of FANN distribution.
 */

#include "fann2hrb.ch"

Function Main

   LOCAL pAnn, pTrainData, pTestData
   LOCAL num_neurons_hidden := 96
   LOCAL desired_error := 0.001, i := 0, iLen
   LOCAL cDataPath := "../../FANN_220/datasets/"
   LOCAL cTrainFile := "robot.train", cTestFile := "robot.test"

   SET DECIMALS TO 6

   ? "Creating network."

   IF ( pTrainData := fann_read_train_from_file( cDataPath+cTrainFile ) ) == Nil
      ? "Can't read from " + cTrainFile
      RETURN Nil
   ENDIF
   IF ( pTestData := fann_read_train_from_file( cDataPath+cTestFile ) ) == Nil
      ? "Can't read from " + cTestFile
      RETURN Nil
   ENDIF

   pAnn := fann_create_standard( { fann_num_input_train_data(pTrainData), ;
              num_neurons_hidden, fann_num_output_train_data(pTrainData) } )

   ? "Training network."

   fann_set_training_algorithm( pAnn, FANN_TRAIN_INCREMENTAL )
   fann_set_learning_momentum( pAnn, 0.4 )

   fann_train_on_data( pAnn, pTrainData, 3000, 10, desired_error )

   ? "Testing network."

   fann_reset_MSE( pAnn )

   iLen := fann_length_train_data( pTestData )
   FOR i := 1 TO iLen
      fann_test( pAnn, fann_get_input_train_data( pTestData,i ), ;
         fann_get_output_train_data( pTestData,i ) )
   NEXT

   ? "MSE error on test data: " + Ltrim(Str(fann_get_MSE( pAnn )))

   ? "Saving network."

   fann_save( pAnn, "robot_float.net" )

   ? "Cleaning up."
   fann_destroy_train( pTrainData )
   fann_destroy_train( pTestData )
   fann_destroy( pAnn )

   RETURN Nil
