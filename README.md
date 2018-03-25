# csformula
C++ class for evaluating of formula expression specified by the string.

Inspired by the idea of the [calculation class Eval](http://mathgl.sourceforge.net/doc_en/Evaluate-expression.html) from the [MathGL](http://mathgl.sourceforge.net/) library. Developed especially for use as part of the CAPSYS application (ver. 2.1).

The csformula contains case insensitive string parser. Each variable in the string can only be set as a single Latin character: ('a','b',...,'z').

***

wget -c 'http://sourceforge.net/projects/boost/files/boost/1.66.0/boost_1_66_0.tar.bz2/download'

tar xf download

cmake -DBoost_INCLUDE_DIR:PATH=./boost_1_66_0 .-DBoost_ROOT:PATH=./boost_1_66_0 --build ./build

cd build

make

bin/csformulalib
bin/csformulatest
