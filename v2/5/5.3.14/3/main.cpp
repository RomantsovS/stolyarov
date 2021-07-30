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
    sigset_t mask_usr1, mask_empty;
    sigemptyset(&mask_usr1);
    sigaddset(&mask_usr1, SIGUSR1);
    sigemptyset(&mask_empty);

    child_ready = 0;
    sigprocmask(SIG_SETMASK, &mask_usr1, NULL);
    signal(SIGUSR1, usr1dhl);

    pid = fork();

    if(pid == 0) {
        sleep(3);
        kill(getppid(), SIGUSR1);
        exit(0);
    }

    while(!child_ready)
        sigsuspend(&mask_empty);

    printf("done\n");

    return 0;
}