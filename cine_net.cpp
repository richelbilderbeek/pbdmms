#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <opencv/cv.h>
#include <opencv/ml.h>
#include <opencv/highgui.h>
#include <vector>		// for vector related commands


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
    double fparam1 = 0;
    double fparam2 = 0;

    CvANN_MLP mlp;
    mlp.create(layers);

    mlp.create(layers, activateFunc, fparam1, fparam2);

    //int xy = mlp.get_layer_count();

    //mlp.get_weights(1);


/*
    cv::FileStorage fs("mlp.xml", cv::FileStorage::WRITE); // or xml
    mlp.write(*fs, "mlp"); // don't think too much about the deref, it casts to a FileNode
*/

// to change weights
    const vector<float> fweights(16, 100);

    {
        ofstream b_file( "mlp_1.yml");

        //Write all lines from template file to the output file
        for (const auto& line: file_to_vector("mlp_template.yml"))
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


mlp.load("mlp_1.yml","mlp");
//mlp.predict(inputs, response);


cv::Mat inputs = cv::Mat(1, 3, CV_32FC1);
inputs.col(0) = float(1000);
inputs.col(1) = float(1000);
inputs.col(2) = float(1000);

cv::Mat response;

mlp.predict(inputs, response);



    cout << "weight so far " << *mlp.get_weights(3) << endl
            << "predicted output so far " << response << endl
            << "weight vector " << fweights[2] << endl;

}





/*

// function to learn
int f(float x, float y, int equation) {
    switch(equation) {
    case 0:
        return y > sin(x*10) ? -1 : 1;
        break;
    case 1:
        return y > cos(x * 10) ? -1 : 1;
        break;
    case 2:
        return y > 2*x ? -1 : 1;
        break;
    case 3:
        return y > tan(x*10) ? -1 : 1;
        break;
    default:
        return y > cos(x*10) ? -1 : 1;
    }
}

// label data with equation
cv::Mat labelData(cv::Mat points, int equation) {
    cv::Mat labels(points.rows, 1, CV_32FC1);
    for(int i = 0; i < points.rows; i++) {
             float x = points.at<float>(i,0);
             float y = points.at<float>(i,1);
             labels.at<float>(i, 0) = f(x, y, equation);
        }
    return labels;
}


void mlp(cv::Mat& trainingData, cv::Mat& trainingClasses, cv::Mat& testData, cv::Mat& testClasses) {

    cv::Mat layers = cv::Mat(4, 1, CV_32SC1);

    layers.row(0) = cv::Scalar(2);
    layers.row(1) = cv::Scalar(10);
    layers.row(2) = cv::Scalar(15);
    layers.row(3) = cv::Scalar(1);

    CvANN_MLP mlp;
    CvANN_MLP_TrainParams params;
    CvTermCriteria criteria;
    criteria.max_iter = 100;
    criteria.epsilon = 0.00001f;
    criteria.type = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;
    params.train_method = CvANN_MLP_TrainParams::BACKPROP;
    params.bp_dw_scale = 0.05f;
    params.bp_moment_scale = 0.05f;
    params.term_crit = criteria;

    mlp.create(layers);

    // train
    mlp.train(trainingData, trainingClasses, cv::Mat(), cv::Mat(), params);

    cv::Mat response(1, 1, CV_32FC1);
    cv::Mat predicted(testClasses.rows, 1, CV_32F);
    for(int i = 0; i < testData.rows; i++) {
        cv::Mat response(1, 1, CV_32FC1);
        cv::Mat sample = testData.row(i);

        mlp.predict(sample, response);
        predicted.at<float>(i,0) = response.at<float>(0,0);

    }

    cout << "Accuracy_{MLP} = " << evaluate(predicted, testClasses) << endl;
    plot_binary(testData, predicted, "Predictions Backpropagation");
}

void knn(cv::Mat& trainingData, cv::Mat& trainingClasses, cv::Mat& testData, cv::Mat& testClasses, int K) {

    CvKNearest knn(trainingData, trainingClasses, cv::Mat(), false, K);
    cv::Mat predicted(testClasses.rows, 1, CV_32F);
    for(int i = 0; i < testData.rows; i++) {
            const cv::Mat sample = testData.row(i);
            predicted.at<float>(i,0) = knn.find_nearest(sample, K);
    }

    cout << "Accuracy_{KNN} = " << evaluate(predicted, testClasses) << endl;
    plot_binary(testData, predicted, "Predictions KNN");

}

void bayes(cv::Mat& trainingData, cv::Mat& trainingClasses, cv::Mat& testData, cv::Mat& testClasses) {

    CvNormalBayesClassifier bayes(trainingData, trainingClasses);
    cv::Mat predicted(testClasses.rows, 1, CV_32F);
    for (int i = 0; i < testData.rows; i++) {
        const cv::Mat sample = testData.row(i);
        predicted.at<float> (i, 0) = bayes.predict(sample);
    }

    cout << "Accuracy_{BAYES} = " << evaluate(predicted, testClasses) << endl;
    plot_binary(testData, predicted, "Predictions Bayes");

}

void decisiontree(cv::Mat& trainingData, cv::Mat& trainingClasses, cv::Mat& testData, cv::Mat& testClasses) {

    CvDTree dtree;
    cv::Mat var_type(3, 1, CV_8U);

    // define attributes as numerical
    var_type.at<unsigned int>(0,0) = CV_VAR_NUMERICAL;
    var_type.at<unsigned int>(0,1) = CV_VAR_NUMERICAL;
    // define output node as numerical
    var_type.at<unsigned int>(0,2) = CV_VAR_NUMERICAL;

    dtree.train(trainingData,CV_ROW_SAMPLE, trainingClasses, cv::Mat(), cv::Mat(), var_type, cv::Mat(), CvDTreeParams());
    cv::Mat predicted(testClasses.rows, 1, CV_32F);
    for (int i = 0; i < testData.rows; i++) {
        const cv::Mat sample = testData.row(i);
        CvDTreeNode* prediction = dtree.predict(sample);
        predicted.at<float> (i, 0) = prediction->value;
    }

    cout << "Accuracy_{TREE} = " << evaluate(predicted, testClasses) << endl;
    plot_binary(testData, predicted, "Predictions tree");

}


int main() {

    cv::Mat trainingData(numTrainingPoints, 2, CV_32FC1);
    cv::Mat testData(numTestPoints, 2, CV_32FC1);

    cv::randu(trainingData,0,1);
    cv::randu(testData,0,1);

    cv::Mat trainingClasses = labelData(trainingData, eq);
    cv::Mat testClasses = labelData(testData, eq);

    plot_binary(trainingData, trainingClasses, "Training Data");
    plot_binary(testData, testClasses, "Test Data");

    svm(trainingData, trainingClasses, testData, testClasses);
    mlp(trainingData, trainingClasses, testData, testClasses);
    knn(trainingData, trainingClasses, testData, testClasses, 3);
    bayes(trainingData, trainingClasses, testData, testClasses);
    decisiontree(trainingData, trainingClasses, testData, testClasses);

    cv::waitKey();

    return 0;
}
*/
