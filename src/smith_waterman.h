#include <iostream>
#include <string>
#include <map>

class dna_comp
{
private:
    std::string _align1, _align2;
    std::string _dna1, _dna2;
    
    
    size_t _matrix_rows, _matrix_cols;
    
    int** _similarity_matrix;
    int _weight_matrix[24][24];
    std::map<char, int>_dna_int;
    int _max;
    int _maxi;
    int _maxj;
public:
    dna_comp(std::string dna1, std::string dna2);
    ~dna_comp();
    
    void create_weight_matrix(std::string matrix_file);
    void init_matrix();
    void print_matrix();
    //int sim(size_t i, size_t j);
    void trace_back();
    void print_strings();
};
int max(int a, int b, int c, int d);