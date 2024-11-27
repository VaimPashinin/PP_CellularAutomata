#pragma once
#include <iostream>

class GameOfLife {              // ����� ���� "�����"
    int _columns;               // ���������� �������� ����
    int _rows;                  // ���������� ����� ����
    bool** _field;              // ����
    bool _is_stable = false;    // ���� �� ������� ��������� ���� - ������������ ���������, �.�. ����� ���� ���������� �������� � �������� �������
    int _generation;            // ����� ���������

    void FindNeighbors(signed int nb[][2], int x, int y);   // ����� �������

    int GetNeighbors(int x, int y);                         // ������� ����� �������

public:
    GameOfLife();                                           // ����������� �� ���������

    GameOfLife(const GameOfLife& gol_table);                // ����������� ����������� (�� ���������, ������ ���)

    GameOfLife(int columns, int rows);                      // �����������

    ~GameOfLife();                                          // ����������

    void Start();                                           // ������ ����

    void InitWorld(std::string file);                       // ������������� ����

    int GetPopulationCount();                               // ������� ����� ����� ������

    void NextGen();                                         // ���������� ���������� ���������

    void Print();                                           // ����� �� �����
};