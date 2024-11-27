#pragma once
#include <iostream>

class GameOfLife {              // Класс игры "Жизнь"
    int _columns;               // Количество столбцов поля
    int _rows;                  // Количество строк поля
    bool** _field;              // Поле
    bool _is_stable = false;    // Одно из условий остановки игры - стабилизация состояния, т.е. когда поле прекращает меняться с течением времени
    int _generation;            // Номер поколения

    void FindNeighbors(signed int nb[][2], int x, int y);   // Поиск соседей

    int GetNeighbors(int x, int y);                         // Подсчёт живых соседей

public:
    GameOfLife();                                           // Конструктор по умолчанию

    GameOfLife(const GameOfLife& gol_table);                // Конструктор копирования (не требуется, однако ООП)

    GameOfLife(int columns, int rows);                      // Конструктор

    ~GameOfLife();                                          // Деструктор

    void Start();                                           // Запуск игры

    void InitWorld(std::string file);                       // Инициализация поля

    int GetPopulationCount();                               // Подсчёт числа живых клеток

    void NextGen();                                         // Вычисление следующего поколения

    void Print();                                           // Вывод на экран
};