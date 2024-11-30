#include "PP_CellularAutomata.hpp"
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstdlib>
#include <map>
#include <string>
#include <map>
#include <filesystem>

#include <omp.h>

int main(int argc, char* argv[])
{
    setlocale(LC_CTYPE, "Russian");
    int _num_threads = omp_get_max_threads();
    int _width = 101;
    int _height = 101;

    for (; _width * _height < 251002; _width += 50, _height += 50) {
        std::string test_worlds = ".\\tests\\world_" + std::to_string(_width) + "_" + std::to_string(_height) + ".txt";
        for (int i = 1; i <= _num_threads; i++) {
            omp_set_num_threads(i);
            std::string time_graph_path = ".\\tests\\time_graph_wd" + std::to_string(_width) + "_ht" + std::to_string(_height)+ "_trd" + std::to_string(i) + ".txt";

            GameOfLife world(_width, _height);

            world.InitWorld(test_worlds);
            world.StartTest(time_graph_path);
        }
    }

    return EXIT_SUCCESS;
}