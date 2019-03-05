#include "../include/base.h"
#include "../include/cnfcontainer.h"
#include "../include/decoder.h"
#include "../include/encoder.h"
#include "../include/generator.h"
#include "../include/solver.h"
#include "../include/sudoku.h"
#include "../include/utils.h"
#include <fstream>
#include <string>
using std::ifstream;
using std::ofstream;
using std::string;

int main(int argc, char* argv[]) {
    if (argc == 1) {
        cout << "Generating sudoku..." << endl;
        Sudoku sudoku(SudokuGenerator::generate(64));
        cout << sudoku << endl;
        cout << "Encoding sudoku..." << endl;
        Encoder enc(sudoku.to_vector());
        cout << "Generate CNF..." << endl;
//        enc.to_file("test/gg.cnf");
        CnfContainer cnf(enc.to_cnf());
        cout << "Solving SAT..." << endl;
        Solver solver(cnf);
        bool status = solver.solve(cout);
        if (status) {
            Vector<int> res(solver.get_res());
            Sudoku sudoku_sol(Decoder::decode(res));
            cout << sudoku << endl;
            cout << sudoku_sol;
        }
    } else if (argc == 2) {
        cout << "Encoding sudoku..." << endl;
        Encoder enc(argv[1]);
        cout << "Generate CNF..." << endl;
//        enc.to_file("gg.cnf");
        CnfContainer cnf(enc.to_cnf());
//        CnfContainer cnf(read_cnf_file(argv[1]));
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
