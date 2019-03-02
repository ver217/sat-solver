#pragma once
#include "types.h"
#include "bitmap.h"
#include "varmatrix.h"

class CnfContainer {
  public:
    size_t unit_cnt, clause_cnt;
    BitMap unit_out, not_unit_out;
    VarMatrix data;
    BitMap mask;
    Vector<size_t> clause_size;
    CnfContainer();
    CnfContainer(size_t unit_cnt, size_t clause_cnt, Vector<size_t> &cnt);
    CnfContainer(const CnfContainer& container);
    virtual ~CnfContainer();
    void release();
    bool has(size_t m, size_t n);
    void set_unit(int unit);
    void unset_unit(int unit);
};
