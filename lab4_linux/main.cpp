#include <string>
#include <cstdlib>
#include <unistd.h>
#include <wait.h>
#include <iostream>

pid_t CreateProc(const char* name){
    pid_t id = fork();
    if(!id){
        execl(name, NULL);
        exit(0);
    }
    else
        return id;
}

int main(){
    char* e[] = {"PROC_TO_KILL=Calculator", NULL};
    std::cout << "user start\n";

    pid_t p1id = CreateProc("myprocess1");
    std::cout << 1 << "\n";
    pid_t id1 = fork();
    if(!id1) {
        execle("killer", "d", "--id", std::to_string(p1id).c_str(), NULL, e);
        exit(0);
    }
    sleep(2);
    
    wait(nullptr);
    wait(nullptr);
    waitpid(id1, nullptr, 0);

    pid_t p2id = CreateProc("myprocess2");
    std::cout << 2 << "\n";
    pid_t id2 = fork();
    if(!id2) {
        execle("killer", "d", "--name", "myprocess2", NULL, e);
        exit(0);
    }
    wait(nullptr);
    wait(nullptr);
    waitpid(id2, nullptr, 0);
    return 0;
}