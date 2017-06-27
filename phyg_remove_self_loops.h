#ifndef PHYG_REMOVE_SELF_LOOPS_H
#define PHYG_REMOVE_SELF_LOOPS_H

#include "remove_self_loops.h"

namespace phyg {

template <class graph>
void remove_self_loops(graph& g)
{
  ::remove_self_loops(g);
}

} //~namespace phyg

#endif // PHYG_REMOVE_SELF_LOOPS_H
