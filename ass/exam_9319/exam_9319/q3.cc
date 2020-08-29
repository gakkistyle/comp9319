// You can write your code in C or C++. Modify this file and the makefile
// accordingly. Please test your program on CSE linux machines before submitting
// your solution.
//

#include<iostream>
#include<string>
#include<cstring>
#include<vector>
#include<map>

using namespace std;

void print_str(const vector<vector<int>>& s_table){
    int tsize = s_table.size();
    for(int i = 0; i < tsize -1;i++){
        cout << '[';
        for(int j = 0;j<s_table[i].size()-1;j++){
            cout << s_table[i][j] << ' ';
        }
        cout << s_table[i][s_table[i].size()-1] << "] ";
    }
    cout << '[';
    for(int j = 0; j< s_table[tsize-1].size() -1;j++){
        cout << s_table[tsize-1][j] << ' ';
    }
    cout << s_table[tsize-1][s_table[tsize-1].size()-1] << "]\n";
}

int main(int argc, const char* argv[])
{
    string tar = argv[1];
    
    vector<char> l_s;
    
    for(int i = 0;i < tar.length()-1;i++){
        if(tar.substr(i,1).compare(tar.substr(i+1,1))>0){
            l_s.push_back('l');
        }
        if(tar.substr(i,1).compare(tar.substr(i+1,1))<0){
            l_s.push_back('s');
        }
        if(tar[i] == tar[i+1]){
            int j = i+1;
            while(j < tar.length()-1){
                if(tar[j] > tar[j+1]){
                    l_s.push_back('l');
                    break;
                }
                else if (tar[j] < tar[j+1]){
                    l_s.push_back('s');
                    break;
                }
                else{
                    j++;
                }
            }
        }
    }
    
    l_s.push_back('s');
    
    int tri = 0;
    if(l_s[0] == 's'){
        tri = 1;
    }
    vector<int> dis;
    dis.push_back(0);
    
    int max = 1;
    for(int i = 1;i < tar.length();i++){
        if(tri == 0 && l_s[i] == 's'){
            dis.push_back(0);
            tri = 1;
        }
        else if(tri == 0 && l_s[i] == 'l'){
            dis.push_back(0);
        }
        else if(tri == 1 && l_s[i-1] == 's'){
            dis.push_back(1);
        }
        else if(tri == 1 && l_s[i-1] == 'l'){
            int num = dis.back();
            if(max < num +1){
                max = num + 1;
            }
            dis.push_back(num + 1);
        }
    }

    //bucket
    
    map<char,vector<int> > bucket;
    
    for(int i =0;i< tar.length();i++){
        if(bucket.count(tar[i]) != 0){
            bucket[tar[i]].push_back(i+1);
        }
        else{
            auto new_v = vector<int> {i+1};
            bucket[tar[i]] = new_v;
        }
    }

    //construct s_string
    vector<vector<int>> s_str;
    for(auto i = bucket.begin();i != bucket.end();i++){
        vector<int> new_v;
        for(int j = 0;j < i->second.size();j++){
            if(l_s[i->second[j]-1] == 's'){
                new_v.push_back(i->second[j]);
            }
        }
        if(new_v.size() > 0){
            s_str.push_back(new_v);
        }
    }
    
    //construct D-list
    map<int,vector<vector<int>>> d_list;
    
    for(int d = 1; d < max+1;d++){
        
        int cnt = 0;
        
        for(auto i = bucket.begin();i!= bucket.end();i++){
            vector<int> new_v;
            for(int j = 0;j < i->second.size();j ++ ){
                if(dis[i->second[j]-1] == d){
                    //cout << i->second[j] << " ";
                    new_v.push_back(i->second[j]);
                }
            }
            if(new_v.size() != 0 && cnt == 0){
                vector<vector<int>> new_vv;
                new_vv.push_back(new_v);
                d_list[d] = new_vv;
                cnt ++;
            }
            else if(new_v.size() != 0 && cnt > 0)
            {
                d_list[d].push_back(new_v);
            }
        }
    }
    
    //print_str(d_list[2]);
    print_str(s_str);
    
    for(int d = 1;d < max + 1;d++){
        vector<vector<int>> d_table;
        for(int i = 0; i < s_str.size();i++){
            if(s_str[i].size() == 1){
                d_table.push_back(s_str[i]);
                continue;
            }
            vector<int> cp = s_str[i];
            //if no change,just push the same as s_str[i]
            int change = 0;
            
            //plus dis
            for(int j = 0;j < cp.size();j++){
                cp[j] = cp[j] + d;
            }
            //go through d-list
            for(int j = 0;j<d_list[d].size();j++){
               
                vector<int> new_v;
                for(int h = 0; h < d_list[d][j].size();h++){
                    int v = d_list[d][j][h];
                    if(find(cp.begin(),cp.end(),v) != cp.end()){
                        new_v.push_back(v-d);
                        change ++;
                    }
                }
                if(new_v.size() != 0){
                    d_table.push_back(new_v);
                }
            }
            if(change == 0){
                d_table.push_back(s_str[i]);
            }
            else if(change != s_str[i].size()){
                for(int o = 0;o < s_str[i].size();o++){
                    int good = 0;
                    for(int p = 0 ; p < d_table.size();p++){
                        for(int t = 0 ; t < d_table[p].size();t++){
                            if(d_table[p][t] == s_str[i][o]){
                                good = 1;
                                break;
                            }
                        }
                        if(good == 1){
                            break;
                        }
                    }
                    if(good != 1){
                        d_table.back().push_back(s_str[i][o]);
                    }
                }
            }
        }
        
        s_str = d_table;
        print_str(s_str);
    }
   
    return 0;
}
