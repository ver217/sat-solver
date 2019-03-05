#pragma once
#include "types.h"
#include "vector.h"
#include "cnfcontainer.h"
#include <tuple>
using std::tie;
using std::make_tuple;
using std::tuple;
using std::get;

class Solver {
    CnfContainer cnf;
    Vector<int> track;
  public:
    Solver();
    Solver(const CnfContainer& cnf);
    virtual ~Solver();
    CnfContainer DPLL(const CnfContainer& cnf, bool& status);
    bool DPLL();
    bool solve(ostream &out, bool copy = false);
    ostream& print_res(ostream &out) const;
    bool check() const;
    Vector<int> get_res();
};
