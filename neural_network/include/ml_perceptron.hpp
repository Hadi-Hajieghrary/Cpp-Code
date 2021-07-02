#ifndef __ML_PERCEPTRON_
#define __ML_PERCEPTRON_

#include <cmath>
#include <random>
#include <functional>
#include <algorithm>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

#include <Eigen/Dense>

#include "exceptions.hpp"
#include "utilities.hpp"
#include "perceptron.hpp"




using Eigen::VectorXd;
using Eigen::MatrixXd;

class MultiLayerPerceptron{


    private:
        
        const int input_dimention_{0};

        const int number_of_layers_{0};
        const int* number_of_perceptrons_in_layers_{nullptr};
        
        Perceptron** perceptrons_ = nullptr;  

    protected:
        void runOneLayer(size_t layer);

    public:

        static void initializePerceptrons(const int input_dimention, const int number_of_layers, 
                                            const int* number_of_perceptrons_in_layers,
                                            double** biases,
                                            std::function<double(double)>** activation_functions,
                                            Perceptron**& perceptrons);

        MultiLayerPerceptron(const int input_dimention, const int number_of_layers, 
                             const int* number_of_perceptrons_in_layers , Perceptron** perceptrons);

        ~MultiLayerPerceptron() = default;

        double run(VectorXd inputs);

        Perceptron* getPerceptron(int layer, int row);

        std::tuple<double**, size_t, size_t> getWeights(int layer);

};




#endif