# csformula - c++11 class for evaluating of formula expression specified by the string.

Inspired by the idea of the [calculation class Eval](http://mathgl.sourceforge.net/doc_en/Evaluate-expression.html) from the [MathGL](http://mathgl.sourceforge.net/) library. Developed especially for use as part of the CAPSYS application (ver. 2.1).

The csformula contains case insensitive string parser.

## Development status
Beta.

## Dependencies
[CMake](https://cmake.org/) 3.9+
[Boost](https://www.boost.org/) 1.65+

## Installation

- If you do not have the boost libraries you can get it from the official repository. For **csformula** there is no need to build Boost. Just download the version of Boost that you require.
[Boost: Getting Started on Microsoft Windows](https://www.boost.org/doc/libs/release/more/getting_started/windows.html)
[Boost: Getting Started on Unix variants (e.g. Linux, MacOS)](https://www.boost.org/doc/libs/release/more/getting_started/unix-variants.html)
For example on Linux:

* wget -c 'http://sourceforge.net/projects/boost/files/boost/1.66.0/boost_1_66_0.tar.bz2/download'
* tar xf download


* cmake -DBoost_INCLUDE_DIR:PATH=./boost_1_66_0 .-DBoost_ROOT:PATH=./boost_1_66_0 --build ./build
* cd build
* make

bin/csformulalib
bin/csformulatest


## Examples

***


***



Each variable in the string can only be set as a single Latin character: ('a','b',...,'z').
The following operations are supported:
 * |
 * &
 * =
 * >
 * <
 * +
 * -
 * /
 * *
 * ^
 * "sin"
 * "asin"
 * "cos"
 * "acos"
 * "tan"
 * "atan"
 * "log"
 * "sqrt"
 * "exp"