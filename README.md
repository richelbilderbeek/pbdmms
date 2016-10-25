# pbdmms

[![Travis CI logo](TravisCI.png)](https://travis-ci.org)
![Whitespace](Whitespace.png)
[![Codecov logo](Codecov.png)](https://www.codecov.io)
[![gplv3](http://www.gnu.org/graphics/gplv3-88x31.png)](http://www.gnu.org/licenses/gpl.html)

[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=master)](https://travis-ci.org/richelbilderbeek/pbdmms)
[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=master)](https://codecov.io/github/richelbilderbeek/pbdmms?branch=master)

Some models:

 * BDM
 * Jaled et al, 2008
 * Van Doorn & Weissing, 2002

## Installation

 * `apt-get install graphviz qtcreator`
 * `./download_other_githubs.sh`

## Building

Either load the `.pro` file from Qt Creator, or use `qmake` to build:

 * `qmake my.pro`: creates a makefile
 * `make`: build that makefile

## Coding standard

The Stroustrup/GNU style, e.g.:

 * Two spaces, no tabs
 * snake_case in variable, function and class names
 * member variables have the `m_` prefix
 * static member variables have the `sm_` prefix

```
void do_it()
{
  int some_thing;
  if (1 == 2) 
  {

  }
  else
  {

  }
}

class my_class
{
  void do_something();
  int m_some_variable;
  static int sm_count;
};
```
Then just follow OCLint

## File management

We have three collaborators, each with his own namespace
 * @JorikDeBoer: `jobo`
 * @Lumphie: `kewe`
 * @richelbilderbeek: `ribi`

Per project:

 * `[namespace].pro`: simulation project file
 * `[namespace]_test.pro`: test project file
 * `[namespace]_main.cpp`: runs simulation
 * `[namespace]_main_test.cpp`: runs test

Per unit:

 * `[namespace]_my_file.h`: contains declarations
 * `[namespace]_my_file.cpp`: contains definitions
 * `[namespace]_my_file_test.cpp`: contains Boost.Test tests

## Quality guidelines

These are all checked by Travis CI:

 * All tests pass cleanly/silently
 * Code coverage above 95%
 * OCLint must be clean

## Schedule

 * 2016-09-05: First day `jobo` and `kewe`
 * 2016-12-06: Midterm progress meeting `jobo`
 * 2016-01-03: ?Midterm progress meeting `kewe`
 * 2017-02-10: Final day `jobo`
 * 2017-?03-??: Final day `kewe`

## Articles 

The article(s)-in-progress are put in private repsoitories:

 * [ribi](https://github.com/richelbilderbeek/distancer_article)
