#include "../include/utils.h"
#include <fstream>
using std::ifstream;

CnfContainer read_cnf_file(string filename) {
    ifstream f(filename);
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
    for (size_t i = 0; i < n_clause; i++) {
        int x, cnt_i = 0;
        while ((f >> x, x != 0))
            cnt_i++;
        cnt.push_back(cnt_i);
    }
    CnfContainer cnf(n_unit, n_clause, cnt);
    f.seekg(pos);
    //getline(f, s);
    for (size_t i = 0; i < n_clause; i++) {
        int x, j = 0;
        while ((f >> x, x != 0))
            cnf.data(i, j++) = x;
    }
    f.close();
    return cnf;
}
