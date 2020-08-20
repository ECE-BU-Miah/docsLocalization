#ifndef XEBB_COM_H
#define XBEE_COM_H

// C Library headers
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// Linux headers
#include <fcntl.h>      // Contains file controls like O_RDWR
#include <errno.h>      // Error integer and strerror() function
#include <termios.h>    // Contains POSIX terminal control definitions
#include <unistd.h>     // write(), read(), close()

// prototypes
int InitPort(char* devPath, unsigned char vTime, unsigned char vMin);
void SendRemoteCommand_DB(int port, uint16_t destAddr);
int ReadCommand(int port, unsigned char* buf, int bufSize);
bool CheckChecksum(unsigned char* msg, int length);
unsigned char GetFrameState(unsigned char* msg, int length);

// Initalize Serial Port with given device path
// @param: C string path to device in dev directory
// @return Port Refrence Number
int InitPort(char* devPath, unsigned char vTime, unsigned char vMin){
        int port = open(devPath, O_RDWR);
        if(port < 0){
                printf("Error %i from open: %s\n", errno, strerror(errno));
		return -1;
        }

        struct termios tty;
        memset(&tty,0,sizeof(tty));
        if(tcgetattr(port, &tty) != 0) {
                printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
		close(port);
		return -2;
        }

        // Control Modes (c_cflag)
        tty.c_cflag &= ~PARENB; // Disable parity
        tty.c_cflag &= ~CSTOPB; // Set Stop bit to 1
        tty.c_cflag |= CS8;     // Set 8 bits ber byte
        tty.c_cflag &= ~CRTSCTS;// Diable RTS/CTS
        tty.c_cflag |= CREAD|CLOCAL; // Turn on Read and ignore ctrl lines (CLOCAL = 1)

        // Loacal Modes (c_lflag)
        tty.c_lflag &= ~ICANON; // Disable Cononical mode (wait for new line)
        tty.c_lflag &= ~ECHO;   // Disable echo
        tty.c_lflag &= ~ECHOE;  // Diable erasure
        tty.c_lflag &= ~ECHONL; // Diable new-line echo
        tty.c_lflag &= ~ISIG;   // Disable signal chars

        // Input Modes (c_iflag)
        tty.c_iflag &= ~(IXON|IXOFF|IXANY); // Turn off s/w flow ctrl
        tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);// Disable special handeling

        // Output Modes
        tty.c_oflag &= ~OPOST;  // Disable output specal interpretation
        tty.c_oflag &= ~ONLCR;  // Disable conversion of \r and \n

        // Reading wait conditions
        tty.c_cc[VTIME] = vTime;    // wait for 1 s or 10 ds
        tty.c_cc[VMIN] = vMin;   // Any amount of chars

        // Baud Rate (in and out)
        cfsetispeed(&tty, B9600);
        cfsetospeed(&tty, B9600);

        //save tty settings
        if(tcsetattr(port, TCSANOW, &tty) != 0){
                printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        	close(port);
		return -3;
	}

        return port;
}

// Send a Remote DB AT Command to given becon
// @peram destAddr: 16-bit destination address(MY) of destination XBee
void SendRemoteCommand_DB(int port, uint16_t destAddr){
	// Create and send header for Remote AT command
	static unsigned char msgHeader[13] = {0x7E, 0x00,0x0F, 0x17, 0x01, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
        write(port,msgHeader,13);

	// Send 16-bit destination peram (Stored post order LINUX)
	write(port,((unsigned char*)&destAddr)+1,1);
	write(port,((unsigned char*)&destAddr),1);

	// Send mode and command perams
	static unsigned char modeNCom[3] = {0x02, 0x64,0x62};
	write(port,modeNCom,3);

	// Calculate and Write out checksum with patern
	// count down from 0x26(at addr 1) and loop back to 0xFF based on addres
	unsigned char checkSum = (0xFF)-((0xD8+destAddr)%(0xFF));
	write(port,&checkSum,1);

#if DEBUG
	// Print out Command that was sent in hex
	printf("[DEBUG] CMD: ");
	for(int i=0; i < sizeof(msgHeader); i++)
		printf("0x%02X ",msgHeader[i]);
	printf("0x%02X 0x%02X ", ((unsigned char*)&destAddr)[0], ((unsigned char*)&destAddr)[1]);
	printf("0x%02X 0x%02X 0x%02X ", modeNCom[0], modeNCom[1], modeNCom[2]);
	printf("0x%02X\n",checkSum);
#endif
}

// Read in AT Command response into given buffer
// @peram buf: pointer to unsigned char buffer to hold input
// @peram bufSize: Size of buffer given to "buf"
// @return message length
int ReadCommand(int port,  unsigned char* buf, int bufSize){
         // Read in response
        int num_Bytes = read(port, buf, bufSize);

        // Check if read Errord out
        if(num_Bytes < 0){
                printf("Error reading: %s\n", strerror(errno));
                return -1;
        }
#if DEBUG
        else {
		printf("[DEBUG] Recived: %d Bytes\n",num_Bytes);
		printf("[DEBUG] MSG: ");
		for(int i=0; i < num_Bytes; i++)
			printf("0x%02X ",buf[i]);
		printf("\n");
	}
#endif

        // Check for valid checksum
        if(!CheckChecksum(buf,num_Bytes)){
                printf("Invalid Checksum :(\n");
                return -1;
        }
#if DEBUG
        else printf("[DEBUG] Valid Checksum!\n");
#endif

        // Check if Error flag set in response
        if(GetFrameState(buf,num_Bytes) > 0){
                printf("Command Errored out :(\n");
                return -1;
        }
#if DEBUG
        else printf("[DEBUG] Command Ran Sucesfuly!\n");
#endif

        return num_Bytes;
}

// Calculate AT Message checksum and compare it
// @peram msg: Pointer to buffer containing AT Message
// @peram length: length of AT Meassage in "msg" buffer
// @return bool for if vaild checksum
bool CheckChecksum(unsigned char* msg, int length){
        unsigned int sum = 0;
        for(int i=3; i <= length-2; i++)
                sum += msg[i];
        unsigned char checkSum = (0xFF - (sum & 0xFF));
        return (checkSum == msg[length-1]);
}

// Extract Return AT Message state from AT Return message
// @peram msg: Pointer to buffer containing AT Message
// @peram length: length of AT Meassage in "msg" buffer
// @return Frame State
unsigned char GetFrameState(unsigned char* msg, int length){
        unsigned char frameType = msg[3];        // Get Frame Type
        if(frameType == 0x97) return msg[17];
        else if(frameType = 0x88) return msg[7];
        else {
                printf("Invalied frame type 0x%02X  encountered.\n",frameType);
                return 255;
        }
}

#endif
