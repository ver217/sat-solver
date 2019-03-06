#include "../include/decoder.h"

Sudoku Decoder::decode(const Vector<int>& res) {
    Vector<int> solution(81);
    size_t idx = 0;
    for (size_t i = 0; i < res.size(); i++) {
        if (res[i] > 0) {
            int k = res[i] % 9;
            solution[idx++] = (k == 0 ? 9 : k);
        }
    }
    return Sudoku(solution);
}
