#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <unordered_map>
#include <algorithm>  

double reccomender(std::unordered_map<std::string, std::unordered_map<std::string, double> > &predizioniHybrid,
                std::unordered_map<std::string, std::unordered_map<std::string, double> > &tolti, std::ofstream &log
    );