#include "ml_perceptron.hpp"




MultiLayerPerceptron::MultiLayerPerceptron(const unsigned int input_dimention, const unsigned int number_of_layers, 
                                           const unsigned int* number_of_perceptrons_in_layers , Perceptron** perceptrons)
    :input_dimention_{input_dimention}, number_of_layers_{number_of_layers}, 
     number_of_perceptrons_in_layers_{number_of_perceptrons_in_layers}, perceptrons_{perceptrons}
    {
        outputs_ = new double*[number_of_layers];
        d_outputs_ = new double*[number_of_layers];
        for(unsigned int i = 0; i<number_of_layers; ++i){
            outputs_[i] = new double[number_of_perceptrons_in_layers[i]];
            d_outputs_[i] = new double[number_of_perceptrons_in_layers[i]];
            for(unsigned int j = 0; j<number_of_perceptrons_in_layers[i]; ++j){
                outputs_[i][j] = 0.0;
                d_outputs_[i][j] = 0.0;
            }
        }
    }


void MultiLayerPerceptron::initializePerceptrons(const unsigned int input_dimention, const unsigned int number_of_layers, 
                                                    const unsigned int* number_of_perceptrons_in_layers,
                                                    double** biases,
                                                    func_ptr_t activation_function,
                                                    Perceptron**& perceptrons)
   {
        perceptrons = new Perceptron*[number_of_layers];

        // First Hiden Layer
        perceptrons[0] = new Perceptron[number_of_perceptrons_in_layers[0]];
        for(int j = 0; j<number_of_perceptrons_in_layers[0]; ++j){
            perceptrons[0][j].reScaleInput(input_dimention);
            perceptrons[0][j].setBias(biases[0][j]);
            perceptrons[0][j].setActivationFunction(activation_function);
            perceptrons[0][j].randomInitializeWeights();
        }
        for(unsigned int i = 1; i<number_of_layers; ++i){
            perceptrons[i] = new Perceptron[number_of_perceptrons_in_layers[i]];
            for(int j = 0; j<number_of_perceptrons_in_layers[i]; ++j){
                perceptrons[i][j].reScaleInput(number_of_perceptrons_in_layers[i-1]);
                perceptrons[i][j].setBias(biases[i][j]);
                perceptrons[i][j].setActivationFunction(activation_function);
                perceptrons[i][j].randomInitializeWeights();
            }
        }
    }

void MultiLayerPerceptron::initializePerceptrons(const unsigned int input_dimention, const unsigned int number_of_layers, 
                                                    const unsigned int* number_of_perceptrons_in_layers,
                                                    double** biases,
                                                    func_ptr_t** activation_functions,
                                                    Perceptron**& perceptrons)
   {
        perceptrons = new Perceptron*[number_of_layers];

        // First Hiden Layer
        perceptrons[0] = new Perceptron[number_of_perceptrons_in_layers[0]];
        for(int j = 0; j<number_of_perceptrons_in_layers[0]; ++j){
            perceptrons[0][j].reScaleInput(input_dimention);
            perceptrons[0][j].setBias(biases[0][j]);
            perceptrons[0][j].setActivationFunction(activation_functions[0][j]);
            perceptrons[0][j].randomInitializeWeights();
        }
        for(unsigned int i = 1; i<number_of_layers; ++i){
            perceptrons[i] = new Perceptron[number_of_perceptrons_in_layers[i]];
            for(int j = 0; j<number_of_perceptrons_in_layers[i]; ++j){
                perceptrons[i][j].reScaleInput(number_of_perceptrons_in_layers[i-1]);
                perceptrons[i][j].setBias(biases[i][j]);
                perceptrons[i][j].setActivationFunction(activation_functions[i][j]);
                perceptrons[i][j].randomInitializeWeights();
            }
        }
    }



Perceptron* MultiLayerPerceptron::getPerceptron(int layer, int row)
    {
        return &(perceptrons_[layer][row]);
    }

std::tuple<double**, unsigned int, unsigned int> MultiLayerPerceptron::getWeights(int layer)
    {
        if(layer > number_of_layers_){
            char msg[50]{0};
            sprintf(msg, "There are %d layer. %d is out of range!", number_of_layers_, layer);
            throw std::out_of_range(msg);
        }
        unsigned int rows{0}, cols{0};
        if (layer == 0) cols = this->input_dimention_+1;
        else cols = this->number_of_perceptrons_in_layers_[layer-1]+1;
        rows = this->number_of_perceptrons_in_layers_[layer];
        double** result = new double*[rows];
        for(unsigned int r = 0; r < rows; ++r){
            result[r] = new double[cols];
        }
        // perceptrons_ indexed as [number of layer][perceptron at layer]
        for(unsigned int r = 0; r < rows; ++r){
            for(unsigned int c = 0; c < cols; ++c){
                result[r][c] = this->perceptrons_[layer][r].getWeight(c);
            }
        }
        return std::make_tuple(result,rows,cols);
    }


void MultiLayerPerceptron::runOneLayer(unsigned int layer, double* inputs)
    {
        for(int i = 0; i<number_of_perceptrons_in_layers_[layer]; ++i){
            const double* W = perceptrons_[layer][i].getWeights();
            printf("\nWeights of the Perceptron %d in Layer %d: %lf %lf %lf\n",i, (int)layer, W[0],W[1],W[2]);
            perceptrons_[layer][i].run(inputs);
            outputs_[layer][i] = perceptrons_[layer][i].getOutput();
            d_outputs_[layer][i] = perceptrons_[layer][i].getDOutput();

        }
    }

double* MultiLayerPerceptron::run(double* inputs)
    {
        runOneLayer(0, inputs);
        for(unsigned int l = 1; l< number_of_layers_; ++l){
            runOneLayer(l, outputs_[l-1]);
        }
        double* result = new double[number_of_perceptrons_in_layers_[number_of_layers_-1]];
        for(unsigned int j = 0; j< number_of_layers_-1; ++j){
            result[j] = 0.0;
        }
        for(unsigned int i = 0; i<number_of_perceptrons_in_layers_[number_of_layers_-1]; ++i){
            result[i] = outputs_[number_of_layers_-1][i];
        }
        return result;
    }

std::tuple<double*, unsigned int> MultiLayerPerceptron::getOutputs(int layer)
    {
        unsigned int number_of_outputs;
        if (layer<number_of_layers_){
            number_of_outputs = number_of_perceptrons_in_layers_[layer];
        }else{
            char msg[50]{0};
                sprintf(msg, "There are %d layer. %d is out of range!", number_of_layers_, layer);
                throw std::out_of_range(msg);
        }
        double* outputs = new double[number_of_outputs];
        for(unsigned int i{0}; i<number_of_outputs; ++i){
            outputs[i] = outputs_[layer][i];
        }
        return std::make_tuple(outputs, number_of_outputs);
    }

std::tuple<double*, unsigned int> MultiLayerPerceptron::getOutputs()
    {
        auto result = getOutputs(this->number_of_layers_ - 1);
        return result;
    }


void MultiLayerPerceptron::calculateErrorTerms(double* reference_outputs)
    { 
        double* outputs;

        error_terms_ = new double*[number_of_layers_];
        for(unsigned int i{0}; i<number_of_layers_; ++i){
            error_terms_[i] = new double[number_of_perceptrons_in_layers_[i]];
            for(unsigned int j{0}; j<number_of_perceptrons_in_layers_[i]; ++j){
                error_terms_[i][j] = 0.0;
            }
        }
        
        for(unsigned int j{0}; j<number_of_perceptrons_in_layers_[number_of_layers_-1]; ++j){
            error_terms_[number_of_layers_-1][j] = d_outputs_[number_of_layers_ - 1][j] *
                                                    (reference_outputs[j] - outputs_[number_of_layers_ - 1][j]);
        }

        for(unsigned int i{number_of_layers_-2}; i>=0; --i){
            for(unsigned int j{0}; j<number_of_perceptrons_in_layers_[i]; ++j){
                
                for(unsigned int k = 0; k < number_of_perceptrons_in_layers_[i+1]; ++k){
                    error_terms_[i][j] += perceptrons_[i+1][k].getWeight(j)*error_terms_[i+1][k];
                }
                error_terms_[i][j] += d_outputs_[i][j];    
            }
        }
    }


void MultiLayerPerceptron::adjustWeights(double learning_rate)
    {
        for(unsigned int l{0}; l<number_of_layers_; ++l){
            for(unsigned int p{0}; p<number_of_perceptrons_in_layers_[l]; ++p){
                for(unsigned int i{0}; i<number_of_perceptrons_in_layers_[l-1]+1; ++i){
                    double delta = learning_rate * error_terms_[l][p]*outputs_[l-1][i];
                    perceptrons_[l][p].setWeight(i, perceptrons_[l][p].getWeight(i) + delta);
                }
            }
        }
    }









