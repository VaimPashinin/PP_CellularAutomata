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

    void StartTest(std::string filename);                                       // ������ ������

    void InitWorld(std::string filename);                       // ������������� ����

    int GetPopulationCount();                               // ������� ����� ����� ������

    void NextGen();                                         // ���������� ���������� ���������

    void CheckStable(bool** nextField);               // �������� ������������ ����

    void Print();                                           // ����� �� �����
};