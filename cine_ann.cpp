#include "cine_ann.h"
#include <vector>		// for vector related commands
#include <numeric>		//needed for accumulate
#include <cmath>        //Mathematical functions
#include <cassert>

using namespace std;



///ANN construction
/// convert node activity to node output through sigmoid function
double activity_to_out(double node_act){
    return 1/(1 + exp(-node_act)); //see page 36 NN&AB, a = 1; b = 0;
}

//layer function
vector<double> layer_calc(const vector<int>& layer_nodes,
                            const vector<double>& weights,
                            vector<double>& input,
                            int& k,
                            const int& i){

    vector<double> node_act(layer_nodes[i]);
    vector<double> output;     //initialize output transfer vector

    for (int g = 0; g < layer_nodes[i]; ++g){
        if (i == 0){            // TRUE for first layer/input
            node_act[g] = input[g];
        }
        else {
            //for all following layers
            for (int h = 0; h < layer_nodes[i-1]; h++){
                node_act[g] += input[g + layer_nodes[i] * h];
            }
        }

        //Add bias
        node_act[g] += weights[k] * (-1.0);
        ++k;

        if (static_cast<int>(layer_nodes.size()) == i+1){
            //TRUE for last layer
            output.push_back(activity_to_out(node_act[g]) * weights[k]);
            k++;
        }

        else {
            //for first and intermediate layers
            for (int j = 0; j < layer_nodes[i+1]; ++j){
                output.push_back(activity_to_out(node_act[g]) * weights[k]);
                k++;
                assert(k < static_cast<int>(weights.size()));
            }
        }
    }
    return output;
}

///network calculation
double network_calc (const vector<int>& layer_nodes,
                     vector<double> input,
                     const vector<double>& weights){

    int k = 0; // weight counter, incremented each time a weight is requested

    vector<double> output;              //initialize output vector

    //loop across layers
    for (int i = 0; i < static_cast<int>(layer_nodes.size()); i++){

        output = layer_calc(layer_nodes, weights, input, k, i);
        input = output;
    }
    return output[0];
}


