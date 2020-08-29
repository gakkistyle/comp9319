//
//  adecode.cpp
//  ass1
//
//  Created by 郑淇文 on 2020/6/20.
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

int main()
{
    int N = 8000;
    auto char_num = std::map<char,int>{};
    
    std::string line;
    mpfr_rnd_t rnd = MPFR_RNDN;
    //long_num is the first value of the last line that we need to decode back.
    mpfr_t long_num;
    mpfr_init2(long_num,N);
    int p = 1;
    int count = 0;
    
    //read in the input and get the char_num map same as encode,
    //also get the long_num of the first value of the last line.
    while(std::getline(std::cin, line)){
        if(line[1] == ' '){
            std::string in{line.begin()+2,line.end()};

            int num = std::stoi(in);
            count += num;
            char_num.emplace(line[0],num);
        }
        else{
            char in[N];
            auto ptr = line.begin();
            while(*ptr!=' '&& ptr!=line.end()){
                ptr ++;
            }
            std::string copy{line.begin(),ptr};
            p = copy.size()-2;

            strcpy(in, copy.c_str());
            mpfr_set_str (long_num, in,10, rnd);
        }
    }
    
     //the same way as encode to get the range for each character.
      struct mpfr_type low_range;
      mpfr_init2(low_range.mpfr_tt,N);
      mpfr_set_d (low_range.mpfr_tt, 0, rnd);
      mpfr_t count_;
      mpfr_init2(count_,N);
      mpfr_set_d(count_,count,rnd);
      
      auto char_up_range = std::map<char,struct mpfr_type>{};

      int count_char = 0;
      for(auto i=char_num.begin();i!=char_num.end();i++){
          //std::cout<<i->first;
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
        mpfr_set (low_range3.mpfr_tt,i->second.mpfr_tt, rnd);
        char_low_range.emplace(cc,low_range3);

    }
    
    //use the ac decode algorithm to decode the value back to words.
    auto result = std::string{""};
    
    while(count!=0){
        for(int i = 0;i < count_char;i++){
            if(mpfr_cmp(long_num,char_low_range.at(all_c[i]).mpfr_tt)>=0 && mpfr_cmp (long_num,char_up_range.at(all_c[i]).mpfr_tt)<0){
                result += all_c[i];
                mpfr_t range;
                mpfr_init2(range,N);
                mpfr_set (range,char_up_range.at(all_c[i]).mpfr_tt, rnd);
                mpfr_sub (range,range,char_low_range.at(all_c[i]).mpfr_tt,rnd);

                mpfr_t low_range;
                mpfr_init2(low_range,N);
                mpfr_set (low_range,char_low_range.at(all_c[i]).mpfr_tt, rnd);
                mpfr_sub (long_num, long_num,low_range,rnd);
                mpfr_div(long_num, long_num, range,rnd);
                count --;

                break;
            }
        }
    }
   std::cout<<result;
    
   mpfr_clear(low_range.mpfr_tt);
   mpfr_clear(low_range2.mpfr_tt);
   mpfr_clear(count_);
   mpfr_clear(long_num);
    
}
