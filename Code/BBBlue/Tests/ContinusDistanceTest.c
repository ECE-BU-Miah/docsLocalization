// Test of continus testing of distance value to xBee using Freespace based distance fomula

#define DEBUG false

#include "../XBeeCom.h"
#include <math.h>
#include <unistd.h>

const float pi =  3.14159265359f;

float RSSIToMetters(int dBm);

int main(void){
	int port = InitPort("/dev/ttyUSB0",1,255);
	if(port < 0) return -1;

	unsigned char readBuf[30];

	while(true){
		SendRemoteCommand_DB(port,1);
		int msgLength = ReadCommand(port,readBuf,sizeof(readBuf));
		if(msgLength < 0) return -1;
		unsigned char dBm = readBuf[msgLength-2];
		float dist = RSSIToMetters(-((int)dBm));
		printf("%f\n",dist);
	}
	printf("close");

	//printf("| Beacon 1 | -0x%02X dBm | %fm |\n",dBm,dist);
	//usleep(100);
	close(port);
}

float RSSIToMetters(int dBm) {
	const float C = 0.29979f/(4*pi*2.4f);
	return C*pow(10,-dBm/20.0f);
}
