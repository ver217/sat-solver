#include "../include/sudoku.h"

Sudoku::Sudoku() {}


Sudoku::Sudoku(const Vector<int>& nums) {
    if (nums.size() == 81) {
        for (size_t i = 0; i < 81; i++)
            data[i] = nums[i];
    }
}

Sudoku::~Sudoku() {}

int& Sudoku::operator()(size_t i, size_t j) {
    return data[i * 9 + j];
}

int Sudoku::operator()(size_t i, size_t j) const {
    return data[i * 9 + j];
}

Vector<int> Sudoku::to_vector() {
    Vector<int> nums(81);
    for (size_t i = 0; i < 81; i++)
        nums[i] = data[i];
    return nums;
}

ostream& operator<<(ostream& out, const Sudoku& sudoku) {
    int col = 0, row = 0;
    for (size_t i = 0; i < 81; i++) {
        int cell = sudoku.data[i];
        col++;
        out << cell << ' ';
        if (col == 3 || col == 6)
            out << "| ";
        else if (col == 9) {
            col = 0;
            row++;
            if (row == 3 || row == 6)
                out << "- - - - - - - - - - -";
        }
    }
    out << endl;
    return out;
}
