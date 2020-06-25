#include <stdio.h>
#include <math.h>

struct Vec3{ float x,y,z; };

struct Vec3 Vec3_Create(float x, float y, float z);
void Vec3_Print(struct Vec3 vec);

float Vec3_Magnitude(struct Vec3 vec);
struct Vec3 Vec3_Add(struct Vec3 vec1, struct Vec3 vec2);
struct Vec3 Vec3_Subtract(struct Vec3 vec1, struct Vec3 vec2);
struct Vec3 Vec3_Cross(struct Vec3 vec1, struct Vec3 vec2);
struct Vec3 Vec3_Multiply(struct Vec3 vec, float f);

struct Vec3 Vec3_Create(float x, float y, float z){
        struct Vec3 ret;
        ret.x=x; ret.y=y; ret.z=z;
        return ret;
}

void Vec3_Print(struct Vec3 vec){
        printf("(%f, %f, %f)", vec.x, vec.y, vec.z);
}

float Vec3_Magnitude(struct Vec3 vec){
        return sqrt((vec.x*vec.x)+(vec.y*vec.y)+(vec.z*vec.z));
}

struct Vec3 Vec3_Add(struct Vec3 vec1, struct Vec3 vec2){
        struct Vec3 ret;
        ret.x = vec1.x + vec2.x;
        ret.y = vec1.y + vec2.y;
        ret.z = vec1.z + vec2.z;
        return ret;
}

struct Vec3 Vec3_Subtract(struct Vec3 vec1, struct Vec3 vec2){
        struct Vec3 ret;
        ret.x = vec1.x - vec2.x;
        ret.y = vec1.y - vec2.y;
        ret.z = vec1.z - vec2.z;
        return ret;
}

struct Vec3 Vec3_Cross(struct Vec3 vec1, struct Vec3 vec2){
        struct Vec3 ret;
        ret.x = (vec1.y*vec2.z) - (vec1.z*vec2.y);
        ret.y = (vec1.z*vec2.x) - (vec1.x*vec2.z);
        ret.z = (vec1.x*vec2.y) - (vec1.y*vec2.x);
        return ret;
}

struct Vec3 Vec3_Multiply(struct Vec3 vec, float f){
        struct Vec3 ret;
        ret.x = vec.x * f;
        ret.y = vec.y * f;
        ret.z = vec.z * f;
}