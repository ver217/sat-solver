#include "../include/cnfcontainer.h"

CnfContainer::CnfContainer() : unit_cnt(0), clause_cnt(0) {}

CnfContainer::CnfContainer(size_t unit_cnt, size_t clause_cnt, Vector<size_t> &cnt) :
    unit_cnt(unit_cnt),
    clause_cnt(clause_cnt),
    unit_out(unit_cnt),
    not_unit_out(unit_cnt),
    data(cnt),
    mask(data.total()),
    clause_size(cnt) {
}

CnfContainer::CnfContainer(const CnfContainer& container) :
    unit_cnt(container.unit_cnt),
    clause_cnt(container.clause_cnt),
    unit_out(container.unit_out),
    not_unit_out(unit_cnt),
    data(container.data),
    mask(container.mask),
    clause_size(container.clause_size) {
}

CnfContainer::~CnfContainer() {}

void CnfContainer::release() {
    delete &unit_out;
    data.release();
    delete &mask;
    clause_size.release();
}

bool CnfContainer::has(size_t m, size_t n) {
    return mask[data.get_idx(m, n)];
}

void CnfContainer::set_unit(int unit) {
    if ( (unit > 0 && !unit_out[unit]) || (unit < 0 && !not_unit_out[-unit]) ) {
        for (size_t i = 0; i < data.length(); i++) {
            if (clause_size[i] > 0) {
                size_t width = data.width(i);
                for (size_t j = 0; j < width; j++) {
                    if (unit == data(i, j)) {
                        mask.set(data.get_idx(i, j));
                        clause_size[i]--;
                    }
                }
                if (clause_size[i] == 0)
                    clause_cnt--;
            }
        }
    }
}

void CnfContainer::unset_unit(int unit) {
    if ( (unit > 0 && unit_out[unit]) || (unit < 0 && not_unit_out[-unit]) ) {
        for (size_t i = 0; i < data.length(); i++) {
            size_t width = data.width(i);
            size_t prev_size = clause_size[i];
            if (clause_size[i] < width) {
                for (size_t j = 0; j < width; j++) {
                    if (unit == data(i, j)) {
                        mask.unset(data.get_idx(i, j));
                        clause_size[i]++;
                    }
                }
                if (prev_size == 0 && clause_size[i] > 0)
                    clause_cnt++;
            }
        }
    }
}
