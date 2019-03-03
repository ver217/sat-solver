#pragma once
#include "types.h"

class BitMap {
    static const unsigned int N_BITS = sizeof(unsigned int) * 8;
    unsigned int n_nums;
    unsigned int *nums;
  public:
    unsigned int size;
    BitMap();
    BitMap(const unsigned int max);
    BitMap(const BitMap& bm);
    virtual ~BitMap();
    void set(const unsigned int pos);
    void unset(const unsigned int pos);
    void clear();
    bool operator[](const unsigned int pos) const;
};
