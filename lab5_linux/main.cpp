#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main (int argc, char ** argv) {
    int i;

    for( i=1; i<argc-1; i++)
    {   //0 - stdin, 1 - stdout
        int pd[2]; // pd[0] - end of read, pd[1] - end of write
        pipe(pd);
        cout << "launched " << argv[i] << "\n";
        if (!fork()) { //child process
            dup2(pd[1], 1); // remap output back to parent (main)
            execlp(argv[i], argv[i], NULL); //running with output headed in main
            perror("exec");
            abort();
        }

        //pipe has the same data but pd[0] is used to remap input, pd[1] - remap output
        dup2(pd[0], 0); // 0 - stdin
        close(pd[1]);
    }

    //last remains with remapped stdin, but with standard stdout

    execlp(argv[i], argv[i], NULL);
    perror("exec");
    abort();
}