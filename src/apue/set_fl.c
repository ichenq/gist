#include <string.h>
#include <errno.h>
#include <fcntl.h>

int set_fl(int fd, int flags) {
    int r = 0;
    int val = fcntl(fd, F_GETTL, 0);
    if (val < 0) {
        strerror(errno);
        return val;
    }
    val |= flags;
    r = fcntl(fd, F_SETFL, val);
    if (r < 0) {
        strerror(errno);
        return r;
    }
    return 0;
}

int clr_fl(int fd, int flags) {
    int r = 0;
    int val = fcntl(fd, F_GETTL, 0);
    if (val < 0) {
        strerror(errno);
        return val;
    }
    val &= ~flags;
    r = fcntl(fd, F_SETFL, val);
    if (r < 0) {
        strerror(errno);
        return r;
    }
    return 0;
}