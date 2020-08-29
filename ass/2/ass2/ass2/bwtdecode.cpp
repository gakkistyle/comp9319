//
//  main.cpp
//  ass2
//
//  Created by 郑淇文 on 2020/7/13.
//  Copyright © 2020 郑淇文. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <map>
#include <vector>

int main(int argc, const char * argv[]) {
    std::string filename = argv[1];
    std::string outname = argv[2];
    std::ifstream is(filename,std::ifstream::binary);
    std::ofstream out(outname,std::ofstream::binary);
    std::filebuf* inbuf  = is.rdbuf();
    std::filebuf* outbuf  = out.rdbuf();
    
    //get size
    is.seekg(0,is.end);
    long size = is.tellg();
    
    //determine the gap of rank table
    int gap = 1;
    auto rT = std::vector<std::vector<int>>{};
    rT.push_back(std::vector<int>{0,0,0,0});
    
    if(size>250000){
        gap = ((int)((double)size/250000) + 1);
    }
    
    //construct a map between AGCT and num
    auto char_num = std::map<char,int>{};
    char_num['A'] = 0;
    char_num['C'] = 1;
    char_num['G'] = 2;
    char_num['T'] = 3;
    
    
    //construct c table
    int num_A=0;
    int num_C=0;
    int num_G=0;
    int num_T=0;
    
    int gap_cnt=0;
    is.seekg(0,is.beg);
    char c;
    while(is.get(c)){
       if(c == 'A'){
            num_A ++;
        }
        else if(c == 'C'){
            num_C ++;
        }
        else if(c == 'G'){
            num_G ++;
        }
        else if(c == 'T'){
            num_T ++;
        }
        gap_cnt++;
        if(gap_cnt == gap){
            gap_cnt = 0;
            rT.push_back(std::vector<int>{num_A,num_C,num_G,num_T});
        }
    }
    
    auto cT = std::map<char,int>{};
    cT[' '] = 0;
    cT['A'] = 1;
    cT['C'] = 1 + num_A;
    cT['G'] = cT['C'] + num_C;
    cT['T'] = cT['G'] + num_G;
    //bwt reverse
    std::string result;
    
    int cnt = 0;
    int ptr = 0;
    is.clear();
    is.seekg(0,std::ios::beg);
    out.seekp(0,out.beg);
   // out.seekp(size-1,out.beg);
   // out.put('\n');
   // out.seekp(-2,out.cur);
    while(cnt < size-1){
        int now_loc = is.tellg();
        is.get(c);
        result.insert(0,std::string{c});
       // out.put(c);
        out.seekp(-2,out.cur);
      // count the rank of c

        now_loc++;
        int rank_pos = now_loc / gap;
        int off_pos = now_loc % gap;

        int rank = 0;
        if(rank_pos != 0){
            rank = rT[rank_pos][char_num[c]];
//            if(c == 'A'){
//                rank = rT[rank_pos][0] ;
//            }else if (c == 'C'){
//                rank = rT[rank_pos][1] ;
//            }else if (c == 'G'){
//                rank = rT[rank_pos][2] ;
//            }else if (c == 'T'){
//                rank = rT[rank_pos][3] ;
//            }
            if(off_pos == 0)
            {
                rank--;
            }
        }


        is.clear();
        is.seekg(rank_pos * gap ,std::ios::beg);
//        if(rank_pos == 0){
//            is.seekg(0,std::ios::beg);
//        }
//        else{
//            is.seekg(rank_pos * gap ,std::ios::beg);
//        }
        int start = 1;
        while(start < off_pos){
            char n;
            is.get(n);
            if(n == c){
                ++rank;
            }
            ++start;
        }

        ptr = cT[c] + rank;
        is.clear();
        is.seekg(ptr,is.beg);
        cnt ++;
    }
    
    out<<result;
    is.close();
    out.close();
}
