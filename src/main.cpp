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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using std::ifstream;
using std::ofstream;
using std::string;

inline void clear() {
#if defined(__linux) || defined(__linux__) || defined(linux) || defined(__APPLE__)
    system("clear");
#else
    system("cls");
#endif
}

int main() {
    srand(time(0));
    int op = 1;
    while (op) {
        clear();
        cout << "  Sudoku or SAT   " << endl;
        cout << "------------------" << endl;
        cout << "  1. Sudoku" << endl;
        cout << "  2. SAT" << endl;
        cout << "  0. Exit   " << endl;
        cout << "------------------" << endl;
        cout << "  Choose operation [0~2]: ";
        cin >> op;
        if (cin.fail()) {
            cin.clear();
            cin.get();
            op = -1;
        }
        switch (op) {
        case 1: {
            cout << "Generating Sudoku..." << endl;
            Sudoku sudoku(SudokuGenerator::generate(64));
            Encoder enc(sudoku.to_vector());
            int op1 = 1;
            while (op1) {
                clear();
                cout << sudoku << endl;
                cout << "  Sudoku" << endl;
                cout << "------------------" << endl;
                cout << "  1. Dump CNF File" << endl;
                cout << "  2. Solve Sudoku" << endl;
                cout << "  3. Dump and Solve" << endl;
                cout << "  0. Exit   " << endl;
                cout << "------------------" << endl;
                cout << "  Choose operation [0~3]: ";
                cin >> op1;
                if (cin.fail()) {
                    cin.clear();
                    cin.get();
                    op1 = -1;
                }
                switch (op1) {
                case 1: {
                    string filename;
                    cout << "Input filename: ";
                    cin >> filename;
                    cout << "Generate CNF..." << endl;
                    enc.to_file(filename);
                    cout << "Done~" << endl;
                    op1 = 0;
                    cout << "Press enter to continue ";
                    getchar();
                    getchar();
                    break;
                }
                case 2: {
                    clock_t start = clock();
                    CnfContainer cnf(enc.to_cnf());
                    Solver solver(cnf);
                    bool status = solver.solve();
                    if (status) {
                        Vector<int> res(solver.get_res());
                        Sudoku sudoku_sol(Decoder::decode(res));
                        cout << sudoku_sol << endl;
                    }
                    clock_t end = clock();
                    cout << "Total time: " << static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000 << endl;
                    op1 = 0;
                    cout << "Press enter to continue ";
                    getchar();
                    getchar();
                    break;
                }
                case 3: {
                    string filename;
                    cout << "Input filename: ";
                    cin >> filename;
                    cout << "Generate CNF..." << endl;
                    enc.to_file(filename);
                    cout << "Done~" << endl;
                    clock_t start = clock();
                    CnfContainer cnf(enc.to_cnf());
                    Solver solver(cnf);
                    bool status = solver.solve();
                    if (status) {
                        Vector<int> res(solver.get_res());
                        Sudoku sudoku_sol(Decoder::decode(res));
                        cout << sudoku_sol << endl;
                    }
                    clock_t end = clock();
                    cout << "Total time: " << static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000 << endl;
                    op1 = 0;
                    cout << "Press enter to continue ";
                    getchar();
                    getchar();
                    break;
                }
                case 0:
                    break;
                default: {
                    op1 = 1;
                    break;
                }
                }
            }
            break;
        }
        case 2: {
            string filename;
            cout << "Input CNF filename: ";
            cin >> filename;
            try {
                CnfContainer cnf(read_cnf_file(filename));
                int op2 = 1;
                while (op2) {
                    clear();
                    cout << "  SAT" << endl;
                    cout << "------------------" << endl;
                    cout << "  1. Display CNF" << endl;
                    cout << "  2. Solve SAT" << endl;
                    cout << "  3. Solve and Dump" << endl;
                    cout << "  0. Exit   " << endl;
                    cout << "------------------" << endl;
                    cout << "  Choose operation [0~3]: ";
                    cin >> op2;
                    if (cin.fail()) {
                        cin.clear();
                        cin.get();
                        op2 = -1;
                    }
                    switch (op2) {
                        case 1: {
                            cout << cnf << endl;
                            cout << "Press enter to continue ";
                            getchar();
                            getchar();
                            break;
                        }
                        case 2: {
                            cout << "Solving SAT..." << endl;
                            Solver solver(cnf);
                            solver.solve(cout, false);
                            cout << "Press enter to continue ";
                            getchar();
                            getchar();
                            break;
                        }
                        case 3: {
                            string rst_filename = filename;
                            rst_filename.replace(rst_filename.end() - 3, rst_filename.end(), "rst");
                            ofstream f(rst_filename);
                            cout << "Solving SAT..." << endl;
                            Solver solver(cnf);
                            solver.solve(f, true);
                            cout << "Press enter to continue ";
                            getchar();
                            getchar();
                            break;
                        }
                        case 0:
                            break;
                        default: {
                            op2 = 1;
                            break;
                        }
                    }
                }
            } catch (const char *const msg) {
                cout << msg << endl;
                cout << "Press enter to continue ";
                getchar();
                getchar();
            }
            break;
        }
        case 0:
            break;
        default: {
            op = 1;
            break;
        }
        }
    }
    cout << "Bye~" << endl;
}
