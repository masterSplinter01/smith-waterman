#include <iostream>

#include <string>

#include "smith_waterman.h"
int main()
{
    
    std::string dna1 = "";
    std::string dna2 = "";
    std::cout<<"enter string1: "; 
    std::cin>>dna1;
    std::cout<<"enter string2: "; 
    std::cin>>dna2;
    
    dna_comp* nw = new dna_comp (dna1, dna2);
    nw->create_weight_matrix("./blosum_orig.txt");
    nw->init_matrix();
    nw->print_matrix();
    nw->trace_back();
    nw->print_strings();
    
    return 0;
}
