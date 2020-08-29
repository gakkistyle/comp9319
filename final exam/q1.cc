// You can write your code in C or C++. Modify this file and the makefile
// accordingly. Please test your program on CSE linux machines before submitting
// your solution.
//

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

int main(int argc, const char* argv[])
{
  //printf("Please modify the source of this program.\n");
    
    string tar = argv[1];
    vector<string> str_vec;
    str_vec.push_back(tar);
    
    string rota_str = tar.back() + tar.substr(0,tar.length()-1);
    
    
    while(rota_str != tar){
        str_vec.push_back(rota_str);
        
        rota_str = rota_str.back() + rota_str.substr(0,rota_str.length() - 1);
        
       
    }
    
    sort(str_vec.begin(),str_vec.end());
    
    for(int i = 0;i < str_vec.size();i++){
        cout<<str_vec[i].back();
    }

    cout << '\n';
    return 0;
}
