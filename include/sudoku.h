#pragma once
#include "types.h"
#include "vector.h"
#include "base.h"

class Sudoku {
    int data[81];
  public:
    Sudoku();
    Sudoku(const Vector<int>& nums);
    Sudoku(const Sudoku& sudoku);
    virtual ~Sudoku();
    int& operator()(size_t i, size_t j);
    int operator()(size_t i, size_t j) const;
    Vector<int> to_vector();
    friend ostream& operator<<(ostream& out, const Sudoku& sudoku);
};
