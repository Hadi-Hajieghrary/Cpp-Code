#ifndef __PERCEPTRON_
#define __PERCEPTRON_

#include <cmath>
#include <random>
#include <functional>
#include <algorithm>
#include <Eigen/Dense>

#include <exceptions.hpp>
#include "utilities.hpp"

using Eigen::VectorXd;
using Eigen::MatrixXd;



class Perceptron{

    private:
        
        VectorXd weights_;
        int input_dimention_{0};
        double bias_{0};
        double output_{0};
        std::function<double(double)> activation_function_;

    protected:
        
        
        
    public:

        Perceptron();
        Perceptron(int input_dimention, double bias = 1.0, std::function<double(double)> activation_function = sigmoid);

        ~Perceptron() = default;

        void randomInitializeWeights();
        void setWeights(VectorXd& weights);
        void setWeights(VectorXd&& weights);
        void setWeight(size_t index, double value);

        void setBias(int bias);
        void setActivationFunction(std::function<double(double)> activation_function);
        void reScaleInput(int input_dimention);

        double run(VectorXd inputs);
  
        VectorXd getWeights() const;
        double getWeight(size_t index) const;

        double getOutput() const;

};


#endif

