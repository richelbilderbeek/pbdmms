//#include "jobo_helper.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cassert>
#include <exception>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <boost/graph/adjacency_list.hpp>
#include "jobo_simulation.h"
//#include "count_undirected_graph_connected_components.h"

void hello_jobo() noexcept
{
  std::cout <<"hello world" << '\n';
}

int add(int a, int b) noexcept
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


