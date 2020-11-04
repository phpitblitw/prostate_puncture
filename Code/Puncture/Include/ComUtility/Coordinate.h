#pragma once
//Coordinate.h
//齐次坐标类
//w=0，则表示向量(x,y,z)
//w!=0,则表示点(x/w,y/w,z/w)。w一般取1

#include <math.h>

/*****************************************************************
Defines
*****************************************************************/


namespace fsutility
{
	class AFX_EXT_CLASS Coordinate
	{
	public:
		double x;
		double y;
		double z;
		double w;

		Coordinate();
		Coordinate(double x, double y, double z, double w);
		~Coordinate();

		//运算符重载，坐标或向量的加减运算
		//不应进行点与向量的加减，但是程序中不作检查，默认保持本对象的类别(点/向量)
		Coordinate operator+(Coordinate c);
		Coordinate operator-(Coordinate c);
		
		Coordinate operator*(const double &c);	//坐标或向量的数乘

		void Normalize();	//将点的w设为1 对向量单位化
		double GetDotProduct(Coordinate c);	//向量点乘
		Coordinate GetCrossProduct(Coordinate c);	//向量叉乘

	private:
	};
}

