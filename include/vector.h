#pragma once
#include "types.h"

const size_t INIT_CAPACITY = 5;
const size_t INCREMENT = 3;

template <typename T>
class Vector {
    size_t _size, _capacity;
    T *data;
  public:
    Vector();
    Vector(size_t n);
    Vector(const Vector<T>& v);
    virtual ~Vector();
    void release();
    size_t size() const;
    size_t capacity() const;
    T& operator[](size_t idx);
    const T& operator[](size_t idx) const;
    void push_back(const T& x);
    const T& pop_back();
};
