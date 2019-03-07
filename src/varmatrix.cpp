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
        throw "matrix width out of range";
    return _width[idx];
}

int* VarMatrix::operator[](size_t i) const {
    return data[i];
}

int* VarMatrix::operator[](size_t i) {
    return data[i];
}
