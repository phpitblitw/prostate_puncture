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
#pragma once

/*****************************************************************
Project Files Included
*****************************************************************/
#include <memory>


/*****************************************************************
Defines
*****************************************************************/
#ifdef NDIOPERATOR_EXPORT
#define NDIOPERATOR_API __declspec(dllexport)
#else
#define NDIOPERATOR_API __declspec(dllimport)
#endif

#define EmptyAlpha 99999

/*****************************************************************
						Routine Definitions
*****************************************************************/
namespace NDIOPERATOR
{
	//定义基本采集图像结构类
	class NDIOPERATOR_API Attitude
	{
	public:
		Attitude();
		virtual ~Attitude();

	public:
		void GetPosition(float &t_fX, float &t_fY, float &t_fZ,
						 float &t_fAlpha, float &t_fBeta, float &t_fGama);	//读取坐标

		void SetPosition(float t_fX, float t_fY, float t_fZ,
						 float t_fAlpha, float t_fBeta, float t_fGama);	//设置坐标
		
		//重载运算符
		Attitude operator+(const Attitude& b);			
		Attitude operator-(const Attitude& b);
		Attitude operator/(const float& b);

		bool Empty();		//判断是否为空（没有初始化）

	private:
		float m_fX;			//平移
		float m_fY;
		float m_fZ;

		float m_fAlpha;		//绕X轴角度旋转
		float m_fBeta;		//绕Y轴角度旋转
		float m_fGama;		//绕Z轴角度旋转
	};
}