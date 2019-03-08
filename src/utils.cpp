#include "../include/utils.h"
#include <fstream>
using std::ifstream;

CnfContainer read_cnf_file(string filename) {
    ifstream f(filename);
    if (!f)
        throw "No such file";
    string s;
    f >> s;
    while (s != "p") {
        getline(f, s);
        f >> s;
    }
    f >> s;
    if (s != "cnf")
        throw "Invalid format";
    size_t n_unit, n_clause;
    f >> n_unit >> n_clause;
    int pos = f.tellg();
    Vector<size_t> cnt(n_clause);
    Vector<size_t> literal_cnt(n_unit * 2);
    for (size_t i = 0; i < n_clause; i++) {
        int x, cnt_i = 0;
        while ((f >> x, x != 0)) {
            cnt_i++;
            size_t table_idx = x < 0 ? x + n_unit : x + n_unit - 1;
            literal_cnt[table_idx]++;
        }
        cnt[i] = cnt_i;
    }
    CnfContainer cnf(n_unit, n_clause, cnt, literal_cnt);
    f.seekg(pos);
    Vector<size_t> literal_table_pos(n_unit * 2);
    for (size_t i = 0; i < n_clause; i++) {
        int x, j = 0;
        while ((f >> x, x != 0)) {
#ifndef OLD
            cnf.data[i][j++] = x;
            size_t table_idx = x < 0 ? x + n_unit : x + n_unit - 1;
            cnf.literal_table[table_idx][literal_table_pos[table_idx]++] = static_cast<int>(i);
#else
            cnf.data(i, j++) = x;
#endif
        }
    }
    f.close();
    return cnf;
}
