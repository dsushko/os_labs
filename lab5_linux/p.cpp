#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int main(){
    //cout << "p launched";

    string input;
    getline(cin, input);
    stringstream parcer(input);
    stringstream output;
    int curr_num;
    while(parcer >> curr_num){
        output << curr_num*curr_num*curr_num << " ";
    }
    cout << output.str();
    return 0;
}
