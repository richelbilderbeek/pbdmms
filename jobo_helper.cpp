#include "jobo_helper.h"
#include <iostream>

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
        std::cout<<"can't divide by 0";
    }
    else{
    return a/b;
    }
}
