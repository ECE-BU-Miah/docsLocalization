// Testing distance from RSSI using fromula from S. Miah Reserch paper

#define DEBUG false

#include "XBeeCom.h"
#include <math.h>

const float pi =  3.14159265359f;

float RSSIToMetters(unsigned char dBm);

int main(void){
        int port = InitPort("/dev/ttyUSB0",1,255);
        if(port < 0) return -1;

        unsigned char readBuf[30];

        printf("| Beacon # |   RSSI    | Distance |\n");
        printf("|----------|-----------|----------|\n");

        SendRemoteCommand_DB(port,1);
        int msgLength = ReadCommand(port,readBuf,sizeof(readBuf));
        if(msgLength < 0) return -1;
        unsigned char dBm = readBuf[msgLength-2];
        float dist = RSSIToMetters(dBm);

        printf("| Beacon 1 | -0x%02X dBm | %fm |\n",dBm,dist);

        SendRemoteCommand_DB(port,2);
        msgLength = ReadCommand(port,readBuf,sizeof(readBuf));
        if(msgLength < 0) return -1;
        dBm = readBuf[msgLength-2];
        dist = RSSIToMetters(dBm);

        printf("| Beacon 2 | -0x%02X dBm | %fm |\n",dBm,dist);

        SendRemoteCommand_DB(port,3);
        msgLength = ReadCommand(port,readBuf,sizeof(readBuf));
        if(msgLength < 0) return -1;
        dBm = readBuf[msgLength-2];
        dist = RSSIToMetters(dBm);

        printf("| Beacon 3 | -0x%02X dBm | %fm |\n",dBm,dist);

        close(port);
}

float RSSIToMetters(unsigned char dBm) {
        return pow(10,((-dBm)+29)/20.0f);
}
