#pragma once

/**************************************************定义错误提示代码*******************************************************/
enum
{
	WatchDogCount = 99999
};

enum ErrorCodeType {
	LIST_NO_ERROR = 0x00000000,	//正常返回

	//Puncture部分
	ER_InitNDIDeviceFailed				= 0x00000001,	//启动NDI设备失败
	ER_InitUSBDeviceFailed				= 0x00000002,	//启动B超图像采集设备失败

	//NDIOperator模块
	ER_WrongNDIConfigFilePathName		= 0x00100001,	//错误的NDI模块ini文件路径及文件名
	ER_OpenNDIConfigFileFailed			= 0x00100002,	//打开NDI模块ini文件失败
	ER_StartTrackingNDIFailed			= 0x00100003,	//
	ER_TrackingNDIFailed				= 0x00100004,
	ER_StopTrackingNDIFailed			= 0x00100005,

	//SurgicalPlan模块
	ER_FileOpenFailed					= 0x00200001,	//无法打开对应的ini文件
	ER_InportSurgicalPlanFailed			= 0x00200002,	//导入手术计划失败
	ER_OpenSurgicalPlanConfigFileFailed = 0x00200003,	//导入手术计划ini文件失败
	ER_InportMaskDataError				= 0x00200004,	//导入Mask数据失败
	ER_InportMRIDataError				= 0x00200005,	//导入MRI数据失败
	ER_ReachedRectumEnd					= 0x00200006,	//到达了直肠中轴线坐标序列的终点
	ER_ImportRectumCenterError			= 0x00200007,	//导入直肠中轴线txt数据失败
	ER_ImportObjDataError				= 0x00200008,	//导入Obj数据失败

	//AnalyseProcess模块
	ER_2DImageSizeError					= 0x00300001,	//2D图像尺寸错误
	ER_3DImageSizeError					= 0x00300002,	//3D图像尺寸错误
	ER_WrongAnalyzeConfigFilePathName	= 0x00300002,	//无法打开对应的ini文件
	ER_WrongAnalyseConfigFilePathName	= 0x00300004,	//错误的病人MRI数据ini文件路径及文件名
	ER_OpenAnalyseConfigFileFailed		= 0x00300005,	//打开病人MRI数据对应的ini文件失败
	ER_CalculateTransformMatrix			= 0x00300006,	//计算超声/MRI模拟采样转换矩阵失败
	ER_NoSurgicalPlan					= 0x00300007,	//ImageSampler设置DataPackage失败
	ER_SampleParameterError				= 0x00300008,	//ImageSampler切割截面参数有误
	ER_InitAnalyseProcessFailed			= 0x00300009,	//启动分析模块失败
	ER_CloseAnalyseProcessFailed		= 0x00300010,	//关闭分析模块失败

	//USBCapturer模块
	ER_WrongUSBConfigFilePathName		= 0x00400001,	//错误的NDI模块ini文件路径及文件名
	ER_OpenUSBConfigFileFailed			= 0x00400002,	//打开NDI模块ini文件失败
	ER_OpenUSBDeviceFailed				= 0x00400003,	//打开USB采集卡设备失败
	ER_NoCapturePerFrameFun				= 0x00400004,	//未设置采集B超帧后调用的回调函数
	ER_NoLetterTemplate					= 0x00400005	//未找到S,T的字符模板

};