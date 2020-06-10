
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main() {
    char byte;
    int fd = open("/dev/pts/0", O_RDWR);
    write(fd, "X", 1);
    ssize_t size = read(fd, &byte, 1);
    printf("Read byte %i\n", byte);
    return 0;
