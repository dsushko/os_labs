#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

int main(){
    //cout << "s launched";

    string input;
    getline(cin, input);
    stringstream parcer(input);
    stringstream output;
    int curr_num;
    long long sum = 0;
    while(parcer >> curr_num){
        sum += curr_num;
    }
    cout << sum;
    return 0;
}
