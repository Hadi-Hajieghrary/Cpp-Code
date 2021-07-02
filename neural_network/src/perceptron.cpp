#include "perceptron.hpp"

Perceptron::Perceptron()
    :input_dimention_{1}, activation_function_{sigmoid}, 
        bias_{1.0}, weights_{VectorXd(input_dimention_+1)}
    {
        // Initialize weights with random [-1,1]
        randomInitializeWeights();

    }

void Perceptron::reScaleInput(int input_dimention)
    {
        input_dimention_ = input_dimention;
        weights_  = VectorXd(input_dimention_+1);
    }

Perceptron::Perceptron(int input_dimention, double bias, std::function<double(double)> activation_function)
    :input_dimention_{input_dimention}, activation_function_{activation_function}, 
        bias_{bias}, weights_{VectorXd(input_dimention_+1)}
    {
        // Initialize weights with random [-1,1]
        randomInitializeWeights();

    }

void Perceptron::randomInitializeWeights()
    {
        weights_ = VectorXd::Random(this->input_dimention_ + 1);
    }

void Perceptron::setWeights(VectorXd& weights)
    {
        if(weights.rows() == this->input_dimention_ + 1){
            weights_ = weights;
        }else{
            throw Dimension_MisMatch(weights.rows(), this->input_dimention_ + 1);
        }
    }

void Perceptron::setWeights(VectorXd&& weights){
    if(weights.rows() == this->input_dimention_ + 1){
        weights_ = weights;
    }else{
        throw Dimension_MisMatch(weights.rows(), this->input_dimention_ + 1);
    }
}

void Perceptron::setWeight(size_t index, double value){
    if(index < this->input_dimention_ + 1){
        weights_(index) = value;
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

void Perceptron::setActivationFunction(std::function<double(double)> activation_function)
    {
        activation_function_ = activation_function;
    }

double Perceptron::run(VectorXd inputs)
    {
        
        double result{0};
        if(inputs.rows() == this->input_dimention_){
            inputs.conservativeResize(this->input_dimention_ + 1);
            inputs(this->input_dimention_) = bias_;
            result = inputs.dot(weights_);
        }else{
            throw Dimension_MisMatch(inputs.rows(), this->input_dimention_);
        }
        
        result = activation_function_(result);
        output_ = result;
        return result;
    }


VectorXd Perceptron::getWeights() const{
    VectorXd result{weights_}; 
    return result;
}

double Perceptron::getWeight(size_t index) const{
    double result;
    if(index < this->input_dimention_ + 1){
        result = weights_(index);
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