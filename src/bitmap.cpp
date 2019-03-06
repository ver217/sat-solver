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
