#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pid;
    pid = fork();
    if(pid == -1) {
        perror("fork");
        exit(1);
    }
    if(pid == 0) {
        execlp("ls", "ls", "-l", "-a" ,"/var", NULL);
        perror("ls");
        exit(1);
    }
    wait(NULL);

    printf("ok\n");

    return 0;
}