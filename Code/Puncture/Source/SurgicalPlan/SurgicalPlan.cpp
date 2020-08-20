//*****************************************************************
// 文件名 :						SurgicalPlan.h
// 版本	 :						1.0
// 目的及主要功能 :				手术计划类
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
#include "SurgicalPlan.h"
#include "ErrorManager//ErrorCodeDefine.h"
#include "ComUtility//IniFile.h"
//#include "ImageSampler.h"

using namespace SURGICALPLAN;
/*****************************************************************
Name:			SurgicalPlan(待完成)
Inputs:
	none
Return Value:
	none
Description:	SurgicalPlan构造函数
*****************************************************************/
SurgicalPlan::SurgicalPlan()
{
	InitData();
}//SurgicalPlan


/*****************************************************************
Name:			~SurgicalPlan(待完成)
Inputs:
	none
Return Value:
	none
Description:	SurgicalPlan析构函数
*****************************************************************/
SurgicalPlan::~SurgicalPlan()
{
	DestroyData();
}//~SurgicalPlan

 /*****************************************************************
 Name:			InitData
 Inputs:
 none
 Return Value:
 none
 Description:	InitData的初始化函数，在构造函数里面使用
 *****************************************************************/
void SurgicalPlan::InitData()
{
	m_vRectumCenterPath.clear();
	m_iCurCenterPtIndex = 0;

	m_nCX = 0;
	m_nCY = 0;
	m_nCZ = 0;

	m_fResX = 0;
	m_fResY = 0;
	m_fResZ = 0;

	m_pProstateMaskData.reset(new MaskData());
	m_pLesionMaskData.reset(new MaskData());
	m_pRectumMaskData.reset(new MaskData());
	m_pMRIData.reset(new MRIData());
	m_pSurface.reset(new SurfaceData());
	//m_pMaskSampler.reset(new ImageSampler());
}

/*****************************************************************
Name:			DestroyData
Inputs:
none
Return Value:
none
Description:	DestroyData的销毁函数，在析构函数里面使用
*****************************************************************/
void SurgicalPlan::DestroyData()
{
	m_vRectumCenterPath.clear();
}

///*****************************************************************
//Name:			ClearData(待完成)
//Inputs:
//	none
//Return Value:
//	none
//Description:	释放已经开辟的图像空间，并复位参数
//				所有添加变量，必须在此处释放
//*****************************************************************/
//void SurgicalPlan::ClearData(void)
//{
//	//释放空间
//	if (m_pMRIData != nullptr)
//	{
//		delete[]m_pMRIData;
//		m_pMRIData = nullptr;
//	}
//
//	if (m_pMaskData != nullptr)
//	{
//		delete[]m_pMaskData;
//		m_pMaskData = nullptr;
//	}
//
//	//复位参数
//	m_nPixNumberX = 0;
//	m_nPixNumberY = 0;
//	m_nPixNumberZ = 0;
//
//	m_fPixSizeX = 1;
//	m_fPixSizeY = 1;
//	m_fPixSizeZ = 1;
//
//	//删除靶点及直肠中心点序列
//	m_vTargetPointList.clear();
//	m_vAnorectalPath.clear();
//
//}//ClearData
 
/*************************************对外接口函数*****************************************/

/*****************************************************************
Name:			ReadDICOMData(待完成)
Inputs:
	CString t_strDICOMFilePathName - DICOM图像文件名和路径
Return Value:
	int - Error Info
Description:	读取DICOM文件至m_pMRIData，并初始化各参数
				开辟相应数据区，
*****************************************************************/
int SurgicalPlan::ReadDICOMData(CString t_strDICOMFilePathName)
{
	return LIST_NO_ERROR;
}//ReadDICOMData


/*****************************************************************
Name:			CountProstateMask(待完成)
Inputs:
	none
Return Value:
	int - Error Info
Description:	计算前列腺三维掩膜，从m_pMRIData
				中取数据，分割后写回m_pMaskData
				自动分析
*****************************************************************/
int SurgicalPlan::CountProstateMask()
{
	return LIST_NO_ERROR;
}//CountProstateMask


/*****************************************************************
Name:			ExPortAsFileSet
Inputs:
	CString t_strPlanFilePath - 导出文件所在文件夹路径
Return Value:
	int - Error Info
Description:	将手术计划导出为文件
*****************************************************************/
int SurgicalPlan::ExPortAsFileSet(CString t_strPlanFilePath)
{
	return LIST_NO_ERROR;
}//ExPortAsFileSet


/*****************************************************************
Name:			InPortAsFileSet
Inputs:
	CString t_strPlanFilePath - 导入文件所在文件夹路径
Return Value:
	int - Error Info
Description:	导入手术计划文件
*****************************************************************/
int SurgicalPlan::InPortAsFileSet(CString t_strFilePathName)
{
	//判断文件是否存在
	if (!PathFileExists(t_strFilePathName))
	{
		return ER_FileOpenFailed;
	}

	//读入相关参数
	fsutility::CIniFile t_ConfigFile;	//config文件
	if (!t_ConfigFile.Open(t_strFilePathName))
	{
		return ER_OpenSurgicalPlanConfigFileFailed;
	}

	t_ConfigFile.Move2Section(_T("PATH"));
	t_ConfigFile.ReadKey(_T("MRIFileName"), m_strMRIFileName);
	t_ConfigFile.ReadKey(_T("ProstateMaskFileName"), m_strProstateMaskFileName);
	t_ConfigFile.ReadKey(_T("LesionMaskFileName"), m_strLesionMaskFileName);
	t_ConfigFile.ReadKey(_T("RectumMaskFileName"), m_strRectumMaskFileName);
	t_ConfigFile.ReadKey(_T("SurfaceFileName"), m_strSurfaceFileName);

	t_ConfigFile.Move2Section(_T("MASKINFO"));
	t_ConfigFile.ReadKey(_T("CX"), m_nCX);
	t_ConfigFile.ReadKey(_T("CY"), m_nCY);
	t_ConfigFile.ReadKey(_T("CZ"), m_nCZ);

	t_ConfigFile.ReadKey(_T("ResX"), m_fResX);
	t_ConfigFile.ReadKey(_T("ResY"), m_fResY);
	t_ConfigFile.ReadKey(_T("ResZ"), m_fResZ);

	////base位置交由AnalyseProcess分析，SurgicalPlan不再负责
	//t_ConfigFile.Move2Section(_T("BASEPLAN"));
	//float x, y, z, alpha, beta, gama;
	//t_ConfigFile.ReadKey(_T("X"), x);
	//t_ConfigFile.ReadKey(_T("Y"), y);
	//t_ConfigFile.ReadKey(_T("Z"), z);

	//t_ConfigFile.ReadKey(_T("Alpha"), alpha);
	//t_ConfigFile.ReadKey(_T("Beta"), beta);
	//t_ConfigFile.ReadKey(_T("Gama"), gama);

	//m_BasePlanAttitude.SetPosition(x, y, z, alpha, beta, gama);

	m_strRootPath = t_strFilePathName.Left(t_strFilePathName.GetLength() - t_strFilePathName.ReverseFind('\\'));

	//导入MASK数据
	if (m_pProstateMaskData->LoadRawMask(m_strProstateMaskFileName, m_nCX, m_nCY, m_nCZ) != LIST_NO_ERROR)
	{
		return ER_InportMaskDataError;
	}
	m_pProstateMaskData->SetMaskVolumeResolution(m_fResX, m_fResY, m_fResZ);
	if (m_pLesionMaskData->LoadRawMask(m_strLesionMaskFileName, m_nCX, m_nCY, m_nCZ) != LIST_NO_ERROR)
	{
		return ER_InportMaskDataError;
	}
	m_pLesionMaskData->SetMaskVolumeResolution(m_fResX, m_fResY, m_fResZ);
	if (m_pRectumMaskData->LoadRawMask(m_strRectumMaskFileName, m_nCX, m_nCY, m_nCZ) != LIST_NO_ERROR)
	{
		return ER_InportMaskDataError;
	}
	//导入MRI数据
	if (m_pMRIData->LoadRawMRI(m_strMRIFileName, m_nCX, m_nCY, m_nCZ) != LIST_NO_ERROR)
	{
		return ER_InportMaskDataError;
	}
	m_pMRIData->SetMRIVolumeResolution(m_fResX, m_fResY, m_fResZ);

	return LIST_NO_ERROR;
}//InPortAsFileSet


/*****************************************************************
Name:			GetOutline
Inputs:
	Mat &t_OutlineImg - 返回的图像
	Attitude t_Attitude - 姿态平面
Return Value:
	int - Error Info
Description:	根据平面截取轮廓图像
*****************************************************************/
int SurgicalPlan::GetOutline(cv::Mat &t_OutlineImg, Attitude t_Attitude)
{
	return LIST_NO_ERROR;
}//GetOutline

/*****************************************************************
Name:			GetMaskDataPtr
Inputs:
	int nMaskType - mask类型 0前列腺 1病灶
Return Value:
	MaskDataPtr - mask指针
Description:	返回mask指针
*****************************************************************/
MaskDataPtr SurgicalPlan::GetMaskDataPtr(int nMaskType)
{
	if (nMaskType == 1)
		return m_pProstateMaskData;
	else if (nMaskType == 2)
		return m_pLesionMaskData;
	else
		return m_pRectumMaskData;
}//GetMaskDataPtr


/*****************************************************************
Name:			GetParameters
Inputs:
	float &t_fX - 平移坐标x
	float &t_fY - 平移坐标y
	float &t_fZ - 平移坐标z
	float &t_MX - X轴MoveDir
	float &t_MY - Y轴MoveDir
	float &t_MZ - Z轴MoveDir
	float &t_UX - X轴UpDir
	float &t_UY - Y轴UpDir
	float &t_UZ - Z轴UpDir
Return Value:
	none
Description:	得到一组平面参数，便于之后显示平面
*****************************************************************/
//平面参数交付AnalyseProcess负责
/*
void SurgicalPlan::GetParameters(double &t_centerX, double &t_centerY, double &t_centerZ,
	double &t_MX, double &t_MY, double &t_MZ,
	double &t_UX, double &t_UY, double &t_UZ)
{
	float t_x;
	float t_y;
	float t_z;
	float t_fAlpha;
	float t_fBeta;
	float t_fGama;

	m_BasePlanAttitude.GetPosition(t_x, t_y, t_z, t_fAlpha, t_fBeta, t_fGama);

	t_centerX = t_x;
	t_centerY = t_y;
	t_centerZ = t_z;
	t_MX = 0;
	t_MY = 0;
	t_MZ = 1;
	t_UX=0;
	t_UY=-1;
	t_UZ = 0;

}
*/

/*****************************************************************
Name:			SetImageSize
Inputs:
	int nImageX - 宽度
	int nImageY - 高度
Return Value:
	int - Error Info
Description:	设置2d显示的超声尺寸(像素数)
*****************************************************************/
void SurgicalPlan::SetImageSize(int nImageX, int nImageY)
{
	this->m_nShowImageX = nImageX;
	this->m_nShowImageY = nImageY;
}


/*****************************************************************
Name:			LoadRectumCenter
Inputs:
	CString strFileName - 存储中心点的txt文件路径
Return Value:
	int - Error Info
Description:	添加直肠中心线坐标序列
*****************************************************************/
/*
int SurgicalPlan::LoadRectumCenter(CString strFileName)
{
	std::ifstream inFile;
	std::string strLine;
	int nBegin, nEnd, i;
	double dPosition[3];
	inFile.open(strFileName.GetBuffer());
	TRACE("\nRectum txt path: %s\n", strFileName.GetBuffer());
	if (!inFile.bad())
	{
		TRACE("\nsuccessfully openend\n");
		while (std::getline(inFile, strLine))
		{
			nBegin = 0;
			nEnd = 0;
			for (i = 0; i < 3; i++)
			{
				nBegin = nEnd + 1;
				while (true)
				{
					nEnd++;
					if (strLine[nEnd] == ')' || strLine[nEnd] == ' ')
						break;
				}
				dPosition[i] = std::stod(strLine.substr(nBegin, nEnd));
			}
			AddRectumCenterPt(dPosition[0] * m_fResX, dPosition[1] * m_fResY, dPosition[2] * m_fResZ);

		}
		inFile.close();
		m_iCurCenterPtIndex = 0;
		return LIST_NO_ERROR;
	}
	else
		return ER_ImportRectumCenterError;
}
*/


/*****************************************************************
Name:			FindLargestSection
Inputs:
Return Value:
	int - Error Info
Description:	找到超声探头截面最大处 对应的姿态
*****************************************************************/
/*
int SurgicalPlan::FindLargestSection()
{

	CFileDialog DatafileDlg1(TRUE,
		_T("*.txt"), _T("*.txt"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
	DatafileDlg1.m_ofn.lpstrTitle = _T("Please choose rectum center txt file");
	if (DatafileDlg1.DoModal() == IDOK)
		m_strRectumCenterName = DatafileDlg1.GetPathName();
	CFileDialog DatafileDlg2(TRUE,
		_T("*.raw"), _T("*.raw"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
	DatafileDlg2.m_ofn.lpstrTitle = _T("Please choose prostate mask file(raw)");
	if (DatafileDlg2.DoModal() == IDOK)
		m_strMaskFileName = DatafileDlg2.GetPathName();
	m_nCX = 320, m_nCY = 320, m_nCZ = 30;
	m_fResX = 0.5625, m_fResY = 0.5625, m_fResZ = 3.85;

	this->LoadRectumCenter(m_strRectumCenterName);
	this->GetMaskDataPtr()->LoadRawMask(m_strMaskFileName, m_nCX, m_nCY, m_nCZ);
	this->GetMaskDataPtr()->SetMaskVolumeResolution(m_fResX, m_fResY, m_fResZ);
	SurgicalPlanPtr myself;
	myself.reset(this);
	m_pMaskSampler->SetDataPackage(myself);

	int iCurArea, iMaxArea = 0;//前列腺切面最大面积
	int idx = 0; //当前点的直肠中心线下标
	Point3D MoveDir, UpDir, ScanCenter;
	UpDir.x = 0;
	UpDir.y = -1;
	UpDir.z = 0;
	BYTE *pMaskData = new BYTE[m_nShowImageX*m_nShowImageY];

	do
	{
		ScanCenter = this->GetCurRectumPt();
		MoveDir = this->GetCurRectumDir();
		m_pMaskSampler->SetMoveDir(MoveDir.x, MoveDir.y, MoveDir.z);
		m_pMaskSampler->SetPlaneDir(UpDir.x, UpDir.y, UpDir.z);
		m_pMaskSampler->SetCenterPt(ScanCenter.x, ScanCenter.y, ScanCenter.z);
		m_pMaskSampler->GetSampleMaskPlan(pMaskData, m_nShowImageX, m_nShowImageY, true);//获取超声探头冠状面图像
		iCurArea = CPubFunc::ByteSum(pMaskData, m_nShowImageX*m_nShowImageY);
		if (iCurArea > iMaxArea)
		{
			iMaxArea = iCurArea;
			//m_pDataPackage //TODO
			m_iBaseCenterPtIndex = idx;
		}
		idx++;
	} while (this->GoAhead()== LIST_NO_ERROR);
	delete[]pMaskData;
	//int temp;
	//this->getBestCurIdx(temp);
	return LIST_NO_ERROR;
}
*/


/*************************************内部函数*****************************************/
/*****************************************************************
Name:			GetCurRectumPt
Inputs:
Return Value:
	Point3D - 当前中心点坐标
Description:	得到当前的直肠中心点
*****************************************************************/
Point3D SurgicalPlan::GetCurRectumPt()
{
	Point3D pt;
	pt.x = pt.y = pt.z = 0;
	if (!m_vRectumCenterPath.empty() && m_iCurCenterPtIndex < m_vRectumCenterPath.size())
		pt = m_vRectumCenterPath[m_iCurCenterPtIndex];
	return pt;
}

/*****************************************************************
Name:			GetCurRectumDir
Inputs:
Return Value:
	Point3D - 当前中心点指向的方向
Description:	得到当前中心点对应方向
*****************************************************************/
Point3D SurgicalPlan::GetCurRectumDir()
{
	ASSERT(m_iCurCenterPtIndex <= m_vRectumCenterPath.size() - 2);
	Point3D dir, curPt, nextPt;
	curPt = m_vRectumCenterPath[m_iCurCenterPtIndex];
	nextPt = m_vRectumCenterPath[m_iCurCenterPtIndex + 1];
	dir.x = nextPt.x - curPt.x;
	dir.y = nextPt.y - curPt.y;
	dir.z = nextPt.z - curPt.z;
	return dir;
}

/*****************************************************************
Name:			GoAhead
Inputs:
Return Value:
	int - Error Info
Description:	当前直肠点 下标+1
*****************************************************************/
int SurgicalPlan::GoAhead()
{
	if (m_iCurCenterPtIndex < m_vRectumCenterPath.size() - 2)
	{
		m_iCurCenterPtIndex++;
		return LIST_NO_ERROR;
	}
	return ER_ReachedRectumEnd;
}

/*****************************************************************
Name:			AddRectumCenterPt
Inputs:
	double x - 点坐标x
	double y - 点坐标y
	double z - 点坐标z
Return Value:
Description:	添加一个直肠中心点坐标，序列数据通过该函数一个个添加
*****************************************************************/
void SurgicalPlan::AddRectumCenterPt(double x, double y, double z)
{
	Point3D pt;
	pt.x = x;
	pt.y = y;
	pt.z = z;
	m_vRectumCenterPath.push_back(pt);
	return;
}