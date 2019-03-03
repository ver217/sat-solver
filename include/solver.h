#pragma once
#include "types.h"
#include "vector.h"
#include "cnfcontainer.h"

class Solver {
    CnfContainer cnf;
    Vector<int> track;
  public:
    Solver();
    Solver(const CnfContainer& cnf);
    virtual ~Solver();
    bool DPLL(const CnfContainer& cnf);
    static CnfContainer simplify(const CnfContainer& cnf, int literal);
    bool DPLL();
    void solve();
    ostream& print_res(ostream &out) const;
};
