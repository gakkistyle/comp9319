//
//  bwtsearch.cpp
//  ass2
//
//  Created by ?????? on 2020/7/18.
//  Copyright �? 2020 ??????. All rights reserved.
//




#include <iostream>
#include <fstream>
#include <vector>
#include <map>

int main(int argc, const char * argv[]){
    std::string filename = argv[1];
    
    std::ifstream is(filename,std::ifstream::binary);
    is.seekg(0,is.end);
    long size = is.tellg();
    
    //determine the gap of rank table
    int gap = 4;
    auto rT = std::vector<std::vector<int>>{};
    rT.push_back(std::vector<int>{0,0,0,0});
    
    if(size>250000){
        gap = 4*((int)((double)size/250000) + 1);
    }
   // std::cout<<size;
    
    //construct a map between AGCT and num
    auto char_num = std::map<char,int>{};
    char_num['A'] = 0;
    char_num['C'] = 1;
    char_num['G'] = 2;
    char_num['T'] = 3;
    
    //construct c table
    auto num_ACGT = std::vector<int>{0,0,0,0};
    
   // int num_A=0;
   // int num_C = 0;
   // int num_G = 0;
   // int num_T = 0;
    int gap_cnt=0;
    is.clear();
    is.seekg(0,is.beg);
    char c;

    
   while(is.get(c)){
       if(c == 'A'){
            num_ACGT[0] ++;
         // num_A++;
        
       }
        else if(c == 'C'){
            num_ACGT[1] ++;
     //       num_C++;
      }
        else if(c == 'G'){
            num_ACGT[2] ++;
    //        num_G ++;
       }
        else if(c == 'T'){
            num_ACGT[3] ++;
      //      num_T++;
       }
       gap_cnt++;
       if(gap_cnt == gap){
            gap_cnt = 0;
            rT.push_back(std::vector<int>{num_ACGT[0],num_ACGT[1],num_ACGT[2],num_ACGT[3]});
        }
    }
    
    auto cT = std::map<char,int>{};
    cT[' '] = 0;
    cT['A'] = 1;
    cT['C'] = 1 + num_ACGT[0];
    cT['G'] = cT['C'] + num_ACGT[1];
    cT['T'] = cT['G'] + num_ACGT[2];
    
    //bwtsearch
    is.clear();
    std::string code;
    while(std::getline (std::cin,code)){
        int size = code.size();
        int i = size-1;
        char c = code[i];

        int first = cT[c];

        int last = cT[c] + num_ACGT[char_num[c]]-1;

        while( (first <= last) && (i >= 1) ){
            c = code[--i];

//            //compute first
            int rank_pos = first / gap;
            int off_pos = first % gap;
            int rank = rT[rank_pos][char_num[c]];

            is.clear();
            is.seekg(rank_pos * gap ,std::ios::beg);

            int start=0;
            while(start < off_pos){
                char n;
                is.get(n);
                if(n == c){
                    ++rank;
                }
                start ++;
            }
            first = cT[c] + rank;

//            //compute last
            rank_pos = (last+1)/gap;
            off_pos = (last+1)%gap;
            rank = rT[rank_pos][char_num[c]];
            is.clear();
            is.seekg(rank_pos * gap ,std::ios::beg);

            start = 0;
            while(start<off_pos){
                char n;
                is.get(n);
                if(n==c){
                    ++rank;
                }
                start++;
            }
            last = cT[c] + rank-1;
        }
        if(last < first){
            std::cout << 0 <<'\n';
        }else{
            std::cout << last-first + 1<<'\n';
        }
    }
}




