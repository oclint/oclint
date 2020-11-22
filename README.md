# OCLint - https://oclint.org

[![GitHub Actions Status](https://github.com/oclint/oclint/workflows/Builds/badge.svg?branch=master)](https://github.com/oclint/oclint/actions) [![Coverage Status](https://coveralls.io/repos/github/oclint/oclint/badge.svg?branch=master)](https://coveralls.io/github/oclint/oclint?branch=master)

OCLint is a static code analysis tool for improving quality and reducing defects
by inspecting C, C++ and Objective-C code.

It looks for potential problems that aren't visible to compilers, for example:

* Possible bugs - empty if/else/try/catch/finally statements
* Unused code - unused local variables and parameters
* Complicated code - high cyclomatic complexity, NPath complexity and high NCSS
* Redundant code - redundant if statement and useless parentheses
* Code smells - long method and long parameter list
* Bad practices - inverted logic and parameter reassignment
* ...

For more information, visit https://oclint.org
