#pragma once
#include "types.h"

const size_t INIT_CAPACITY = 50;
const size_t INCREMENT = 30;

template <typename T>
class Vector {
    size_t _size, _capacity;
    T *data;
  public:
    static const size_t npos = -1;
    Vector();
    Vector(size_t n);
    Vector(const Vector<T>& v);
    virtual ~Vector();
    size_t size() const;
    size_t capacity() const;
    T& operator[](size_t idx);
    const T& operator[](size_t idx) const;
    void push_back(const T& x);
    const T& pop_back();
    size_t find(const T& x) const;
    void reserve(size_t n);
    friend class VarMatrix;
    friend class BitMap2D;
    friend class Sudoku;
};
