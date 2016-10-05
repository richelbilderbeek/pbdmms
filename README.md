# pbdmms

Some models:

 * BDM
 * Jaled et al, 2008
 * Van Doorn & Weissing, 2002

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

Per unit:

 * `my_file.h`: contains declarations
 * `my_file.cpp`: contains definitions
 * `my_file_test.cpp`: contains Boost.Test tests

 * jobo_main.cpp: run simulation from @JorikDeBoer
 * jobo_main_tests.cpp: run tests @JorikDeBoer
 * jobo.pro: @JorikDeBoer simulation project file
 * jobo_test.pro: @JorikDeBoer test project file
 * kewe_main.cpp: run simulation from @Lumphie
 * kewe_main.cpp_tests: run tests @Lumphie
 * kewe.pro: @Lumphie simulation project file
 * kewe_test.pro: @Lumphie test project file
 * ribi_main.cpp: run simulation from @richelbilderbeek
 * ribi_main_test.cpp: run tests @richelbilderbeek
 * ribi.pro: @richelbilderbeek simulation project file
 * ribi_test.pro: @richelbilderbeek test project file

## Namespaces

General namespace is `jkr` (pronounce 'joker'),
to be used for general things.

Specialized namespaces:

 * jkr::jobo: @JorikDeBoer
 * jkr::kewe: @Lumphie
 * jkr::ribi: @richebilderbeek

## Quality guidelines

These are all checked by Travis CI:

 * All tests pass cleanly/silently
 * Code coverage above 95%
 * OCLint must be clean


