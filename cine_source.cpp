#include "cine_simulation.h"
#include <initializer_list>

int main()
{
    /*
    const parameters p = read_parameter_from_file(argv[1]);
    simulation s(p);
    s.run();
    const results r = s.get_results();
    save_to_file(r, "results.csv");
    */

    //const int edge = read_edge_from_file("parameter.txt");
    const int n_cols{10};
    const int n_rows{10};



    do_simulation(n_cols, n_rows);
}
