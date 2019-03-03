#include "../include/solver.h"

Solver::Solver() {};

Solver::Solver(const CnfContainer& cnf) : cnf(cnf) {}

Solver::~Solver() {};

bool Solver::DPLL(const CnfContainer& cnf) {
    CnfContainer p = cnf;
    if (p.clause_cnt == 0)
        return true;
    int literal;
    while ((literal = p.pick_unit()) != 0) {
        if (p.exist_unit(-literal))
            return false;
        else {
            p = simplify(p, literal);
            //track.push_back(literal);
        }
    }
    if (p.clause_cnt == 0)
        return true;
    literal = p.pick_literal();
    if (DPLL(simplify(p, literal)) == true) {
        //track.push_back(literal);
        return true;
    } else {
        //track.push_back(-literal);
        return DPLL(simplify(p, -literal));
    }
}

CnfContainer Solver::simplify(const CnfContainer& cnf, int literal) {
    CnfContainer p = cnf;
    for (size_t i = 0; i < p.data.length(); i++) {
        if (!p.clause_out[i]) {
            size_t width = p.data.width(i);
            for (size_t j = 0; j < width; j++) {
                if (literal == p.data(i, j)) {
                    p.clause_out.set(i);
                    p.clause_cnt--;
                }
            }
        }
    }
    for (size_t i = 0; i < p.data.length(); i++) {
        if (!p.clause_out[i]) {
            size_t width = p.data.width(i);
            for (size_t j = 0; j < width; j++) {
                if (-literal == p.data(i, j))
                    p.mask.set(p.data.get_idx(i, j));
            }
        }
    }
    p.unit_out.set(literal > 0 ? literal : -literal);
    return p;
}

bool Solver::DPLL() {
    if (cnf.clause_cnt == 0)
        return true;
    int literal;
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
    if (DPLL() == true)
        return true;
    else {
        int l;
        while (track.size() > 0 && (l = track.pop_back()) != literal)
            cnf.unset_unit(l);
        cnf.unset_unit(literal);
    }
    cnf.set_unit(-literal);
    if (DPLL() == true)
        return true;
    else {
        int l;
        while (track.size() > 0 && (l = track.pop_back()) != -literal)
            cnf.unset_unit(l);
        cnf.unset_unit(-literal);
    }
    return false;
}

void Solver::solve() {
    DPLL();
}

ostream& Solver::print_res(ostream& out) const {
    for (size_t i = 0; i < track.size(); i++)
        out << track[i] << ' ';
    out << endl;
    return out;
}
