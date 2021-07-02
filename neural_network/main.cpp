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


void Test_Perceptron(){

    Perceptron* p = new Perceptron(2);
    p->setWeights({10,10,-15});
    
    printf("*** Testing The Perceptron Class ***\n");
        printf("%d AND %d = %f\n", 0, 0, p->run({0,0}));
        printf("%d AND %d = %f\n", 1, 0, p->run({1,0}));
        printf("%d AND %d = %f\n", 0, 1, p->run({0,1}));
        printf("%d AND %d = %f\n", 1, 1, p->run({1,1}));
    printf("*** END ***\n");

}


void Test_MLPerceptron(){

    printf("*** Testing The MLP Class ***\n");
    constexpr int input_dimention = 4;
    constexpr int number_of_layers = 4;
    constexpr int number_of_perceptrons_in_layers[number_of_layers]{2,5,3,2};
    Perceptron** perceptrons;
    double** biases = new double*[number_of_layers];
    std::function<double(double)>** activation_functions = new std::function<double(double)>*[number_of_layers];
    for (int i = 0; i<number_of_layers; ++i){
        biases[i] = new double[number_of_perceptrons_in_layers[i]]{1.0};
        activation_functions[i] = new std::function<double(double)>[number_of_perceptrons_in_layers[i]]{sigmoid};
    }
    MultiLayerPerceptron::initializePerceptrons(input_dimention, number_of_layers, 
                                                number_of_perceptrons_in_layers,
                                                biases,
                                                activation_functions,
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

}



double func(double arg){
    return arg;
}

int main(int atgc, char** argv){

    Test_Perceptron();
    Test_MLPerceptron();
 
    return 0;
}