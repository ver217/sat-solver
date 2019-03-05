#include "../include/generator.h"
#include <stdlib.h>
#include <time.h>

bool SudokuGenerator::row_conflict(int i, int num, const Sudoku& sudoku)
{
    for (int j = 0; j < size; j++)
        if (sudoku(i, j) == num)
            return true;
    return false;
}

bool SudokuGenerator::col_conflict(int j, int num, const Sudoku& sudoku)
{
    for (int i = 0; i < size; i++)
        if (sudoku(i, j) == num)
            return true;
    return false;
}

bool SudokuGenerator::square_conflict(int rowStart, int colStart, int num, const Sudoku& sudoku)
{
    for (int i = 0; i < sqrt_size; i++)
        for (int j = 0; j < sqrt_size; j++)
            if (sudoku(rowStart + i, colStart + j) == num)
                return true;
    return false;
}

bool SudokuGenerator::fill(int i, int j, Sudoku& sudoku)
{
    if (j >= size && i < size - 1) {
        i = i + 1;
        j = 0;
    }
    if (i >= size && j >= size)
        return true;
    if (i < sqrt_size) {
        if (j < sqrt_size)
            j = sqrt_size;
    } else if (i < size - sqrt_size) {
        if (j == (int)(i / sqrt_size) * sqrt_size)
            j = j + sqrt_size;
    } else {
        if (j == size - sqrt_size) {
            i = i + 1;
            j = 0;
            if (i >= size)
                return true;
        }
    }
    for (int num = 1; num <= size; num++) {
        if (!row_conflict(i, num, sudoku) && !col_conflict(j, num, sudoku) && !square_conflict(i - i % sqrt_size, j - j % sqrt_size, num, sudoku)) {
            sudoku(i, j) = num;
            if (fill(i, j + 1, sudoku))
                return true;
            sudoku(i, j) = 0;
        }
    }
    return false;
}

Sudoku SudokuGenerator::generate(uint n_holes)
{
    srand(time(0));
    Sudoku sudoku;
    for (size_t k = 0; k < size; k += sqrt_size) {
        int num;
        for (size_t i = 0; i < sqrt_size; i++) {
            for (size_t j = 0; j < sqrt_size; j++) {
                do
                    num = (rand() % (size)) + 1;
                while (square_conflict(k, k, num, sudoku));
                sudoku(k + i, k + j) = num;
            }
        }
    }
    fill(0, sqrt_size, sudoku);
    while (n_holes != 0) {
        size_t id = rand() % (size * size);
        size_t i = id / size;
        size_t j = id % size;
        if (j != 0)
            j = j - 1;
        if (sudoku(i, j) != 0) {
            n_holes--;
            sudoku(i, j) = 0;
        }
    }
    return sudoku;
}
