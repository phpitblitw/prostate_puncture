//*****************************************************************
// 文件名 :						USBCapturer.h
// 版本	 :						1.0
// 目的及主要功能 :				超声图像采集设备控制
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
#include "USBCapturer.h"
#include "USBConfig.h"
#include "ErrorManager//ErrorCodeDefine.h"
#include "ComUtility/SysPathManager.h"
#include <string>
//#include "opencv2/highgui.hpp"
//#include "opencv2/imgproc/imgproc.hpp"

using namespace USBCAPTURER;
using namespace fsutility;

///////////////////////////////USBCapturer成员函数///////////////////////////////////
/*****************************************************************
Name:			USBCapturer
Inputs:
	none
Return Value:
	none
Description:	USBCapturer构造函数
*****************************************************************/
USBCapturer::USBCapturer()
{
	this->m_pDevice = nullptr;
	this->m_bRunning = false;
	this->m_CapturePerFrameFun = nullptr;
	//this->m_bGetParameters = false;

	m_scanType = UNDEFINED;
	m_dPixelSizeT = -1;
	m_dPixelSizeS = -1;
	m_bUpdateFlag = true;
}//USBCapturer


/*****************************************************************
Name:			~USBCapturer
Inputs:
	none
Return Value:
	none
Description:	USBCapturer析构函数
*****************************************************************/
USBCapturer::~USBCapturer()
{
	this->ReleaseUSBDevice();
}//~USBCapturer


/*****************************************************************
Name:			InitUSBDevice
Inputs:
	CString t_strFilePathName - Config文件名及路径
Return Value:
	int - Error Info
Description:	初始化USB设备
*****************************************************************/
int USBCapturer::InitUSBDevice(CString t_strFilePathName)
{
	Mat image;	//用于计算B超参数
	FrmGrabNet_Init(); //初始化FrmGrab库的数据结构。在windows上非必须步骤。dll会自行初始化

	m_pDevice = FrmGrabLocal_Open();//打开默认支持设备,适用于单台设备连接
	if (NULL == m_pDevice)
	{
		FrmGrabNet_Deinit();
		return ER_OpenUSBDeviceFailed;
	}

	//导入参数设置
	if (USBConfig::Instance().LoadConfigFile(t_strFilePathName) != LIST_NO_ERROR)
	{

		return ER_OpenUSBConfigFileFailed;
	}

	//载入字母T,S的截图
	char exePath[MAX_PATH];
	memset(exePath, 0, MAX_PATH);
	GetModuleFileNameA(NULL, exePath, MAX_PATH);  //找到当前exe文件名
	std::string strResourceRootPath = std::string(exePath);
	strResourceRootPath = strResourceRootPath.substr(0, strResourceRootPath.rfind('\\')) + "\\Res\\";  //生成config文件夹路径
	m_imgCharS = cv::imread(strResourceRootPath + "charS.bmp", cv::IMREAD_GRAYSCALE);
	m_imgCharT = cv::imread(strResourceRootPath + "charT.bmp", cv::IMREAD_GRAYSCALE);
	if (!m_imgCharS.data || !m_imgCharT.data)
		return ER_NoLetterTemplate;

	return LIST_NO_ERROR;
}//InitNDIDevice

int USBCapturer::InitUSBDevice(std::string t_strFilePathName)
{
	CString t_cstrFilePathName(t_strFilePathName.data());
	return InitUSBDevice(t_cstrFilePathName);
}

 /*****************************************************************
 Name:			ReleaseUSBDevice
 Inputs:
 none
 Return Value:
 int - Error Info
 Description:	释放USB采集设备
 *****************************************************************/
int USBCapturer::ReleaseUSBDevice()
{
	if (m_pDevice == nullptr)
	{
		FrmGrabNet_Deinit();
		return LIST_NO_ERROR;
	}

	//停止采集
	if (m_bRunning)
	{
		this->StopGrab();
		m_bRunning = false;
	}

	FrmGrab_Close(m_pDevice);//关闭设备
	FrmGrabNet_Deinit();//设备环境释放

	return LIST_NO_ERROR;
}//ReleaseUSBDevice

/*****************************************************************
Name:			BindCapturePerFrameEvent
Inputs:
	Fun_CapturePerFrameEvent eventFun - 读取一帧函数指针
Return Value:
	none
Description:	绑定读取一帧图像回调函数
*****************************************************************/
void USBCapturer::BindCapturePerFrameEvent(Fun_CapturePerFrameEvent eventFun)
{
	m_CapturePerFrameFun = eventFun;
}//BindCapturePerFrameEvent


 /*****************************************************************
 Name:			StartGrab(待完成)
 Inputs:
 none
 Return Value:
 int - Error Info
 Description:	开始采集B超数据
 *****************************************************************/
int USBCapturer::StartGrab()
{
	//检查采集卡设备对象指针
	if (m_pDevice == nullptr)
	{
		return ER_OpenUSBDeviceFailed;
	}


	//检查是否正在采集
	m_CaptureMutex.lock();
	if (m_bRunning)
	{
		m_CaptureMutex.unlock();
		return LIST_NO_ERROR;
	}
	m_bRunning = true;
	m_CaptureMutex.unlock();
	FrmGrab_Start(m_pDevice);//设置为以尽可能大的帧率采集图像
	
	//用线程的形式开始采集数据
	m_tGrabThread = thread(bind(&USBCapturer::Grab, this));
	m_tGrabThread.detach();		//和线程对象分离，这样线程可以独立地执行
	return LIST_NO_ERROR;
}//StartGrab

 /*****************************************************************
 Name:			StopGrab
 Inputs:
 none
 Return Value:
 int - Error Info
 Description:	停止采集B超数据
 *****************************************************************/
int USBCapturer::StopGrab()
{
	if (m_pDevice == nullptr || m_bRunning == false)
	{
		return LIST_NO_ERROR;
	}

	m_CaptureMutex.lock();
	m_bRunning = false;
	m_CaptureMutex.unlock();
	FrmGrab_Stop(m_pDevice);

	return LIST_NO_ERROR;
}//StopGrab

 /*****************************************************************
 Name:			GrabOneFrame
 Inputs:
 none
 Return Value: B超图像
 int - Error Info
 Description:	获取一帧B超图像
 *****************************************************************/
BOOL USBCapturer::GrabOneFrame(Mat &t_Image)
{
#ifdef USE_LOCAL_US_IMAGE
	//使用本地B超图片
	CString t_strUSImagePath;
	t_strUSImagePath = CSysPathManager::Instance().GetConfigPath() + _T("us_example.jpg");
	USES_CONVERSION;
	cv::String imgPath = W2A(t_strUSImagePath);
	t_Image = imread(W2A(t_strUSImagePath),CV_LOAD_IMAGE_COLOR);
	return TRUE;
#endif
	V2U_VideoMode iVideoMode;//视频特征，包括宽、高、帧率
	V2U_BOOL bSuc;//视频特征获取成功标志
	bSuc = FrmGrab_DetectVideoMode(m_pDevice, &iVideoMode);//获取视频特征
	m_CaptureMutex.lock();
	V2U_GrabFrame2* iFrame = FrmGrab_Frame(m_pDevice, V2U_GRABFRAME_FORMAT_YV12, NULL);//获取YV12格式的完整帧
	if (!iFrame || iFrame->imagelen <= 0)
	{
		FrmGrab_Release(m_pDevice, iFrame);
		m_CaptureMutex.unlock();
		return FALSE;
	}
	else
	{
		cv::Mat iSrc(iVideoMode.height + iVideoMode.height / 2, iVideoMode.width, CV_8UC1, (uchar*)iFrame->pixbuf);
		FrmGrab_Release(m_pDevice, iFrame);
		m_CaptureMutex.unlock();

		cv::Mat iDst(iVideoMode.height, iVideoMode.width, CV_8UC3);//这里nHeight为760,nWidth为1024,8UC3表示8bit uchar 无符号类型,3通道值
		cvtColor(iSrc, iDst, cv::COLOR_YUV2BGR_YV12);
		t_Image = iDst.clone();
	}
	return TRUE;
}//GrabOneFrame


 /*****************************************************************
 Name:			Grab
 Inputs:
 none
 Return Value:
 int - Error Info
 Description:	持续采集B超数据
 *****************************************************************/
void USBCapturer::Grab()
{
	if (m_CapturePerFrameFun == NULL)
	{
		return;
	}

	V2U_VideoMode iVideoMode;//视频特征，包括宽、高、帧率
	V2U_BOOL bSuc;//视频特征获取成功标志
	bSuc = FrmGrab_DetectVideoMode(m_pDevice, &iVideoMode);//获取视频特征
	
	while (m_bRunning)
	{
#ifdef USE_LOCAL_US_IMAGE
		//使用本地B超图片
		CString t_strUSImagePath;
		cv::Mat iDst;
		cv::Mat t_ImageA;
		cv::Mat t_ImageB;
		t_strUSImagePath = CSysPathManager::Instance().GetConfigPath() + _T("us_example.jpg");
		USES_CONVERSION;
		cv::String imgPath = W2A(t_strUSImagePath);
		iDst = imread(W2A(t_strUSImagePath), CV_LOAD_IMAGE_COLOR);
		if (m_nScreenType == 1)	//根据布局拷贝对应图像区域图像，采用clone
		{
			t_ImageA = iDst(USBConfig::Instance().m_FullRect).clone();
		}
		else
		{
			t_ImageA = iDst(USBConfig::Instance().m_UpRect).clone();
			t_ImageB = iDst(USBConfig::Instance().m_DownRect).clone();
		}
		//检查回调函数
		if (m_CapturePerFrameFun != nullptr)
		{
			m_CapturePerFrameFun(t_ImageA, t_ImageB, m_dImageRes);
		}
		continue;
#endif
		m_CaptureMutex.lock();
		V2U_GrabFrame2* iFrame = FrmGrab_Frame(m_pDevice, V2U_GRABFRAME_FORMAT_YV12, NULL);//获取YV12格式的完整帧
		if (!iFrame || iFrame->imagelen <= 0)
		{
			FrmGrab_Release(m_pDevice, iFrame);
			m_CaptureMutex.unlock();
		}
		else
		{ 
			cv::Mat iSrc(iVideoMode.height + iVideoMode.height / 2, iVideoMode.width, CV_8UC1, (uchar*)iFrame->pixbuf);
			FrmGrab_Release(m_pDevice, iFrame);
			m_CaptureMutex.unlock();

			cv::Mat iDst(iVideoMode.height, iVideoMode.width, CV_8UC3);//这里nHeight为760,nWidth为1024,8UC3表示8bit uchar 无符号类型,3通道值
			cvtColor(iSrc, iDst, cv::COLOR_YUV2BGR_YV12);

			//更新B超参数
			if (m_bUpdateFlag || CalScanType(iDst) != m_scanType)  //对于每一帧图像 都检测是否切换了扫描平面类型
			{
				m_bUpdateFlag = false;
				CalParameters(iDst);  //如果扫描平面类型改变了，则更新所有b超参数(包括扫描类型和像素物理尺寸)
			}

			m_scanType = DUAL_PLANE;  //TODO
			cv::Mat t_imgT;
			cv::Mat t_imgS;
			if (m_scanType == DUAL_PLANE)
			{
				t_imgT = iDst(USBConfig::Instance().m_DualUpROI).clone();
				t_imgS = iDst(USBConfig::Instance().m_DualDownROI).clone();
			}
			else if (m_scanType == TRANSVERSE_ONLY)
				t_imgT = iDst(USBConfig::Instance().m_OnePlaneROI).clone();
			else if (m_scanType == SAGITTAL_ONLY)
				t_imgS = iDst(USBConfig::Instance().m_OnePlaneROI).clone();
			
			//检查回调函数
			if (m_CapturePerFrameFun != nullptr)
			{
				m_CapturePerFrameFun(t_imgT, t_imgS, m_dPixelSizeT, m_dPixelSizeS);
			}			
		}
	}
}//Grab


 /*****************************************************************
 Name:			CalParameters
 Inputs: t_img cv::Mat(8UC3) - 输入图片
 none
 Return Value:
 int - Error Info
 Description:	对采集图像进行分析，更新参数
 *****************************************************************/
void USBCapturer::CalParameters(Mat t_img)
{
	m_scanType = CalScanType(t_img);

	m_dPixelSizeT = -1;
	m_dPixelSizeS = -1;
	if (m_scanType == DUAL_PLANE)
	{
		m_dPixelSizeT = CalPixelSize(t_img(USBConfig::Instance().m_DualPlaneUpRightAxisRect));
		m_dPixelSizeS = CalPixelSize(t_img(USBConfig::Instance().m_DualPlaneDownRightAxisRect));
	}
	else if (m_scanType == TRANSVERSE_ONLY)
		m_dPixelSizeT = CalPixelSize(t_img(USBConfig::Instance().m_OnePlaneRightAxisRect));
	else if (m_scanType == SAGITTAL_ONLY)
		m_dPixelSizeS = CalPixelSize(t_img(USBConfig::Instance().m_OnePlaneRightAxisRect));
}//GetParameters

 /*****************************************************************
 Name:			CalScreenType
 Inputs:		
 none
 Return Value:	int - 单屏/双屏
 Description:	对采集图像进行分析，得到单/双屏信息
 *****************************************************************/
ScanType USBCapturer::CalScanType(cv::Mat t_img)
{
	int numForeGround;
	cv::Mat imgLeftBar = t_img(USBConfig::Instance().m_DualLeftBarRect).clone();
	cv::Mat imgCharArea = t_img(USBConfig::Instance().m_CharSensorTypeRect).clone();
	cv::Mat imgDiffT, imgDiffS;
	//判断是否为双平面
	cv::cvtColor(imgLeftBar, imgLeftBar, cv::COLOR_BGR2GRAY);
	cv::threshold(imgLeftBar, imgLeftBar, 50, 1, CV_THRESH_BINARY);
	numForeGround = cv::countNonZero(imgLeftBar);
	if (numForeGround > 200)
		return DUAL_PLANE;
	//判断是否为横断面(T)
	cv::cvtColor(imgCharArea, imgCharArea, cv::COLOR_BGR2GRAY);
	cv::absdiff(imgCharArea, m_imgCharT, imgDiffT);
	cv::threshold(imgDiffT, imgDiffT, 50, 1, CV_THRESH_BINARY);
	numForeGround = cv::countNonZero(imgDiffT);
	if (numForeGround < 20)
		return TRANSVERSE_ONLY;
	//判断是否为矢状面(S)
	cv::absdiff(imgCharArea, m_imgCharS, imgDiffS);
	numForeGround = cv::countNonZero(imgDiffS);
	if (numForeGround < 20)
		return SAGITTAL_ONLY;
	//都不是的话 返回"未定义"的结果
	return UNDEFINED;
}

/*****************************************************************
Name:			CalPixelSize
Inputs: t_imgAxis cv::Mat(CV_8UC3) - 坐标轴区域图片
none
Return Value: double - 像素的物理尺寸
int - Error Info
Description:	对采集图像进行分析，得到缩放比例，即1个像素对应的物理尺寸(mm)
*****************************************************************/
double USBCapturer::CalPixelSize(cv::Mat t_imgAxis)
{
	int y, y1 = -1, y2 = -1;
	int distance;  //两个坐标点之间的距离(像素数),即1cm对应多少像素点
	int *hist = new int[t_imgAxis.rows];
	double pixelSize;

	//做前景点数量的水平积分
	cv::cvtColor(t_imgAxis, t_imgAxis, cv::COLOR_BGR2GRAY);
	cv::threshold(t_imgAxis, t_imgAxis, 50, 1, CV_THRESH_BINARY);
	memset(hist, 0, sizeof(int)*t_imgAxis.rows);
	for (y = 0; y < t_imgAxis.rows; y++)
	{
		hist[y] = cv::countNonZero(t_imgAxis(CvRect(0, y, t_imgAxis.cols, 1)));
	}

	//找出两个坐标点之间的距离(像素数)
	//for (y = 0; y < t_imgAxis.rows - 1; y++)
	//{
	//	if (hist[y] == 0 && hist[y + 1] > 0)
	//	{
	//		y1 = y;
	//		break;
	//	}
	//}
	//for (y++; y < t_imgAxis.rows - 1; y++)
	//{
	//	if (hist[y] == 0 && hist[y + 1] > 0)
	//	{
	//		y2 = y;
	//		break;
	//	}
	//}

	for (y = t_imgAxis.rows - 1; y >= 1; y--)
	{
		if (hist[y] == 0 && hist[y - 1] > 0)
		{
			y2 = y;
			break;
		}
	}
	for (y--; y >= 1; y--)
	{
		if (hist[y] == 0 && hist[y - 1] > 0)
		{
			y1 = y;
			break;
		}
	}

	//计算
	if (y1 >= 0 && y2 >= 0)
		distance = y2 - y1;
	pixelSize = 10.0 / distance;

	delete[] hist;
	return pixelSize;

}

