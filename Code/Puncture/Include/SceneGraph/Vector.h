/*_____________________________________________________
 |                                          
 |  文件：Vector.h
 |
 |  功能：三维矢量运算
 |
 |	源程序测试：进行了单步跟踪测试
 |_____________________________________________________*/


#if !defined(VECTOR_H)
#define		 VECTOR_H

// 定义三维矢量 VECTOR
struct VECTOR
{
	float x,y,z;
};

struct VECTOR3ui
{
	unsigned int x,y,z;
};

// 矢量求模，返回值 = |A|
 float _declspec(dllexport) VectorMagnitude(const VECTOR A);

// 矢量归一化, 如果 |A|=0，输出值 = A；否则输出值 = A/(|A|)
 void  _declspec(dllexport) VectorNormalize(VECTOR *A);

// 矢量相加，返回值 = A + B
 VECTOR _declspec(dllexport) VectorAdd(const VECTOR A, const VECTOR B);
 VECTOR _declspec(dllexport) operator+(const VECTOR A, const VECTOR B);

// 矢量相减，返回值 = A - B
 VECTOR _declspec(dllexport) VectorSub(const VECTOR A, const VECTOR B);
 VECTOR _declspec(dllexport) operator-(const VECTOR A, const VECTOR B);

// 矢量缩放，返回值 = s * A
 VECTOR _declspec(dllexport) VectorScale(const VECTOR A, const float  s);
 VECTOR _declspec(dllexport) operator * (const VECTOR A, const float  s);
 VECTOR _declspec(dllexport) VectorScale(const float  s, const VECTOR A);
 VECTOR _declspec(dllexport) operator * (const float  s, const VECTOR A);

// 矢量叉积，返回值 = A x B
 VECTOR _declspec(dllexport) VectorCross(const VECTOR A, const VECTOR B);
 VECTOR _declspec(dllexport) operator * (const VECTOR A, const VECTOR B);

// 矢量点积，返回值 = A . B
 float  _declspec(dllexport) VectorDot(const VECTOR A, const VECTOR B);
 float  _declspec(dllexport) operator^(const VECTOR A, const VECTOR B);


#endif