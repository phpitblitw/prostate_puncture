//*****************************************************************
// 文件名 :						Attitude.h
// 版本	 :						1.0
// 目的及主要功能 :				姿态数据
// 创建日期 :					2019.11.1
// 修改日期 :					
// 作者 :						Fiki
// 修改者 :						
// 联系方式 :					fiki@seu.edu.cn
//*****************************************************************/

///////////////////////////////////////////////////////
////////////////////////include////////////////////////
///////////////////////////////////////////////////////
#include "stdafx.h"
#include "Attitude.h"


using namespace NDIOPERATOR;

/*****************************************************************
Name:			Attitude(待完成)
Inputs:
	none
Return Value:
	none
Description:	Attitude构造函数
*****************************************************************/
Attitude::Attitude()
{
	m_fX = 0;
	m_fY = 0;
	m_fZ = 0;
	m_fAlpha = EmptyAlpha;
	m_fBeta = 0;
	m_fGama = 0;
}//Attitude


/*****************************************************************
Name:			~Attitude(待完成)
Inputs:
	none
Return Value:
	none
Description:	Attitude析构函数
*****************************************************************/
Attitude::~Attitude()
{
}//~Attitude


/*****************************************************************
Name:			GetPosition
Inputs:
	float &t_fX - 平移坐标x
	float &t_fY - 平移坐标y
	float &t_fZ - 平移坐标z
	float &m_fAlpha - 绕X轴角度旋转
	float &m_fBeta - 绕Y轴角度旋转
	float &m_fGama - 绕Z轴角度旋转
Return Value:
	none
Description:	读取坐标
*****************************************************************/
void Attitude::GetPosition(float &t_fX, float &t_fY, float &t_fZ,
						   float &t_fAlpha, float &t_fBeta, float &t_fGama)
{
	t_fX = float(m_fX);
	t_fY = float(m_fY);
	t_fZ = float(m_fZ);
	t_fAlpha = float(m_fAlpha);
	t_fBeta = float(m_fBeta);
	t_fGama = float(m_fGama);
}//GetPosition


/*****************************************************************
Name:			SetPosition
Inputs:
	float t_fX - 平移坐标x
	float t_fY - 平移坐标y
	float t_fZ - 平移坐标z
	float t_fAlpha - 绕X轴角度旋转
	float t_fBeta - 绕Y轴角度旋转
	float t_fGama - 绕Z轴角度旋转
Return Value:
	none
Description:	读取坐标
*****************************************************************/
void Attitude::SetPosition(float t_fX, float t_fY, float t_fZ,
						   float t_fAlpha, float t_fBeta, float t_fGama)
{
	m_fX = t_fX;
	m_fY = t_fY;
	m_fZ = t_fZ;
	m_fAlpha = t_fAlpha;
	m_fBeta = t_fBeta;
	m_fGama = t_fGama;
}//SetPosition


/*****************************************************************
Name:			Empty
Inputs:
	none
Return Value:
	bool - 是否为空（未经过初始化），true未初始化
Description:	向量相加
*****************************************************************/
bool Attitude::Empty()
{
	if (abs(m_fAlpha - EmptyAlpha) < 0.0001)
	{
		return true;
	}
	else
	{
		return false;
	}
}//Empty


/*****************************************************************
Name:			operator+
Inputs:
	const Attitude& b - 待加的向量
Return Value:
	none
Description:	向量相加
*****************************************************************/
Attitude Attitude::operator+(const Attitude& b)
{
	Attitude result;
	result.m_fX = this->m_fX + b.m_fX;
	result.m_fY = this->m_fY + b.m_fY;
	result.m_fZ = this->m_fZ + b.m_fZ;
	result.m_fAlpha = this->m_fAlpha + b.m_fAlpha;
	result.m_fBeta = this->m_fBeta + b.m_fBeta;
	result.m_fGama = this->m_fGama + b.m_fGama;

	return result;
}//operator+


/*****************************************************************
Name:			operator-
Inputs:
	const Attitude& b - 待减的向量
Return Value:
	none
Description:	向量相加
*****************************************************************/
Attitude Attitude::operator-(const Attitude& b)
{
	Attitude result;
	result.m_fX = this->m_fX - b.m_fX;
	result.m_fY = this->m_fY - b.m_fY;
	result.m_fZ = this->m_fZ - b.m_fZ;
	result.m_fAlpha = this->m_fAlpha - b.m_fAlpha;
	result.m_fBeta = this->m_fBeta - b.m_fBeta;
	result.m_fGama = this->m_fGama - b.m_fGama;

	return result;
}//operator-


/*****************************************************************
Name:			operator-
Inputs:
	const float& b - 待除的标量
Return Value:
	none
Description:	向量除
*****************************************************************/
Attitude Attitude::operator/(const float& b)
{
	Attitude result;
	if (b != 0)
	{
		result.m_fX = this->m_fX / b;
		result.m_fY = this->m_fY / b;
		result.m_fZ = this->m_fZ / b;
		result.m_fAlpha = this->m_fAlpha / b;
		result.m_fBeta = this->m_fBeta / b;
		result.m_fGama = this->m_fGama / b;
		return result;
	}

	return *this;
}//operator/