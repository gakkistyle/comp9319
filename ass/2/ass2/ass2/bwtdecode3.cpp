#include <iostream>
#include <fstream>

int main(int argc, const char * argv[])
{
    std::string filename = argv[1];
    std::string outname = argv[2];
    std::ifstream is(filename,std::ifstream::binary);
    std::ofstream out(outname,std::ofstream::binary);
    
 //   auto char_num = std::map<char,int>{};
 //   char_num['A'] = 0;
 //   char_num['C'] = 1;
//    char_num['G'] = 2;
//    char_num['T'] = 3;
    
    is.seekg(0,is.end);
    int size = is.tellg();
    
    int num_A,num_C,num_G,num_T;
    num_A = num_C = num_G = num_T = 0;

    char c;
    is.clear();
    is.seekg(0,is.beg);

    //every 8 charater become one short integer
    int short_size = 1+(size-1)/8;
    unsigned short * compres_data = new unsigned short[short_size];
    int new_line_ptr = 0;
    int data_ptr = 0;
    int cnt = 0;
    int data_once = 0;
    
    int gap = 20;
    int gap_cnt =0 ;
    int occ_size = 1 + short_size/gap;
    int **occ_t = new int*[occ_size];

    for(int i = 0; i < occ_size; ++i)
        occ_t[i] = new int[4];
    int occ_ptr=1;
    for(int i = 0;i<4;i++){
        occ_t[0][i] = 0;
    }
    while (is.get(c)) {
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
    
    out.seekp(size-1,out.beg);
    out.put('\n');
    out.seekp(-1,out.cur);
    size--;
    
    int chunk_size = 1000;
    char *out_char;
    if(size < chunk_size){
        chunk_size = size;
    }
    out_char = new char[chunk_size + 1];
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
            out.seekp(-chunk_size,out.cur);
            out.write(out_char,chunk_size);
            out.seekp(-chunk_size,out.cur);
            delete []out_char;
            int loc = out.tellp();
            if(loc < chunk_size){
                chunk_size = loc;
            }
            out_ptr = chunk_size - 1;
            out_char = new char[chunk_size+1];
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
    //std::cout<<ptr<<" ";
        size -- ;
    }
//    if(out_ptr != chunk_size - 1){
//        out.seekp(0,out.beg);
//        out.write(out_char,chunk_size);
//    }
    out.close();
    is.close();

}

