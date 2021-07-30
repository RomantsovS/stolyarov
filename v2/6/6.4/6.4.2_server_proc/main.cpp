#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>

int main() {
    enum {listen_queue_len = 16};
    int ls, ok;
    
    ls = socket(AF_INET, SOCK_STREAM, 0);

    if(ls == -1) {
        perror("failed to start listen socket\n");
        return 1;
    }

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(7654);
    if(bind(ls, (sockaddr*)&addr, sizeof(addr))) {
        perror("bind failed\n");
        return 1;
    }

    if(listen(ls, listen_queue_len) == -1) {
        perror("listen error");
        return 1;
    }

    printf("server started\n");

    const size_t buf_len = 100;
    char buf[buf_len];
    while(1) {
        int cls, pid;
        socklen_t socklen = sizeof(addr);
        cls = accept(ls, (sockaddr*)&addr, &socklen);

        printf("connected: %s\n", inet_ntoa(addr.sin_addr));

        pid = fork();
        if(pid == 0) {
            close(ls);

            while(read(cls, buf, buf_len)) {
                printf("%d %s", pid, buf);
            }
            printf("%d disconnected: %s\n", pid, inet_ntoa(addr.sin_addr));
            exit(0);
        }
        close(cls);

        do {
            pid = wait4(-1, NULL, WNOHANG, NULL);
        }while(pid > 0);
    }
    
    printf("done\n");

    return 0;
}