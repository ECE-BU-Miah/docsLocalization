// Test that a RSSI value can be read Back with XBeeCom.h

#define DEBUG false

#include "XBeeCom.h"

int main(void){
        int port = InitPort("/dev/ttyUSB0",1,255);
        if(port < 0) return -1;

        unsigned char readBuf[30];

        printf("| Beacon # |   RSSI   |\n");
        printf("|----------|----------|\n");

        SendRemoteCommand_DB(port,1);
        int msgLength = ReadCommand(port,readBuf,sizeof(readBuf));
        if(msgLength < 0) return -1;

        printf("| Beacon 1 | 0x%02X dBm |\n",readBuf[msgLength-2]);

        SendRemoteCommand_DB(port,2);
        msgLength = ReadCommand(port,readBuf,sizeof(readBuf));
        if(msgLength < 0) return -1;

        printf("| Beacon 2 | 0x%02X dBm |\n",readBuf[msgLength-2]);

        SendRemoteCommand_DB(port,3);
        msgLength = ReadCommand(port,readBuf,sizeof(readBuf));
        if(msgLength < 0) return -1;

        printf("| Beacon 3 | 0x%02X dBm |\n",readBuf[msgLength-2]);

        close(port);
}
