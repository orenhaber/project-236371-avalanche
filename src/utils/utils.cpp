
#define BOOST_LOG_DYN_LINK 1
#include "utils.h"
#include <iostream>

#include <fstream>


std::ofstream g_file ( std::string(getenv("MY_POD_NAME")) + "_res");

void print_log(std::string message){
    std::cout << message << std::endl;
}

void print_csv(std::string message){
    g_file << message << std::endl;
}

int CountSampleResults(std::map<std::string, color_t > &sample_results, color_t color) {
    int count = 0;
    for (auto sample: sample_results) {
        if (sample.second == color) {
            count++;
        }
    }
    return count;
}
