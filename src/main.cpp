#include "../include/base.h"
#include "../include/cnfcontainer.h"
#include "../include/solver.h"
#include "../include/encoder.h"
#include "../include/utils.h"
#include "../include/sudoku.h"
#include "../include/decoder.h"
#include <fstream>
#include <string>
using std::ifstream;
using std::ofstream;
using std::string;

int main(int argc, char *argv[]) {
    if (argc == 2) {
        cout << "Reading sudoku..." << endl;
        Encoder enc(argv[1]);
        cout << "Generate CNF..." << endl;
        //enc.to_file("gg.cnf");
        CnfContainer cnf(enc.to_cnf());
        cout << "Solving SAT..." << endl;
        Solver solver(cnf);
        bool status = solver.solve(cout);
        if (status) {
            Vector<int> res(solver.get_res());
            Sudoku sudoku(Decoder::decode(res));
            cout << sudoku;
        }
    }
}
