#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

typedef std::vector<std::vector<int> > Matrix;

void parse_verilog_file(std::string filename, Matrix& addr_mat, Matrix data_mat) {
    std::ifstream in(filename);
    std::string s;
    while(std::getline(in, s)) {
        std::cout << s << std::endl;
    }
}