//*****************************************************************
// 文件名 :						NDIOperator.h
// 版本	 :						1.0
// 目的及主要功能 :				NID设备控制
// 创建日期 :					2019.11.1
// 修改日期 :					2020.9.25
// 作者 :						Fiki
// 修改者 :						孙易辙
// 联系方式 :					fiki@seu.edu.cn
//*****************************************************************/
#pragma once


/*****************************************************************
Project Files Included
*****************************************************************/
#include <memory>
#include <functional>
#include <thread>
#include <atomic>

#include "Attitude.h"
#include "afxmt.h"
#include <vector>
#include "CAPIcommon/CombinedApi.h"
#include "CAPIcommon/PortHandleInfo.h"
#include "CAPIcommon/ToolData.h"

using namespace std;
/*****************************************************************
Defines
*****************************************************************/
#ifdef NDIOPERATOR_EXPORT
#define NDIOPERATOR_API __declspec(dllexport)
#else
#define NDIOPERATOR_API __declspec(dllimport)
#endif


/*****************************************************************
						Routine Definitions

NDI数据读取模块
	a)	NDI设备信息读取，配置文件；
	b)	采用回调函数的形式，实时读取B超探头上的数据；
	c)	应支持多个传感器，各传感器同步刷新；
	d)	可采用定时器/多媒体定时器，定时刷新所有传感器信息；
	e)	数据通过回调函数主动写给控制模块，而非被动调用数据；
	f)	定义姿态数据类型：Attitude类，该类包含6个float型参数；
	g)	模块输出为Attitude类对象。
	h)	NDI坐标系与B超探头坐标系间标定功能；
	i)	输出结果为标定文件，其内容为转换矩阵，结果以rom文件保存
	j)	多传感器数据融合（B超探头上建议安装多个传感器）
*****************************************************************/
namespace NDIOPERATOR
{
	//定义智能指针类
	class NDIOperator;
	typedef std::shared_ptr<NDIOperator> NDIOperatorPtr;
	//UINT TrackingThreadFun(LPVOID lpParam);		//线程函数（不是类的成员函数，供CreateThread调用)

	//定义基本采集图像结构类
	class NDIOPERATOR_API NDIOperator
	{
	public:
		NDIOperator();
		virtual ~NDIOperator();

	public:
		int InitNDIDevice(CString t_strFilePathName);		//初始化NDI设备

		//传递数据至外部回调函数
		typedef std::function < void(std::vector<Attitude>) > Fun_UpdateAttitudeEvent;//因为可能有多个感应线，所以使用vector进行返回
		void BindUpdateAttitudeEvent(Fun_UpdateAttitudeEvent eventFun);  //绑定刷新坐标回调函数

		int Calibrate(void);				//NDI坐标系与B超探头坐标系间标定功能

		int StartTracking();				//开始采集
		void Tracking();					//采集函数
		int StopTracking();					//停止采集
		
		Attitude QuaternionToAttitude(double, double, double, double, double, double, double);	//从四元数得到Attitude类
		void ConstructMatRtoTusingQuaternion(double, double, double, double, double, double, double, double pMatRtoT[16]);		//从四元数得到转换矩阵
		static void ConstructMatRtoTusingAttitude(Attitude attitude, double pMatRtoT[16]);		//从Attitude类得到转换矩阵
	
	private:
		Fun_UpdateAttitudeEvent m_UpdateAttitudeFun;		//更新预览图

		CombinedApi m_capi;			//自带的操作类
		double m_pMatRtoT[16];		//存放4*4转换矩阵（Receiver to Transmitter）

		atomic<bool> m_bTracking;		//是否在工作状态
		std::thread m_tNDIThread;		//NDI采集线程
		CCriticalSection m_critical_section;
	};
}