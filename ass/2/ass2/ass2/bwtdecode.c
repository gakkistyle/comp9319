//
//  bwtdecode.c
//  ass2
//
//  Created by 郑淇文 on 2020/7/20.
//  Copyright © 2020 郑淇文. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
int main(int argc, const char * argv[])
{
    
    FILE * inptr;
    FILE * outptr;
    inptr = fopen(argv[1], "r");
    outptr = fopen(argv[2],"w");
    
    fseek(inptr, 0, SEEK_END);
  //  is.seekg(0,is.end);
    int size = (int)ftell(inptr);
    fseek(inptr, 0, SEEK_SET);
    
    int num_A,num_C,num_G,num_T;
    num_A = num_C = num_G = num_T = 0;

    char c;

    //every 8 charater become one short integer
    int short_size = 1+(size-1)/8;
    unsigned short * compres_data = malloc(short_size * sizeof(short));
    int new_line_ptr = 0;
    int data_ptr = 0;
    int cnt = 0;
    int data_once = 0;
    
    int gap = 20;
    int gap_cnt =0 ;
    int occ_size = 1 + short_size/gap;
   
    int *occ_t[occ_size] ;
    for(int i =0;i<occ_size;i++){
        occ_t[i] = malloc(4*sizeof(int));
    }

//    for(int i = 0; i < occ_size; ++i)
//        occ_t[i] = new int[4];
    int occ_ptr=1;
    for(int i = 0;i<4;i++){
        occ_t[0][i] = 0;
    }
    while (1) {
        c = fgetc(inptr);
        if(c == EOF){
            break;
        }
        if(c!='\n'){
            cnt ++;
            if(c== 'A'){
                num_A ++;
                data_once |=( 0 << (16-2*cnt));
            }
         //   num_ACGT[char_num[c]] ++;
            else if(c == 'C'){
                num_C++;
                data_once |=( 1 << (16-2*cnt));
            }
            else if(c == 'G'){
                num_G++;
                data_once |=( 2 << (16-2*cnt));
            }
            else if(c == 'T'){
                num_T++;
                data_once |=( 3 << (16-2*cnt));
            }
            if(cnt == 8){
                compres_data[data_ptr] = data_once;
                data_ptr ++;
                gap_cnt ++;
                data_once = 0;
                cnt = 0;
            }
            if(gap_cnt == gap){
                occ_t[occ_ptr][0] = num_A;
                occ_t[occ_ptr][1] = num_C;
                occ_t[occ_ptr][2] = num_G;
                occ_t[occ_ptr][3] = num_T;
                gap_cnt = 0;
                occ_ptr ++;

            }
        }
        else{
            new_line_ptr = data_ptr*8 + cnt;
        }
    }

    if(cnt != 0){
    compres_data[data_ptr] = data_once;
    }

    //cT
    int cT[4] = {1,1+num_A,1+num_A+num_C,1+num_A+num_C+num_G};
    
    fseek(outptr,size-1,SEEK_SET);
    fputc('\n',outptr);
    fseek(outptr,-1,SEEK_CUR);

//    out.seekp(size-1,out.beg);
//    out.put('\n');
//    out.seekp(-1,out.cur);
    size--;

    int chunk_size = 1000;
    char *out_char;
    if(size < chunk_size){
        chunk_size = size;
    }
    out_char = malloc((chunk_size + 1)*sizeof(char));
    int out_ptr = chunk_size-1;


    int ptr = 0;
    while(size>0){
        int now_ptr = ptr/8;
        int now_off = ptr %8;
        int num = compres_data[now_ptr];
        num  = num >> (14-2*now_off);
        num &= 3;

    //std::cout<<"ptr: "<<ptr<<" num: "<<num<<std::endl;
        char n;
        int rank = 0;
        int cT_once = 0;
        int r_ptr = now_ptr/gap;

        //get the current char,rank offset and cT
        if(num == 0){
            n = 'A';
            rank = occ_t[r_ptr][0];
            cT_once = cT[0];
        }else if(num == 1){
            n = 'C';
            rank = occ_t[r_ptr][1];
            cT_once = cT[1];
        }else if (num == 2){
            n = 'G';
            rank = occ_t[r_ptr][2];
            cT_once = cT[2];
        }else{
            n = 'T';
            rank = occ_t[r_ptr][3];
            cT_once = cT[3];
        }

        out_char[out_ptr] = n;
        out_ptr --;
        if(out_ptr < 0 ){
            fseek(outptr, -chunk_size, SEEK_CUR);
            fwrite(out_char, sizeof(char),chunk_size, outptr);
            fseek(outptr, -chunk_size, SEEK_CUR);


//            out.seekp(-chunk_size,out.cur);
//            out.write(out_char,chunk_size);
//            out.seekp(-chunk_size,out.cur);
            free(out_char);
            int loc = (int)ftell(outptr);
            if(loc < chunk_size){
                chunk_size = loc;
            }
            out_ptr = chunk_size - 1;
            out_char = malloc((chunk_size + 1)*sizeof(char));
        }
     //   out.put(n);
    //    out.seekp(-2,out.cur);

        //get the rank
        int off_set = ptr - r_ptr*gap*8;
        int start = 0;
        int get_r_ptr = r_ptr*gap;
        while(start < off_set){
            int i =0;
            int num_once = compres_data[get_r_ptr];
            while(i<8 && start < off_set){
                int once = num_once >> (14-2*i);
                once&=3;
                if(once == num){
                    rank ++;
                }
                i++;
                start ++;
            }
            get_r_ptr ++;
        }

        ptr = cT_once + rank;
        if(ptr > new_line_ptr){
            ptr --;
        }
        size -- ;
    }

    fclose(outptr);
    fclose(inptr);
}


/*
 
 312/100=3 occ_table[3] offset = 312 % 100 = 12
 fseek move to the 300,read the next 12,stored in an arrary,
 
 gap = 100
 0 100 200 300 ...
 
 3 table 4 occ,int num_a,c,g,t
 
 offset = 12
 
 char * record = fread(12,fp)
 
 record[-1] == 'ATCG';
 
 
 
 
 */
