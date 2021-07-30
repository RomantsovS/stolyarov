#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

int main() {
    int fd[2];
    pipe(fd);
    if(fork() == 0) {
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        execlp("ls", "ls", "-lR", NULL);
        perror("ls");
        exit(1);
    }
    if(fork() == 0) {
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        execlp("grep", "grep", "m", NULL);
        perror("grep");
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);

    printf("done\n");

    return 0;
}