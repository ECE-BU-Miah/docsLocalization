#define DEBUG false
#define DEBUG_RSSI

#include "XBeeCom.h"
#include "RSSILocal.h"
#include <stdlib.h>

struct Vec3 ReadInVec3(FILE* file);
float GetRSSIDistance(int port, int beconId, unsigned char* readBuf, int bufSize);

int main(int argc, char* argv[]){
        // Open Beanon Info File
        FILE* file = fopen("cords.txt", "r");
        if(file == NULL) {
                printf("Failed to open \"cords.txt\"!\n");
                return -1;
        }

	// Open port to Cordinator XBee
	int port = InitPort("/dev/ttyUSB0",1,255);
        if(port < 0) return -1;
        unsigned char readBuf[30];

        // Read in Beacon Cordinats
        struct Vec3 p1, p2, p3;
        p1 = ReadInVec3(file);
        p2 = ReadInVec3(file);
        p3 = ReadInVec3(file);

	printf("P1: "); Vec3_Print(p1); printf("\n");
	printf("P2: "); Vec3_Print(p2); printf("\n");
	printf("P3: "); Vec3_Print(p3); printf("\n");

	// Determin mode from Arguments
	int mode = 0; // Defual single sweep mode
	if(argc > 1 && !strcmp(argv[1],"-L"))
		if(argc>2) // Loop Count mode
			mode = atoi(argv[2]);
		else // Continus Loop mode
			mode =-1;

	// Main loop for updating and checking cordinator position
	// mode > 0 -> will loop for mode # of times
	// mode = 0 -> will loop once
	// mode < 0 -> will loop infinitly
	do{
        // Get RSSI Distances
        float d1, d2, d3;
        d1=GetRSSIDistance(port,1,readBuf,sizeof(readBuf));
        d2=GetRSSIDistance(port,2,readBuf,sizeof(readBuf));
        d3=GetRSSIDistance(port,3,readBuf,sizeof(readBuf));
//	d1 = 1.2308f; d2 = 1.5129f; d3 = 1.0258f;
//	d1 = 1.1181f; d2 = 1.5812f; d3 = 1.0308f;
	if(d1==-1 || d2==-1 || d3==-1) return -1;
	printf("D1:%f, D2:%f, D3:%f\n",d1,d2,d3);

        // Run Trilateration to get position estimate
        struct Vec3 p4 =  Trilateration(p1,p2,p3, d1,d2,d3);
        printf("Cordinator Position: "); Vec3_Print(p4); printf("\n");

	if(mode>0) mode--;
	}while(mode!=0);

        return 0;
}

// Function to read in a Vec3 from a text file
struct Vec3 ReadInVec3(FILE* file){
        struct Vec3 ret;
        fscanf(file, "%f %f %f",&ret.x,&ret.y,&ret.z);
        return ret;
}

float GetRSSIDistance(int port, int beconId, unsigned char* readBuf, int bufSize){
	SendRemoteCommand_DB(port,beconId);
        int msgLength = ReadCommand(port,readBuf,bufSize);
        if(msgLength < 0) return -1;
        int dBm = -(readBuf[msgLength-2]);
        return RSSIToMetters(dBm,8,0);
}
