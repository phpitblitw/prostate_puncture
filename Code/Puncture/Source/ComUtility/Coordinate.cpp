//Coordinate.cpp
#include "stdafx.h"
#include "Coordinate.h"

using namespace fsutility;

Coordinate::Coordinate()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 0;
}

Coordinate::Coordinate(double x, double y, double z, double w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Coordinate Coordinate::operator+(Coordinate c)
{
	Coordinate result;
	//如果是点坐标，则将w置为1
	if (this->w != 0)
		this->Normalize();
	if (c.w != 0)
		c.Normalize();
	result.x = this->x + c.x;
	result.y = this->y + c.y;
	result.z = this->z + c.z;
	result.w = this->w;
	return result;
}

Coordinate Coordinate::operator-(Coordinate c)
{
	Coordinate result;
	//如果是点坐标，则将w置为1
	if (this->w != 0)
		this->Normalize();
	if (c.w != 0)
		c.Normalize();
	result.x = this->x - c.x;
	result.y = this->y - c.y;
	result.z = this->z - c.z;
	result.w = this->w;
	return result;
}

Coordinate Coordinate::operator*(const double &c)
{
	Coordinate result;
	result.x = this->x*c;
	result.y = this->y*c;
	result.z = this->z*c;
	result.w = this->w;
	return result;
}

void Coordinate::Normalize()
{
	if (this->w == 0)	//如果是向量，则对向量单位化
	{
		double dDenominator;
		dDenominator = sqrt(x*x + y * y + z * z);
		x /= dDenominator;
		y /= dDenominator;
		z /= dDenominator;
	}
	else				//	如果是点，则将点的w置为1
	{
		x /= w;
		y /= w;
		z /= w;
		w = 1;
	}
	return;
}

Coordinate::~Coordinate()
{}

double Coordinate::GetDotProduct(Coordinate c)
{
	double result;
	result = this->x*c.x + this->y*c.y + this->z*c.z;
	return result;
}

Coordinate Coordinate::GetCrossProduct(Coordinate c)
{
	Coordinate result;
	result.x = this->y*c.z - this->z*c.y;
	result.y = this->z*c.x - this->x*c.z;
	result.z = this->x*c.y - this->y*c.x;
	result.w = 0;
	return result;
}