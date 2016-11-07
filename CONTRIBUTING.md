# Contributing

Awesome that you are reading this!

 * For questions, you can create an Issue
 * Code changes go via Pull Requests

## Branching policy

 * The `master` branch should always build successfully
 * Every collaborator has one (or more) branch(es)

## Submitting code

Submitted code should follow these quality guidelines:

 * All tests pass cleanly/silently
 * Code coverage above 95%
 * OCLint must be clean

These are all checked by Travis CI when submitting
a Pull Request. 


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

