#include "../include/varmatrix.h"
#include <string.h>

VarMatrix::VarMatrix(Vector<size_t> &cnt) : _length(cnt.size()), offsets(NULL), data(NULL) {
    offsets = new size_t[_length + 1];
    offsets[0] = 0;
    size_t total = 0;
    for (size_t i = 0; i < _length; i++) {
        total += cnt[i];
        offsets[i + 1] = total;
    }
    data = new int[total];
}

VarMatrix::VarMatrix(const VarMatrix &matrix) : _length(matrix.length()), offsets(NULL), data(NULL) {
    offsets = new size_t[_length + 1];
    memcpy(offsets, matrix.offsets, sizeof(size_t) * (_length + 1));
    data = new int[matrix.total()];
    memcpy(data, matrix.data, sizeof(int) * matrix.total());
}

VarMatrix::~VarMatrix() {}

void VarMatrix::release() {
    if (offsets) {
        delete []offsets;
        offsets = NULL;
    }
    if (data) {
        delete []data;
        data = NULL;
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

int& VarMatrix::operator()(int m, int n) {
    return data[offsets[m] + n];
}
