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
        
        const unsigned int input_dimention_{0};
        double* inputs_{nullptr};

        const unsigned int number_of_layers_{0};
        const unsigned int* number_of_perceptrons_in_layers_{nullptr};
        
        Perceptron** perceptrons_ = nullptr; 
        double** outputs_{nullptr};
        double** d_outputs_{nullptr}; 
        double** error_terms_{nullptr}; 

        typedef double(*func_ptr_t)(double);

    protected:
        
        void runOneLayer(unsigned int layer, double* inputs);

        void calculateErrorTerms(double* reference_outputs);
        
    public:

        static void initializePerceptrons(const unsigned int input_dimention, const unsigned int number_of_layers, 
                                            const unsigned int* number_of_perceptrons_in_layers,
                                            double bias,
                                            func_ptr_t activation_function,
                                            Perceptron**& perceptrons);

        static void initializePerceptrons(const unsigned int input_dimention, const unsigned int number_of_layers, 
                                            const unsigned int* number_of_perceptrons_in_layers,
                                            double** biases,
                                            func_ptr_t activation_function,
                                            Perceptron**& perceptrons);

        static void initializePerceptrons(const unsigned int input_dimention, const unsigned int number_of_layers, 
                                            const unsigned int* number_of_perceptrons_in_layers,
                                            double** biases,
                                            func_ptr_t** activation_functions,
                                            Perceptron**& perceptrons);


        MultiLayerPerceptron(const unsigned int input_dimention, const unsigned int number_of_layers, 
                             const unsigned int* number_of_perceptrons_in_layers , Perceptron** perceptrons);

        ~MultiLayerPerceptron() = default;

        // Run inputs through the Network and return the outputs
        double* run(double* inputs);


        Perceptron* getPerceptron(int layer, int row);

        std::tuple<double**, unsigned int, unsigned int> getWeights(int layer);
        
        // Get the current outputs of any layers.
        // The outputs are going to be set after each time an input run through the network
        std::tuple<double*, unsigned int> getOutputs(int layer);
        // Get the outputs of the last layer of the network - Outputs of the Network
        std::tuple<double*, unsigned int> getOutputs();

        // Perform Backpropagathin for just one set of Inputs - Outputs
        // This Function will not do forward feed. Befor running this th inputs
        // must have been run through the network
        void adjustWeights(double* reference_outputs, double learning_rate = 0.01);

};




#endif