#include <iostream>
#include <fstream>
#include <random>
#include <locale.h>
#include <windows.h>
#ifdef _DEBUG
#undef _DEBUG
#include <omp.h>
#define _DEBUG
#else
#include <omp.h>
#endif
#ifdef _WIN32
#include <windows.h>
#define msleep(x) Sleep(x)
#else
#include <unistd.h>
#define msleep(x) usleep(x * 1000)
#endif

#define __THREADS__ 4

class GameOfLife {              // Класс игры "Жизнь"
    int _columns;               // Количество столбцов поля
    int _rows;                  // Количество строк поля
    bool** _field;              // Поле
    bool _is_stable = false;    // Одно из условий остановки игры - стабилизация состояния, т.е. когда поле прекращает меняться с течением времени
    int _generation;            // Номер поколения

    void FindNeighbors(signed int nb[][2], int x, int y)    // Поиск соседей
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

    int GetNeighbors(int x, int y) {                        // Подсчёт живых соседей
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

public:
    GameOfLife() {                                          // Конструктор по умолчанию
        _columns = 0;
        _rows = 0;
        _field = new bool*[0];
        _generation = 0;
    }

    GameOfLife(const GameOfLife& gol_table) {               // Конструктор копирования (не требуется, однако ООП)
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

    GameOfLife(int columns, int rows) {                     // Конструктор
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

    ~GameOfLife() {                                         // Деструктор
        for (int i = 0; i < _columns; i++)
            delete _field[i];
        delete[] _field;
    }

    void Start() {                                          // Запуск игры
        int population = GetPopulationCount();
        while (population > 0 && !_is_stable) {
            Print();
            NextGen();
            population = GetPopulationCount();
            _generation++;
        }
        if (population == 0)
            std::cout << "Колония вымерла, игра закончена";
        if (_is_stable)
            std::cout << "Колония завершила своё развитие, игра закончена";
    }

    void InitWorld(std::string file) {                      // Инициализация поля
        if (file.empty()) {
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
    }

    int GetPopulationCount()                                // Подсчёт числа живых клеток
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

    void NextGen() {
        bool** nextField = new bool*[_columns];
        for (int i = 0; i < _columns; i++)
            nextField[i] = new bool[_rows];
        _is_stable = true;
        omp_set_num_threads(__THREADS__);

#pragma omp parallel
        {
#pragma omp for nowait
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
                    if (_is_stable)
                        _is_stable &= _field[i][j] == nextField[i][j];
                }
            }
        }
            

        for (int i = 0; i < _columns; i++) {
            for (int j = 0; j < _rows; j++) {
                _field[i][j] = nextField[i][j];
            }
            delete nextField[i];
        }
        delete[] nextField;
    }

    void Print()
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
            std::cout << std::endl;
        }

        std::cout << "Число живых клеток: " << GetPopulationCount() << std::endl;
        std::cout << "Поколение " << _generation << std::endl;
    }
};

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

