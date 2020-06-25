// Note: Requires a "cord.txt" file in running directory
//       file is 3 wide by 3 high grid of floats that are
//       read in as 3 Vec3's where each row is a Vec3

#include <stdio.h>
#include <stdlib.h>

struct Vec3{ float x,y,z; };

struct Vec3 ReadInVec3(FILE* file);
void PrintVec3(struct Vec3 val);

int main(){
        struct Vec3 pos1, pos2, pos3;
        FILE* file;

        file = fopen("cords.txt", "r");
        if(file == NULL){
                printf("Failed to open cords.txt!\n");
                return -1;
        }

        pos1 = ReadInVec3(file);
        pos2 = ReadInVec3(file);
        pos3 = ReadInVec3(file);

        fclose(file);

        PrintVec3(pos1); printf("\n");
        PrintVec3(pos2); printf("\n");
        PrintVec3(pos3); printf("\n");

        return 0;
}

struct Vec3 ReadInVec3(FILE* file){
        struct Vec3 ret;
        fscanf(file, "%f %f %f",&ret.x,&ret.y,&ret.z);
        return ret;
}

void PrintVec3(struct Vec3 val){
        printf("%f, %f, %f", val.x, val.y, val.z);
}
