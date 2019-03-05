#include "../include/bitmap.h"
#include <string.h>

BitMap::BitMap() : n_nums(0), nums(NULL), bias(0), size(0) {}

BitMap::BitMap(uint max) : bias(0), size(max + 1) {
    n_nums = size / N_BITS;
    n_nums += size % N_BITS == 0 ? 0 : 1;
    nums = new uint[n_nums];
    memset(nums, 0, sizeof(uint) * n_nums);
}

BitMap::BitMap(const BitMap& bm) : n_nums(bm.n_nums), nums(NULL), bias(bm.bias), size(bm.size) {
    nums = new uint[n_nums];
    memcpy(nums, bm.nums, sizeof(uint) * n_nums);
}

BitMap::BitMap(int min, int max) : bias(min), size(max - min + 1) {
    n_nums = size / N_BITS;
    n_nums += size % N_BITS == 0 ? 0 : 1;
    nums = new uint[n_nums];
    memset(nums, 0, sizeof(uint) * n_nums);
}

BitMap::~BitMap() {
    if (nums) {
        delete []nums;
        nums = NULL;
    }
}

void BitMap::set(int pos) {
    pos -= bias;
    unsigned int idx = pos / N_BITS;
    unsigned int offset = pos % N_BITS;
    if (idx < n_nums)
        nums[idx] |= (1 << offset);
}

void BitMap::unset(int pos) {
    pos -= bias;
    unsigned int idx = pos / N_BITS;
    unsigned int offset = pos % N_BITS;
    if (idx < n_nums)
        nums[idx] &= ~(1 << offset);
}

void BitMap::clear() {
    memset(nums, 0, sizeof(uint) * n_nums);
}

bool BitMap::operator[](int pos) const {
    pos -= bias;
    unsigned int idx = pos / N_BITS;
    unsigned int offset = pos % N_BITS;
    if (idx < n_nums)
        return (nums[idx] >> offset) & 1;
    return false;
}
