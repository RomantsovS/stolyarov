#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

volatile sig_atomic_t child_ready;

void usr1dhl(int s) {
    child_ready = 1;
}

int main() {
    int pid;

    child_ready = 0;
    signal(SIGUSR1, usr1dhl);

    pid = fork();

    if(pid == 0) {
        sleep(3);
        kill(getppid(), SIGUSR1);
        exit(0);
    }

    while(!child_ready)
        pause();

    printf("done\n");

    return 0;
}