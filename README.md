## hrb4fann

Harbour bindings for FANN - Fast Artificial Neural Networks 2.2.0.

### Preface

Harbour is a modern programming language, primarily used to create database/business programs. It is a modernized, open sourced and cross-platform version of the older and largely DOS-only Clipper system, which in turn developed from the dBase database market of the 1980s and 90s.
See more info at http://www.kresin.ru/en/harbour.html

Fast Artificial Neural Network Library is a free open source neural network library, which implements multilayer artificial neural networks in C with support for both fully connected and sparsely connected networks.
FANN official site is http://leenissen.dk/

### Installation

First of all you need to download _FANN_ library, unpack and compile it, using the C compiler of your choice - the same, which you use with Harbour.
There are compile scripts in _hrb4fann/fann_ directory:

* lib_fann.sh         - for Linux
* lib_fann_mingw.bat  - for Windows, Mingw C compiler

Before using one of them, you may need to edit it for to set the correct path to the unpacked FANN directory ( FANN_DIR variable ) and, if you work in Windows, the path to the C compiler.
In case of successful compiling you'll find the library in _hrb4fann/lib_ directory.

The next step is the compiling of _hrb4funn_ itself with one of scripts in _hrb4fann/_ directory:

* lib_hrb4fann.sh         - for Linux
* lib_hrb4fann_mingw.bat  - for Windows, Mingw C compiler

As in case of compiling the _FANN_ library, you need to set correct paths to the unpacked FANN directory and, for Windows, the path to the C compiler.
Additionally, you need to set the path to the Harbour in HRB_DIR variable.

### Functions list

* pAnn = fann_create_standard( num_layers, { num_input, ..., num_output } )
* pAnn = fann_create_sparse( connection_rate, num_layers, { num_input, ..., num_output } )
* pAnn = fann_copy( pAnn )
* fann_destroy( pAnn )
* aOutput = fann_run( pAnn, aInput )
* fann_randomize_weights( pAnn, min_weight, max_weight )
* fann_init_weights( pAnn, pdata )
* pAnn = fann_create_from_file( pAnn )
* fann_save( pAnn, cFileName )
* fann_get_num_input( pAnn )
* fann_get_num_output( pAnn )
* fann_get_total_neurons( pAnn )
* fann_get_total_connections( pAnn )
* fann_get_network_type( pAnn )
* fann_get_connection_rate( pAnn )
* fann_get_num_layers( pAnn )
* fann_get_layer_array( pAnn )
* fann_get_bias_array( pAnn )
* fann_get_connection_array( pAnn )
* fann_set_weight( pAnn )

* fann_train( pAnn, aInputs, aDesired_outputs )
* fann_test( pAnn, aInputs, aDesired_outputs )
* fann_get_MSE( pAnn )
* fann_get_bit_fail( pAnn )
* fann_reset_MSE( pAnn )
* fann_train_on_data( pAnn, pData, max_epochs, epochs_between_reports, desired_error )
* fann_train_on_file( pAnn, cFileName, max_epochs, epochs_between_reports, desired_error )
* fann_train_epoch( pAnn, pData )
* fann_test_data( pAnn, pData )
* fann_read_train_from_file( cFileName )
* fann_create_train( num_data, num_input, num_output )
* fann_destroy_train( pData )
* fann_get_input_train_data( pData, num_input )
* fann_get_output_train_data( pData, num_input )
* fann_set_train_data( pData, num, pInput, pOutput )
* fann_length_train_data( pData )
* fann_num_input_train_data( pData )
* fann_num_output_train_data( pData )
* fann_save_train( pData, cFileName )
* fann_get_training_algorithm( pAnn )
* fann_set_training_algorithm( pAnn )
* fann_get_learning_rate( pAnn )
* fann_set_learning_rate( pAnn )
* fann_get_learning_momentum( pAnn )
* fann_set_learning_momentum( pAnn )
* fann_get_activation_function( pAnn, ilayer, iNeuron )
* fann_set_activation_function( pAnn, iType, ilayer, iNeuron )
* fann_set_activation_function_layer( pAnn, iType, ilayer )
* fann_set_activation_function_hidden( pAnn, iType )
* fann_set_activation_function_output( pAnn, iType )
* fann_get_activation_steepness( pAnn, ilayer, iNeuron )
* fann_set_activation_steepness( pAnn, dSteepness, ilayer, iNeuron )
* fann_set_activation_steepness_layer( pAnn, dSteepness, ilayer )
* fann_set_activation_steepness_hidden( pAnn, dSteepness )
* fann_set_activation_steepness_output( pAnn, dSteepness )
* fann_get_train_error_function( pAnn )
* fann_set_train_error_function( pAnn, iType )
* fann_get_train_stop_function( pAnn )
* fann_set_train_stop_function( pAnn, iType )
* fann_get_bit_fail_limit( pAnn )
* fann_set_bit_fail_limit( pAnn, dLimit )
* fann_set_callback( pAnn, cFuncName )
* fann_get_quickprop_decay( pAnn )
* fann_set_quickprop_decay( pAnn, dDecay )
* fann_get_quickprop_mu( pAnn )
* fann_set_quickprop_mu( pAnn, dMU )
* fann_get_rprop_increase_factor( pAnn )
* fann_set_rprop_increase_factor( pAnn, dFactor )
* fann_get_rprop_decrease_factor( pAnn )
* fann_set_rprop_decrease_factor( pAnn, dFactor )
* fann_get_rprop_delta_min( pAnn )
* fann_set_rprop_delta_min( pAnn, ddelta_min )
* fann_get_rprop_delta_max( pAnn )
* fann_set_rprop_delta_max( pAnn, ddelta_max )
* fann_get_rprop_delta_zero( pAnn )
* fann_set_rprop_delta_zero( pAnn, ddelta_zero )
* fann_get_sarprop_weight_decay_shift( pAnn )
* fann_set_sarprop_weight_decay_shift( pAnn, dShift )
* fann_get_sarprop_step_error_threshold_factor( pAnn )
* fann_set_sarprop_step_error_threshold_factor( pAnn, dFactor )
* fann_get_sarprop_step_error_shift( pAnn )
* fann_set_sarprop_step_error_shift( pAnn, dShift )
* fann_get_sarprop_temperature( pAnn )
* fann_get_sarprop_temperature( pAnn )
* fann_set_sarprop_temperature( pAnn, dTemp )
