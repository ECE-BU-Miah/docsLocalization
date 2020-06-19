// Basic Communication test with multiple XBees(3) from Cordinator XBee and Beglebone

// Core Definitions
#define DEBUG false

// C Library headers
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Linux headers
#include <fcntl.h>      // Contains file controls like O_RDWR
#include <errno.h>      // Error integer and strerror() function
#include <termios.h>    // Contains POSIX terminal control definitions
#include <unistd.h>     // write(), read(), close()

int InitPort();
bool CheckChecksum(unsigned char* msg, int length);
int RunATCommand(int port, unsigned char* cmd, int cmdSize, unsigned char* buf, int bufSize);
unsigned char GetFrameState(unsigned char* msg, int length);

struct DBPair{
        bool failed;
        unsigned char cord;
        unsigned char bcon;
};

struct DBPair GetDBs(int port, int bconNum);

// Set up Command codes
unsigned char localDB[8] = {0x7E,0x00,0x04,0x08,0x01,0x64,0x62,0x30};
unsigned char remoteDB_1[19] = {0x7E, 0x00,0x0F, 0x17, 0x01, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, 0x00,0x01, 0x02, 0x64,0x62, 0x26};
unsigned char remoteDB_2[19] = {0x7E, 0x00,0x0F, 0x17, 0x01, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, 0x00,0x02, 0x02, 0x64,0x62, 0x25};
unsigned char remoteDB_3[19] = {0x7E, 0x00,0x0F, 0x17, 0x01, 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, 0x00,0x03, 0x02, 0x64,0x62, 0x24};
unsigned char* remoteCMDs[3] = {remoteDB_1, remoteDB_2, remoteDB_3};

// Create Read Buffer
#define bufferSize 50
unsigned char read_buf[bufferSize];

int main(){
        // Initalize port
        int port = InitPort();
        struct DBPair dBm;

        // Run DB Check with Beacon 1
        dBm = GetDBs(port,1);
        if(dBm.failed){ close(port); return 1; }

        printf("Beacon 1 [Forward] : %X dBm\n",dBm.bcon);
        printf("Beacon 1 [Backward]: %X dBm\n",dBm.cord);

        // Run DB Check with Beacon 2
        dBm = GetDBs(port,2);
        if(dBm.failed){ close(port); return 1; }

        printf("Beacon 2 [Forward] : %X dBm\n",dBm.bcon);
        printf("Beacon 2 [Backward]: %X dBm\n",dBm.cord);

        // Run DB Check with Beacon 3
        dBm = GetDBs(port,3);
        if(dBm.failed){ close(port); return 1; }

        printf("Beacon 3 [Forward] : %X dBm\n",dBm.bcon);
        printf("Beacon 3 [Backward]: %X dBm\n",dBm.cord);

        close(port);
}

struct DBPair GetDBs(int port, int bconNum){
        // Init return pair struct
        struct DBPair tmp;
        tmp.failed = false;
        tmp.cord = 0;
        tmp.bcon = 0;

        // Run DB Remote AT Command for Beacon 1
        int numBytes = RunATCommand(port, remoteCMDs[bconNum-1], 19, read_buf,bufferSize);
        if(numBytes < 0){ tmp.failed = true; return tmp; }
        tmp.bcon = read_buf[numBytes-2];

        // Run DB AT Command for Cordinator
        numBytes = RunATCommand(port, localDB, 8, read_buf,bufferSize);
        if(numBytes < 0){ tmp.failed = true; return tmp; }
        tmp.cord = read_buf[numBytes-2];

        return tmp;
}

int RunATCommand(int port, unsigned char* cmd, int cmdSize, unsigned char* buf, int bufSize){
        // Send out Command
        write(port,cmd,cmdSize);

        // Read in response
        int num_Bytes = read(port, buf, bufSize);

        // Check if read Errord out
        if(num_Bytes < 0){
                printf("Error reading: %s\n", strerror(errno));
                return -1;
        }
#if DEBUG
        else printf("[DEBUG] Recived: %d Bytes\n",num_Bytes);
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

unsigned char GetFrameState(unsigned char* msg, int length){
        unsigned char frameType = msg[3];        // Get Frame Type
        if(frameType == 0x97) return msg[17];
        else if(frameType = 0x88) return msg[7];
        else {
                printf("Invalied frame type %X  encountered.\n",frameType);
                return 255;
        }
}

int InitPort(){
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
        tty.c_cc[VTIME] = 1;    // wait for 1 s or 10 ds
        tty.c_cc[VMIN] = 255;   // Any amount of chars

        // Baud Rate (in and out)
        cfsetispeed(&tty, B9600);
        cfsetospeed(&tty, B9600);

        //save tty settings
        if(tcsetattr(port, TCSANOW, &tty) != 0){
                printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        }

        return port;
}

bool CheckChecksum(unsigned char* msg, int length){
        unsigned int sum = 0;
        for(int i=3; i <= length-2; i++)
                sum += msg[i];
        unsigned char checkSum = (0xFF - (sum & 0xFF));
        return (checkSum == msg[length-1]);
}