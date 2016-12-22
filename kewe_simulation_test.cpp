#include <iostream>
#include <boost/algorithm/string/split.hpp>

#include "kewe_individual.h"
#include "kewe_simulation.h"
#include "kewe_ses.h"
#include "kewe_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace kewe;

#pragma GCC diagnostic pop
