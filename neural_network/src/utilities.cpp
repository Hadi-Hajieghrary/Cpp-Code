#include "utilities.hpp"


double sigmoid(double x){
    double result{0};
    result = 1.0/(1.0 + std::exp(-x));
    return result;
}