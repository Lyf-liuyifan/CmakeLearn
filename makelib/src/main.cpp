#include <iostream>
#include "arithmetic.h"

int main(){
    double a = 12.0;
    double b = 3.0;

    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "a + b = " << arithmetic::add(a, b) << std::endl;
    std::cout << "a - b = " << arithmetic::subtract(a, b) << std::endl;
    std::cout << "a * b = " << arithmetic::multiply(a, b) << std::endl;

    try{
        std::cout << "a / b = " << arithmetic::divide(a, b) << std::endl;
    }catch(const std::exception& ex){
        std::cerr << "Divide error: " << ex.what() << std::endl;
    }

    return 0;
}