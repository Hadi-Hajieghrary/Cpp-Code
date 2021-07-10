#include "perceptron.hpp"
#include "ml_perceptron.hpp"
#include <iostream>
#include <vector>

#include "TAxis.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TApplication.h"
#include "TStyle.h"
#include "TPad.h"
#include "TROOT.h"
#include "TColor.h"
#include "TGFrame.h"
#include "TVirtualPad.h"

#include "mnist/mnist_reader.hpp"


void Print(uint8_t* vector, const unsigned int row, const char* title = "") {
    std::cout<<"\n***** "<<title<<" *****"<<std::endl;
    for(unsigned int i = 0; i < row; ++i){
        std::cout<<static_cast<int>(vector[i])<<" ";
    }
    std::cout<<"===== "<<title<<" =====\n"<<std::endl;
}

void Print(double** matrix, const unsigned int row, const unsigned int col, const char* title = "") {
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

template <typename T>
void PlotVector(std::vector<T> arr){
    
	TCanvas *canv_err = new TCanvas("canv_err","Output Error",200,10,700,500);
	canv_err->SetFillColor(42);
	canv_err->SetGrid();

	const int n = arr.size();
	double* x = new double[n];
    double* y = arr.data();
	for (int i=0;i<n;i++) {
		x[i] = i;
	}
    
	TGraph *graph_err = new TGraph(n,x,y);
	graph_err->SetLineColor(2);
	graph_err->SetLineWidth(4);
	graph_err->SetMarkerColor(4);
	graph_err->SetMarkerStyle(21);
	graph_err->SetTitle("Error Plot");
	graph_err->GetXaxis()->SetTitle("Sample");
	graph_err->GetYaxis()->SetTitle("Error");
	graph_err->Draw("ACP");
    
	canv_err->Update();
	canv_err->Modified();
	canv_err->Connect("Closed()", "TApplication", gApplication, "Terminate()");
    
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
    Print(inputs, input_dimention, "Inputs");
    
    constexpr int number_of_itteration = 100;
    std::vector<double> residual_err;
    residual_err.reserve(number_of_itteration);

    for(int itr{0}; itr<number_of_itteration; ++itr){
        std::cout<<"Iteration: "<<itr<<std::endl;
        double* output = mlp.run(inputs);
        residual_err.push_back(std::abs(*output - *reference_outputs));
        for(unsigned int i{0}; i<number_of_layers; ++i){
            double* outputs{nullptr};
            unsigned int number_of_outputs{0};
            std::tie(outputs, number_of_outputs) = mlp.getOutputs(i);
            char* title = new char[30];
            sprintf(title, "Outputs of Layer %d", (int)i);
            Print(outputs, number_of_outputs, title );
            
        }
        mlp.adjustWeights(reference_outputs,1.0);
    }


    PlotVector(residual_err);


}


void Test_TrainingMLP_MNIST(){

    //load the dataset
    mnist::MNIST_dataset<std::vector, std::vector<uint8_t>, uint8_t> dataset =
                    mnist::read_dataset<std::vector, std::vector, uint8_t, uint8_t>(MNIST_DATA_LOCATION);
    
    // Set up the Network
    const unsigned int input_dimention = 784;
    const unsigned int number_of_layers = 3;
    unsigned int number_of_perceptrons_in_layers[number_of_layers]{50,10,1};
    auto activation_function{sigmoid};
    Perceptron** perceptrons;
    double bias = 1.0;

    MultiLayerPerceptron::initializePerceptrons(input_dimention, number_of_layers, 
                                                number_of_perceptrons_in_layers,
                                                bias,
                                                activation_function,
                                                perceptrons);

    MultiLayerPerceptron mlp(input_dimention, number_of_layers, number_of_perceptrons_in_layers , perceptrons);


    constexpr int number_of_itteration = 10;
    std::vector<double> residual_err;
    residual_err.reserve(number_of_itteration);

    const size_t trining_data_number = dataset.training_images.size();

    for(int itr{0}; itr<number_of_itteration; ++itr){
        std::cout<<"Iteration: "<<itr<<std::endl;
        residual_err[itr] = 0.0;
        for(size_t n{0}; n<trining_data_number; ++n){
            // TODO - Make the MLP/Perceptron a Template Classs
            // TODO - Memory management - MLP should release memory 
            std::vector<double> input_vec(dataset.training_images[n].begin(), dataset.training_images[n].end());
            double* output = mlp.run(input_vec.data());
            double reference_output = static_cast<double>(dataset.training_labels[n]);
            mlp.adjustWeights(&reference_output, 0.1);
            residual_err[itr]+=std::abs(*output - reference_output);
        }
        std::cout<<residual_err[itr]<<' '<<std::endl;
    }
}


int main(int argc, char** argv){

    
    //mnist::MNIST_dataset<std::vector, std::vector<uint8_t>, uint8_t> dataset =
    //                mnist::read_dataset<std::vector, std::vector, uint8_t, uint8_t>(MNIST_DATA_LOCATION);
    // 
    /*
    std::cout << "Nbr of training images = " << dataset.training_images.size() << std::endl;
    std::cout << "Nbr of training labels = " << dataset.training_labels.size() << std::endl;
    std::cout << "Nbr of test images = " << dataset.test_images.size() << std::endl;
    std::cout << "Nbr of test labels = " << dataset.test_labels.size() << std::endl;
    

    std::cout<<dataset.training_images[0].size()<<std::endl;
    std::cout<<(uint)dataset.training_labels[1]<<std::endl;
    std::cout<<(uint)dataset.training_labels[2]<<std::endl;
    std::cout<<(uint)dataset.training_labels[3]<<std::endl;
    std::cout<<(uint)dataset.training_labels[4]<<std::endl;
    */
    Test_TrainingMLP_MNIST();

    /*
    TApplication theApp("App", &argc, argv);
	if (gROOT->IsBatch()) {
	    fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
	    return 1;
	}
    

	//Test_Perceptron();
    //Test_MLPerceptron();
    Test_TrainingMLP();

	theApp.Run();
    */
    return 0;
}