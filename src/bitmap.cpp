#include "../include/bitmap.h"
#include <string.h>

BitMap::BitMap() : n_nums(0), nums(NULL), size(0) {}

BitMap::BitMap(const unsigned int max) : size(max) {
    unsigned int n = max / N_BITS;
    n += max % N_BITS == 0 ? 0 : 1;
    nums = new unsigned int[n];
    n_nums = n;
    for (unsigned int i = 0; i < n; ++i)
        nums[i] = 0;
}

BitMap::BitMap(const BitMap& bm) : n_nums(bm.n_nums), nums(NULL), size(bm.size) {
    nums = new unsigned int[n_nums];
    memcpy(nums, bm.nums, sizeof(unsigned int) * n_nums);
}

BitMap::~BitMap() {
    if (nums) {
        delete []nums;
        nums = NULL;
    }
}

void BitMap::set(const unsigned int pos) {
    unsigned int idx = pos / N_BITS;
    unsigned int offset = pos % N_BITS;
    if (idx < n_nums)
        nums[idx] |= (1 << offset);
}

void BitMap::unset(const unsigned int pos) {
    unsigned int idx = pos / N_BITS;
    unsigned int offset = pos % N_BITS;
    if (idx < n_nums)
        nums[idx] &= ~(1 << offset);
}

void BitMap::clear() {
    for (unsigned int i = 0; i < size; ++i)
        nums[i] = 0;
}

bool BitMap::operator[](const unsigned int pos) const {
    unsigned int idx = pos / N_BITS;
    unsigned int offset = pos % N_BITS;
    if (idx < n_nums)
        return (nums[idx] >> offset) & 1;
    return false;
}
