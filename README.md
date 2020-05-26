# pl0-compiler

A simple compiler for PL/0, which can print Grammar Tree, Identifier Table and Object Codes.

There is also an Interpreter to run your source code.

## Install

```shell
mkdir build
cd build
cmake ..
make
```

To print the Grammar Tree, you should uncomment `add_compile_options(-DPRINT_GRAMMAR_TREE)` in file `CMakeLists.txt`

## Usage

In the build directory, you can run compiler with following command.

```shell
./pl0 <src>
```

There are some source codes for testing the directory `./test`.

## LICENSE

[The Star And Thank Author License (SATA)](https://github.com/jesHrz/pl0-compiler/blob/master/LICENSE)
