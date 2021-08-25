#include "utils.h"
#include <iostream>
using namespace std;


int main(){
    vector<string> tokens;
    string str("0-1231");
    s_split(str,tokens,'-');
    for(auto el:tokens){
        cout<<el<<endl;
    }

    return 0;
    
}