#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "Sudoku.h"

using namespace std;

int times_try = 0;

void OutputSudoku(Sudoku &sudoku, int (&color_mask)[9][9])
{
    for (int i = 0; i < 9; i++)
    {
        if (i == 3 || i == 6)
        {
            for (int j = 0; j < 9; j++)
            {
                cout << "--";
                if (j == 2 || j == 5)
                    cout << '|' << '-';
            }
            cout << endl;
        }
        for (int j = 0; j < 9; j++)
        {
            // if (sudoku.cells[i][j].value == 3)
            //     cout << "\033[31m" << sudoku.cells[i][j].value << "\033[0m" << ' ';
            // else if (sudoku.cells[i][j].value == 8)
            //     cout << "\033[34m" << sudoku.cells[i][j].value << "\033[0m" << ' ';
            // else
            //     cout << sudoku.cells[i][j].value << ' ';
            if (color_mask[i][j] == 0)
                cout << "\033[31m" << sudoku.cells[i][j].value << "\033[0m" << ' ';
            else
                cout << sudoku.cells[i][j].value << ' ';
            if (j == 2 || j == 5)
                cout << '|' << ' ';
        }

        cout << endl;
    }
}

bool solve(Sudoku &sudoku)
{
    if (sudoku.num_null_cells == 0)
        return true;
    vector<int> ordered_alternatives_mrv;
    int row_mrv = sudoku.coord_mrv.row;
    int col_mrv = sudoku.coord_mrv.col;
    bool flag;
    ordered_alternatives_mrv = sudoku.GetAltenatives(row_mrv, col_mrv);
    for (auto alternative_mrv : ordered_alternatives_mrv)
    {
        times_try++;
        sudoku.SetCell(row_mrv, col_mrv, alternative_mrv);
        if (!sudoku.ForwardCheck())
            sudoku.ClearCell(row_mrv, col_mrv);
        else
        {
            flag = solve(sudoku);
            if (flag)
                return true;
            else
                sudoku.ClearCell(row_mrv, col_mrv);
        }
    }
    return false;
}

int main(int argc, char **argv)
{
    string infile_path;
    bool is_X_sudoku = true;
    if (argc == 1)
    {
        cout << "Please specify the input file: ";
        cin >> infile_path;
    }
    else
        infile_path = argv[1];
    if (argc == 3 && !strcmp(argv[2], "-s"))
        is_X_sudoku = false;

    // infile_path = "/mnt/d/2020Spring/AI/lab/lab1/sudoku/input/sudoku04.txt";
    Sudoku sudoku(infile_path, is_X_sudoku);

    int color_mask[9][9];
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            color_mask[i][j] = sudoku.cells[i][j].value;

    bool flag = solve(sudoku);
    if (flag)
    {
        cout << "succeed: " << times_try << " steps" << endl;
    }
    else
    {
        cout << "fail" << endl;
    }
    OutputSudoku(sudoku, color_mask);
    return 0;
}