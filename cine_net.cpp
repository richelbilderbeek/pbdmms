#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <opencv/cv.h>
#include <opencv/ml.h>
#include <opencv/highgui.h>
#include <vector>		// for vector related commands
#include <typeinfo>

#include "cine_simulation.h"


using namespace cv;
using namespace std;

bool is_regular_file(const std::string& filename) noexcept
{
  std::fstream f;
  f.open(filename.c_str(),std::ios::in);
  return f.is_open();
}

std::vector<std::string> file_to_vector(const std::string& filename)
{
  if(!is_regular_file(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "can only convert existing files, "
      << "filename supplied: '"
      << filename << "' was not found"
    ;
    throw std::invalid_argument(msg.str());
  }
  assert(is_regular_file(filename));
  std::vector<std::string> v;
  std::ifstream in{filename.c_str()};
  assert(in.is_open());
  //Without this test in release mode,
  //the program might run indefinitely when the file does not exists
  for (int i = 0; !in.eof(); ++i)
  {
    std::string s;
    std::getline(in,s);
    v.push_back(s); //Might throw std::bad_alloc
  }
  //Remove empty line at back of vector
  if (!v.empty() && v.back().empty()) v.pop_back();
  return v;
}


void create_ANN(){
    cv::Mat layers = cv::Mat(3, 1, CV_32SC1);
    layers.row(0) = cv::Scalar(3);
    layers.row(1) = cv::Scalar(3);
    layers.row(2) = cv::Scalar(1);
    int activateFunc=CvANN_MLP::SIGMOID_SYM;
    double fparam1 = 0;    double fparam2 = 0;
    CvANN_MLP mlp;
    mlp.create(layers, activateFunc, fparam1, fparam2);
    //int xy = mlp.get_layer_count();

// to change weights
    const vector<float> fweights(16, 100);
    {
        ofstream b_file( "cine_mlp_1.yml");

        //Write all lines from template file to the output file
        for (const auto& line: file_to_vector("cine_mlp_template.yml"))
        {
          b_file << line << '\n';
        }

        b_file << "   weights:" << endl << "      - [ ";
        for (int j = 0; j < 16; ++j){
            if (j == 11)
                b_file << fweights[j] << " ]" << endl << "      - [ ";
            if (j == 15)
                b_file << fweights[j] << " ]";
            else
                b_file << fweights[j] << ", ";
        }
    }

mlp.load("cine_mlp_1.yml","mlp");
cv::Mat inputs = cv::Mat(1, 3, CV_32FC1);
inputs.col(0) = float(1000);
inputs.col(1) = float(1000);
inputs.col(2) = float(1000);

cv::Mat response;

mlp.predict(inputs, response);

cv::Mat response_2 = cv::Mat(1, 1, CV_32FC1);
int type = 0;
response.assignTo(response_2, type);
    cout << "weight so far " << *mlp.get_weights(3) << endl
         << "predicted output so far " << response.at<float>(0,0) << endl
            << "type of response is " << typeid(response.at<float>(0,0)).name() << endl
            << "weight vector " << fweights[2] << endl;
}



