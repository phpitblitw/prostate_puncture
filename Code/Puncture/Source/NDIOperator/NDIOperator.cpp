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

///////////////////////////////////////////////////////
////////////////////////include////////////////////////
///////////////////////////////////////////////////////
#include "stdafx.h"
#include "NDIOperator.h"
#include "NDIConfig.h"
#include "ErrorManager//ErrorCodeDefine.h"
#include "ComUtility/Coordinate.h"
#define M_PI 3.141592653589793

using namespace NDIOPERATOR;
using namespace fsutility;
using namespace std;

/*****************************************************************
Name:			NDIOperator(待完成)
Inputs:
	none
Return Value:
	none
Description:	NDIOperator构造函数
*****************************************************************/
NDIOperator::NDIOperator()
{
	//m_capi = CombinedApi();//不知道这个地方需要不需要
	m_UpdateAttitudeFun = nullptr;

	m_bTracking = false;
}//NDIOperator


/*****************************************************************
Name:			~NDIOperator(待完成)
Inputs:
	none
Return Value:
	none
Description:	NDIOperator析构函数
*****************************************************************/
NDIOperator::~NDIOperator()
{
	if (m_bTracking)
	{
		StopTracking();
		m_bTracking = false;
	}
}//~NDIOperator


/*****************************************************************
Name:			InitNDIDevice(待完成)
Inputs:
	CString t_strFilePathName - Config文件名及路径
Return Value:
	int - Error Info
Description:	初始化NDI设备
*****************************************************************/
int NDIOperator::InitNDIDevice(CString t_strFilePathName)
{	
	int i;
	//导入参数设置
	if (NDIConfig::Instance().LoadConfigFile(t_strFilePathName) != LIST_NO_ERROR)
	{
		return ER_OpenNDIConfigFileFailed;
	}

	//连接设备
	//bool t_bConnected = false;		//标志位，是否正确链接
	//for (i = 1; i < 10; ++i)
	//{
	//	char t_cHostName[5];
	//	//sprintf_s(t_cHostName,"COM%d", i);//暂时先设置COM3口，之后会再另行解决
	//	sprintf_s(t_cHostName,"COM3", i);//暂时先设置COM3口，之后会再另行解决
	//	string hostname(t_cHostName);
	//	if (m_capi.connect(hostname) == 0)
	//	{
	//		t_bConnected = true;
	//		break;
	//	}
	//}
	//
	//if (!t_bConnected)
	//{
	//	return ER_InitNDIDeviceFailed;
	//}

	//连接设备
	if (m_capi.connect(NDIConfig::Instance().m_strHostname) != 0)
	{
		return ER_InitNDIDeviceFailed;
	}

	//初始化
	if (m_capi.initialize() != 0)
	{
		return ER_InitNDIDeviceFailed;
	}

	//初始化Tools
	std::vector<PortHandleInfo> portHandles = m_capi.portHandleSearchRequest(PortHandleSearchRequestOption::NotInit);
	for (i = 0; i < (int)portHandles.size(); ++i)
	{
		m_capi.portHandleInitialize(portHandles[i].getPortHandle());
		m_capi.portHandleEnable(portHandles[i].getPortHandle());
	}

	//初始化标定矩阵
	m_CalibrationMatrix = NDIConfig::Instance().m_CalibrationMatrix;

	//初始化姿态参数
	Coordinate RightDir;
	RightDir = NDIConfig::Instance().m_MoveDir.GetCrossProduct(NDIConfig::Instance().m_UpDir);
	RightDir.Normalize();
	m_InitialAttitude.SetValue(Coordinate(0, 0, 0, 1), RightDir, NDIConfig::Instance().m_UpDir, NDIConfig::Instance().m_MoveDir);

	return LIST_NO_ERROR;
}//InitNDIDevice


/*****************************************************************
Name:			BindUpdateAttitudeEvent
Inputs:
	Fun_UpdateAttitudeEvent eventFun - 更新坐标函数指针
Return Value:
	none
Description:	绑定刷新坐标回调函数
*****************************************************************/
void NDIOperator::BindUpdateAttitudeEvent(Fun_UpdateAttitudeEvent eventFun)
{
	m_UpdateAttitudeFun = eventFun;
}//BindUpdateAttitudeEvent


 /*****************************************************************
 Name:			StartTracking
 Inputs:
	none
 Return Value:
	int - Error Info
 Description:	开始采集
 *****************************************************************/
int NDIOperator::StartTracking()
{
	// TODO: 在此处添加实现代码.
	//检查是否正在采集
	if (m_bTracking)
	{
		return ER_StartTrackingNDIFailed;
	}

	//开始采集
	if (m_capi.startTracking() != 0)
	{
		return ER_StartTrackingNDIFailed;
	}
		
	m_bTracking = true;

	//开始线程
	//m_hNDIThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TrackingThreadFun, this, 0, &m_nNDIThreadID);
	m_tNDIThread = thread(bind(&NDIOperator::Tracking, this));
	m_tNDIThread.detach();
	
	return LIST_NO_ERROR;
}//StartTracking


/*****************************************************************
Name:			StopTracking
Inputs:
none
Return Value:
int - Error Info
Description:	停止采集
*****************************************************************/
int NDIOperator::StopTracking()
{
	// TODO: 在此处添加实现代码.
	m_bTracking = false;

	if (m_capi.stopTracking() != 0)
	{
		return ER_StopTrackingNDIFailed;
	}

	return LIST_NO_ERROR;
}//StopTracking


/*****************************************************************
Name:			Tracking
Inputs:
	none
Return Value:
	none
Description:	采集函数
*****************************************************************/
void NDIOperator::Tracking()
{
	// TODO: 在此处添加实现代码.
	if (m_UpdateAttitudeFun == NULL)
	{
		return;
	}
	while (m_bTracking)
	{
		m_critical_section.Lock();
		if (!m_bTracking)
		{
			m_critical_section.Unlock();
			break;
		}
		//得到NDI数据
		std::vector<ToolData> toolData = m_capi.getTrackingDataBX();
		std::vector<Attitude> attitude(toolData.size());	//大小为在工作的Tools的数量，一个Tool对应一个Attitude

															//构造姿态数据
		for (int i = 0; i < toolData.size(); i++)
		{
			if (i != NDIConfig::Instance().m_nSensorNumber)	//筛选目前使用的sensor
			{
				continue;
			}
			if (toolData[i].transform.isMissing())	//如果超出范围，则姿态数据全为0
			{
				attitude[i] = Attitude();
			}
			else
			{
				m_NDIMatrix.ConstructQuaternionTransform(toolData[i].transform.q0, toolData[i].transform.qx, toolData[i].transform.qy,
					toolData[i].transform.qz, toolData[i].transform.tx, toolData[i].transform.ty, toolData[i].transform.tz);	//用四元数构造变换矩阵
				m_CurAttitude = m_InitialAttitude.Transform(m_CalibrationMatrix);
				m_CurAttitude = m_CurAttitude.Transform(m_NDIMatrix);


				//用四元数构造Attitude
				//attitude[i] = QuaternionToAttitude(toolData[i].transform.q0, toolData[i].transform.qx, toolData[i].transform.qy,
				//	toolData[i].transform.qz, toolData[i].transform.tx, toolData[i].transform.ty, toolData[i].transform.tz);
			}
		}
		//hsh:这一句是一个例子，表示可以在Tracking函数中用四元数得到转换矩阵；
		//当然也可以在外面的回调函数里用得到的Attitude数据调用ConstructMatRtoTusingAttitude()这个函数来解决
		/*ConstructMatRtoTusingQuaternion(toolData[0].transform.q0, toolData[0].transform.qx, toolData[0].transform.qy,
		toolData[0].transform.qz, toolData[0].transform.tx, toolData[0].transform.ty, toolData[0].transform.tz);*/

		//传送姿态数据
		if (m_UpdateAttitudeFun != NULL)
		{
			//m_UpdateAttitudeFun(attitude);
			m_UpdateAttitudeFun(m_CurAttitude);
		}

		m_critical_section.Unlock();
	}

}//采集函数

/*****************************************************************
Name:			Calibrate
Inputs:
	none
Return Value:
	int - Error Info
Description:	NDI坐标系与B超探头坐标系间标定功能
*****************************************************************/
int NDIOperator::Calibrate(void)
{
	return LIST_NO_ERROR;
}//Calibrate


 /*****************************************************************
 Name:			ConstructMatRtoTusingQuaternion
 Inputs:
 Attitude attitude - 姿态数据
 double pMatRtoT[16] - 得到的转换矩阵存放在这里
 Return Value:
 none
 Description:	用Attitude形式的位置数据得到转换矩阵
 *****************************************************************/
void NDIOperator::ConstructMatRtoTusingAttitude(NDIOPERATOR::Attitude attitude, double pMatRtoT[16])
{
	// TODO: 在此处添加实现代码.
	float x, y, z, roll, pitch, yaw;
	attitude.GetPosition(x, y, z, roll, pitch, yaw);

	pMatRtoT[0] = cos(yaw) * cos(pitch);
	pMatRtoT[1] = cos(yaw) * sin(pitch) * sin(roll) - sin(yaw) * cos(roll);
	pMatRtoT[2] = cos(yaw) * sin(pitch) * cos(roll) + sin(yaw) * sin(roll);
	pMatRtoT[3] = x;

	pMatRtoT[4] = sin(yaw) * cos(pitch);
	pMatRtoT[5] = sin(yaw) * sin(pitch)*sin(roll) + cos(yaw) * cos(roll);
	pMatRtoT[6] = sin(yaw) * sin(pitch)*cos(roll) - cos(yaw) * sin(roll);
	pMatRtoT[7] = y;

	pMatRtoT[8] = -sin(pitch);
	pMatRtoT[9] = cos(pitch) * sin(roll);
	pMatRtoT[10] = cos(pitch) * cos(roll);
	pMatRtoT[11] = z;

	pMatRtoT[12] = 0;
	pMatRtoT[13] = 0;
	pMatRtoT[14] = 0;
	pMatRtoT[15] = 1;
}//ConstructMatRtoTusingAttitude


/*****************************************************************
Name:			ConstructMatRtoTusingQuaternion
Inputs:
double q0,double qx,double qy,double qz：四元数
double tx,double ty,double tz：位置信息
double pMatRtoT[16]：得到的转换矩阵存放在这里
Return Value:
none
Description:	用四元数形式的位置数据得到转换矩阵
*****************************************************************/
void NDIOperator::ConstructMatRtoTusingQuaternion(double q0, double qx, double qy, double qz, double tx, double ty, double tz, double pMatRtoT[16])
{
	pMatRtoT[0] = 1 - 2 * (qy*qy + qz * qz);
	pMatRtoT[1] = 2 * (qx*qy - q0 * qz);
	pMatRtoT[2] = 2 * (q0*qy + qx * qz);
	pMatRtoT[3] = tx;

	pMatRtoT[4] = 2 * (qx*qy + q0 * qz);
	pMatRtoT[5] = 1 - 2 * (qx*qx + qz * qz);
	pMatRtoT[6] = 2 * (qy*qz - q0 * qx);
	pMatRtoT[7] = ty;

	pMatRtoT[8] = 2 * (qx*qz - q0 * qy);
	pMatRtoT[9] = 2 * (q0*qx + qy * qz);
	pMatRtoT[10] = 1 - 2 * (qx*qx + qy * qy);
	pMatRtoT[11] = tz;

	pMatRtoT[12] = 0;
	pMatRtoT[13] = 0;
	pMatRtoT[14] = 0;
	pMatRtoT[15] = 0;
}//ConstructMatRtoTusingQuaternion


/*****************************************************************
Name:			QuaternionToAttitude
Inputs:
double q0,double qx,double qy,double qz：四元数
double tx,double ty,double tz：位置信息
Return Value:
Attitude - 姿态数据类
Description:	将四元数形式的位置数据转换为Attitude（欧拉角）形式
*****************************************************************/
NDIOPERATOR::Attitude NDIOperator::QuaternionToAttitude(double q0, double qx, double qy, double qz, double tx, double ty, double tz)
{
	// TODO: 在此处添加实现代码.
	Attitude attitude;

	// roll (x-axis rotation)
	double sinr_cosp = 2 * (q0 * qx + qy * qz);
	double cosr_cosp = 1 - 2 * (qx * qx + qy * qy);
	double roll = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	double sinp = 2 * (q0 * qy - qz * qx);
	double pitch;
	if (std::abs(sinp) >= 1)
		pitch = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
	else
		pitch = std::asin(sinp);

	// yaw (z-axis rotation)
	double siny_cosp = 2 * (q0 * qz + qx * qy);
	double cosy_cosp = 1 - 2 * (qy * qy + qz * qz);
	double yaw = std::atan2(siny_cosp, cosy_cosp);
	attitude.SetPosition((float)tx, (float)ty, (float)tz, (float)roll, (float)pitch, (float)yaw);
	return attitude;
}//QuaternionToAttitude
