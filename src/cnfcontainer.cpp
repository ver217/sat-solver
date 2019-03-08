#include "../include/cnfcontainer.h"
#include <cmath>
#include "../include/base.h"

CnfContainer::CnfContainer() : unit_cnt(0), clause_cnt(0) {}

#ifndef OLD
CnfContainer::CnfContainer(size_t unit_cnt, size_t clause_cnt, const Vector<size_t> &cnt, const Vector<size_t>& literal_cnt) :
    unit_cnt(unit_cnt),
    clause_cnt(clause_cnt),
    unit_out(-unit_cnt, unit_cnt),
    clause_out(clause_cnt - 1),
    data(cnt),
    mask(cnt),
    clause_size(cnt),
    literal_table(literal_cnt) {
    for (size_t i = 0; i < clause_size.size(); i++)
        if (clause_size[i] == 1)
            units_idx.push_front(i);
}

CnfContainer::CnfContainer(const CnfContainer &container) :
    unit_cnt(container.unit_cnt),
    clause_cnt(container.clause_cnt),
    unit_out(container.unit_out),
    clause_out(container.clause_out),
    data(container.data),
    mask(container.mask),
    clause_size(container.clause_size),
    units_idx(container.units_idx),
    literal_table(container.literal_table) {}


inline bool CnfContainer::has(size_t m, size_t n) const {
    return !mask[m][n];
}

void CnfContainer::set_unit(int unit) {
    unit_out.set(unit);
    size_t table_idx = unit < 0 ? unit + unit_cnt : unit + unit_cnt - 1;
    size_t n_clauses_have_unit = literal_table.width(table_idx);
    for (size_t k = 0; k < n_clauses_have_unit; k++) {
        size_t i = literal_table[table_idx][k];
        size_t width = data.width(i);
        for (size_t j = 0; j < width; j++) {
            if (unit == data[i][j]) {
                mask.set(i, j);
                if (!clause_out[i]) {
                    clause_out.set(i);
                    clause_cnt--;
                    if (clause_size[i] == 1)
                        units_idx.remove_first(i);
                }
                break;
            }
        }
    }
    table_idx = -unit < 0 ? -unit + unit_cnt : -unit + unit_cnt - 1;
    n_clauses_have_unit = literal_table.width(table_idx);
    for (size_t k = 0; k < n_clauses_have_unit; k++) {
        size_t i = literal_table[table_idx][k];
        size_t width = data.width(i);
        for (size_t j = 0; j < width; j++) {
            if (-unit == data[i][j]) {
                mask.set(i, j);
                --clause_size[i];
                if (!clause_out[i]) {
                    if (clause_size[i] == 0) {
                        clause_out.set(i);
                        clause_cnt--;
                        units_idx.remove_first(i);
                    } else if (clause_size[i] == 1)
                        units_idx.push_front(i);
                }
                break;
            }
        }
    }
//    for (size_t i = 0; i < data.length(); i++) {
//        size_t width = data.width(i);
//        for (size_t j = 0; j < width; j++) {
//            if (unit == data[i][j]) {
//                mask.set(i, j);
//                if (!clause_out[i]) {
//                    clause_out.set(i);
//                    clause_cnt--;
//                    if (clause_size[i] == 1)
//                        units_idx.remove_first(i);
//                }
//                break;
//            }
//            if (-unit == data[i][j]) {
//                mask.set(i, j);
//                --clause_size[i];
//                if (!clause_out[i]) {
//                    if (clause_size[i] == 0) {
//                        clause_out.set(i);
//                        clause_cnt--;
//                        units_idx.remove_first(i);
//                    } else if (clause_size[i] == 1)
//                        units_idx.push_front(i);
//                }
//                break;
//            }
//        }
//    }
}

void CnfContainer::unset_unit(int unit) {
    unit_out.unset(unit);
    size_t table_idx = unit < 0 ? unit + unit_cnt : unit + unit_cnt - 1;
    size_t n_clauses_have_unit = literal_table.width(table_idx);
    for (size_t k = 0; k < n_clauses_have_unit; k++) {
        size_t i = literal_table[table_idx][k];
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
                if (clause_size[i] == 1)
                    units_idx.push_front(i);
            }
        }
    }
    table_idx = -unit < 0 ? -unit + unit_cnt : -unit + unit_cnt - 1;
    n_clauses_have_unit = literal_table.width(table_idx);
    for (size_t k = 0; k < n_clauses_have_unit; k++) {
         size_t i = literal_table[table_idx][k];
         size_t width = data.width(i);
          bool recover = true, find = false;
         for (size_t j = 0; j < width; j++) {
             if (-unit == data[i][j]) {
                 find = true;
                 mask.unset(i, j);
                 ++clause_size[i];
             } else if (unit_out[data[i][j]])
                 recover = false;
             if (find && recover) {
                 if (clause_size[i] == 1) {
                     clause_out.unset(i);
                     clause_cnt++;
                     units_idx.push_front(i);
                 } else if (clause_size[i] == 2)
                     units_idx.remove_first(i);
                 break;
             }
         }
     }
//    for (size_t i = 0; i < data.length(); i++) {
//        size_t width = data.width(i);
//        bool recover = true, find = false;
//        for (size_t j = 0; j < width; j++) {
//            if (-unit == data[i][j]) {
//                find = true;
//                mask.unset(i, j);
//                ++clause_size[i];
//            } else if (unit_out[data[i][j]])
//                recover = false;
//            if (find && recover) {
//                if (clause_size[i] == 1) {
//                    clause_out.unset(i);
//                    clause_cnt++;
//                    units_idx.push_front(i);
//                } else if (clause_size[i] == 2)
//                    units_idx.remove_first(i);
//                break;
//            }
//        }
//        if (clause_out[i]) {
//            bool recover = true, find = false;
//            for (size_t j = 0; j < width; j++) {
//                if (unit == data[i][j]) {
//                    find = true;
//                    mask.unset(i, j);
//                } else if (unit_out[data[i][j]])
//                    recover = false;
//            }
//            if (find && recover) {
//                clause_out.unset(i);
//                clause_cnt++;
//                if (clause_size[i] == 1)
//                    units_idx.push_front(i);
//            }
//        }
//    }
}

int CnfContainer::pick_unit() {
    if (units_idx.size() > 0) {
        size_t i = units_idx.front();
        size_t width = data.width(i);
        for (size_t j = 0; j < width; j++)
            if (!mask[i][j])
                return data[i][j];
        throw "pick unit ERROR!!!!";
    }
    return 0;
}

bool CnfContainer::exist_unit(int literal) {
//    if (units_idx.size() > 0) {
//        Node* node = units_idx.head;
//        if (node != units_idx.head)
//            throw "WTF ERROR";
//        while (node) {
//            size_t i = node->value;
//            size_t width = data.width(i);
//            for (size_t j = 0; j < width; j++)
//                if (has(i, j) && literal == data[i][j])
//                    return true;
//            node = node->next;
//        }
//    }
    if (units_idx.size() > 0) {
        units_idx.cur_to_begin();
        while (!units_idx.is_cur_end()) {
            size_t i = units_idx.get_cur_val();
            size_t width = data.width(i);
            for (size_t j = 0; j < width; j++)
                if (has(i, j) && literal == data[i][j])
                    return true;
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
            if (min_size == 2)
                break;
        }
    }
    size_t min_width = data.width(min_size_idx);
    for (size_t j = 0; j < min_width; j++)
        if (has(min_size_idx, j))
            return data[min_size_idx][j];
    throw "pick literal ERROR!!!!";
}

ostream &operator<<(ostream &out, const CnfContainer &container) {
    out << "n_varibles: " << container.unit_cnt << ", n_clauses: " << container.clause_cnt << endl;
    for (size_t i = 0; i < container.clause_cnt; i++) {
        if (!container.clause_out[i]) {
            size_t width = container.data.width(i);
            out << '[' << i + 1 << "] ";
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
#else
CnfContainer::CnfContainer(size_t unit_cnt, size_t clause_cnt, const Vector<size_t> &cnt) :
    unit_cnt(unit_cnt),
    clause_cnt(clause_cnt),
    unit_out(-unit_cnt, unit_cnt),
    clause_out(clause_cnt - 1),
    data(cnt),
    mask(data.total() - 1),
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

bool CnfContainer::has(size_t m, size_t n) const {
    return !mask[data.get_idx(m, n)];
}

void CnfContainer::set_unit(int unit) {
    unit_out.set(unit);
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
    unit_out.unset(unit);
    for (size_t i = 0; i < data.length(); i++) {
        if (clause_out[i]) {
            size_t width = data.width(i);
            bool recover = true, find = false;
            for (size_t j = 0; j < width; j++) {
                if (unit == data(i, j)) {
                    find = true;
                    mask.unset(data.get_idx(i, j));
                } else if (unit_out[data(i, j)])
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
            } else if (unit_out[data(i, j)])
                recover = false;
        }
        if (recover && clause_out[i] && clause_size[i] == width) {
            clause_out.unset(i);
            clause_cnt++;
        }
    }
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

ostream &operator<<(ostream &out, const CnfContainer &container) {
    for (size_t i = 0; i < container.clause_cnt; i++) {
        if (!container.clause_out[i]) {
            size_t width = container.data.width(i);
            out << '[' << i + 1 << "] ";
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
#endif
