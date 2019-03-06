#include "../include/varmatrix.h"
#include <string.h>

VarMatrix::VarMatrix() : _length(0), _width(nullptr), data(nullptr) {}

VarMatrix::VarMatrix(const Vector<size_t> &cnt) : _length(cnt.size()), _width(nullptr), data(nullptr) {
    _width = new size_t[_length];
    memcpy(_width, cnt.data, sizeof(size_t) * _length);
    data = new int*[_length];
    for (size_t i = 0; i < _length; i++) {
        data[i] = new int[_width[i]];
        memset(data[i], 0, sizeof(int) * _width[i]);
    }
}

VarMatrix::VarMatrix(const VarMatrix &matrix) : _length(matrix.length()), _width(nullptr), data(nullptr) {
    _width = new size_t[_length];
    memcpy(_width, matrix._width, sizeof(size_t) * _length);
    data = new int*[_length];
    for (size_t i = 0; i < _length; i++) {
        data[i] = new int[_width[i]];
        memcpy(data[i], matrix.data[i], sizeof(int) * _width[i]);
    }
}

VarMatrix::~VarMatrix() {
    release();
}

void VarMatrix::release() {
    if (_width) {
        delete []_width;
        _width = nullptr;
    }
    if (data) {
        for (size_t i = 0; i < _length; i++)
            delete []data[i];
        data = nullptr;
    }
}

size_t VarMatrix::length() const {
    return _length;
}

size_t VarMatrix::width(size_t idx) const {
    if (idx >= _length)
        throw "out of range";
    return _width[idx];
}

size_t VarMatrix::total() const {
//    size_t total = 0;
//    for (size_t i = 0; i < _length; i++)
//        total += _width[i];
//    return total;
    return _length * 32;
}

size_t VarMatrix::get_idx(size_t m, size_t n) const {
//    return offsets[m] + n;
    return 32 * m + n;
}

int& VarMatrix::operator()(size_t m, size_t n) {
    return data[m][n];
}

int VarMatrix::operator()(size_t m, size_t n) const {
    return data[m][n];
}
