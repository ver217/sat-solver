#pragma once
#include "types.h"
#include "bitmap.h"
#include "varmatrix.h"
#include "base.h"
#include "linkedlist.h"

#ifndef OLD
class CnfContainer {
  public:
    size_t unit_cnt, clause_cnt;
    BitMap unit_out, clause_out;
    VarMatrix data;
    BitMap2D mask;
    Vector<size_t> clause_size;
    LinkedList units_idx;
    VarMatrix literal_table;
    CnfContainer();
    CnfContainer(size_t unit_cnt, size_t clause_cnt, const Vector<size_t> &cnt, const Vector<size_t> &literal_cnt);
    CnfContainer(const CnfContainer& container);
    virtual ~CnfContainer() = default;
    inline bool has(size_t m, size_t n) const;
    void set_unit(int unit);
    void unset_unit(int unit);
    friend ostream& operator<<(ostream &out, const CnfContainer &container);
    int pick_unit();
    bool exist_unit(int literal);
    int pick_literal();
};
#else
class CnfContainer {
  public:
    size_t unit_cnt, clause_cnt;
    BitMap unit_out, clause_out;
    VarMatrix data;
    BitMap mask;
    Vector<size_t> clause_size;
    CnfContainer();
    CnfContainer(size_t unit_cnt, size_t clause_cnt, const Vector<size_t> &cnt);
    CnfContainer(const CnfContainer& container);
    virtual ~CnfContainer() = default;
    bool has(size_t m, size_t n) const;
    void set_unit(int unit);
    void unset_unit(int unit);
    friend ostream& operator<<(ostream &out, const CnfContainer &container);
    int pick_unit();
    bool exist_unit(int literal);
    int pick_literal();
};
#endif