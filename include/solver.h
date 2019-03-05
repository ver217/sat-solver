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
    bool DPLL();
    bool solve(ostream &out);
    bool solve();
    ostream& print_res(ostream &out) const;
    bool check() const;
    Vector<int> get_res();
};
