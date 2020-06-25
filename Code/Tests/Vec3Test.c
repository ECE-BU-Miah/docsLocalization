#include "Vec3.h"

int main() {
        struct Vec3 v1 = Vec3_Create(1,2,3);
        struct Vec3 v2 = Vec3_Create(3,2,1);
        printf("Vec3 v1: "); Vec3_Print(v1); printf("\n");
        printf("Vec3 v2: "); Vec3_Print(v2); printf("\n");

        printf("v1-v2:   "); Vec3_Print(Vec3_Subtract(v1,v2)); printf("\n");
        printf("v1+v2:   "); Vec3_Print(Vec3_Add(v1,v2)); printf("\n");
        printf("v1*3:    "); Vec3_Print(Vec3_Multiply(v1,3)); printf("\n");
        printf("v1 X v2: "); Vec3_Print(Vec3_Cross(v1,v2)); printf("\n");
        printf("|v1|: %f\n", Vec3_Magnitude(v1));
}
