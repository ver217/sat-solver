#pragma once
#include "types.h"

class BitMap {
    static const uint N_BITS = sizeof(uint) * 8;
    uint n_nums;
    uint *nums;
    int bias;
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
