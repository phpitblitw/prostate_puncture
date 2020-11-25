//*****************************************************************
// 文件名 :						NDIConfig.h
// 版本	 :						1.0
// 目的及主要功能 :				NID设备设置
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
#include <string>
#include "ComUtility/singleton.h"
#include "ComUtility/Coordinate.h"
#include "ComUtility/Matrix.h"


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
*****************************************************************/
namespace NDIOPERATOR
{
	//定义基本采集图像结构类
	class NDIOPERATOR_API NDIConfig : public fsutility::Singleton<NDIConfig>
	{
		friend class fsutility::Singleton<NDIConfig>;

	private:
		NDIConfig();
		virtual ~NDIConfig();

	public:
		int LoadConfigFile(CString t_strFilePathName);

	public:
		double						m_CalibrationMatrixElem[16];	//NDI sensor-超声探头 标定矩阵元素
		std::string					m_strHostname;					//电脑与NDI设备连接的端口号
		int							m_nSensorNumber;				//当插了多个NDI sensor时，使用的NDI sensor序号
		fsutility::Coordinate		m_MoveDir;
		fsutility::Coordinate		m_UpDir;
		fsutility::Matrix			m_CalibrationMatrix;			//NDI sensor-超声探头 标定矩阵
		double						m_dMoveDirOffset;				//矢状面中心点，相对于横断面中心点，在探头朝向moveDir上的偏移量(mm)

	};
}