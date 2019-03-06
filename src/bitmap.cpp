#include "../include/bitmap.h"
#include <string.h>

BitMap::BitMap() : bias(0), table(nullptr), size(0) {}

BitMap::BitMap(uint max) : bias(0), table(nullptr), size(max + 1) {
    table = new bool[size];
    memset(table, 0, sizeof(bool) * size);
}

BitMap::BitMap(const BitMap& bm) : bias(bm.bias), size(bm.size) {
    table = new bool[size];
    memcpy(table, bm.table, sizeof(bool) * size);
}

BitMap::BitMap(int min, int max) : bias(min), size(max - min + 1) {
    table = new bool[size];
    memset(table, 0, sizeof(bool) * size);
}

BitMap::~BitMap() {
    if (table) {
        delete []table;
        table = nullptr;
    }
}

void BitMap::set(int pos) {
    table[pos - bias] = true;
}

void BitMap::unset(int pos) {
    table[pos - bias] = false;
}

void BitMap::clear() {
    memset(table, 0, sizeof(bool) * size);
}

bool BitMap::operator[](int pos) const {
    return table[pos - bias];
}

BitMap2D::BitMap2D() : _length(0), matrix(nullptr) {}

BitMap2D::BitMap2D(const Vector<size_t> &cnt) : _length(cnt.size()) {
    matrix = new bool*[_length];
    for (size_t i = 0; i < _length; i++) {
        matrix[i] = new bool[cnt[i]];
        memset(matrix[i], 0, sizeof(bool) * cnt[i]);
    }
}

BitMap2D::~BitMap2D() {
    if (matrix) {
        for (size_t i = 0; i < _length; i++)
            delete []matrix[i];
        matrix = nullptr;
    }
}

void BitMap2D::set(size_t i, size_t j) {
    matrix[i][j] = true;
}

void BitMap2D::unset(size_t i, size_t j) {
    matrix[i][j] = false;
}

bool* BitMap2D::operator[](size_t i) const {
    return matrix[i];
}
