#include <unistd.h>
#include <stdio.h>

int main() {
    for (int n = 0; n < 5; ++n)
    {
        int i = fork();

        if (i == 0)
            printf("fork\n");
        else
            printf("main\n");
    }

   return 0;
}