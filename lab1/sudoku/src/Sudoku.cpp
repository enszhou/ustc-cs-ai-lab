#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <algorithm>
#include "Sudoku.h"
using namespace std;

Sudoku::Sudoku(string infile_path, bool is_X_sudoku)
{
    ifstream infile(infile_path);
    if (!infile)
    {
        cout << "file doesn't exist";
        exit(1);
    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            infile >> cells[i][j].value;
        }
    }
    infile.close();
    this->is_X_sudoku = is_X_sudoku;
    Init();
}

void Sudoku::Init()
{
    vector<Point> neighbors;
    int value_neighbor;
    num_null_cells = 0;

    // mrv = 10;
    // coord_mrv = {0, 0};

    // degree & MRV
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (cells[i][j].value == 0)
            {
                num_null_cells++;
                memset(cells[i][j].occupy_values, 0, sizeof(cells[i][j].occupy_values));
                GetNeighborIndex(i, j, neighbors);
                for (auto neighbor : neighbors)
                {
                    value_neighbor = cells[neighbor.row][neighbor.col].value;
                    if (value_neighbor == 0)
                        cells[i][j].degree++;
                    else
                    {
                        cells[i][j].occupy_values[value_neighbor]++;
                    }
                }
                for (int k = 1; k < 10; k++)
                {
                    if (cells[i][j].occupy_values[k] == 0)
                        cells[i][j].occupy_values[0]++;
                }
                // set the mrv
                // if (cells[i][j].occupy_values[0] < mrv || (cells[i][j].occupy_values[0] == mrv && cells[i][j].degree > cells[coord_mrv.row][coord_mrv.col].degree))
                // {
                //     mrv = cells[i][j].occupy_values[0];
                //     coord_mrv = {i, j};
                // }
            }
        }
    }
    SetMrv();
}

void Sudoku::GetNeighborIndex(int row, int col, vector<Point> &neighbors)
{
    int count = 0;
    if (row == col && col == 4 && is_X_sudoku)
        neighbors.resize(32);
    else if ((row == col || row + col == 8) && is_X_sudoku)
    {
        neighbors.resize(26);
        if (row % 3 == 1)
        {
            if (row == col)
            {
                neighbors[count++] = {row + 1, col - 1};
                neighbors[count++] = {row - 1, col + 1};
            }
            else
            {
                neighbors[count++] = {row + 1, col + 1};
                neighbors[count++] = {row - 1, col - 1};
            }
        }
        else
        {
            neighbors[count++] = {row + 1 * ((row % 3) * (-1) + 1), col + 2 * ((col % 3) * (-1) + 1)};
            neighbors[count++] = {row + 2 * ((row % 3) * (-1) + 1), col + 1 * ((col % 3) * (-1) + 1)};
        }
    }
    else
    {
        neighbors.resize(20);
        neighbors[count++] = {row - row % 3 + (row % 3 + 1) % 3, col - col % 3 + (col % 3 + 1) % 3};
        neighbors[count++] = {row - row % 3 + (row % 3 + 2) % 3, col - col % 3 + (col % 3 + 1) % 3};
        neighbors[count++] = {row - row % 3 + (row % 3 + 1) % 3, col - col % 3 + (col % 3 + 2) % 3};
        neighbors[count++] = {row - row % 3 + (row % 3 + 2) % 3, col - col % 3 + (col % 3 + 2) % 3};
    }
    for (int i = 0; i < 9; i++)
        if (i != row)
            neighbors[count++] = {i, col};
    for (int j = 0; j < 9; j++)
        if (j != col)
            neighbors[count++] = {row, j};
    if (row == col && is_X_sudoku)
        for (int k = 0; k < 9; k++)
            if (k != row)
                neighbors[count++] = {k, k};
    if (row + col == 8 && is_X_sudoku)
        for (int k = 0; k < 9; k++)
            if (k != row)
                neighbors[count++] = {k, 8 - k};
}

bool Sudoku::SetCell(int row, int col, int value)
{
    if (cells[row][col].value == 0)
        cells[row][col].value = value;
    else
        return false;
    num_null_cells--;
    // fresh neighbors' state
    vector<Point> neighbors;
    GetNeighborIndex(row, col, neighbors);
    for (auto neighbor : neighbors)
    {
        if (cells[neighbor.row][neighbor.col].value == 0)
        {
            cells[neighbor.row][neighbor.col].degree--;
            if (cells[neighbor.row][neighbor.col].occupy_values[value] == 0)
                cells[neighbor.row][neighbor.col].occupy_values[0]--;
            cells[neighbor.row][neighbor.col].occupy_values[value]++;
        }
    }
    SetMrv();
    return true;
}

bool Sudoku::ClearCell(int row, int col)
{
    int old_value = cells[row][col].value;
    if (old_value != 0)
        cells[row][col].value = 0;
    else
        return false;

    num_null_cells++;

    memset(cells[row][col].occupy_values, 0, sizeof(cells[row][col].occupy_values));

    vector<Point> neighbors;
    int value_neighbor;

    GetNeighborIndex(row, col, neighbors);

    for (auto neighbor : neighbors)
    {
        if (cells[neighbor.row][neighbor.col].value == 0) // fresh null neighbors' state & self degree
        {
            cells[row][col].degree++;

            cells[neighbor.row][neighbor.col].degree++;
            if (cells[neighbor.row][neighbor.col].occupy_values[old_value] == 1)
                cells[neighbor.row][neighbor.col].occupy_values[0]++;
            cells[neighbor.row][neighbor.col].occupy_values[old_value]--;
        }
        else // fresh self
        {
            value_neighbor = cells[neighbor.row][neighbor.col].value;
            cells[row][col].occupy_values[value_neighbor]++;
        }
    }

    for (int k = 1; k < 10; k++)
        if (cells[row][col].occupy_values[k] == 0)
            cells[row][col].occupy_values[0]++;

    SetMrv();

    return true;
}

void Sudoku::SetMrv()
{
    mrv = 10;
    coord_mrv = {0, 0};
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (cells[i][j].value == 0)
            {
                if (cells[i][j].occupy_values[0] < mrv || (cells[i][j].occupy_values[0] == mrv && cells[i][j].degree > cells[coord_mrv.row][coord_mrv.col].degree))
                {
                    mrv = cells[i][j].occupy_values[0];
                    coord_mrv = {i, j};
                }
            }
        }
    }
}

vector<int> Sudoku::GetAltenatives(int row, int col)
{
    vector<Point> neighbors;
    // vector<int> alternatives(cells[row][col].occupy_values[0]);
    // vector<int> num_reduce_value(cells[row][col].occupy_values[0], 0);
    map<int, int> alternatives; //{ alternatives : num_reduce_value }
    vector<int> ordered_alternatives;

    for (int i = 1; i < 10; i++)
    {
        if (cells[row][col].occupy_values[i] == 0)
            alternatives.insert({i, 0});
    }

    GetNeighborIndex(row, col, neighbors);
    for (auto neighbor : neighbors)
    {
        for (auto alternative : alternatives)
        {
            if (cells[neighbor.row][neighbor.col].occupy_values[alternative.first] == 0)
                alternative.second++;
        }
    }

    int size = alternatives.size();
    for (int i = 0; i < size; i++)
    {
        int min_key, min_value = 100;
        for (auto alternative : alternatives)
        {
            if (alternative.second < min_value)
            {
                min_key = alternative.first;
                min_value = alternative.second;
            }
        }
        ordered_alternatives.emplace_back(min_key);
        alternatives[min_key] = 101;
    }
    return ordered_alternatives;
}

bool Sudoku::ForwardCheck()
{
    if (mrv > 0)
        return true;
    else
        return false;
}