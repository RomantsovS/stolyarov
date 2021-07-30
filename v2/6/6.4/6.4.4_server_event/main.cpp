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
#include <vector>
#include <algorithm>

int main() {
    enum {listen_queue_len = 16};
    int ls, ok, cls;
    
    std::vector<int> client_sockets;

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
        int fd, res;
        fd_set readfds, writefds;
        int max_d = ls;

        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(ls, &readfds);

        for(auto & fd : client_sockets) {
            FD_SET(fd, &readfds);

            if(fd > max_d)
                max_d = fd;
        }

        timeval timeout;
        timeout.tv_sec = 3;
        timeout.tv_usec = 0;

        res = select(max_d + 1, &readfds, &writefds, NULL, NULL);
        if(res == -1) {
            if(errno = EINTR) {

            }
            else {
                perror("select error\n");
            }
            continue;
        }
        if(res == 0) {
            continue;
        }
        if(FD_ISSET(ls, &readfds)) {
            socklen_t socklen = sizeof(addr);
            cls = accept(ls, (sockaddr *)&addr, &socklen);

            printf("connected: %d %s\n", cls, inet_ntoa(addr.sin_addr));

            client_sockets.push_back(cls);
        }

        for(auto & fd : client_sockets) {
            if(FD_ISSET(fd, &readfds)) {
                res = read(fd, buf, buf_len);
                if(res == -1) {
                    printf("error read from %d", fd);
                }
                else if(res == 0) {
                    printf("disconnected %d\n", fd);
                    client_sockets.erase(std::remove(client_sockets.begin(), 
                    client_sockets.end(), fd), client_sockets.end());
                }
                else {
                    printf("%d %s", fd, buf);
                }
            }
        }
    }
    
    printf("done\n");

    return 0;
}