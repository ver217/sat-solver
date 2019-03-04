#include "../include/base.h"
#include "../include/cnfcontainer.h"
#include "../include/solver.h"
#include "../include/encoder.h"
#include "../include/utils.h"
#include <fstream>
#include <string>
using std::ifstream;
using std::ofstream;
using std::string;

int main(int argc, char *argv[]) {
    if (argc == 2) {
        CnfContainer cnf(read_cnf_file(argv[1]));
        Solver solver(cnf);
        solver.solve(cout);
        //cout << "check: " << solver.check() << endl;
    }
}
