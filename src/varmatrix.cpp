#include "../include/varmatrix.h"
#include <string.h>

#ifndef OLD
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

LiteralTable::LiteralTable() : n_unit(0) {}

LiteralTable::LiteralTable(size_t unit_cnt, const Vector<size_t> &cnt) : VarMatrix(cnt), n_unit(unit_cnt) {}

LiteralTable::LiteralTable(const LiteralTable &table) : n_unit(table.n_unit) {}

size_t LiteralTable::width(int x) const {
    size_t table_idx = x < 0 ? x + n_unit : x + n_unit - 1;
    if (table_idx >= _length)
        throw "matrix width out of range";
    return _width[table_idx];
}

int* LiteralTable::operator[](int x) const {
    size_t table_idx = x < 0 ? x + n_unit : x + n_unit - 1;
    return data[table_idx];
}

int* LiteralTable::operator[](int x) {
    size_t table_idx = x < 0 ? x + n_unit : x + n_unit - 1;
    return data[table_idx];
}

#else
VarMatrix::VarMatrix() : _length(0), offsets(nullptr), data(nullptr) {}

VarMatrix::VarMatrix(const Vector<size_t> &cnt) : _length(cnt.size()), offsets(nullptr), data(nullptr) {
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

VarMatrix::~VarMatrix() {
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

size_t VarMatrix::get_idx(size_t m, size_t n) const {
    return offsets[m] + n;
}

int& VarMatrix::operator()(size_t m, size_t n) {
    return data[get_idx(m, n)];
}

int VarMatrix::operator()(size_t m, size_t n) const {
    return data[get_idx(m, n)];
}
#endif
