#include "../include/solver.h"
#include "../include/base.h"
#include <time.h>

Solver::Solver() {};

Solver::Solver(const CnfContainer &cnf) : cnf(cnf) {}

Solver::~Solver() {};

bool Solver::DPLL() {
    if (cnf.clause_cnt == 0)
        return true;
    int literal = 0;
    while ((literal = cnf.pick_unit()) != 0) {
        if (cnf.exist_unit(-literal))
            return false;
        else {
            cnf.set_unit(literal);
            track.push_back(literal);
        }
    }
    if (cnf.clause_cnt == 0)
        return true;
    literal = cnf.pick_literal();
    cnf.set_unit(literal);
    track.push_back(literal);
    if (DPLL())
        return true;
    else {
        int l;
        while (track.size() > 0 && (l = track.pop_back()) != literal)
            cnf.unset_unit(l);
        cnf.unset_unit(literal);
    }
    cnf.set_unit(-literal);
    track.push_back(-literal);
    if (DPLL())
        return true;
    else {
        int l;
        while (track.size() > 0 && (l = track.pop_back()) != -literal)
            cnf.unset_unit(l);
        cnf.unset_unit(-literal);
    }
    return false;
}

bool Solver::solve(ostream &out) {
    clock_t start = clock();
    bool res = DPLL();
    clock_t end = clock();
//    cout << "------------------" << endl;
//    cout << "------debug-------" << endl;
//    cout << "var cnt: " << cnf.unit_cnt << ", res cnt: " << track.size() << endl;
//    cout << "error: " << (res ^ check()) << endl;
//    cout << "track: ";
//    for (size_t i = 0; i < track.size(); i++)
//        cout << track[i] << ' ';
//    cout << endl;
//    cout << "------------------" << endl;
    out << "s " << res << endl;
    out << "v ";
    if (!res)
        out << endl;
    else
        print_res(out);
    out << "t " << static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000 << endl;
    return res;
}

bool Solver::solve() {
    return DPLL();
}

ostream &Solver::print_res(ostream &out) const {
    for (int i = 1; i <= static_cast<int>(cnf.unit_cnt); i++) {
        if (cnf.unit_out[i])
            out << i << ' ';
        else if (cnf.unit_out[-i])
            out << -i << ' ';
    }
    out << endl;
    return out;
}

bool Solver::check() const {
    bool res = true;
    for (size_t i = 0; i < cnf.data.length(); i++) {
        size_t width = cnf.data.width(i);
        bool line = false;
        for (size_t j = 0; j < width; j++)
            line |= cnf.unit_out[cnf.data(i, j)];
        if (!line)
            break;
    }
    return res;
}

Vector<int> Solver::get_res() const {
    Vector<int> res;
    for (int i = 1; i <= static_cast<int>(cnf.unit_cnt); i++) {
        if (cnf.unit_out[i])
            res.push_back(i);
        else if (cnf.unit_out[-i])
            res.push_back(-i);
    }
    return res;
}
