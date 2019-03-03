#include "../include/base.h"
#include "../include/cnfcontainer.h"
#include "../include/solver.h"
#include <fstream>
#include <string>
using std::ifstream;
using std::ofstream;
using std::string;

int main(int argc, char *argv[]) {
    if (argc == 2) {
        ifstream f(argv[1]);
        string s;
        f >> s;
        while (s != "p") {
            getline(f, s);
            f >> s;
        }
        f >> s;
        if (s != "cnf") {
            cout << "Invalid format" << endl;
            return 0;
        }
        size_t n_unit, n_clause;
        f >> n_unit >> n_clause;
        int pos = f.tellg();
        Vector<size_t> cnt(n_clause);
        for (size_t i = 0; i < n_clause; i++) {
            int x, cnt_i = 0;
            while ((f >> x, x != 0))
                cnt_i++;
            cnt[i] = cnt_i;
        }
        CnfContainer cnf(n_unit, n_clause, cnt);
        f.seekg(pos);
        //getline(f, s);
        for (size_t i = 0; i < n_clause; i++) {
            int x, j = 0;
            while ((f >> x, x != 0))
                cnf.data(i, j++) = x;
        }
        //cout << cnf;
        Solver solver(cnf);
        solver.solve(cout);
        //cout << "check: " << solver.check() << endl;
    }
}
