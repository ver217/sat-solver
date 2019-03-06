#include "../include/cnfcontainer.h"
#include <cmath>
#include "../include/base.h"

CnfContainer::CnfContainer() : unit_cnt(0), clause_cnt(0) {}

CnfContainer::CnfContainer(size_t unit_cnt, size_t clause_cnt, const Vector<size_t> &cnt) :
    unit_cnt(unit_cnt),
    clause_cnt(clause_cnt),
    unit_out(-unit_cnt, unit_cnt),
    clause_out(clause_cnt - 1),
    data(cnt),
    mask(cnt),
    clause_size(cnt) {
}

CnfContainer::CnfContainer(const CnfContainer &container) :
    unit_cnt(container.unit_cnt),
    clause_cnt(container.clause_cnt),
    unit_out(container.unit_out),
    clause_out(container.clause_out),
    data(container.data),
    mask(container.mask),
    clause_size(container.clause_size) {
}

CnfContainer::~CnfContainer() {
}

bool CnfContainer::has(size_t m, size_t n) const {
    return !mask[m][n];
}

void CnfContainer::set_unit(int unit) {
    unit_out.set(unit);
    for (size_t i = 0; i < data.length(); i++) {
        size_t width = data.width(i);
        for (size_t j = 0; j < width; j++) {
            if (unit == data[i][j]) {
                mask.set(i, j);
                if (!clause_out[i]) {
                    clause_out.set(i);
                    clause_cnt--;
                }
                break;
            }
            if (-unit == data[i][j]) {
                mask.set(i, j);
                if (--clause_size[i] == 0 && !clause_out[i]) {
                    clause_out.set(i);
                    clause_cnt--;
                }
                break;
            }
        }
    }
//    for (size_t i = 0; i < data.length(); i++) {
//        size_t width = data.width(i);
//        for (size_t j = 0; j < width; j++) {
//            if (-unit == data[i][j]) {
//                mask.set(data.get_idx(i, j));
//                if (--clause_size[i] == 0 && !clause_out[i]) {
//                    clause_out.set(i);
//                    clause_cnt--;
//                }
//                break;
//            }
//        }
//    }
}

void CnfContainer::unset_unit(int unit) {
    unit_out.unset(unit);
    for (size_t i = 0; i < data.length(); i++) {
        if (clause_out[i]) {
            size_t width = data.width(i);
            bool recover = true, find = false;
            for (size_t j = 0; j < width; j++) {
                if (unit == data[i][j]) {
                    find = true;
                    mask.unset(i, j);
                } else if (unit_out[data[i][j]])
                    recover = false;
            }
            if (find && recover) {
                clause_out.unset(i);
                clause_cnt++;
            }
        }
    }
    for (size_t i = 0; i < data.length(); i++) {
        size_t width = data.width(i);
        bool recover = true, find = false;
        for (size_t j = 0; j < width; j++) {
            if (-unit == data[i][j]) {
                find = true;
                mask.unset(i, j);
                ++clause_size[i];
            } else if (unit_out[data[i][j]])
                recover = false;
        }
        if (find && recover && clause_out[i] && clause_size[i] == 1) { // TODO: prev clause_size[i] == width and no find
            clause_out.unset(i);
            clause_cnt++;
        }
    }
}

ostream &operator<<(ostream &out, const CnfContainer &container) {
    for (size_t i = 0; i < container.clause_cnt; i++) {
        if (!container.clause_out[i]) {
            size_t width = container.data.width(i);
            out << '(';
            for (size_t j = 0; j < width; j++) {
                if (container.has(i, j)) {
                    int unit = container.data[i][j];
                    if (unit > 0)
                        out << unit;
                    else if (unit < 0)
                        out << '~' << -unit;
                    if (j == width - 1)
                        out << ')';
                    else
                        out << " + ";
                }
            }
            out << endl;
        }
    }
    return out;
}

int CnfContainer::pick_unit() {
    for (size_t i = 0; i < clause_size.size(); i++) {
        if (clause_size[i] == 1 && !clause_out[i]) {
            size_t width = data.width(i);
            for (size_t j = 0; j < width; j++) {
                if (has(i, j))
                    return data[i][j];
            }
        }
    }
    return 0;
}

bool CnfContainer::exist_unit(int literal) {
    for (size_t i = 0; i < clause_size.size(); i++) {
        if (clause_size[i] == 1 && !clause_out[i]) {
            size_t width = data.width(i);
            for (size_t j = 0; j < width; j++) {
                if (has(i, j) && literal == data[i][j])
                    return true;
            }
        }
    }
    return false;
}

int CnfContainer::pick_literal() {
    size_t min_size = static_cast<size_t>(-1);
    size_t min_size_idx = min_size;
    for (size_t i = 0; i < data.length(); i++) {
        if (!clause_out[i]) {
            if (clause_size[i] < min_size) {
                min_size = clause_size[i];
                min_size_idx = i;
            }
//            for (size_t j = 0; j < width; j++) {
//                if (has(i, j))
//                    return data[i][j];
//            }
        }
    }
    size_t min_width = data.width(min_size_idx);
    for (size_t j = 0; j < min_width; j++)
        if (has(min_size_idx, j))
            return data[min_size_idx][j];
    return 0;
}

//OptimCnf::OptimCnf() : shortest_clause_idx(static_cast<size_t>(-1)) {}
//
//OptimCnf::OptimCnf(size_t unit_cnt, size_t clause_cnt, const Vector<size_t> &cnt)
//        : CnfContainer(unit_cnt, clause_cnt, cnt) {
//    size_t min = static_cast<size_t>(-1);
//    for (size_t i = 0; i < clause_size.size(); i++) {
//        if (clause_size[i] == 1)
//            unit_clauses.push_back(i);
//        else if (clause_size[i] > 1 && clause_size[i] < min)
//            shortest_clause_idx = i;
//    }
//}
//
//OptimCnf::OptimCnf(const OptimCnf& cnf)
//        : CnfContainer(cnf),
//          shortest_clause_idx(cnf.shortest_clause_idx),
//          unit_clauses(cnf.unit_clauses) {
//}
//
//OptimCnf::~OptimCnf() {}
//
//void OptimCnf::set_unit(int unit) {
//    unit_out.set(unit);
//    for (size_t i = 0; i < data.length(); i++) {
//        size_t width = data.width(i);
//        for (size_t j = 0; j < width; j++) {
//            if (unit == data(i, j)) {
//                mask.set(data.get_idx(i, j));
//                if (!clause_out[i]) {
//                    clause_out.set(i);
//                    clause_cnt--;
//                    if (clause_size[i] == 1)
//                        unit_clauses.remove_first(i);
//                }
//                break;
//            }
//        }
//    }
//    for (size_t i = 0; i < data.length(); i++) {
//        size_t width = data.width(i);
//        for (size_t j = 0; j < width; j++) {
//            if (-unit == data(i, j)) {
//                mask.set(data.get_idx(i, j));
//                if (--clause_size[i] == 0 && !clause_out[i]) {
//                    clause_out.set(i);
//                    clause_cnt--;
//                    unit_clauses.remove_first(i);
//                } else if (clause_size[i] == 1)
//                    unit_clauses.push_back(i);
//                break;
//            }
//        }
//    }
//}
//
//void OptimCnf::unset_unit(int unit) {
//    unit_out.unset(unit);
//    for (size_t i = 0; i < data.length(); i++) {
//        if (clause_out[i]) {
//            size_t width = data.width(i);
//            bool recover = true, find = false;
//            for (size_t j = 0; j < width; j++) {
//                if (unit == data(i, j)) {
//                    find = true;
//                    mask.unset(data.get_idx(i, j));
//                } else if (unit_out[data(i, j)])
//                    recover = false;
//            }
//            if (find && recover) {
//                clause_out.unset(i);
//                clause_cnt++;
//                if (clause_size[i] == 1)
//                    unit_clauses.push_back(i);
//            }
//        }
//    }
//    for (size_t i = 0; i < data.length(); i++) {
//        size_t width = data.width(i);
//        bool recover = true, find = false;
//        size_t prev_clause_size = clause_size[i];
//        for (size_t j = 0; j < width; j++) {
//            if (-unit == data(i, j)) {
//                find = true;
//                mask.unset(data.get_idx(i, j));
//                ++clause_size[i];
//            } else if (unit_out[data(i, j)])
//                recover = false;
//        }
//        if (find && recover && clause_out[i] && clause_size[i] == 1) {
//            clause_out.unset(i);
//            clause_cnt++;
//            unit_clauses.push_back(i);
//        } else if (find && !clause_out[i] && prev_clause_size == 1 && clause_size[i] > 1)
//            unit_clauses.remove_first(i);
//    }
//}
//
//int OptimCnf::pick_unit() {
//    if (unit_clauses.size() > 0) {
//        size_t i = unit_clauses.pop_back();
//        size_t width = data.width(i);
//        for (size_t j = 0; j < width; j++)
//            if (has(i, j))
//                return data(i, j);
//    }
//    return 0;
//}
//
//bool OptimCnf::exist_unit(int literal) {
//    for (size_t k = 0; k < unit_clauses.size(); k++) {
//        size_t i = unit_clauses[k];
//        size_t width = data.width(i);
//        for (size_t j = 0; j < width; j++)
//            if (has(i, j) && data(i, j) == literal)
//                return true;
//    }
//    return false;
//}
