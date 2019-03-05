#pragma once
#include "sudoku.h"
#include "types.h"

class SudokuGenerator {
    static const int size = 9;
    static const int sqrt_size = 3;

    inline static bool row_conflict(int i, int num, const Sudoku& sudoku);
    inline static bool col_conflict(int j, int num, const Sudoku& sudoku);
    inline static bool square_conflict(int rowStart, int colStart, int num, const Sudoku& sudoku);
    inline static bool fill(int i, int j, Sudoku& sudoku);

public:
    static Sudoku generate(uint n_holes);
};
