//
//  main.cpp
//  ass1
//
//  Created by 郑淇文 on 2020/6/17.
//  Copyright © 2020 郑淇文. All rights reserved.
//
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <gmp.h>
#include <mpfr.h>

struct mpfr_type{
    mpfr_t mpfr_tt;
   
};

int main(int argc, const char * argv[]) {
    //8000 is enough for 1024 maximum input.
    mpfr_rnd_t rnd = MPFR_RNDN;
    int N = 8000;
    
    //char_num is the map that record the character and how many.
    auto char_num = std::map<char,int>{};
    char c;
    auto s = std::string{};
    int count = 0;
    
    //read from the std input and form the map
    while(std::cin.get(c))
    {
        if(c!='\n'){
            s += c;
            count++;
            if(char_num.count(c)>0){
                char_num.at(c) ++;
            }
            else{
                char_num.emplace(c,1);
            }
        }
    }
    //output the the sorted ascii character and times.
    for(auto i = char_num.begin();i!=char_num.end();i++){
        std::cout<<i->first<<" "<<i->second<<std::endl;
    }
    
    //get the high and low range of each character.
    struct mpfr_type low_range;
    mpfr_init2(low_range.mpfr_tt,N);
    mpfr_set_d (low_range.mpfr_tt, 0, rnd);
    mpfr_t count_;
    mpfr_init2(count_,N);
    mpfr_set_d(count_,count,rnd);
    
    auto char_up_range = std::map<char,struct mpfr_type>{};

    int count_char = 0;
    for(auto i=char_num.begin();i!=char_num.end();i++){
        struct mpfr_type up_range;
        mpfr_init2(up_range.mpfr_tt,N);
        
        mpfr_t nums,dis;
        mpfr_init2(nums,N);
        mpfr_set_d(nums,i->second,rnd);
        mpfr_init2(dis,N);
        mpfr_div(dis,nums,count_,rnd);
        mpfr_add(up_range.mpfr_tt,dis,low_range.mpfr_tt, rnd);
        
        char_up_range.emplace(i->first,up_range);
        
        mpfr_set(low_range.mpfr_tt,up_range.mpfr_tt,rnd);

        count_char ++;
    }

    char all_c[count_char+1];
    int pp = 0;
    for(auto i = char_up_range.begin();i!=char_up_range.end();i++){
        all_c[pp] = i->first;
        pp++;
    }
    auto char_low_range = std::map<char,struct mpfr_type>{};
    struct mpfr_type low_range2;
    mpfr_init2(low_range2.mpfr_tt,N);
    mpfr_set_d (low_range2.mpfr_tt, 0, rnd);
    char_low_range.emplace(all_c[0],low_range2);
    int happy=1;
    for(auto i = char_up_range.begin();i!=char_up_range.end();i++){
        if(happy == count_char){
            break;
        }
        char cc = all_c[happy];
        happy++;
        struct mpfr_type low_range3;
        mpfr_init2(low_range3.mpfr_tt,N);
        mpfr_set (low_range3.mpfr_tt,i->second.mpfr_tt,rnd);
        char_low_range.emplace(cc,low_range3);
    }

  
    //use the ac alorithm to get the encoded low and high range value.
    mpfr_t low, high,code_range,temp;

    mpfr_init2(low,N);
    mpfr_init2(high,N);
    mpfr_init2(code_range,N);
    mpfr_init2(temp,N);
    mpfr_set_d (low, 0, rnd);
    mpfr_set_d (high, 1, rnd);
    mpfr_sub (code_range, high,low, rnd);

    for(char const& ch:s){
        mpfr_sub (code_range, high,low, rnd);
        mpfr_set (temp, char_up_range.at(ch).mpfr_tt,rnd);
        mpfr_mul(temp,code_range, temp, rnd);
        mpfr_add (high, low,temp, rnd);
        mpfr_set (temp, char_low_range.at(ch).mpfr_tt,rnd);
        mpfr_mul(temp,code_range, temp, rnd);
        mpfr_add (low, low,temp, rnd);
    }
    mpfr_sub (code_range, high,low, rnd);

    //get the precision
    int p = 0;
    mpfr_t one,ten;
    mpfr_init2(one,32);
    mpfr_init2(ten,32);
    mpfr_set_d (one, 1, rnd);
    mpfr_set_d (ten, 10,rnd);

    while(mpfr_cmp(one,code_range)>0){
        p++;
        mpfr_mul(code_range,code_range, ten,MPFR_RNDZ);
    }

    mpfr_printf ("%.*R*f",p+1,MPFR_RNDA, low);
    std::cout<<" ";
    mpfr_printf ("%.*R*f",p+1,MPFR_RNDZ, high);
    std::cout<<"\n";
    mpfr_clear(low);
    mpfr_clear(high);
    mpfr_clear(code_range);
    mpfr_clear(temp);
    return 0;
}
