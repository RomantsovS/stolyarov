#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/cdrom.h>
#include <dirent.h>

int main() {
    DIR *dir;
    dirent *dent;
    const char *name;
    dir = opendir(".");

    if(!dir) {
        perror(name);
        return 1;
    }

    while((dent = readdir(dir)) != NULL) {
        printf("%s\n", dent->d_name);
    }

   return 0;
}