#include "../include/encoder.h"
#include "../include/base.h"
#include <fstream>
using std::ofstream;
using std::ifstream;

Encoder::Encoder() : var_cnt(0), clause_cnt(0) {}

Encoder::Encoder(string filename) : var_cnt(729) {
    size_t cnt = 0, row = 1, col = 1;
    char def[81];
    ifstream f(filename);
    if (!f)
        throw "No such file";
    char c;
    size_t i = 0;
    while ((f >> c, !f.eof())) {
        if ('0' <= c && c <= '9')
            def[i++] = c;
    }
    if (i != 81)
        throw "Invalid sudoku file";
    f.close();
    for (i = 0; i < 81; i++) {
        if (col == 10) {
            row++;
            col = 1;
        }
        int num = def[i] - '0';
        if (num != 0) {
            int input_num = 81 * (row - 1) + 9 * (col - 1) + num;
            res.push_back(input_num);
            res.push_back(0);
            cnt++;
        }
        col++;
    }
    clause_cnt = 11988 + cnt;
    cell_atleast_one();
    row_atmost_one();
    col_atmost_one();
    square_atmost_one();
    // extended
    cell_atmost_one();
    row_atleast_one();
    col_atleast_one();
    square_atleast_one();
}

Encoder::Encoder(const Vector<int>& def) : var_cnt(729) {
    size_t cnt = 0, row = 1, col = 1;
    for (size_t i = 0; i < def.size(); i++) {
        if (col == 10) {
            row++;
            col = 1;
        }
        if (def[i] != 0) {
            int input_num = 81 * (row - 1) + 9 * (col - 1) + def[i];
            res.push_back(input_num);
            res.push_back(0);
            cnt++;
        }
        col++;
    }
    clause_cnt = 11988 + cnt;
    cell_atleast_one();
    row_atmost_one();
    col_atmost_one();
    square_atmost_one();
    // extended
    cell_atmost_one();
    row_atleast_one();
    col_atleast_one();
    square_atleast_one();
}

Encoder::~Encoder() {}

void Encoder::to_file(string filename) {
    ofstream f(filename);
    f << "p cnf " << var_cnt << ' ' << clause_cnt << endl;
    for (size_t i = 0; i < res.size(); i++) {
        f << res[i];
        if (res[i] == 0)
            f << endl;
        else
            f << ' ';
    }
    f.close();
}

CnfContainer Encoder::to_cnf() {
    Vector<size_t> cnt(clause_cnt);
    int cnt_i = 0;
    size_t idx = 0;
    for (size_t i = 0; i < res.size(); i++) {
        if (res[i] == 0) {
            cnt[idx++] = cnt_i;
            cnt_i = 0;
        } else
            cnt_i++;
    }
    CnfContainer cnf(var_cnt, clause_cnt, cnt);
    size_t i = 0, j = 0;
    for (size_t k = 0; k < res.size(); k++) {
        if (res[k] == 0) {
            j = 0;
            i++;
        } else
            cnf.data(i, j++) = res[k];
    }
    return cnf;
}

void Encoder::cell_atleast_one() {
    for (size_t i = 1; i < 10; i++) {
        for (size_t j = 1; j < 10; j++) {
            for (size_t k = 1; k < 10; k++) {
                int input_num = 81 * (i - 1) + 9 * (j - 1) + (k - 1) + 1;
                res.push_back(input_num);
            }
            res.push_back(0);
        }
    }
}

void Encoder::row_atmost_one() {
    for (size_t i = 1; i < 10; i++) {
        for (size_t k = 1; k < 10; k++) {
            for (size_t j = 1; j < 9; j++) {
                for (size_t l = j + 1; l < 10; l++) {
                    int first_num = 81 * (i - 1) + 9 * (j - 1) + (k - 1) + 1;
                    int second_num = 81 * (i - 1) + 9 * (l - 1) + (k - 1) + 1;
                    res.push_back(-first_num);
                    res.push_back(-second_num);
                    res.push_back(0);
                }
            }
        }
    }
}
void Encoder::col_atmost_one() {
    for (size_t j = 1; j < 10; j++) {
        for (size_t k = 1; k < 10; k++) {
            for (size_t i = 1; i < 9; i++) {
                for (size_t l = i + 1; l < 10; l++) {
                    int first_num = 81 * (i - 1) + 9 * (j - 1) + (k - 1) + 1;
                    int second_num = 81 * (l - 1) + 9 * (j - 1) + (k - 1) + 1;
                    res.push_back(-first_num);
                    res.push_back(-second_num);
                    res.push_back(0);
                }
            }
        }
    }
}
void Encoder::square_atmost_one() {
    for (size_t k = 1; k < 10; k++) {
        for (size_t a = 0; a < 3; a++) {
            for (size_t b = 0; b < 3; b++) {
                for (size_t u = 1; u < 4; u++) {
                    for (size_t v = 1; v < 3; v++) {
                        for (size_t w = v + 1; w < 4; w++) {
                            int first_num = 81 * ((3 * a + u) - 1) + 9 * ((3 * b + v) - 1) + (k - 1) + 1;
                            int second_num = 81 * ((3 * a + u) - 1) + 9 * ((3 * b + w) - 1) + (k - 1) + 1;
                            res.push_back(-first_num);
                            res.push_back(-second_num);
                            res.push_back(0);
                        }
                    }
                }
            }
        }
    }
    for (size_t k = 1; k < 10; k++) {
        for (size_t a = 0; a < 3; a++) {
            for (size_t b = 0; b < 3; b++) {
                for (size_t u = 1; u < 3; u++) {
                    for (size_t v = 1; v < 4; v++) {
                        for (size_t w = u + 1; w < 4; w++) {
                            for (size_t t = 1; t < 4; t++) {
                                int first_num = 81 * ((3 * a + u) - 1) + 9 * ((3 * b + v) - 1) + (k - 1) + 1;
                                int second_num = 81 * ((3 * a + w) - 1) + 9 * ((3 * b + t) - 1) + (k - 1) + 1;
                                res.push_back(-first_num);
                                res.push_back(-second_num);
                                res.push_back(0);
                            }
                        }
                    }
                }
            }
        }
    }
}

// The functions below are for the extended encoding
void Encoder::cell_atmost_one() {
    for (size_t x = 1; x < 10; x++) {
        for (size_t y = 1; y < 10; y++) {
            for (size_t z = 1; z < 9; z++) {
                for (size_t i = z + 1; i < 10; i++) {
                    int first_num = 81 * (x - 1) + 9 * (y - 1) + (z - 1) + 1;
                    int second_num = 81 * (x - 1) + 9 * (y - 1) + (i - 1) + 1;
                    res.push_back(-first_num);
                    res.push_back(-second_num);
                    res.push_back(0);
                }
            }
        }
    }
}
void Encoder::row_atleast_one() {
    for (size_t y = 1; y < 10; y++) {
        for (size_t z = 1; z < 10; z++) {
            for (size_t x = 1; x < 10; x++) {
                int input_num = 81 * (x - 1) + 9 * (y - 1) + (z - 1) + 1;
                res.push_back(input_num);
            }
            res.push_back(0);
        }
    }
}
void Encoder::col_atleast_one() {
    for (size_t x = 1; x < 10; x++) {
        for (size_t z = 1; z < 10; z++) {
            for (size_t y = 1; y < 10; y++) {
                int input_num = 81 * (x - 1) + 9 * (y - 1) + (z - 1) + 1;
                res.push_back(input_num);
            }
            res.push_back(0);
        }
    }
}
void Encoder::square_atleast_one() {
    for (size_t z = 1; z < 10; z++) {
        for (size_t i = 0; i < 3; i++) {
            for (size_t j = 0; j < 3; j++) {
                for (size_t x = 1; x < 4; x++) {
                    for (size_t y = 1; y < 4; y++) {
                        int input_num = 81 * ((3 * i + x) - 1) + 9 * ((3 * j + y) - 1) + (z - 1) + 1;
                        res.push_back(input_num);
                    }
                }
                res.push_back(0);
            }
        }
    }
}
