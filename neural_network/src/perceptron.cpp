#include "perceptron.hpp"

Perceptron::Perceptron()
    :input_dimention_{1}, activation_function_{sigmoid}, 
        bias_{1.0}, weights_{new double[input_dimention_+1]}
    {
        // Initialize weights with random [-1,1]
        randomInitializeWeights();

    }

void Perceptron::reScaleInput(int input_dimention)
    {
        input_dimention_ = input_dimention;
        weights_  = new double[input_dimention_+1]{0};
    }

Perceptron::Perceptron(int input_dimention, double bias, double (*activation_function)(double))
    :input_dimention_{input_dimention}, activation_function_{activation_function}, 
        bias_{bias}, weights_{new double[input_dimention_+1]}
    {
        // Initialize weights with random [-1,1]
        randomInitializeWeights();

    }

void Perceptron::randomInitializeWeights()
    {
        std::random_device rd;
        std::mt19937 random_generator(rd()); 
        std::uniform_real_distribution<> uniform_distribution(-1.0, 1.0);
        for(int  i = 0; i< this->input_dimention_ + 1; i++){
            weights_[i] = uniform_distribution(random_generator);
        }
    }

void Perceptron::setWeights(const double* weights)
    {
        for(size_t i = 0; i<this->input_dimention_ + 1; ++i){
            weights_[i] = weights[i];
        }
    }


void Perceptron::setWeight(size_t index, double value){
    if(index < this->input_dimention_ + 1){
        weights_[index] = value;
    }else{
        char msg[50]{0};
        sprintf(msg, "There are %d weights. %d is out of range!", this->input_dimention_ + 1, (int)index);
        throw std::out_of_range(msg);
    }
}

void Perceptron::setBias(int bias)
    {
        bias_ = bias;
    }

void Perceptron::setActivationFunction(double (*activation_function)(double))
    {
        activation_function_ = activation_function;
    }

double Perceptron::run(double* inputs)
    {
        
        double result{bias_*weights_[input_dimention_]};
        for(size_t i{0}; i < input_dimention_; ++i){
            result += weights_[i]*inputs[i];
        }
        result = activation_function_(result);
        output_ = result;
        return result;
    }


const double* Perceptron::getWeights() const{
    return weights_;
}

double Perceptron::getWeight(size_t index) const{
    double result;
    if(index < this->input_dimention_ + 1){
        result = weights_[index];
    }else{
        char msg[50]{0};
        sprintf(msg, "There are %d weights. %d is out of range!", this->input_dimention_ + 1, (int)index);
        throw std::out_of_range(msg);
    }
    return result;
}

double Perceptron::getOutput() const{
    return output_;
}