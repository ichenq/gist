#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    struct stat buf;
    for (int i = 1; i < argc; i++) {
        int r = 0;
        printf("%s: ", argv[i]);
        r = lstat(argv[i], &buf);
        if (r < 0) {
            strerror(errno);
            continue;
        }
        if (S_ISREG(buf.st_mode)) {
            printf("regular file\n");
        } else if (S_ISDIR(buf.st_mode)) {
            printf("directory\n");
        } else if (S_ISCHR(buf.st_mode)) {
            printf("character special\n");
        } else if (S_ISBLK(buf.st_mode)) {
            printf("block special\n");
        } else if (S_ISFIFO(buf.st_mode)) {
            printf("fifo\n");
        } else if (S_ISLNK(buf.st_mode)) {
            printf("symbolic link\n");
        } else if (S_ISSOCK(buf.st_mode)) {
            printf("socket\n");
        } else {
            printf("** unknown mode **\n");
        }
    }
    return 0;
}
