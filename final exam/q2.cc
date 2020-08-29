// You can write your code in C or C++. Modify this file and the makefile
// accordingly. Please test your program on CSE linux machines before submitting
// your solution.
//

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, const char* argv[]){
    string tar = argv[1];
    
    vector<int> out;
    out.push_back(int(tar.length()-2));

    string sub_tar = tar.substr(0,tar.length()-1);
   // cout<<sub_tar<<'\n';
    
    for(int i = int(tar.length()-2);i > -1;i--){
        string suffix = tar.substr(i,tar.length()-i);
     //   cout << suffix;
        if(sub_tar.find(suffix) != string::npos){
            out.insert(out.begin(),int(i-tar.length()));
            continue;
        }
        string suffix2 = tar.substr(i+1,tar.length()-i-1);
        size_t found = sub_tar.find(suffix2);
        if(found != string::npos){
            out.insert(out.begin(),int(found)-1);
            continue;
        }
        
        int v = out[0];
        if(v >= 0){
            out.insert(out.begin(), i-int(tar.length()));
        }else{
            out.insert(out.begin(), v-1);
        }
    }

    
    
    
    //output
    for(int i = 0;i < out.size() - 1;i++){
        cout << out[i] << ' ';
    }
    cout << out[out.size()-1]<<'\n';
    
}
