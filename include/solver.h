#pragma once
#include "types.h"
#include "vector.h"
#include "cnfcontainer.h"

class Solver {
    CnfContainer cnf;
    Vector<int> track;
    bool DPLL();
  public:
    Solver() = default;
    Solver(const CnfContainer& cnf);
    virtual ~Solver() = default;
    bool solve(ostream &out, bool to_file);
    bool solve();
    ostream& print_res(ostream &out) const;
    bool check() const;
    Vector<int> get_res() const;
};
