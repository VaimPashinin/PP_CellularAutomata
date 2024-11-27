#include "PP_CellularAutomata.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char** argv[])
{
    setlocale(LC_CTYPE, "Russian");
    time_t timeElapsed = time(NULL);
    if (argc == 1) {
        int width = 41;
        int height = 41;
        GameOfLife world(width, height);

        world.InitWorld("");
        world.Start();
        std::ofstream out;
        out.open("Time.txt");
        out.close();
    }

    return EXIT_SUCCESS;
}