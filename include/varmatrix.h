#pragma once
#include "types.h"
#include "vector.h"

class VarMatrix {
    size_t _length;
    size_t *_width;
    int **data;
  public:
    VarMatrix();
    VarMatrix(const Vector<size_t> &cnt);
    VarMatrix(const VarMatrix &matrix);
    virtual ~VarMatrix();
    size_t length() const;
    size_t width(size_t idx) const;
    int* operator[](size_t i) const;
    int* operator[](size_t i);
};
