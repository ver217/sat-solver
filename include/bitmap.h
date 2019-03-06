#pragma once
#include "types.h"
#include "vector.h"

class BitMap {
    int bias;
    bool *table;
  public:
    uint size;
    BitMap();
    BitMap(uint max);
    BitMap(int min, int max);
    BitMap(const BitMap& bm);
    virtual ~BitMap();
    void set(int pos);
    void unset(int pos);
    void clear();
    bool operator[](int pos) const;
};

class BitMap2D {
    size_t _length;
//    size_t *_width;
    bool **matrix;
  public:
    BitMap2D();
    BitMap2D(const Vector<size_t>& cnt);
    BitMap2D(const BitMap2D& bm);
    virtual ~BitMap2D();
    void set(size_t i, size_t j);
    void unset(size_t i, size_t j);
    bool* operator[](size_t i) const;
};
