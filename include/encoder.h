#pragma once
#include "types.h"
#include "vector.h"
#include "cnfcontainer.h"
#include <string>
using std::string;

class Encoder {
    size_t var_cnt, clause_cnt;
    Vector<int> res;
    void cell_atleast_one();
    void row_atmost_one();
    void col_atmost_one();
    void square_atmost_one();
    // The functions below are for the extended encoding
    void cell_atmost_one();
    void row_atleast_one();
    void col_atleast_one();
    void square_atleast_one();
  public:
    Encoder();
    Encoder(string filename);
    Encoder(const Vector<int>& def);
    Encoder(const Encoder& encoder);
    virtual ~Encoder();
    void to_file(string filename);
    CnfContainer to_cnf();
};
