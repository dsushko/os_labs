#include <ctime>
#include <cstdlib>
#include <iostream>

int main(){
    std::cout << "myprocess start\n";
    srand(time(NULL));
    while(true){
        int r = rand() % 10000;
        //if(r == 100)
            //break;
    }
    return 0;
}