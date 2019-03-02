#pragma once

class BitMap {
    unsigned int *nums;
    static const unsigned int N_BITS = sizeof(unsigned int) * 8;
    unsigned int n_nums;
  public:
    unsigned int size;
    BitMap();
    BitMap(const unsigned int max);
    virtual ~BitMap();
    void set(const unsigned int pos);
    void unset(const unsigned int pos);
    void clear();
    bool operator[](const unsigned int pos) const;
};
