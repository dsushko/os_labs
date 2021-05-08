#include <string>
#include <signal.h>
#include <dirent.h>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;

int ProcessExistByName(string procName)
{

    string str = "pgrep " + string(procName);
    FILE* cmd;
    pid_t pid = 1;
    pid_t prev = 0;
    char buff[256];
    while(pid != prev) {
        cmd = popen(str.c_str(), "r");
        fgets(buff, 256, cmd);
        //cout << buff << "\n";
        if(strlen(buff)) {
            prev = pid;
            pid = atoi(buff);
            return pid;
            //kill(pid, SIGKILL);
        }
        return 0;
    }
    return false;
}

void KillProcessByName(char* name) {
    int id =ProcessExistByName(name);
    std::cout << (id ? "process exists" : "process doesn't exist") << "\n";
    cout << name << " - " << id << "\n";
    if(id){
        kill(id, SIGKILL);
    }
    id =ProcessExistByName(name);
    std::cout << (id ? "process exists" : "process doesn't exist") << "\n";
}

bool ProcessExistById(pid_t id) {
    return kill(id, 0) == 0;
}

void KillProcessById(pid_t process_id){
    std::cout << (ProcessExistById(process_id) ? "process exists" : "process doesn't exist") << "\n";
    kill(process_id, SIGKILL);
    sleep(1);
    std::cout << (ProcessExistById(process_id) ? "process exists" : "process doesn't exist") << "\n";
}

int main(int argc, char *argv[]){
    std::cout << "killer start\n";
    std::string proc_to_kill;

    char const* temp = getenv("PROC_TO_KILL");
    std::cout << "environment var is " << temp << "\n";
    if (temp != NULL) {
        proc_to_kill = std::string(temp);
        const int buffSize = 65535;
        char buffer[buffSize];
        std::stringstream ss(proc_to_kill);
        while (ss.getline(reinterpret_cast<char *>(&buffer), buffSize, ',')) {

        }
    }
    sleep(1);
    if(argc > 2){
        std::cout << "has args " << argv[1] << " " << argv[2] << "\n";
        if(strcmp(argv[1], "--id") == 0){
            std::cout << "try to kill by id\n";
            KillProcessById(atoi(argv[2]));
            std::cout << "done\n";
        }
        if(strcmp(argv[1],"--name") == 0){
            std::cout<< "try to kill by name\n";
            KillProcessByName(argv[2]);
            std::cout << "done\n";
        }
    }
    stringstream ss((string(temp)));
    char buffer[300];
    int buff_size = 300;
    std::cout << "killing from var\n";
    while (ss.getline(reinterpret_cast<char*>(&buffer), buff_size, ',')) {
        KillProcessByName(buffer);
    }
    std::cout << "done\n";

    unsetenv("PROC_TO_KILL");
    return 0;
}
