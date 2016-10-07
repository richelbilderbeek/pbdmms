#include "jobo_helper.h"
#include <iostream>
#include <stdexcept>

void hello_jobo()
{
   std::cout <<"hello world" << '\n';
}

int add(int a, int b)
{
   return a + b;
}

double divide(double a, double b)
{
    if(b==0.0){
      throw std::invalid_argument("can't divide by 0");
    }
    return a/b;
}

