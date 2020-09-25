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
	this->m_bGetParameters = false;

	m_dImageRes = 1;
	m_nScreenType = 1;
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
	
	//从图像计算B超参数
	GrabOneFrame(image);
	CalParameters(image);
	return LIST_NO_ERROR;
}//InitNDIDevice

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

			cv::Mat t_ImageA;
			cv::Mat t_ImageB;
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
		}
	}
}//Grab


 /*****************************************************************
 Name:			CalParameters
 Inputs:
 none
 Return Value:
 int - Error Info
 Description:	对采集图像进行分析，得到参数，
				参数A：缩放比例
				参数B：单/双屏
 *****************************************************************/
void USBCapturer::CalParameters(Mat t_Image)
{
	this->m_dImageRes = 0;
	this->m_nScreenType = 0;	//0-初始 1-单平面 2-双平面
	//转为灰度图
	Mat gryImage;
	cv::cvtColor(t_Image, gryImage, cv::COLOR_BGR2GRAY);
	//计算参数
	this->m_dImageRes = this->CalResolution(gryImage);
	this->m_nScreenType = this->CalScreenType(gryImage);
}//GetParameters

 /*****************************************************************
 Name:			CalScale
 Inputs:
 none
 Return Value:
 int - Error Info
 Description:	对采集图像进行分析，得到缩放比例，即1个像素对应的物理尺寸(mm)
 *****************************************************************/
double USBCapturer::CalResolution(Mat t_Image)
{
	int x, y, y1, y2;
	int dst;//为两个坐标点的距离（像素）,即1cm对应多少像素点
	double resolution;
	CvRect m_RightAxisRect=USBConfig::Instance().m_RightAxisRect;
	int *hist = new int[m_RightAxisRect.height];
	memset(hist, 0, sizeof(int)*(m_RightAxisRect.height));

	//对于坐标轴区域，对前景点的数量作水平方向的积分
	for (y = m_RightAxisRect.y; y < m_RightAxisRect.y + m_RightAxisRect.height; y++)
	{
		for (x = m_RightAxisRect.x; x < m_RightAxisRect.x + m_RightAxisRect.width; x++)
		{
			if (t_Image.at<uchar>(y, x) > 50)	//对于unsigned char 类型的像素点，判断是否为前景点
			{
				hist[y - m_RightAxisRect.y]++;
			}
		}
	}

	//找出两个坐标点之间的距离（像素数）
	for (y = m_RightAxisRect.y; y < m_RightAxisRect.y + m_RightAxisRect.height; y++)
	{
		if (hist[y - m_RightAxisRect.y] == 0 && hist[y - m_RightAxisRect.y + 1] > 0)
		{
			y1 = y;
			break;
		}
	}
	for (y++; y < m_RightAxisRect.y + m_RightAxisRect.height; y++)
	{
		if (hist[y - m_RightAxisRect.y] == 0 && hist[y - m_RightAxisRect.y + 1] > 0)
		{
			y2 = y;
			break;
		}
	}
	dst = y2 - y1;
	resolution = 10.0 / dst;
	delete[] hist;
	return resolution;
}//GetScale

 /*****************************************************************
 Name:			CalScreenType
 Inputs:		
 none
 Return Value:	int - 单屏/双屏
 Description:	对采集图像进行分析，得到单/双屏信息
 *****************************************************************/
int USBCapturer::CalScreenType(Mat t_Image)
{
	int x, y;
	int cntSumUp = 0, cntSumDown = 0;
	double division; //上下区域 前景点数量之比
	CvRect m_UpAxisRect=USBConfig::Instance().m_UpAxisRect;
	CvRect m_DownAxisRect=USBConfig::Instance().m_DownAxisRect;
	/*m_UpAxisRect.x = 600;
	m_UpAxisRect.width = 100;
	m_UpAxisRect.y = 240;
	m_UpAxisRect.height = 260;
	m_DownAxisRect.x = 600;
	m_DownAxisRect.width = 100;
	m_DownAxisRect.y = 760;
	m_DownAxisRect.height = 260;*/
	//坐标轴上半区域的前景点数量累加
	for (y = m_UpAxisRect.y; y < m_UpAxisRect.y + m_UpAxisRect.height; y++)
	{
		for (x = m_UpAxisRect.x; x < m_UpAxisRect.x + m_UpAxisRect.width; x++)
		{
			if (t_Image.at<uchar>(y, x) > 50)	//对于unsigned char 类型的像素点，判断是否为前景点
			{
				cntSumUp++;
			}
		}
	}
	//坐标轴下半区域的前景点数量累加
	for (y = m_DownAxisRect.y; y < m_DownAxisRect.y + m_DownAxisRect.height; y++)
	{
		for (x = m_DownAxisRect.x; x < m_DownAxisRect.x + m_DownAxisRect.width; x++)
		{
			if (t_Image.at<uchar>(y, x) > 50)	//对于unsigned char 类型的像素点，判断是否为前景点
			{
				cntSumDown++;
			}
		}
	}
	
	division = double(cntSumUp) / cntSumDown;
	if (division < 0.5)
	{
		return 1;//单平面
	}
	else
	{
		return 2;//双平面
	}
}

double USBCapturer::GetResolution()
{
	return this->m_dImageRes;
}

/*****************************************************************
Name:			GetImageSize
Inputs:
	cx int - 冠状面宽度（像素数）
	cx int - 冠状面高度（像素数）
Return Value:	
	nono
Description:	返回图片尺寸(由单/双平面确定)
*****************************************************************/
void USBCapturer::GetImageSize(int &cx, int &cy)
{
	if (m_nScreenType == 1)//单平面
	{
		cx = USBConfig::Instance().m_FullRect.width;
		cy = USBConfig::Instance().m_FullRect.height;
	}
	else if (m_nScreenType == 2)//双平面
	{
		cx = USBConfig::Instance().m_UpAxisRect.width;
		cy = USBConfig::Instance().m_UpAxisRect.height;
	}
	else//默认值
	{
		cx = 0;
		cy = 0;
	}
	return;
}