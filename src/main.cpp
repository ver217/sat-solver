#include "../include/base.h"

class Matrix {
    int data[10][10];
    //int** data;
  public:
    //size_t length, width;
    
    //Matrix(size_t m, size_t n) : length(m), width(n) {
    
    //}
    //
    Matrix() {
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                data[i][j] = 0;
    }
    
    virtual  ~Matrix() {}
    
    int& operator ()(int i, int j) {
        return data[i][j];
    }
};
