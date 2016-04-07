#ifndef SUDOKU_CLASS_H
#define SUDOKU_CLASS_H
#include "utils.h"
#include <map>

typedef std::vector<int> Row;
typedef std::vector<Row> Puzzle;
typedef std::map<int, Row> PROBS;
typedef unsigned int uint;
class Sudoku
{
    Sudoku(Sudoku &cpy) = delete;
    PROBS _possibValues;
    int _size;
    int _sqSize;
    int **_puzzle;
public:
    Sudoku(int siz);
    ~Sudoku();
    //setters and getters
    bool setRow(uint row, int *arr);
    bool getRow(uint row, int *arr) const;
    bool setVal(uint row, uint col, int val);
    bool getVal(uint row, uint col, int &val) const;
    void printPuzzle();
    void printRow(uint row);
};
#endif
