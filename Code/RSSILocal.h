#include "Vec3.h"
#include "Matrices.h"

const float pi =  3.14159265359f;

float RSSIToMetters(unsigned char dBm);
struct Vec3 Trilateration(struct Vec3 p1, struct Vec3 p2, struct Vec3 p3, float d1, float d2, float d3);

double C2Det(struct Vec3 p, struct Vec3 q);

// Converts Resived Signal Streangth Indegator(RSSI) in dBm to distance value in meaters
float RSSIToMetters(unsigned char dBm) {
        const float C = 0.29979f/(4*pi*2.4f);
        return C*pow(10,dBm/20.0f);
}

// Calculates position of 4th point from 3 given points positions and distances to 4th point
struct Vec3 Trilateration(struct Vec3 p1, struct Vec3 p2, struct Vec3 p3, float d1, float d2, float d3){
        // Calculate Squared Distances
        double d1s = d1*d1, d2s = d2*d2, d3s = d3*d3;

        // Calculate v1 and v2 vector offsets for p2 and p3 from  p1
        struct Vec3 v1 = Vec3_Subtract(p2,p1);
        struct Vec3 v2 = Vec3_Subtract(p3,p1);

        // Precalculate 2x2 Cayley-Manger determinants that are needed
        double cmDet12 = C2Det(p1,p2), cmDet13 = C2Det(p1,p3);
        double cmDet21 = C2Det(p2,p1), cmDet23 = C2Det(p2,p3);
        double cmDet31 = C2Det(p3,p1), cmDet32 = C2Det(p3,p2);

        // D(p1,p2,p3) 4x4 Matrix
        double d123[4*4]  = {
        0, 1      , 1      , 1      ,
        1, 0      , cmDet12, cmDet13,
        1, cmDet21, 0      , cmDet23,
        1, cmDet31, cmDet32, 0      };
        Mat4_t mat123 = d123;

        // D(p1,p2,p3;p1,p3,p4) 4x4  Matrix
        double d134[4*4]  = {
        0, 1      , 1      , 1  ,
        1, 0      , cmDet13, d1s,
        1, cmDet21, cmDet23, d2s,
        1, cmDet31, 0      , d3s};
        Mat4_t mat134 = d134;

        // D(p1,p2,p3;p1,p2,p4) 4x4 Matrix
        double d124[4*4]  = {
        0, 1      , 1      , 1  ,
        1, 0      , cmDet12, d1s,
        1, cmDet21, 0      , d2s,
        1, cmDet31, cmDet32, d3s};
        Mat4_t mat124 = d124;

        // D(p1,p2,p3,p4) 5x5 Matrix
        double d1234[5*5] = {
        0, 1      , 1      , 1      , 1  ,
        1, 0      , cmDet12, cmDet13, d1s,
        1, cmDet21, 0      , cmDet23, d2s,
        1, cmDet31, cmDet32, 0      , d3s,
        1, d1s    , d2s    , d3s    , 0  };
        Mat5_t mat1234 = d1234;

        // Calculate Cayley-Menger Determinates for matrices
        double cmDet123 = (-0.25)*Mat4_Determinate(mat123);
        double cmDet134 = (-0.25)*Mat4_Determinate(mat134);
        double cmDet124 = (-0.25)*Mat4_Determinate(mat124);
        double cmDet1234 = (0.125)*Mat5_Determinate(mat1234);

        // Implementation of trilateration formula
        struct Vec3 retVec = Vec3_Add(
                Vec3_Multiply(v1,-cmDet134),
                Vec3_Subtract(
                        Vec3_Multiply(v2,cmDet124),
                        Vec3_Multiply(Vec3_Cross(v1,v2),sqrt(cmDet1234))
                )
        );
        retVec = Vec3_Add(p1, Vec3_Multiply(retVec,(1/cmDet123)));

        // Return Results of trilateration with Cayley-Menger Determinates
#if DEBUG
        printf("%f,%f,%f,%f\n",cmDet123,cmDet134,cmDet124,cmDet1234);
#endif
        return retVec;
}

// Cayley-Menger Determinate for a 2 3D vectors
double C2Det(struct Vec3 p, struct Vec3 q){
        // Original formula is (Magnitude(p1-p3))^2
        // Where the outer square root of the magnitude equation
        // cancels out with the square outside around that
        // leaving the inner commponent of the magnitude quation
        struct Vec3 SVec = Vec3_Subtract(p,q);
        return ((SVec.x*SVec.x) + (SVec.y*SVec.y) + (SVec.z*SVec.z));
}