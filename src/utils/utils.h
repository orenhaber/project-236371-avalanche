#ifndef TECH_PROJECT_UTILS_H
#define TECH_PROJECT_UTILS_H
#include <map>
#include <string>
typedef enum color {
    RED=0,
    BLUE=1
} color_t;
static const color_t colors[] = {RED, BLUE};
int CountSampleResults(std::map<std::string, color_t > &sample_results, color_t color);
void print_log(std::string message);
void print_csv(std::string message);
#endif //TECH_PROJECT_UTILS_H
