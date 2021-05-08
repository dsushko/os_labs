#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int main(){
    //cout << "m launched";
    string input;
    getline(cin, input);
    stringstream parcer(input);
    stringstream output;
    int curr_num;
    while(parcer >> curr_num){
        output << curr_num*7 << " ";
    }
    cout << output.str();
    return 0;
}
