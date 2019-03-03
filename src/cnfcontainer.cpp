#include "../include/cnfcontainer.h"
#include <cmath>

CnfContainer::CnfContainer() : unit_cnt(0), clause_cnt(0) {}

CnfContainer::CnfContainer(size_t unit_cnt, size_t clause_cnt, const Vector<size_t> &cnt) :
    unit_cnt(unit_cnt),
    clause_cnt(clause_cnt),
    unit_out(unit_cnt * 2 + 1),
    clause_out(clause_cnt),
    data(cnt),
    mask(data.total()),
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

void CnfContainer::release() {
    data.release();
    clause_size.release();
}

bool CnfContainer::has(size_t m, size_t n) const {
    return !mask[data.get_idx(m, n)];
}

bool CnfContainer::is_unit_out(int unit) const {
    return unit_out[unit + unit_cnt];
}

void CnfContainer::set_unit(int unit) {
    //if ( (unit > 0 && !unit_out[unit]) || (unit < 0 && !unit_out[-unit]) ) {
    //for (size_t i = 0; i < data.length(); i++) {
    //if (clause_size[i] > 0) {
    //size_t width = data.width(i);
    //for (size_t j = 0; j < width; j++) {
    //if (unit == data(i, j)) {
    //mask.set(data.get_idx(i, j));
    //clause_size[i]--;
    //}
    //}
    //if (clause_size[i] == 0)
    //clause_cnt--;
    //}
    //}
    //unit_out.set(abs(unit));
    //}
    unit_out.set(unit + unit_cnt);
    for (size_t i = 0; i < data.length(); i++) {
        size_t width = data.width(i);
        for (size_t j = 0; j < width; j++) {
            if (unit == data(i, j)) {
                mask.set(data.get_idx(i, j));
                if (!clause_out[i]) {
                    clause_out.set(i);
                    clause_cnt--;
                }
                break;
            }
        }
    }
    for (size_t i = 0; i < data.length(); i++) {
        size_t width = data.width(i);
        for (size_t j = 0; j < width; j++) {
            if (-unit == data(i, j)) {
                mask.set(data.get_idx(i, j));
                if (--clause_size[i] == 0 && !clause_out[i]) {
                    clause_out.set(i);
                    clause_cnt--;
                }
                break;
            }
        }
    }
}

void CnfContainer::unset_unit(int unit) {
    //if ( (unit > 0 && unit_out[unit]) || (unit < 0 && unit_out[-unit]) ) {
    //for (size_t i = 0; i < data.length(); i++) {
    //size_t width = data.width(i);
    //size_t prev_size = clause_size[i];
    //if (clause_size[i] < width) {
    //for (size_t j = 0; j < width; j++) {
    //if (unit == data(i, j)) {
    //mask.unset(data.get_idx(i, j));
    //clause_size[i]++;
    //}
    //}
    //if (prev_size == 0 && clause_size[i] > 0)
    //clause_cnt++;
    //}
    //}
    //unit_out.unset(abs(unit));
    //}
    unit_out.unset(unit + unit_cnt);
    for (size_t i = 0; i < data.length(); i++) {
        if (clause_out[i]) {
            size_t width = data.width(i);
            bool recover = true, find = false;
            for (size_t j = 0; j < width; j++) {
                if (unit == data(i, j)) {
                    find = true;
                    mask.unset(data.get_idx(i, j));
                } else if (is_unit_out(data(i, j))) // TODO
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
        bool recover = true;
        for (size_t j = 0; j < width; j++) {
            if (-unit == data(i, j)) {
                mask.unset(data.get_idx(i, j));
                ++clause_size[i];
            } else if (is_unit_out(data(i, j))) // TODO
                recover = false;
        }
        if (recover && clause_out[i] && clause_size[i] == width) {
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
                    int unit = container.data(i, j);
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
                    return data(i, j);
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
                if (has(i, j) && literal == data(i, j))
                    return true;
            }
        }
    }
    return false;
}

int CnfContainer::pick_literal() {
    for (size_t i = 0; i < data.length(); i++) {
        if (!clause_out[i]) {
            size_t width = data.width(i);
            for (size_t j = 0; j < width; j++) {
                if (has(i, j))
                    return data(i, j);
            }
        }
    }
    return 0;
}
