# csformula

## c++11 class for evaluating of formula expression specified by the string.

Inspired by the idea of the [calculation class Eval](http://mathgl.sourceforge.net/doc_en/Evaluate-expression.html) from the [MathGL](http://mathgl.sourceforge.net/) library. Developed for use as part of the CAPSYS application.

The csformula contains case insensitive string parser. Let's imagine that you have a string, for example `"(x^2+y)/sin(a*z)"`, and you pass it to the `csformula` constructor. Also, you know that, for example `y=0.0000000000000000000000555, x=0.001, a=-1, z=-2`. It is enough to call the `Get(...)` method or the `GetD(...)` for calculate to get the value of the expression or the derivative of the expression at this point.

## Development status
Beta. Version: 1.0.0

## Dependencies
[CMake](https://cmake.org/) 3.9+
[Boost](https://www.boost.org/) 1.65+

## Building and installation

If you don't have the Boost libraries you can get it from the official repository. For **csformula** there is no need to build Boost. Just download the version of Boost that you require.

[Boost: Getting Started on Microsoft Windows](https://www.boost.org/doc/libs/release/more/getting_started/windows.html)
[Boost: Getting Started on Unix variants (e.g. Linux, MacOS)](https://www.boost.org/doc/libs/release/more/getting_started/unix-variants.html)
For example, on Linux you can go to the root directory of the project and run the following commands:

* wget -c 'http://sourceforge.net/projects/boost/files/boost/1.66.0/boost_1_66_0.tar.bz2/download'
* tar xf download

After this operation the `boost_1_66_0` directory appears in the root directory of the project.

***
To build **csformula** just run the following commands:

* `mkdir build && cd build`
* `cmake -B../build -DBoost_INCLUDE_DIR:PATH=./boost_1_66_0 -DBOOST_ROOT:PATH=./boost_1_66_0 ..`
* `cmake --build . --target all`

After building the library, you can find several files:

* build/bin/csformulalib
* build/bin/csformulatest
* build/bin/libcsformula.so

Header files:

* csformula/cseval.h
* csformula/csformula.h

## Details

Each variable in the string can only be set as a single Latin character: ('a','b',...,'z').
The following operations are supported:
 * `|`
 * `&`
 * `=`
 * `>`
 * `<`
 * `+`
 * `-`
 * `/`
 * `*`
 * `^`
 * `sin`
 * `asin`
 * `cos`
 * `acos`
 * `tan`
 * `atan`
 * `log`
 * `sqrt`
 * `exp`
 
 See examples and tests in the `tests/csformulatest/csformulatest.cpp` file.
