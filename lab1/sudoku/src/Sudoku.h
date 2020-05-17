#include <string>
#include <vector>

typedef struct point
{
    int row;
    int col;
} Point;

typedef struct cell
{
    int value;
    int degree;            //the number of constraints on remaining variables
    int occupy_values[10]; //index=0 is the number of remian values, 1-9 is the frequency of bring occupied by neighbors
} Cell;

class Sudoku
{
private:
    /* data */
    void Init();   //init degree and ...
    void SetMrv(); // global
    bool is_X_sudoku;

public:
    //TODO: should be private
    Cell cells[9][9];
    Point coord_mrv;
    int mrv; //minimum remianing values
    int num_null_cells;

    Sudoku(std::string infile_path, bool is_X_sudoku);
    void GetNeighborIndex(int row, int col, std::vector<Point> &neighbors);
    bool SetCell(int row, int col, int value);         // cells[row][col].value = value,
    bool ClearCell(int row, int col);                  // cells[row][col].value = 0;
    std::vector<int> GetAltenatives(int row, int col); // select which values to fill the cell[row][col] by the ascending order of constrains on its neighbors,
    bool ForwardCheck();                               // forward check
};
