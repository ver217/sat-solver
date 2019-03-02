#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define MAX_CLAUSE_LEN 16

struct variable_info_t;
using two_size_t = size_t[2];
using variable_info_pair_t = variable_info_t[2];
using bool_vector_t = bool[16];

inline int Abs(int l) {
    return l > 0 ? l : -l;
}

struct variable_info_t {
    size_t *deleted_clause_index = NULL;
    size_t deleted_clause_length = 0;
    two_size_t *deleted_variable_index = NULL;
    size_t deleted_variable_length = 0;
};

struct solution_t {
    int *data = nullptr;
    size_t length = 0;
    
    solution_t(size_t length) : length(length) {
        data = new int[length];
    }
};

struct formula_t {
    /* need to be kept 👇 */
    int **formula = nullptr;
    
    /* need to be copied 👇 */
    size_t clause_count = 0;                        // size_t, current_count of all clauses
    size_t current_clause_count = 0;                // size_t, count of all clauses
    size_t variable_count = 0;                      // size_t, count of all variables
    size_t *clause_length = nullptr;                // size_t[], length of all clauses
    size_t *current_clause_length = nullptr;        // size_t[], current_length of all clauses
    bool *is_clause_hidden = nullptr;               // bool[], if a clause is hidden
    bool_vector_t *is_literal_hidden = nullptr;     // bool[][32], if a literial is hidden
    two_size_t *literal_referenced_count = nullptr; // size_t[][2], count of reference
    
    formula_t Copy() {
        formula_t ret;
        ret.formula = formula;
        ret.clause_count = clause_count;
        ret.current_clause_count = current_clause_count;
        ret.variable_count = variable_count;
        ret.clause_length = new size_t[clause_count];
        ret.current_clause_length = new size_t[clause_count];
        ret.is_clause_hidden = new bool[clause_count];
        ret.is_literal_hidden = new bool_vector_t[clause_count];
        ret.literal_referenced_count = new two_size_t[variable_count];
        for (size_t i = 0; i < clause_count; i++) {
            ret.clause_length[i] = clause_length[i];
            ret.current_clause_length[i] = current_clause_length[i];
            ret.is_clause_hidden[i] = is_clause_hidden[i];
            memcpy(ret.is_literal_hidden[i], is_literal_hidden[i], sizeof(bool_vector_t));
        }
        for (size_t i = 0; i < variable_count; i++)
            memcpy(ret.literal_referenced_count[i], literal_referenced_count[i], sizeof(two_size_t));
        return ret;
    };
    
    int PickOne() {
        for (size_t i = 0; i < clause_count; i++)
            for (size_t j = 0; j < clause_length[i]; j++)
                if (!is_clause_hidden[i] && !is_literal_hidden[i][j])
                    return formula[i][j];
        return 0;
    }
    
    int PickUnit() {
        for (size_t i = 0; i < clause_count; i++)
            if (!is_clause_hidden[i] && current_clause_length[i] == 1)
                for (size_t j = 0; j < clause_length[i]; j++)
                    if (!is_literal_hidden[i][j])
                        return formula[i][j];
        return 0;
    }
    
    void PrintFormula(FILE *target = stdout) {
        fprintf(target, "current_clause_count: %lu\n", current_clause_count);
        fprintf(target, "clause_count: %lu\n", clause_count);
        for (size_t i = 0; i < clause_count; i++) {
            if (is_clause_hidden[i])
                continue;
            fprintf(target, "line: %3lu, length: %lu, hidden: %d:", i + 1, current_clause_length[i], is_clause_hidden[i]);
            for (size_t j = 0; j < clause_length[i]; j++)
                if (!is_literal_hidden[i][j])
                    fprintf(target, "%4d,%d ", formula[i][j], is_literal_hidden[i][j]);
            fprintf(target, "\n");
        }
    }
    
    void PrintRef(FILE *target = stdout) {
        for (size_t i = 0; i < variable_count; i++)
            fprintf(target, "%4lu: %4lu,%4lu\n", i + 1, literal_referenced_count[i][1], literal_referenced_count[i][0]);
    }
    
    void IncrRef(int l) {
        literal_referenced_count[Abs(l) - 1][l > 0 ? 1 : 0]++;
    }
    
    void DecrRef(int l) {
        literal_referenced_count[Abs(l) - 1][l > 0 ? 1 : 0]--;
    }
    
    void Release() {
        delete[] clause_length;
        delete[] current_clause_length;
        delete[] is_clause_hidden;
        delete[] is_literal_hidden;
        delete[] literal_referenced_count;
        clause_count = 0;
        current_clause_count = 0;
        variable_count = 0;
    };
    
    void PrintAsCNF(FILE *target = stdout) {
        fprintf(target, "p cnf %lu %lu\n", variable_count, current_clause_count);
        for (size_t i = 0; i < clause_count; i++) {
            if (is_clause_hidden[i])
                continue;
            for (size_t j = 0; j < clause_length[i]; j++)
                if (!is_literal_hidden[i][j])
                    fprintf(target, "%d ", formula[i][j]);
            fprintf(target, "0\n");
        }
    }
    
    static formula_t FromFile(FILE *f) {
        formula_t ret;
        char c = '\n';
        while (true) {
            char pre = c;
            fread(&c, sizeof(char), 1, f);
            if (pre == '\n' && c == 'p')
                break;
        }
        fscanf(f, " cnf %lu %lu\n", &ret.variable_count, &ret.clause_count);
        if (ret.variable_count == 0 || ret.clause_count == 0)
            throw;
        ret.literal_referenced_count = new two_size_t[ret.variable_count];
        ret.formula = new int *[ret.clause_count];
        ret.is_literal_hidden = new bool_vector_t[ret.clause_count];
        ret.clause_length = new size_t[ret.clause_count];
        ret.current_clause_length = new size_t[ret.clause_count];
        ret.is_clause_hidden = new bool[ret.clause_count];
        ret.current_clause_count = ret.clause_count;
        for (size_t i = 0; i < ret.clause_count; i++) {
            ret.formula[i] = NULL;
            ret.clause_length[i] = 0;
            ret.current_clause_length[i] = 0;
            ret.is_clause_hidden[i] = false;
        }
        int var;
        size_t i = 0;
        int temp_var[MAX_CLAUSE_LEN];
        bool temp_hidden[MAX_CLAUSE_LEN];
        while (fscanf(f, "%d", &var) == 1)
            // read all vars
            if (var == 0) {
                ret.formula[i] = new int[ret.clause_length[i]];
                // ret.is_literal_hidden[i] = new bool[ret.clause_length[i]];
                memcpy(ret.formula[i], temp_var, sizeof(int) * ret.clause_length[i]);
                memcpy(ret.is_literal_hidden[i], temp_hidden, sizeof(bool_vector_t));
                i++;
            } else {
                temp_var[ret.clause_length[i]] = var;
                temp_hidden[ret.clause_length[i]] = false;
                ret.clause_length[i]++;
                ret.current_clause_length[i]++;
                ret.IncrRef(var);
            }
        return ret;
    }
};

class sudoku_t {
  private:
    unsigned int sqrt_size = 0;
    
    inline bool UnusedInRow(int i, int num) {
        for (int j = 0; j < size; j++)
            if (numbers[i][j] == num)
                return false;
        return true;
    }
    
    inline bool UnusedInCol(int j, int num) {
        for (int i = 0; i < size; i++)
            if (numbers[i][j] == num)
                return false;
        return true;
    }
    
    inline bool UnusedInBox(int rowStart, int colStart, int num) {
        for (int i = 0; i < sqrt_size; i++)
            for (int j = 0; j < sqrt_size; j++)
                if (numbers[rowStart + i][colStart + j] == num)
                    return false;
        return true;
    }
    
    inline bool FillRemaining(int i, int j) {
        if (j >= size && i < size - 1) {
            i = i + 1;
            j = 0;
        }
        if (i >= size && j >= size)
            return true;
        if (i < sqrt_size) {
            if (j < sqrt_size)
                j = sqrt_size;
        } else if (i < size - sqrt_size) {
            if (j == (int)(i / sqrt_size) * sqrt_size)
                j = j + sqrt_size;
        } else {
            if (j == size - sqrt_size) {
                i = i + 1;
                j = 0;
                if (i >= size)
                    return true;
            }
        }
        for (int num = 1; num <= size; num++) {
            if (UnusedInRow(i, num) &&
                    UnusedInCol(j, num) &&
                    UnusedInBox(i - i % sqrt_size, j - j % sqrt_size, num)) {
                numbers[i][j] = num;
                if (FillRemaining(i, j + 1))
                    return true;
                numbers[i][j] = 0;
            }
        }
        return false;
    }
    
  public:
    int **numbers = nullptr;
    unsigned int size = 0;
    sudoku_t(int **numbers, unsigned int size) : numbers(numbers),
        size(size),
        sqrt_size((unsigned int)sqrt(size)) {}
    sudoku_t(unsigned int size) : size(size) {
        sqrt_size = (unsigned int)sqrt(size);
        numbers = new int *[size];
        for (size_t i = 0; i < size; i++) {
            numbers[i] = new int[size];
            for (size_t j = 0; j < size; j++)
                numbers[i][j] = 0;
        }
    }
    
    void PrintSudoku(FILE *file) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++)
                fprintf(file, "%3d", numbers[i][j]);
            fprintf(file, "\n");
        }
        fprintf(file, "\n");
    }
    
    static inline int ThreeSubscriptToVariable(int x, int y, int z, int size) {
        return (size * size) * (x - 1) + (size) * (y - 1) + (z - 1) + 1;
    }
    
    static inline void VariableToThreeSubscript(int variable, int size, int &x, int &y, int &z) {
        // return (size * size) * (x - 1) + (size) * (y - 1) + (z - 1) + 1;
        variable = Abs(variable) - 1;
        x = variable / (size * size) + 1;
        y = variable / size % size + 1;
        z = variable % size + 1;
    }
    
    static formula_t EncodeSoduku(sudoku_t sudoku) {
        auto size = sudoku.size;
        auto sqrt_size = sudoku.sqrt_size;
        size_t new_size = 0;
        for (size_t i = 0; i < size; i++)
            for (size_t j = 0; j < size; j++)
                if (sudoku.numbers[i][j] > 0 && sudoku.numbers[i][j] <= (int)size)
                    new_size++;
        for (size_t y = 1; y < size + 1; y++)
            for (size_t z = 1; z < size + 1; z++)
                new_size++;
        for (size_t y = 1; y < size + 1; y++)
            for (size_t z = 1; z < size + 1; z++)
                for (size_t x = 1; x < size; x++)
                    for (size_t i = x + 1; i < size + 1; i++)
                        new_size++;
        for (size_t x = 1; x < size + 1; x++)
            for (size_t z = 1; z < size + 1; z++)
                for (size_t y = 1; y < size; y++)
                    for (size_t i = y + 1; i < size + 1; i++)
                        new_size++;
        for (size_t z = 1; z < size + 1; z++)
            for (size_t i = 0; i < sqrt_size; i++)
                for (size_t j = 0; j < sqrt_size; j++)
                    for (size_t x = 1; x < sqrt_size + 1; x++)
                        for (size_t y = 1; y < sqrt_size + 1; y++)
                            for (size_t k = y + 1; k < sqrt_size + 1; k++)
                                new_size++;
        for (size_t z = 1; z < size + 1; z++)
            for (size_t i = 0; i < sqrt_size; i++)
                for (size_t j = 0; j < sqrt_size; j++)
                    for (size_t x = 1; x < sqrt_size + 1; x++)
                        for (size_t y = 1; y < sqrt_size + 1; y++)
                            for (size_t k = x + 1; k < sqrt_size + 1; k++)
                                for (size_t l = 1; l < sqrt_size + 1; l++)
                                    new_size++;
        formula_t ret;
        ret.clause_count = new_size;
        ret.current_clause_count = new_size;
        ret.variable_count = size * size * size;
        ret.formula = new int *[new_size];
        ret.clause_length = new size_t[new_size];
        ret.current_clause_length = new size_t[new_size];
        ret.is_clause_hidden = new bool[new_size];
        ret.is_literal_hidden = new bool_vector_t[new_size];
        ret.literal_referenced_count = new two_size_t[new_size];
        memset(ret.clause_length, 0, sizeof(size_t) * new_size);
        memset(ret.current_clause_length, 0, sizeof(size_t) * new_size);
        memset(ret.is_clause_hidden, false, sizeof(bool) * new_size);
        memset(ret.is_literal_hidden, false, sizeof(bool_vector_t) * new_size);
        memset(ret.literal_referenced_count, 0, sizeof(two_size_t) * new_size);
        new_size = 0;
        for (size_t i = 0; i < size; i++)
            for (size_t j = 0; j < size; j++)
                if (sudoku.numbers[i][j] > 0 && sudoku.numbers[i][j] <= (int)size) {
                    ret.clause_length[new_size] = 1;
                    ret.current_clause_length[new_size] = 1;
                    ret.formula[new_size] = new int[1];
                    auto t = ret.formula[new_size][0] = ThreeSubscriptToVariable(i + 1, j + 1, sudoku.numbers[i][j], size);
                    ret.IncrRef(t);
                    new_size++;
                }
        for (size_t x = 1; x < size + 1; x++)
            for (size_t y = 1; y < size + 1; y++) {
                ret.clause_length[new_size] = size;
                ret.current_clause_length[new_size] = size;
                ret.formula[new_size] = new int[size];
                for (size_t z = 1; z < size + 1; z++) {
                    auto t = ret.formula[new_size][z - 1] = ThreeSubscriptToVariable(x, y, z, size);
                    ret.IncrRef(t);
                }
                new_size++;
            }
        for (size_t y = 1; y < size + 1; y++)
            for (size_t z = 1; z < size + 1; z++)
                for (size_t x = 1; x < size; x++)
                    for (size_t i = x + 1; i < size + 1; i++) {
                        ret.clause_length[new_size] = 2;
                        ret.current_clause_length[new_size] = 2;
                        ret.formula[new_size] = new int[2];
                        auto t1 = ret.formula[new_size][0] = -ThreeSubscriptToVariable(x, y, z, size);
                        auto t2 = ret.formula[new_size][1] = -ThreeSubscriptToVariable(i, y, z, size);
                        ret.IncrRef(t1);
                        ret.IncrRef(t2);
                        new_size++;
                    }
        for (size_t x = 1; x < size + 1; x++)
            for (size_t z = 1; z < size + 1; z++)
                for (size_t y = 1; y < size; y++)
                    for (size_t i = y + 1; i < size + 1; i++) {
                        ret.clause_length[new_size] = 2;
                        ret.current_clause_length[new_size] = 2;
                        ret.formula[new_size] = new int[2];
                        auto t1 = ret.formula[new_size][0] = -ThreeSubscriptToVariable(x, y, z, size);
                        auto t2 = ret.formula[new_size][1] = -ThreeSubscriptToVariable(x, i, z, size);
                        ret.IncrRef(t1);
                        ret.IncrRef(t2);
                        new_size++;
                    }
        for (size_t z = 1; z < size + 1; z++)
            for (size_t i = 0; i < sqrt_size; i++)
                for (size_t j = 0; j < sqrt_size; j++)
                    for (size_t x = 1; x < sqrt_size + 1; x++)
                        for (size_t y = 1; y < sqrt_size + 1; y++)
                            for (size_t k = y + 1; k < sqrt_size + 1; k++) {
                                ret.clause_length[new_size] = 2;
                                ret.current_clause_length[new_size] = 2;
                                ret.formula[new_size] = new int[2];
                                auto t1 = ret.formula[new_size][0] = -ThreeSubscriptToVariable((sqrt_size * i + x), (sqrt_size * j + y), z, size);
                                auto t2 = ret.formula[new_size][1] = -ThreeSubscriptToVariable((sqrt_size * i + x), (sqrt_size * j + k), z, size);
                                ret.IncrRef(t1);
                                ret.IncrRef(t2);
                                new_size++;
                            }
        for (size_t z = 1; z < size + 1; z++)
            for (size_t i = 0; i < sqrt_size; i++)
                for (size_t j = 0; j < sqrt_size; j++)
                    for (size_t x = 1; x < sqrt_size + 1; x++)
                        for (size_t y = 1; y < sqrt_size + 1; y++)
                            for (size_t k = x + 1; k < sqrt_size + 1; k++)
                                for (size_t l = 1; l < sqrt_size + 1; l++) {
                                    ret.clause_length[new_size] = 2;
                                    ret.current_clause_length[new_size] = 2;
                                    ret.formula[new_size] = new int[2];
                                    auto t1 = ret.formula[new_size][0] = -ThreeSubscriptToVariable((sqrt_size * i + x), (sqrt_size * j + y), z, size);
                                    auto t2 = ret.formula[new_size][1] = -ThreeSubscriptToVariable((sqrt_size * i + k), (sqrt_size * j + l), z, size);
                                    ret.IncrRef(t1);
                                    ret.IncrRef(t2);
                                    new_size++;
                                }
        return ret;
    }
    
    static sudoku_t Generate(unsigned int size, unsigned int holes_count) {
        sudoku_t ret(size);
        auto sqrt_size = ret.sqrt_size;
        for (size_t k = 0; k < size; k += sqrt_size) {
            int num;
            for (size_t i = 0; i < sqrt_size; i++) {
                for (size_t j = 0; j < sqrt_size; j++) {
                    do
                        num = (rand() % (size)) + 1;
                    while (!ret.UnusedInBox(k, k, num));
                    ret.numbers[k + i][k + j] = num;
                }
            }
        }
        ret.FillRemaining(0, sqrt_size);
        while (holes_count != 0) {
            size_t id = rand() % (size * size);
            size_t i = id / size;
            size_t j = id % size;
            if (j != 0)
                j = j - 1;
            if (ret.numbers[i][j] != 0)
                holes_count--, ret.numbers[i][j] = 0;
        }
        return ret;
    }
    
    static sudoku_t FromSolution(solution_t solution, unsigned int size) {
        // solution.data
        sudoku_t s(size);
        size_t positive_count = 0;
        for (size_t i = 0; i < solution.length; i++)
            if (solution.data[i] > 0)
                positive_count++;
        if (positive_count != size * size)
            throw;
        for (size_t i = 0; i < solution.length; i++)
            if (solution.data[i] > 0) {
                int x, y, z;
                VariableToThreeSubscript(solution.data[i], size, x, y, z);
                s.numbers[x - 1][y - 1] = z;
            }
        return s;
    }
};

class old_solver_t {
  public:
    formula_t f;
    
  private:
    int *variable_stack = nullptr;
    size_t variable_stack_length = 0;
    solution_t solution;
    
    // Use `SetVar(-v)` to set variable v to `0`. Use `SetVar(v)` to set variable v to `1`
    void SetVar(formula_t &f, int l) {
        for (size_t i = 0; i < f.clause_count; i++)
            for (size_t j = 0; j < f.clause_length[i]; j++)
                if (!f.is_clause_hidden[i] && !f.is_literal_hidden[i][j] && f.formula[i][j] == l) {
                    f.is_clause_hidden[i] = true;
                    for (size_t k = 0; k < f.clause_length[i]; k++)
                        if (!f.is_literal_hidden[i][k])
                            f.DecrRef(f.formula[i][k]);
                    f.current_clause_count--;
                }
        for (size_t i = 0; i < f.clause_count; i++)
            for (size_t j = 0; j < f.clause_length[i]; j++)
                if (!f.is_clause_hidden[i] && !f.is_literal_hidden[i][j] && f.formula[i][j] == -l) {
                    f.is_literal_hidden[i][j] = true;
                    f.DecrRef(f.formula[i][j]);
                    f.current_clause_length[i]--;
                }
        variable_stack[variable_stack_length++] = l;
        // for (size_t i = 0; i < variable_stack_length; i++)
        //     printf("%4d ", variable_stack[i]);
        // printf("\n");
    }
    
    bool DPLL(formula_t original_formula) {
        if (original_formula.current_clause_count == 0)
            return true;
        formula_t f = original_formula.Copy();
        size_t __original = variable_stack_length;
        while (true) {
            // pick a unit
            int l = f.PickUnit();
            if (l != 0) {
                for (size_t i = 0; i < f.clause_count; i++)
                    if (!f.is_clause_hidden[i] && f.current_clause_length[i] == 1)
                        for (size_t j = 0; j < f.clause_length[i]; j++)
                            if (!f.is_literal_hidden[i][j] && f.formula[i][j] == -l)
                                return variable_stack_length = __original, f.Release(), false;
                SetVar(f, l);
            } else
                break;
        }
        if (f.current_clause_count == 0)
            return true;
        int p = f.PickOne();
        formula_t left_copy_f = f.Copy();
        SetVar(left_copy_f, p);
        bool left = DPLL(left_copy_f);
        if (left == false)
            left_copy_f.Release();
        else
            return f.Release(), true;
        formula_t right_copy_f = f.Copy();
        SetVar(right_copy_f, -p);
        bool right = DPLL(right_copy_f);
        if (right == false)
            right_copy_f.Release();
        else
            return f.Release(), true;
        return variable_stack_length = __original, f.Release(), false;
    }
    
  public:
    bool Solve() {
        auto result = DPLL(f);
        if (result)
            for (size_t i = 0; i < variable_stack_length; i++)
                solution.data[i] = variable_stack[i];
        return result;
    }
    
    old_solver_t(formula_t f) : solution(f.variable_count) {
        this->f = f;
        variable_stack = new int[f.variable_count];
    }
    
    void PrintResult(FILE *file) {
        auto result = new int[f.variable_count];
        for (size_t i = 0; i < f.variable_count; i++)
            result[i] = i + 1;
        for (size_t i = 0; i < variable_stack_length; i++)
            result[Abs(variable_stack[i]) - 1] = variable_stack[i];
        for (size_t i = 0; i < f.variable_count; i++)
            fprintf(file, "%d ", result[i]);
        delete[] result;
    }
    
    solution_t GetSolution() {
        return solution;
    }
};

class solver_t {

  public:
    formula_t f;
    
  private:
    variable_info_pair_t *info = NULL; // var infomation pair array
    
    size_t stack_length = 0;
    int *variable_stack = NULL;
    solution_t solution;
    
    inline void SetVar(int l) {
        variable_info_t *variable_info = &info[(l >= 0 ? l : -l) - 1][l > 0 ? 1 : 0];
        for (size_t i = 0; i < f.clause_count; i++)
            for (size_t j = 0; j < f.clause_length[i]; j++)
                if (!f.is_clause_hidden[i] && !f.is_literal_hidden[i][j] && f.formula[i][j] == l)
                    variable_info->deleted_clause_length++;
        variable_info->deleted_clause_index = new size_t[variable_info->deleted_clause_length];
        variable_info->deleted_clause_length = 0;
        for (size_t i = 0; i < f.clause_count; i++)
            for (size_t j = 0; j < f.clause_length[i]; j++)
                if (!f.is_clause_hidden[i] && !f.is_literal_hidden[i][j] && f.formula[i][j] == l) {
                    f.is_clause_hidden[i] = true;
                    variable_info->deleted_clause_index[variable_info->deleted_clause_length++] = i;
                    f.current_clause_count--;
                }
        for (size_t i = 0; i < f.clause_count; i++)
            for (size_t j = 0; j < f.clause_length[i]; j++)
                if (!f.is_clause_hidden[i] && !f.is_literal_hidden[i][j] && f.formula[i][j] == -l)
                    variable_info->deleted_variable_length++;
        variable_info->deleted_variable_index = new two_size_t[variable_info->deleted_variable_length];
        variable_info->deleted_variable_length = 0;
        for (size_t i = 0; i < f.clause_count; i++)
            for (size_t j = 0; j < f.clause_length[i]; j++)
                if (!f.is_clause_hidden[i] && !f.is_literal_hidden[i][j] && f.formula[i][j] == -l) {
                    f.is_literal_hidden[i][j] = true;
                    variable_info->deleted_variable_index[variable_info->deleted_variable_length][0] = i;
                    variable_info->deleted_variable_index[variable_info->deleted_variable_length][1] = j;
                    variable_info->deleted_variable_length++;
                    f.current_clause_length[i]--;
                }
        variable_stack[stack_length++] = l;
    }
    
    inline void UnsetOneVar(int l) {
        variable_info_t variable_info = info[(l >= 0 ? l : -l) - 1][l > 0 ? 1 : 0];
        for (size_t i = 0; i < variable_info.deleted_clause_length; i++) {
            if (f.is_clause_hidden[variable_info.deleted_clause_index[i]]) {
                f.is_clause_hidden[variable_info.deleted_clause_index[i]] = false;
                f.current_clause_count++;
            }
        }
        for (size_t i = 0; i < variable_info.deleted_variable_length; i++) {
            auto ij = variable_info.deleted_variable_index[i];
            if (f.is_literal_hidden[ij[0]][ij[1]]) {
                f.is_literal_hidden[ij[0]][ij[1]] = false;
                f.current_clause_length[ij[0]]++;
            }
        }
        variable_stack[--stack_length] = 0;
    }
    
    inline void UnsetVar(int l) {
        if (l == 0)
            return;
        bool has = false;
        for (size_t i = 0; i < stack_length; i++)
            if (variable_stack[i] == l) {
                has = true;
                break;
            }
        if (!has)
            return;
        // use info struct to recover formula
        while (stack_length > 0 && variable_stack[stack_length - 1] != l)
            UnsetOneVar(variable_stack[stack_length - 1]);
        if (stack_length > 0)
            UnsetOneVar(l);
    }
    
    bool DPLL() {
        if (f.current_clause_count == 0)
            return true;
        int first_l = 0;
        size_t set_var_count = 0;
        while (true) {
            // pick a unit
            int l = 0;
            for (size_t i = 0; i < f.clause_count; i++)
                if (!f.is_clause_hidden[i] && f.current_clause_length[i] == 1)
                    for (size_t j = 0; j < f.clause_length[i]; j++)
                        if (!f.is_literal_hidden[i][j]) {
                            l = f.formula[i][j];
                            if (first_l == 0)
                                first_l = l;
                        }
            if (l != 0) {
                for (size_t i = 0; i < f.clause_count; i++)
                    if (!f.is_clause_hidden[i] && f.current_clause_length[i] == 1)
                        for (size_t j = 0; j < f.clause_length[i]; j++)
                            if (!f.is_literal_hidden[i][j] && f.formula[i][j] == -l) {
                                if (set_var_count > 0)
                                    UnsetVar(first_l);
                                return false;
                            }
                set_var_count++;
                SetVar(l);
            } else
                break;
        }
        if (f.current_clause_count == 0)
            return true;
        int p = f.PickOne();
        SetVar(p);
        bool left = DPLL();
        if (left == false)
            UnsetVar(p);
        else
            return true;
        SetVar(-p);
        bool right = DPLL();
        if (right == false)
            UnsetVar(-p);
        else
            return true;
        if (first_l != 0)
            UnsetVar(first_l);
        return false;
    }
    
  public:
    solver_t(formula_t f) : solution(f.variable_count) {
        this->f = f;
        info = new variable_info_pair_t[f.variable_count];
        variable_stack = new int[f.variable_count];
    }
    
    bool Solve() {
        auto result = DPLL();
        if (result) {
            for (size_t i = 0; i < stack_length; i++)
                solution.data[i] = variable_stack[i];
            UnsetVar(variable_stack[0]);
        }
        return result;
    }
    
    void PrintResult(FILE *file) {
        auto result = new int[f.variable_count];
        for (size_t i = 0; i < f.variable_count; i++)
            result[i] = i + 1;
        for (size_t i = 0; i < stack_length; i++)
            result[Abs(variable_stack[i]) - 1] = variable_stack[i];
        for (size_t i = 0; i < f.variable_count; i++)
            fprintf(file, "%d ", result[i]);
        delete[] result;
    }
    
    solution_t GetSolution() {
        return solution;
    }
};

int main(int argc, char **argv) {
    srand(time(0));
    printf("Generate a sudoku:\n");
    auto sudoku = sudoku_t::Generate(9, 60);
    sudoku.PrintSudoku(stdout);
    solver_t s(sudoku_t::EncodeSoduku(sudoku));
    printf("Solve OK: %d\n", s.Solve());
    sudoku_t::FromSolution(s.GetSolution(), 9).PrintSudoku(stdout);
}
