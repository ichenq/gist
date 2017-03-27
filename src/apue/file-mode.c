#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    int fl = 0;
    int fd = -1;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fd>\n", argv[0]);
        return EXIT_FAILURE;
    }
    fd = atoi(argv[1]);
    fl = fcntl(fd, F_GETFL, 0);
    if (fl < 0) {
        strerror(errno);
        return EXIT_FAILURE;
    }
    switch (fl & O_ACCMODE) {
        case O_RDONLY:
            printf("read only");
            break;

        case O_WRONLY:
            printf("write only");
            break;

        case O_RDWR:
            printf("read & write");
            break;

        default:
            printf("unknown access mode");
    }
    if (fl & O_APPEND) {
        printf(", append");
    }
    if (fl & O_NONBLOCK) {
        printf(", nonblocking");
    }
    if (fl & O_SYNC) {
        printf(", synchronous writes");
    }
#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
    if (fl & O_FSYNC) {
        printf(", synchronous writes");
    }
#endif
    putchar('\n');
    return 0;
}