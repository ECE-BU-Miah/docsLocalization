// Basic com Test with one XBee and using second XBee conected to Beaglebone

// C Library headers
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Linux headers
#include <fcntl.h>	// Contains file controls like O_RDWR
#include <errno.h>	// Error integer and strerror() function
#include <termios.h>	// Contains POSIX terminal control definitions
#include <unistd.h>	// write(), read(), close()

bool CheckChecksum(unsigned char* msg, int length);

int main() {
	unsigned char localBytes[8] = {0x7E,0x00,0x04,0x08,0x01,0x64,0x62,0x30};

	int port = open("/dev/ttyUSB0", O_RDWR);
	if(port < 0){
		printf("Error %i from open: %s\n", errno, strerror(errno));
	}

	struct termios tty;
	memset(&tty,0,sizeof(tty));
	if(tcgetattr(port, &tty) != 0) {
   		 printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	}

	// Control Modes (c_cflag)
	tty.c_cflag &= ~PARENB; // Disable parity
	tty.c_cflag &= ~CSTOPB; // Set Stop bit to 1
	tty.c_cflag |= CS8;	// Set 8 bits ber byte
	tty.c_cflag &= ~CRTSCTS;// Diable RTS/CTS
	tty.c_cflag |= CREAD|CLOCAL; // Turn on Read and ignore ctrl lines (CLOCAL = 1)

	// Loacal Modes (c_lflag)
	tty.c_lflag &= ~ICANON;	// Disable Cononical mode (wait for new line)
	tty.c_lflag &= ~ECHO;	// Disable echo
	tty.c_lflag &= ~ECHOE;	// Diable erasure
	tty.c_lflag &= ~ECHONL;	// Diable new-line echo
	tty.c_lflag &= ~ISIG;	// Disable signal chars

	// Input Modes (c_iflag)
	tty.c_iflag &= ~(IXON|IXOFF|IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);// Disable special handeling

	// Output Modes
	tty.c_oflag &= ~OPOST;	// Disable output specal interpretation
	tty.c_oflag &= ~ONLCR;	// Disable conversion of \r and \n

	// Reading wait conditions
	tty.c_cc[VTIME] = 1;	// wait for 1 s or 10 ds
	tty.c_cc[VMIN] = 255;	// Any amount of chars

	// Baud Rate (in and out)
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);

	//save tty settings
	if(tcsetattr(port, TCSANOW, &tty) != 0){
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
	}

	// Send DB AT Command to Cordinator
	write(port,localBytes,8);

	// Read in response
	unsigned char read_buf[256];
	memset(&read_buf,'\0',256);
	int num_Bytes = read(port, &read_buf, 256);

	if(num_Bytes < 0){
		printf("Error reading: %s\n", strerror(errno));
	} else {
		printf("Recived: %d Bytes\n",num_Bytes);
	}

	if(CheckChecksum(read_buf,num_Bytes))
		printf("Valid Checksum!\n");
	else
		printf("Invalid Checksum :(\n");

	close(port);
}

bool CheckChecksum(unsigned char* msg, int length){
	unsigned int sum = 0;
	for(int i=3; i <= length-2; i++)
		sum += msg[i];
	unsigned char checkSum = (0xFF - (sum & 0xFF));
	return (checkSum == msg[length-1]);
}
