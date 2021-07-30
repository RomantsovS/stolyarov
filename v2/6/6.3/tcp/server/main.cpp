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
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(listen_fd == -1) {
        perror("failed to start listen socket\n");
        return 1;
    }

    sockaddr_in addr, client;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(7654);
    if(bind(listen_fd, (sockaddr*)&addr, sizeof(addr))) {
        perror("bind failed\n");
        return 1;
    }

    if(listen(listen_fd, 10) == -1) {
        perror("listen error");
        return 1;
    }

    printf("server started\n");

    socklen_t socklen = sizeof(client);

    char buf[100];
    while(1) {
        int conn_fd = accept(listen_fd, (sockaddr*)&client, &socklen);

        printf("connected: %s\n", inet_ntoa(client.sin_addr));

        auto ticks = time(NULL);
        snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
        write(0, buf, strlen(buf));
        
        while(read(conn_fd, buf, sizeof(buf))) {
            printf("%s", buf);
        }

        close(conn_fd);
        printf("disconnected: %s\n", inet_ntoa(client.sin_addr));

        sleep(1);
    }
    
    printf("done\n");

    return 0;
}