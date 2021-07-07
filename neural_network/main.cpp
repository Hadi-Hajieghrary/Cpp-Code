#include "perceptron.hpp"
#include "ml_perceptron.hpp"
#include <iostream>

void Print(double** matrix, const unsigned int row, const unsigned int col, const char* title) {
    std::cout<<"\n***** "<<title<<" *****"<<std::endl;
    for(unsigned int i = 0; i < row; ++i){
        for(unsigned int j = 0; j < col; ++j){
            std::cout<<matrix[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<"===== "<<title<<" =====\n"<<std::endl;
}

void Print(double* vector, const unsigned int row, const char* title) {
    std::cout<<"\n***** "<<title<<" *****"<<std::endl;
    for(unsigned int i = 0; i < row; ++i){
        std::cout<<vector[i]<<"\n";
    }
    std::cout<<"===== "<<title<<" =====\n"<<std::endl;
}


void Test_Perceptron(){

    constexpr unsigned int number_of_inputs{2};
    Perceptron* p = new Perceptron(number_of_inputs);
    double inp[number_of_inputs]{0.0};
    double W[number_of_inputs+1]{10,10,-15};
    p->setWeights(W);
    
    printf("*** Testing The Perceptron Class ***\n");
        inp[0] = 0; inp[1] = 0;
        printf("%d AND %d = %f\n", 0, 0, p->run(inp));
        inp[0] = 1; inp[1] = 0;
        printf("%d AND %d = %f\n", 1, 0, p->run(inp));
        inp[0] = 0; inp[1] = 1;
        printf("%d AND %d = %f\n", 0, 1, p->run(inp));
        inp[0] = 1; inp[1] = 1;
        printf("%d AND %d = %f\n", 1, 1, p->run(inp));
    printf("*** END ***\n");

}


void Test_MLPerceptron(){
    typedef double(*func_ptr_t)(double);
    printf("*** Testing The MLP Class ***\n");
    unsigned int input_dimention = 4;
    unsigned int number_of_layers = 4;
    unsigned int number_of_perceptrons_in_layers[number_of_layers]{2,5,3,2};
    Perceptron** perceptrons;
    double** biases = new double*[number_of_layers];
    
    func_ptr_t** activation_functions = new func_ptr_t*[number_of_layers];
    for(unsigned int i{0}; i<number_of_layers; ++i){
        activation_functions[i] = new func_ptr_t[number_of_perceptrons_in_layers[i]];
        for(unsigned int j{0}; j< number_of_perceptrons_in_layers[i]; ++j){
            activation_functions[i][j] = sigmoid;
        }
    }

    for (unsigned int i = 0; i<number_of_layers; ++i){
        biases[i] = new double[number_of_perceptrons_in_layers[i]];
        for(unsigned int j = 0; j< number_of_perceptrons_in_layers[i]; ++j){
            biases[i][j] = 1.0;
        }
    }
    MultiLayerPerceptron::initializePerceptrons(input_dimention, number_of_layers, 
                                                number_of_perceptrons_in_layers,
                                                biases,
                                                activation_functions,
                                                perceptrons);
    MultiLayerPerceptron mlp(input_dimention, number_of_layers, 
                             number_of_perceptrons_in_layers , perceptrons);

    unsigned int rows{0}, cols{0};
    double** mtx{nullptr};
    std::tie (mtx, rows, cols) = mlp.getWeights(0);
    Print(mtx, rows, cols, "First Layer");

    std::tie (mtx, rows, cols) = mlp.getWeights(1);
    Print(mtx, rows, cols, "Second Layer");

    std::tie (mtx, rows, cols) = mlp.getWeights(2);
    Print(mtx, rows, cols, "Third Layer");

    std::tie (mtx, rows, cols) = mlp.getWeights(3);
    Print(mtx, rows, cols, "Fourth Layer");
    double* inputs = new double[input_dimention];
    for(unsigned int i = 0; i<input_dimention; ++i){
        inputs[i] = 1.0 - 2.0 * static_cast<double>(rand())/RAND_MAX;
    }
    double * output = mlp.run(inputs);
    Print(output, number_of_perceptrons_in_layers[number_of_layers-1], "Output");

    for(unsigned int i{0}; i<number_of_layers; ++i){
        double* outputs;
        unsigned int number_of_outputs;
        std::tie(outputs, number_of_outputs) = mlp.getOutputs(i);
        char* title = new char[30];
        sprintf(title, "Outputs of Layer %d", (int)i);
        Print(outputs, number_of_outputs, title );
    }
    
    printf("\n*** Creating XOR Gate ***\n");
    
    for(int i{0}; i<number_of_layers; ++i){
        delete[] perceptrons[i];
        delete[] biases[i];
    }
    delete[] perceptrons;
    delete[] biases;

    input_dimention = 2;
    number_of_layers = 2;
    number_of_perceptrons_in_layers[0] = 2;
    number_of_perceptrons_in_layers[1] = 1;
    biases = new double*[number_of_layers];
    for (unsigned int i = 0; i<number_of_layers; ++i){
        biases[i] = new double[number_of_perceptrons_in_layers[i]];
        for(unsigned int j = 0; j< number_of_perceptrons_in_layers[i]; ++j){
            biases[i][j] = 1.0;
        }
    }
    for (unsigned int i = 0; i<number_of_layers; ++i){
        for (unsigned int j = 0; j<number_of_perceptrons_in_layers[i]; ++j){
            std::cout<< biases[i][j]<<' ';
        }
        std::cout<<std::endl;
    }
    MultiLayerPerceptron::initializePerceptrons(input_dimention, number_of_layers, 
                                                number_of_perceptrons_in_layers,
                                                biases,
                                                activation_functions[0][0],
                                                perceptrons);
    // First Layer
    perceptrons[0][0].setWeight(0,-10);
    perceptrons[0][0].setWeight(1,-10);
    perceptrons[0][0].setWeight(2,15);
    perceptrons[0][1].setWeight(0,15);
    perceptrons[0][1].setWeight(1,15);
    perceptrons[0][1].setWeight(2,-10);
    // Second Layer   
    perceptrons[1][0].setWeight(0,10);
    perceptrons[1][0].setWeight(1,10);
    perceptrons[1][0].setWeight(2,-15);
    MultiLayerPerceptron mlp_xor(input_dimention, number_of_layers, 
                             number_of_perceptrons_in_layers , perceptrons);
    
    double inp[input_dimention]{0.0};
    inp[0] = 0; inp[1] = 0;
    double* layer_output{nullptr};
    unsigned int dim{0};
    printf("\n%d XOR %d = %f\n", 0, 0, mlp_xor.run(inp)[0]);
    std::tie(layer_output, dim) =  mlp_xor.getOutputs(0);
    printf("Outputs of the first Layer: %f %f\n", layer_output[0], layer_output[1]);
    
    inp[0] = 1; inp[1] = 0;
    printf("\n%d XOR %d = %f\n", 1, 0, mlp_xor.run(inp)[0]);
    std::tie(layer_output, dim) =  mlp_xor.getOutputs(0);
    printf("Outputs of the first Layer: %f %f\n", layer_output[0], layer_output[1]);
    
    inp[0] = 0; inp[1] = 1;
    printf("\n%d XOR %d = %f\n", 0, 1, mlp_xor.run(inp)[0]);
    std::tie(layer_output, dim) =  mlp_xor.getOutputs(0);
    printf("Outputs of the first Layer: %f %f\n", layer_output[0], layer_output[1]);
    
    inp[0] = 1; inp[1] = 1;
    printf("\n%d XOR %d = %f\n", 1, 1, mlp_xor.run(inp)[0]);
    std::tie(layer_output, dim) =  mlp_xor.getOutputs(0);
    printf("Outputs of the first Layer: %f %f\n", layer_output[0], layer_output[1]);
    
    printf("*** END ***\n");

}


void Test_TrainingMLP(){
    
        
    printf("*** Testing  Training The MLP ***\n");
    
    typedef double(*func_ptr_t)(double);
    unsigned int input_dimention = 2;
    unsigned int number_of_layers = 3;
    unsigned int number_of_perceptrons_in_layers[number_of_layers]{2,3,1};
    Perceptron** perceptrons;
    double** biases = new double*[number_of_layers];
    func_ptr_t** activation_functions = new func_ptr_t*[number_of_layers];
    for(unsigned int i{0}; i<number_of_layers; ++i){
        activation_functions[i] = new func_ptr_t[number_of_perceptrons_in_layers[i]];
        for(unsigned int j{0}; j< number_of_perceptrons_in_layers[i]; ++j){
            activation_functions[i][j] = sigmoid;
        }
    }
    for (unsigned int i = 0; i<number_of_layers; ++i){
        biases[i] = new double[number_of_perceptrons_in_layers[i]];
        for(unsigned int j = 0; j< number_of_perceptrons_in_layers[i]; ++j){
            biases[i][j] = 1.0;
        }
    }
    // Initialize the MLP with Random weights
    MultiLayerPerceptron::initializePerceptrons(input_dimention, number_of_layers, 
                                                number_of_perceptrons_in_layers,
                                                biases,
                                                activation_functions,
                                                perceptrons);
    MultiLayerPerceptron mlp(input_dimention, number_of_layers, 
                             number_of_perceptrons_in_layers , perceptrons);

    double* inputs = new double[input_dimention]{1,1};
    double* reference_outputs = new double[1]{0.0};
    double * output = mlp.run(inputs);
    Print(inputs, input_dimention, "Inputs");

    for(int itr{0}; itr<100; ++itr){
        std::cout<<"Iteration: "<<itr<<std::endl;
        for(unsigned int i{0}; i<number_of_layers; ++i){
            double* outputs;
            unsigned int number_of_outputs;
            std::tie(outputs, number_of_outputs) = mlp.getOutputs(i);
            char* title = new char[30];
            sprintf(title, "Outputs of Layer %d", (int)i);
            Print(outputs, number_of_outputs, title );
        }

        mlp.adjustWeights(reference_outputs,1.0);
        output = mlp.run(inputs);
    }




}


int main(int atgc, char** argv){

    //Test_Perceptron();
    //Test_MLPerceptron();
    Test_TrainingMLP();
    return 0;
}