#pragma once
#include "types.h"
#include "vector.h"

class VarMatrix {
    size_t _length;
    size_t *offsets;
    int *data;
  public:
    VarMatrix();
    VarMatrix(Vector<size_t> &cnt);
    VarMatrix(const VarMatrix &matrix);
    virtual ~VarMatrix();
    void release();
    size_t length() const;
    size_t width(size_t idx) const;
    size_t total() const;
    size_t get_idx(size_t m, size_t n);
    int& operator()(size_t m, size_t n);
};
