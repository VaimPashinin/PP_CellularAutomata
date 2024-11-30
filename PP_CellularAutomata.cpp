#include "PP_CellularAutomata.hpp"
#include <iostream>
#include <random>
#include <fstream>
#include <chrono>
#include <locale.h>
#include <windows.h>

#include <omp.h>

void GameOfLife::FindNeighbors(signed int nb[][2], int x, int y)
{
    int k = 0;

    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i == x && j == y) {
                continue;
            }
            nb[k][0] = i;
            nb[k][1] = j;
            k++;
        }
    }
}

int GameOfLife::GetNeighbors(int x, int y) {
    int count = 0;

    signed int nb[8][2];
    signed int _x, _y;

    FindNeighbors(nb, x, y);

    for (int i = 0; i < 8; i++) {
        _x = nb[i][0];
        _y = nb[i][1];

        if (_x < 0 || _y < 0) {
            continue;
        }
        if (_x >= _columns || _y >= _rows) {
            continue;
        }

        if (_field[_x][_y]) {
            count++;
        }
    }

    return count;
}

GameOfLife::GameOfLife() {
    _columns = 0;
    _rows = 0;
    _field = new bool*[0];
    _generation = 0;
}

GameOfLife::GameOfLife(const GameOfLife& gol_table) {
    _columns = gol_table._columns;
    _rows = gol_table._rows;
    _field = new bool*[_columns];
    for (int i = 0; i < _columns; i++)
        _field[i] = new bool[_rows];
    _is_stable = gol_table._is_stable;

    for (int i = 0; i < _columns; i++)
        for (int j = 0; j < _rows; j++)
            _field[i][j] = gol_table._field[i][j];
    _generation = gol_table._generation;
}

GameOfLife::GameOfLife(int columns, int rows) {
    _columns = columns;
    _rows = rows;
    _field = new bool*[_columns];
    for (int i = 0; i < _columns; i++)
        _field[i] = new bool[_rows];

    for (int i = 0; i < _columns; i++)
        for (int j = 0; j < _rows; j++)
            _field[i][j] = false;
    _generation = 0;
}

GameOfLife::~GameOfLife() {
    for (int i = 0; i < _columns; i++)
        delete _field[i];
    delete[] _field;
}

void GameOfLife::Start() {
    int population = GetPopulationCount();
    Print();
    while (population > 0 && !_is_stable && _generation < 10000) {
        NextGen();
        population = GetPopulationCount();
        _generation++;
        Print();
    }
    if (population == 0)
        std::cout << "Колония вымерла, игра закончена";
}

void GameOfLife::StartTest(std::string filename) {
    int population = GetPopulationCount();
    auto start = std::chrono::high_resolution_clock::now();
    std::ofstream file;
    file.open(filename);

    while (population > 0 && !_is_stable && _generation < 10000) {
        NextGen();
        population = GetPopulationCount();
        _generation++;
        auto current = std::chrono::high_resolution_clock::now();
        double time = (current - start).count();
        file << _generation << " " << time << std::endl;
    }

    file.close();
}

void GameOfLife::InitWorld(std::string filename) {
    if (filename.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 10000);

        for (int i = 0; i < _columns; i++)
        {
            for (int j = 0; j < _rows; j++)
            {
                int num = dis(gen);
                if (num % 2 == 0)
                    _field[i][j] = true;
            }
        }

        return;
    }

    else {
        std::ifstream file;
        file.open(filename);
        bool is_empty = true;
        if (file.is_open()) {
            int x = 0;
            int y = 0;
            while (file >> x >> y) {
                if (is_empty) is_empty = false;
                _field[x][y] = true;
            }
        }
        file.close();

        if (is_empty) {
            std::ofstream file;
            file.open(filename);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 10000);

            for (int i = 0; i < _columns; i++)
            {
                for (int j = 0; j < _rows; j++)
                {
                    int num = dis(gen);
                    if (num % 2 == 0)
                    {
                        _field[i][j] = true;
                        file << i << " " << j << "\n";
                    }
                }
            }
            file.close();
        }
    }
}

int GameOfLife::GetPopulationCount()
{
    int count = 0;

    for (int i = 0; i < _columns; i++) {
        for (int j = 0; j < _rows; j++) {
            if (_field[i][j]) {
                count++;
            }
        }
    }

    return count;
}

void GameOfLife::NextGen() {
    bool** nextField = new bool*[_columns];
    for (int i = 0; i < _columns; i++)
        nextField[i] = new bool[_rows];
    _is_stable = true;

#pragma omp parallel for
    for (int i = 0; i < _columns; i++) {
        for (int j = 0; j < _rows; j++) {
            switch (this->GetNeighbors(i, j))
            {
            default: {
                nextField[i][j] = false;
                break;
            }
            case 2: {
                if (_field[i][j])
                    nextField[i][j] = true;
                else nextField[i][j] = false;
                break;
            }
            case 3: {
                nextField[i][j] = true;
                break;
            }
            }
        }
    }
       
    CheckStable(nextField);

    for (int i = 0; i < _columns; i++) {
        for (int j = 0; j < _rows; j++) {
            _field[i][j] = nextField[i][j];
        }
        delete nextField[i];
    }
    delete[] nextField;
}

void GameOfLife::CheckStable(bool** nextField) {
    for (int i = 0; i < _columns; i++)
        for (int j = 0; j < _rows; j++)
            _is_stable &= _field[i][j] == nextField[i][j];
}

void GameOfLife::Print()
{
    system("cls");
    for (int j = 0; j < _rows; j++) {
        for (int i = 0; i < _columns; i++) {
            if (_field[i][j]) {
                std::cout << '*';
            }
            else {
                std::cout << ' ';
            }
            std::cout << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "Число живых клеток: " << GetPopulationCount() << '\n';
    std::cout << "Поколение " << _generation;
}

