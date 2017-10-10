#include <iostream>
#include <string>
#include <algorithm>
#include <iomanip>
#include "smith_waterman.h"
#include <ctype.h>
#include <sstream>
#include <map>
#include <fstream>
#include <cstring>
int max(int a, int b, int c, int d){
    return std::max({a, b, c, d});
}

dna_comp::dna_comp(std::string dna1, std::string dna2){
    this->_dna1 = dna1;
    this->_dna2 = dna2;
    
    /*  d n a 2
     *d
     *n 
     *a 
     *1 */
    this->_matrix_rows = dna1.length() + 1; 
    this->_matrix_cols = dna2.length() + 1;
    
    this-> _similarity_matrix = new int* [this->_matrix_rows]; //init similarity matrix with zeros
    for (size_t i = 0; i < (this->_matrix_rows); ++i ){
        this->_similarity_matrix[i] = new int [this->_matrix_cols];
    }
    
    for (size_t i = 0; i < _matrix_rows; ++i){
        for (size_t j = 0; j < _matrix_cols; j++)
            _similarity_matrix[i][j] = 0;
    }
    this->_max=0;

}

dna_comp::~dna_comp(){
    for (size_t i = 0; i < (_matrix_rows); i++){
        delete[] this->_similarity_matrix[i];
	}
	delete[] this->_similarity_matrix;
};

/*int dna_comp::sim(size_t i, size_t j){ //compare two current symbols from dna1 and dna2
    
    if (std::toupper(_dna1[i-1]) == std::toupper(_dna2[j-1]))
        return _match_penalty;
    else 
        return _mismatch_penalty;
}*/
void dna_comp::create_weight_matrix(std::string matrix_file)
{
    std::fstream matrix_f(matrix_file);
    if (!matrix_f)
    {
        std::cerr << " Substition matrix file not found! \n";
    }
    std::string templine, ch;
    int i = 0; 
    int j = 0;
    while (std::getline(matrix_f, templine)){
        std::stringstream row(templine);
        j = 0;
        while (std::getline(row, ch, ' ')){
            if (ch != "")
            {
                if (i == 0){
                    _dna_int.insert(std::pair<char, int>(ch[0],j));
                }

                if (i > 0 && j > 0){
                    _weight_matrix[i-1][j-1] = atoi (ch.c_str());
                }
                j++;
            }
        }
        i++;

    }
   
}

void dna_comp::init_matrix(){ 
    //filling 0 column and 0 row 

    int _gap_penalty =_weight_matrix[0][23];
    
    for (size_t i = 1; i < _matrix_rows; ++i){
        _similarity_matrix[i][0] = i*_gap_penalty; //_similarity_matrix[i-1][0] + _gap_penalty;
    }
    
    for (size_t j = 1; j < _matrix_cols; ++j){
        _similarity_matrix[0][j] = j*_gap_penalty; //_similarity_matrix[0][j-1] + _gap_penalty;
    }
    
    //filling remaining cols and rows. a[i,j] = max (diag+sim(i,j), left_elem+gap, upper_elem + gap) 
    for (size_t i = 1; i < _matrix_rows; ++i){
        for (size_t j = 1; j < _matrix_cols; ++j){
            int match =  _similarity_matrix[i-1][j-1] + _weight_matrix[ _dna_int[std::toupper(_dna1[i-1])] ] [ _dna_int[ std::toupper(_dna2[j-1]) ] ]; //(_dna1[j-1] == _dna2[i-1] ? _similarity_matrix[i-1][j-1] + match_penalty : _similarity_matrix[i-1][j-1] + mismatch_penalty);
            int del = _similarity_matrix[i-1][j] + _gap_penalty;
            int insert = _similarity_matrix[i][j-1] + _gap_penalty;
            _similarity_matrix[i][j] = max(match, del, insert, 0);
            if (_similarity_matrix[i][j] >= _max){
                _max = _similarity_matrix[i][j];
                _maxi = i;
                _maxj = j;
            }

            //std::cout<<_dna1[i-1]<<" "<<_dna2[j-1]<<" "<<_similarity_matrix[i][j]<<"\n";
        }
    }
std::cout<<_max<<" i: "<<_maxi<<" j: "<<_maxj<<"\n";
    
}

void dna_comp::print_matrix(){
    for (size_t i = 0; i < _matrix_rows; ++i){
        for (size_t j = 0; j < _matrix_cols; ++j)
            std::cout<<std::setw(3)<<_similarity_matrix[i][j]<<' ';
        std::cout<<"\n";
    }
}
//building aligned strings
void dna_comp::trace_back(){
    int _gap_penalty =_weight_matrix[0][23];
    _align1 = "";
    _align2 = "";
    size_t i = _maxi; // _dna1.length();
    size_t j = _maxj; // _dna2.length();
    while ((i > 0 || j > 0) && _similarity_matrix[i][j] > 0){ 
        //to i-1, j-1
        //std::cout<<_similarity_matrix[i][j]<<" "<<_similarity_matrix[i-1][j-1]<<" + "<<_weight_matrix[_dna_int[_dna1[i - 1]]][_dna_int[_dna2[j - 1]]]<<"\n";
        if ( i > 0 && j > 0 && (_similarity_matrix[i][j] == _similarity_matrix[i-1][j-1] + _weight_matrix[_dna_int[std::toupper(_dna1[i - 1]) ]][ _dna_int[std::toupper(_dna2[j - 1] )] ])){
            //std::cout<<"similar: "<<i<<" "<<_dna1[i-1]<<" "<<j<<" "<<_dna2[j-1]<<"\n";
            //std::cout<<_align1<<"\n"<<_align2<<"\n";
            _align1 = _dna1[i-1] + _align1;
            _align2 = _dna2[j-1] + _align2;
            i--;
            j--;
            
        }
        //to i-1, j
        else if (i > 0 && _similarity_matrix[i][j] == _similarity_matrix[i-1][j] + _gap_penalty) {
            //std::cout<<"non_similar: "<<i<<" "<<_dna1[i-1]<<" "<<j<<" "<<_dna2[j-1]<<"\n";
            //std::cout<<_align1<<"\n"<<_align2<<"\n";
            _align1 = _dna1[i-1] + _align1;
            _align2 = '-' + _align2;
            i--;
            
        }
        //to i, j-1
        else {
            //std::cout<<"non_similar: "<<i<<" "<<_dna1[i-1]<<" "<<j<<" "<<_dna2[j-1]<<"\n";
            //std::cout<<_align1<<"\n"<<_align2<<"\n";
            _align1 = '-' + _align1;
            _align2 = _dna2[j-1] + _align2;
            j--;
            
        }
        
    }
}

void dna_comp::print_strings(){
    std::cout<<"Alignment\n";
    for (size_t i = 0; i < _align1.length(); i++)
        std::cout<<_align1[i];
    std::cout<<"\n";
    for (size_t j = 0; j < _align2.length(); j++)
        std::cout<<_align2[j];
    std::cout<<"\n";
}


