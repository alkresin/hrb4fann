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
