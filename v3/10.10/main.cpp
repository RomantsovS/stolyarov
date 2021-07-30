#include <stdio.h>
#include "chat.hpp"

static int port = 7777;

int main()
{
    EventSelector *selector = new EventSelector;
    ChatServer *serv = ChatServer::Start(selector, port);
    if(!serv) {
        perror("server");
        return 1;
    }
    printf("server started\n");
    selector->Run();
    return 0;
}
