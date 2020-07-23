#define DEBUG true

#include "XBeeCom.h"
#include "RSSILocal.h"
#include <stdlib.h>

struct Vec3 ReadInVec3(FILE* file);

int main(){
        // Open Beanon Info File
        FILE* file = fopen("cords.txt", "r");
        if(file == NULL) {
                printf("Failed to open \"cords.txt\"!\n");
                return -1;
        }

        // "Read in Beacon Cordinats"
        // (Test operation) Results not used in this test
        struct Vec3 p1, p2, p3;
        p1 = ReadInVec3(file);
        p2 = ReadInVec3(file);
        p3 = ReadInVec3(file);

        // Overwrite Read in point locations
        // with testing point position values
        p1 = Vec3_Create(5,4,3);
        p2 = Vec3_Create(3,8,3);
        p3 = Vec3_Create(-3,5,3);

        // "Get RSSI Distances"
        // Not Geting actual distances using testing values
        float d1, d2, d3;
        d1=sqrt(17);
        d2=sqrt(45);
        d3=sqrt(54);

        // Run Trilateration to get position estimate
        struct Vec3 p4 =  Trilateration(p1,p2,p3, d1,d2,d3);
        printf("Cordinator Position: "); Vec3_Print(p4); printf("\n");

        return 0;
}

// Function to read in a Vec3 from a text file
struct Vec3 ReadInVec3(FILE* file){
        struct Vec3 ret;
        fscanf(file, "%f %f %f",&ret.x,&ret.y,&ret.z);
        return ret;
}
