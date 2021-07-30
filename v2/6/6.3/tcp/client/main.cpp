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

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock == -1) {
        perror("failed to start socket\n");
        return 1;
    }

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7654);
    if(inet_aton("127.0.0.1", &addr.sin_addr) == 0) {
        perror("iten_aton failed'n");
        return 1;
    }

    const size_t buf_size = 100;

    char buf[buf_size];
   
   if(connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
       perror("connect failed\n");
       return 1;
   }

   printf("connect succesfull\n");

   while(read(0, buf, buf_size)) {
       write(sock, buf, strlen(buf));
   }

   close(sock);

    printf("done\n");

    return 0;
}