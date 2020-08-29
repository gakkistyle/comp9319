//
//  bwtdecode2.cpp
//  ass2
//
//  Created by 郑淇文 on 2020/7/18.
//  Copyright © 2020 郑淇文. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

int main(int argc, const char * argv[])
{
    std::string filename = argv[1];
    std::string outname = argv[2];
    std::ifstream is(filename,std::ifstream::binary);
    std::ofstream out(outname,std::ofstream::binary);
    
 //   auto char_num = std::map<char,int>{};
 //   char_num['A'] = 0;
 //   char_num['C'] = 1;
  //  char_num['G'] = 2;
 //   char_num['T'] = 3;
    
    is.seekg(0,is.end);
    long size = is.tellg();
    
    int num_A=0;
    int num_C=0;
    int num_G=0;
    int num_T=0;
   // auto char_freq = std::vector<std::pair<char,int>>{};
    char all[size+1];
    int all_ptr = 0;
    
    int gap = 400;
    int gap_cnt = 0;
    int gap_ptr = 1;
    int occ_t[1+size/gap][4];
    for(int i =0;i<4;i++){
        occ_t[0][i] = 0;
    }
    is.seekg(0,is.beg);
    char c;
    while(is.get(c)){
      // std::pair<char,int> once;
       all[all_ptr] = c;
       all_ptr ++;
       if(c == 'A'){
           num_A ++;
         //  once.first = 'A';
         //  once.second = num_A;
           
        }
        else if(c == 'C'){
            num_C ++;
          //  once.first = 'C';
          //  once.second = num_C;
        }
        else if(c == 'G'){
            num_G ++;
          //  once.first = 'G';
          //  once.second = num_G;
        }
        else if(c == 'T'){
            num_T ++;
          //  once.first = 'T';
          //  once.second = num_T;
        }
       // char_freq.push_back(once);
        gap_cnt++;
        if(gap_cnt == gap){
            occ_t[gap_ptr][0] = num_A;
            occ_t[gap_ptr][1] = num_C;
            occ_t[gap_ptr][2] = num_G;
            occ_t[gap_ptr][3] = num_T;
        }
    }
    
    auto cT = std::map<char,int>{};
    cT[' '] = 0;
    cT['A'] = 1;
    cT['C'] = 1 + num_A;
    cT['G'] = cT['C'] + num_C;
    cT['T'] = cT['G'] + num_G;
     
    out.seekp(size-1,out.beg);
    out.put('\n');
    out.seekp(-2,out.cur);
    size--;
    
    int ptr = 0;
    while(size>0){
        char n = all[ptr];
    //std::cout<<n;
        out.put(n);
        out.seekp(-2,out.cur);
        
        int rank = 0;
        int ptr_pos = ptr/gap;
        if(n == 'A'){
            rank = occ_t[ptr_pos][0];
        }else if(n=='C'){
            rank = occ_t[ptr_pos][1];
        }else if(n == 'G'){
            rank = occ_t[ptr_pos][2];
        }else if(n == 'T'){
            rank = occ_t[ptr_pos][3];
        }
        int start_ptr = ptr_pos * gap;
        while(start_ptr < ptr){
            if (all[start_ptr] == n){
                rank ++;
            }
            start_ptr ++;
        }
        ptr = cT[n] +rank;
        size -- ;
    }
    
    out.close();
    is.close();
    
}
