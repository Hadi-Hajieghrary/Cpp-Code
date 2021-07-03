#ifndef __ML_PERCEPTRON_
#define __ML_PERCEPTRON_

#include <cmath>
#include <random>
#include <iostream>
#include <thread>
#include <mutex>

#include "exceptions.hpp"
#include "utilities.hpp"
#include "perceptron.hpp"


class MultiLayerPerceptron{


    private:
        
        const int input_dimention_{0};

        const int number_of_layers_{0};
        const int* number_of_perceptrons_in_layers_{nullptr};
        
        Perceptron** perceptrons_ = nullptr; 
        double** outputs_ = nullptr; 

        typedef double(*func_ptr_t)(double);

    protected:
        
        void runOneLayer(size_t layer, double* inputs);

    public:

        static void initializePerceptrons(const int input_dimention, const int number_of_layers, 
                                            const int* number_of_perceptrons_in_layers,
                                            double** biases,
                                            func_ptr_t activation_function,
                                            Perceptron**& perceptrons);

        static void initializePerceptrons(const int input_dimention, const int number_of_layers, 
                                            const int* number_of_perceptrons_in_layers,
                                            double** biases,
                                            func_ptr_t** activation_functions,
                                            Perceptron**& perceptrons);


        MultiLayerPerceptron(const int input_dimention, const int number_of_layers, 
                             const int* number_of_perceptrons_in_layers , Perceptron** perceptrons);

        ~MultiLayerPerceptron() = default;

        double* run(double* inputs);

        Perceptron* getPerceptron(int layer, int row);

        std::tuple<double**, size_t, size_t> getWeights(int layer);
        std::tuple<double*, size_t> getOutputs(int layer);



};




#endif