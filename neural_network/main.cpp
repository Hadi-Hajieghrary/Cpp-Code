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
    constexpr int input_dimention = 4;
    constexpr int number_of_layers = 4;
    constexpr int number_of_perceptrons_in_layers[number_of_layers]{2,5,3,2};
    Perceptron** perceptrons;
    double** biases = new double*[number_of_layers];
    func_ptr_t activation_function{sigmoid};
    for (int i = 0; i<number_of_layers; ++i){
        biases[i] = new double[number_of_perceptrons_in_layers[i]]{1.0};
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

}



double func(double arg){
    return arg;
}

int main(int atgc, char** argv){

    Test_Perceptron();
    Test_MLPerceptron();
 
    return 0;
}