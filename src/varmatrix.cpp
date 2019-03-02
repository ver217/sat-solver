#include "../include/varmatrix.h"
#include <string.h>

VarMatrix::VarMatrix() : _length(0), offsets(nullptr), data(nullptr) {}

VarMatrix::VarMatrix(Vector<size_t> &cnt) : _length(cnt.size()), offsets(nullptr), data(nullptr) {
    offsets = new size_t[_length + 1];
    offsets[0] = 0;
    size_t total = 0;
    for (size_t i = 0; i < _length; i++) {
        total += cnt[i];
        offsets[i + 1] = total;
    }
    data = new int[total];
}

VarMatrix::VarMatrix(const VarMatrix &matrix) : _length(matrix.length()), offsets(nullptr), data(nullptr) {
    offsets = new size_t[_length + 1];
    memcpy(offsets, matrix.offsets, sizeof(size_t) * (_length + 1));
    data = new int[matrix.total()];
    memcpy(data, matrix.data, sizeof(int) * matrix.total());
}

VarMatrix::~VarMatrix() {}

void VarMatrix::release() {
    if (offsets) {
        delete []offsets;
        offsets = nullptr;
    }
    if (data) {
        delete []data;
        data = nullptr;
    }
}

size_t VarMatrix::length() const {
    return _length;
}

size_t VarMatrix::width(size_t idx) const {
    if (idx >= _length)
        throw "out of range";
    return offsets[idx + 1] - offsets[idx];
}

size_t VarMatrix::total() const {
    return offsets[_length];
}

size_t VarMatrix::get_idx(size_t m, size_t n) {
    return offsets[m] + n;
}

int& VarMatrix::operator()(size_t m, size_t n) {
    return data[get_idx(m, n)];
}
