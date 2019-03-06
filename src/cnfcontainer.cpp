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
                --clause_size[i];
                break;
            }
        }
    }
}

void CnfContainer::unset_unit(int unit) {
    unit_out.unset(unit);
    for (size_t i = 0; i < data.length(); i++) {
        size_t width = data.width(i);
        for (size_t j = 0; j < width; j++) {
            if (-unit == data[i][j]) {
                mask.unset(i, j);
                ++clause_size[i];
            }
        }
        if (clause_out[i]) {
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
        }
    }
    size_t min_width = data.width(min_size_idx);
    for (size_t j = 0; j < min_width; j++)
        if (has(min_size_idx, j))
            return data[min_size_idx][j];
    return 0;
}
