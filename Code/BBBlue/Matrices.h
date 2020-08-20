#include <stdlib.h>

typedef double* Mat4_t;
typedef double* Mat5_t;

Mat4_t Mat4_Create(Mat4_t mat);
double Mat4_Determinate(Mat4_t mat);

Mat5_t Mat5_Create(Mat5_t mat);
double Mat5_Determinate(Mat5_t mat);

Mat4_t Mat4_Create(Mat4_t mat){
	Mat4_t retMat = calloc(sizeof(double), 4*4);

	if(mat){
		retMat[0] =  mat[0];
		retMat[1] =  mat[1];
		retMat[2] =  mat[2];
		retMat[3] =  mat[3];
		retMat[4] =  mat[4];
		retMat[5] =  mat[5];
		retMat[6] =  mat[6];
		retMat[7] =  mat[7];
		retMat[8] =  mat[8];
		retMat[9] =  mat[9];
		retMat[10] = mat[10];
		retMat[11] = mat[11];
		retMat[12] = mat[12];
		retMat[13] = mat[13];
		retMat[14] = mat[14];
		retMat[15] = mat[15];
	}

	return retMat;
}

double Mat4_Determinate(Mat4_t mat){
	double m00 = mat[0],  m01 = mat[1],  m02 = mat[2],  m03 = mat[3];
	double m10 = mat[4],  m11 = mat[5],  m12 = mat[6],  m13 = mat[7];
	double m20 = mat[8],  m21 = mat[9],  m22 = mat[10], m23 = mat[11];
	double m30 = mat[12], m31 = mat[13], m32 = mat[14], m33 = mat[15];

	return (m30 * m21 * m12 * m03 - m20 * m31 * m12 * m03 - m30 * m11 * m22 * m03 + m10 * m31 * m22 * m03 +
	        m20 * m11 * m32 * m03 - m10 * m21 * m32 * m03 - m30 * m21 * m02 * m13 + m20 * m31 * m02 * m13 +
	        m30 * m01 * m22 * m13 - m00 * m31 * m22 * m13 - m20 * m01 * m32 * m13 + m00 * m21 * m32 * m13 +
	        m30 * m11 * m02 * m23 - m10 * m31 * m02 * m23 - m30 * m01 * m12 * m23 + m00 * m31 * m12 * m23 +
	        m10 * m01 * m32 * m23 - m00 * m11 * m32 * m23 - m20 * m11 * m02 * m33 + m10 * m21 * m02 * m33 +
	        m20 * m01 * m12 * m33 - m00 * m21 * m12 * m33 - m10 * m01 * m22 * m33 + m00 * m11 * m22 * m33);
}

Mat5_t Mat5_Create(Mat5_t mat){
	Mat5_t retMat = calloc(sizeof(double),5*5);

	if(mat){
		// TO be done coppy contructor copying
	}

	return retMat;
}

double Mat5_Determinate(Mat5_t mat){
	double m00 = mat[0] , m01 = mat[1] , m02 = mat[2] , m03 = mat[3] , m04 = mat[4];
	double m10 = mat[5] , m11 = mat[6] , m12 = mat[7] , m13 = mat[8] , m14 = mat[9];
	double m20 = mat[10], m21 = mat[11], m22 = mat[12], m23 = mat[13], m24 = mat[14];
	double m30 = mat[15], m31 = mat[16], m32 = mat[17], m33 = mat[18], m34 = mat[19];
	double m40 = mat[20], m41 = mat[21], m42 = mat[22], m43 = mat[23], m44 = mat[24];

	// Section 0 calc
	double mat4[4*4] = {
		m11, m12, m13, m14,
		m21, m22, m23, m24,
		m31, m32, m33, m34,
		m41, m42, m43, m33
	};
	double t0 = m00*Mat4_Determinate(mat4);

	// Section 1 calc
	mat4[0]=m01; mat4[1]=m02; mat4[2]=m03; mat4[3]=m04;
	double t1 = m10*Mat4_Determinate(mat4);

	// Section 2 calc
	mat4[4]=m11; mat4[5]=m12; mat4[6]=m13; mat4[7]=m14;
        double t2 = m20*Mat4_Determinate(mat4);

	// Section 3 calc
	mat4[8]=m21; mat4[9]=m22; mat4[10]=m23; mat4[11]=m24;
        double t3 = m30*Mat4_Determinate(mat4);

	// Section 4 calc
	mat4[12]=m31; mat4[13]=m32; mat4[14]=m33; mat4[15]=m34;
        double t4 = m40*Mat4_Determinate(mat4);

	// Fianl combined return mat5 determinate
	return t0-t1+t2-t3+t4;
}
