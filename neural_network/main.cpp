#include "perceptron.hpp"
#include "ml_perceptron.hpp"
#include <iostream>

void Print(double** matrix, const size_t row, const size_t col, const char* title) {
    std::cout<<"\n***** "<<title<<" *****"<<std::endl;
    for(size_t i = 0; i < row; ++i){
        for(size_t j = 0; j < col; ++j){
            std::cout<<matrix[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<"===== "<<title<<" =====\n"<<std::endl;
}

void Print(double* vector, const size_t row, const char* title) {
    std::cout<<"\n***** "<<title<<" *****"<<std::endl;
    for(size_t i = 0; i < row; ++i){
        std::cout<<vector[i]<<"\n";
    }
    std::cout<<"===== "<<title<<" =====\n"<<std::endl;
}


void Test_Perceptron(){

    constexpr size_t number_of_inputs{2};
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
    int input_dimention = 4;
    int number_of_layers = 4;
    int number_of_perceptrons_in_layers[number_of_layers]{2,5,3,2};
    Perceptron** perceptrons;
    double** biases = new double*[number_of_layers];
    func_ptr_t activation_function{sigmoid};
    for (int i = 0; i<number_of_layers; ++i){
        biases[i] = new double[number_of_perceptrons_in_layers[i]];
        for(size_t j = 0; j< number_of_perceptrons_in_layers[i]; ++j){
            biases[i][j] = 1.0;
        }
    }
    MultiLayerPerceptron::initializePerceptrons(input_dimention, number_of_layers, 
                                                number_of_perceptrons_in_layers,
                                                biases,
                                                activation_function,
                                                perceptrons);
    MultiLayerPerceptron mlp(input_dimention, number_of_layers, 
                             number_of_perceptrons_in_layers , perceptrons);

    int rows{0}, cols{0};
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
    for(size_t i = 0; i<input_dimention; ++i){
        inputs[i] = 1.0 - 2.0 * static_cast<double>(rand())/RAND_MAX;
    }
    double * output = mlp.run(inputs);
    Print(output, number_of_perceptrons_in_layers[number_of_layers-1], "Output");

    for(size_t i{0}; i<number_of_layers; ++i){
        double* outputs;
        size_t number_of_outputs;
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
    for (int i = 0; i<number_of_layers; ++i){
        biases[i] = new double[number_of_perceptrons_in_layers[i]];
        for(size_t j = 0; j< number_of_perceptrons_in_layers[i]; ++j){
            biases[i][j] = 1.0;
        }
    }
    for (int i = 0; i<number_of_layers; ++i){
        for (int j = 0; j<number_of_perceptrons_in_layers[i]; ++j){
            std::cout<< biases[i][j]<<' ';
        }
        std::cout<<std::endl;
    }
    MultiLayerPerceptron::initializePerceptrons(input_dimention, number_of_layers, 
                                                number_of_perceptrons_in_layers,
                                                biases,
                                                activation_function,
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
    size_t dim{0};
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



double func(double arg){
    return arg;
}

int main(int atgc, char** argv){

    Test_Perceptron();
    Test_MLPerceptron();
 
    return 0;
}