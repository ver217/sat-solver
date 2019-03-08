#pragma once
#include "types.h"
#include "vector.h"

#ifndef OLD
class VarMatrix {
  protected:
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

class LiteralTable : public VarMatrix {
    size_t n_unit;
  public:
    LiteralTable();
    LiteralTable(size_t unit_cnt, const Vector<size_t> &cnt);
    LiteralTable(const LiteralTable& table);
    virtual ~LiteralTable() = default;
    size_t width(int x) const;
    int* operator[](int x) const;
    int* operator[](int x);
};
#else
class VarMatrix {
    size_t _length;
    size_t *offsets;
    int *data;
  public:
    VarMatrix();
    VarMatrix(const Vector<size_t> &cnt);
    VarMatrix(const VarMatrix &matrix);
    virtual ~VarMatrix();
    size_t length() const;
    size_t width(size_t idx) const;
    size_t total() const;
    size_t get_idx(size_t m, size_t n) const;
    int& operator()(size_t m, size_t n);
    int operator()(size_t m, size_t n) const;
};
#endif
