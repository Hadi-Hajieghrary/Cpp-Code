#include "ml_perceptron.hpp"




MultiLayerPerceptron::MultiLayerPerceptron(const int input_dimention, const int number_of_layers, 
                                           const int* number_of_perceptrons_in_layers , Perceptron** perceptrons)
    :input_dimention_{input_dimention}, number_of_layers_{number_of_layers}, 
     number_of_perceptrons_in_layers_{number_of_perceptrons_in_layers}
    {
        perceptrons_ = perceptrons;
    }


void MultiLayerPerceptron::initializePerceptrons(const int input_dimention, const int number_of_layers, 
                                                    const int* number_of_perceptrons_in_layers,
                                                    double**  biases,
                                                    std::function<double(double)>** activation_functions,
                                                    Perceptron**& perceptrons)
   {
        perceptrons = new Perceptron*[number_of_layers];

        // First Hiden Layer
        perceptrons[0] = new Perceptron[number_of_perceptrons_in_layers[0]];
        for(int j = 0; j<number_of_perceptrons_in_layers[0]; ++j){
            perceptrons[0][j].reScaleInput(input_dimention);
            perceptrons[0][j].setBias(biases[0][j]);
            std::cout<<&activation_functions[0][j]<<std::endl;
            perceptrons[0][j].setActivationFunction(activation_functions[0][j]);
            perceptrons[0][j].randomInitializeWeights();
        }
        for(size_t i = 1; i<number_of_layers; ++i){
            perceptrons[i] = new Perceptron[number_of_perceptrons_in_layers[i]];
            for(int j = 0; j<number_of_perceptrons_in_layers[i]; ++j){
                perceptrons[i][j].reScaleInput(number_of_perceptrons_in_layers[i-1]);
                perceptrons[i][j].setBias(biases[i][j]);
                perceptrons[i][j].setActivationFunction(activation_functions[i][j]);
                perceptrons[i][j].randomInitializeWeights();
            }
        }
    }

Perceptron* MultiLayerPerceptron::getPerceptron(int layer, int row){
    return &(perceptrons_[layer][row]);
}

std::tuple<double**, size_t, size_t> MultiLayerPerceptron::getWeights(int layer){
    if(layer > this->input_dimention_){
        char msg[50]{0};
        sprintf(msg, "There are %d layer. %d is out of range!", this->input_dimention_, layer);
        throw std::out_of_range(msg);
    }
    size_t rows{0}, cols{0};
    if (layer == 0) cols = this->input_dimention_+1;
    else cols = this->number_of_perceptrons_in_layers_[layer-1]+1;
    rows = this->number_of_perceptrons_in_layers_[layer];
    double** result = new double*[rows];
    for(size_t r = 0; r < rows; ++r){
        result[r] = new double[cols];
    }
    // perceptrons_ indexed as [number of layer][perceptron at layer]
    for(size_t r = 0; r < rows; ++r){
        for(size_t c = 0; c < cols; ++c){
            result[r][c] = this->perceptrons_[layer][r].getWeight(c);
        }
    }
    return std::make_tuple(result,rows,cols);
}
