#ifndef CINE_ANN_H
#define CINE_ANN_H

#include <vector>		// for vector related commands

/// ANN node function activity to output
double activity_to_out(double node_act);


/// Function of intermediate layers
std::vector<double> layer_calc(const std::vector<int>& layer_nodes,
                            const std::vector<double>& weights,
                            std::vector<double>& input,
                            int& k,
                            const int& i);

/// Calculation of a feedforward network with architecture "layer_nodes",
/// inputs and weights
double network_calc (const std::vector<int>& layer_nodes,
                     std::vector<double> input,
                     const std::vector<double>& weights);



#endif // CINE_ANN_H
