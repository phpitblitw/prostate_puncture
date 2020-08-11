/*_____________________________________________________
 |  文件：Matrix.h
 |
 |  功能：矩阵运算
 |
 |	源程序测试：进行了黑匣测试
 |_____________________________________________________*/


#if !defined(MATRIX_H)
#define		 MATRIX_H

#include "Vector.h"
#include "Rotation.h"

class _declspec(dllexport) SceneGraphMatrix
{
public:
	SceneGraphMatrix();
	float m[16];
	float *getMatrix(){return (float *)m;}
};

// 矩阵复制 
// 输入参数：A(i,j)
// 输出参数：M=A
void _declspec(dllexport) MatrixCopy(float *M, 
			const float *A, const int i=4, const int j=4); 

// 矩阵相加 
// 输入参数：A(i,j)  B(i,j)
// 输出参数：M=A+B
void _declspec(dllexport) MatrixAdd(float *M, 
		   const float *A, const float *B,
		   const int  i=4, const int  j=4); 

// 矩阵相减 
// 输入参数：A(i,j)  B(i,j)
// 输出参数：M=A-B
void _declspec(dllexport) MatrixSub(float *M, 
		   const float *A, const float *B,
		   const int  i=4, const int  j=4); 

// 矩阵相乘 
// 输入参数：M1(i1,j1)   M2(i2,j2)
// 输出参数：M (i1,j2) = M1*M2
// 约束条件：i2=j1，否则不计算 M 
void _declspec(dllexport) MatrixMultiply(float *M, 
			    const float *M1, const float *M2, 
			    const int  i1, const int  j1, 
			    const int  i2, const int  j2);

// 矩阵转置 
// 输入参数：A(i,j)
// 输出参数：M(j,i) = A 的转置
void _declspec(dllexport) MatrixTranspose(float *M, 
				 const float *A, const int i=4, const int j=4); 

// 矩阵转置 
// 输入参数：A(i,j)
// 输出参数：A(j,i) = A 的转置
void _declspec(dllexport) MatrixTranspose(float *A, const int i=4, const int j=4); 

// 矩阵求逆 
// 输入参数：A(n, n)
// 输出参数：M = A 的逆
// 返回值  ：true 表示成功，false 表示该逆矩阵不存在（M 保持不变）
bool _declspec(dllexport) MatrixInverse(float *M, 
			   const float *A, const int n=4);

// 矩阵求逆 
// 输入参数：A(n, n)
// 输出参数：A = A 的逆
// 返回值  ：true 表示成功，false 表示该逆矩阵不存在（A 保持不变）
bool _declspec(dllexport) MatrixInverse(float *A, const int n=4);

// 矩阵归一化 
// 输入参数：A(n, n)
// 输出参数：A = A 的归一化矩阵
void _declspec(dllexport) MatrixIdentity(float *A, const int n=4);


// V 经过 M 变换后为 V', 函数返回 V'
VECTOR _declspec(dllexport) VectorTransform(const VECTOR V, const float  M[16]);

QUATERNION _declspec(dllexport) QuaternionTransform(const QUATERNION V,const float M[16]);

#endif