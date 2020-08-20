#define DEBUG false

#include "../XBeeCom.h"
#include "../RSSILocal.h"
#include <math.h>

float GetRSSIDistance(int port, int beconId, int offDBm, float offConst, unsigned char* readBuf, int bufSize);

int main(int argc, char* argv[]){
	if(argc < 3){
		printf("Error: Please enter a <dBm Offset>[int] and a <Const offset>[float].\n");
		return -1;
	}
	int offDBm = atoi(argv[1]);
        int offConst = atof(argv[2]);
	printf("dBm Offset: %d\nConst Offset: %f\n",offDBm,offConst);

	int port = InitPort("/dev/ttyUSB0",1,255);
        if(port < 0) return -1;

        unsigned char readBuf[30];
        int msgLength;
        int dBm;
        float dist;

        printf("| Beacon # | Distance |\n");
        printf("|----------|----------|\n");

	dist = GetRSSIDistance(port,1,offDBm,offConst,readBuf,sizeof(readBuf));
	printf("| 1        | %f |\n",dist);

	dist = GetRSSIDistance(port,2,offDBm,offConst,readBuf,sizeof(readBuf));
        printf("| 2        | %f |\n",dist);

	dist = GetRSSIDistance(port,3,offDBm,offConst,readBuf,sizeof(readBuf));
        printf("| 3        | %f |\n",dist);

	close(port);
}

float GetRSSIDistance(int port, int beconId, int offDBm, float offConst, unsigned char* readBuf, int bufSize){
        SendRemoteCommand_DB(port,beconId);
        int msgLength = ReadCommand(port,readBuf,bufSize);
        if(msgLength < 0) return -1;
        int dBm = -(readBuf[msgLength-2]);
        return RSSIToMetters(dBm,offDBm,offConst);
}
