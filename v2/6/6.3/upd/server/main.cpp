#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(sock == -1) {
        perror("failed to start socket\n");
        return 1;
    }

    timeval tv;
    tv.tv_sec = 15;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    sockaddr_in addr, client;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(7654);
    if(bind(sock, (sockaddr*)&addr, sizeof(addr))) {
        perror("bind failed\n");
        return 1;
    }

    socklen_t socklen = sizeof(client);

    char buf_in[100];
    while(1) {
        int res = recvfrom(sock, buf_in, sizeof(buf_in), 0, (sockaddr*)&client, &socklen);
        printf("received %d bytes\n", res);
        if(res == -1) {
            perror("failed to receive\n");
            //return 1;
        }
        else if(res == 0) {
            return 0;
        }
        else
            printf("received %s\n", buf_in);
    }

    close(sock);
    
    printf("done\n");

    return 0;
}