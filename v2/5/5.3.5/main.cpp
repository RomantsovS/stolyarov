#include <unistd.h>
#include <stdio.h>

int main() {

    execlp("ls", "ls", "-l", "-a" ,"/var", NULL);

   return 0;
}