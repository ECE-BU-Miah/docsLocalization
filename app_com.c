#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main() {
    int tx_byte[8] = {0x7E, 0x00, 0x04, 0x08, 0x01, 0x64, 0x62, 0x30};
	int rx_byte[100]={0};
	int *buff,*rx;
    int fd = open("/dev/pts/0", O_RDWR);
	int i;
	rx = rx_byte;
	buff = tx_byte;
	write(fd, &buff, 8);
	
    ssize_t size = read(fd, &rx, 8);
	
	for(i = 0; i<sizeof(rx);i++)
		printf("Read byte %X\n", rx[i]);
    return 0;
}
