#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

volatile sig_atomic_t n = 0;
const char msg[] = "press it agai\n";

void handler(int s) {
    int save_errno = errno;

    signal(SIGINT, handler);

    ++n;
    write(1, msg, sizeof(msg) - 1);
    errno = save_errno;
}

int main() {
    signal(SIGINT, handler);
    while(n < 10)
        sleep(1);

    return 0;
}