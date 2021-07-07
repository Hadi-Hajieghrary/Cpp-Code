#ifndef __PERCEPTRON_
#define __PERCEPTRON_

#include <cmath>
#include <random>
#include <iostream>
#include "exceptions.hpp"
#include "utilities.hpp"



class Perceptron{

    private:
        
        double* weights_;
        unsigned int input_dimention_{0};
        double bias_{0};
        double sum_of_synapses_{0};
        double output_{0};
        double d_output_{0};
        double (*activation_function_)(double);

    protected:
        
        
        
    public:

        Perceptron();
        Perceptron(unsigned int input_dimention, double bias = 1.0, double (*activation_function)(double) = sigmoid);

        ~Perceptron() = default;

        void randomInitializeWeights();
        void setWeights(const double* weights);
        void setWeight(unsigned int index, double value);

        void setBias(int bias);
        void setActivationFunction(double (*activation_function)(double));
        void reScaleInput(int input_dimention);

        double run(double* inputs);
  
        const double* getWeights() const;
        double getWeight(unsigned int index) const;

        int getBias() const;

        double getOutput() const;
        double getDOutput(double dx = 0.01);

};


#endif

