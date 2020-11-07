# prostate_puncture
main program of prostate puncture

# v1.0
以 Calibrate 作为单启动项目，实现标定辅助的信息采集。0.3s的时间间隔，记录360个时刻，所有探头的位置以及相应的一帧B超图像
以 Puncture 作为单启动项目，读取两个ini配置文件，实时跟踪B超 NDI设备状态，跟随NDI探头(注意 不是超声探头)，实时更新MRI模拟采样探头位置，更新prostate mask

# v1.1
叠加显示mask轮廓线,prostate lesion rectum三种mask，分别以红/绿/蓝色标注轮廓
USBCapturer模块添加USE_LOCAL_US_IMAGE宏，用于在B超探头不在时，使用硬盘图片替代B超图片。
将Debug/Config添加入git库

# v1.2
与mevislab预处理软件mr_processor v2.1联调通过。可以实现对应读取ini配置文件以及raw数据文件。由于SurgicalPlan.ini记录的是数据的绝对路径，如果移动了数据文件夹，仍然需要手动进入ini文件设置绝对路径。

# v1.2.1
修改了AnalyseConfig和SurgicalPlan，MR数据的像素数变量名如m_nSizeX,MR数据的体素大小如m_fVoxelSizeX。
为Puncture\Output\x64\Debug目录添加了若干dll文件，从而可以从git库clone后直接编译生成运行。
出现了mat stackoverflower问题，待解决
帧率目测只有约2fps,需要处理吗？

# v1.3
添加了3个objFile的路径
修正了Calibrate项目的中间目录
添加了.gitignore，不再跟踪不需要的文件
可编译生成，不可运行(缺少SceneGraph.dll)

# v1.3.1
添加了SceneGraph.dll，可git后直接编译生成运行

# v1.4
修复了CUsImageWnd.cpp onPaint()双缓冲引起的内存泄露问题。 AnalyseProcess.cpp引起的stack overflow待修复

# v1.4.1
通过修改Puncture项目的堆栈保留大小，解决了stack overflow问题。现在可以稳定运行(测试了三小时)

# v1.4.2
在PositionManager::SetCurUSPosition()中 设置了长条形NDI探头的初始姿态。以NDI探头作为B超探头姿态(即忽略标定过程)的前提下，测试通过，探头的移动与MASK的变换一致

# v1.4.3
将Coordinate和Matrix类 由AnalyseProcess项目 移动至 ComUtility项目
为3d显示窗口 添加截取平面示意

# v1.5
全部用std::thread替代了c语言的CreateThread()
完成了NDIConfig，从ini文件读取 NDI探头端口号、探头初始姿态、标定变换矩阵。使用了标定矩阵，定性测试通过
添加了fsutility::Attitude类，用四个其次坐标表示姿态，全面替换了NDIOperator::Attitude中欧拉角形式的姿态

# v1.5.1 (未打tag)
release版本生成完成,3d窗口创建过程中 m_p3DShowWnd->CreateWnd() 出现asseration failure debug 版本无问题。很可能是因为父窗口初始化出错

# v1.6
使用qt替换了mfc 原版本功能复现
出现问题  窗口显示的轮廓 红色/蓝色残影

# v1.6.1 
修复了红色/蓝色残影的问题
修复了固定值的内存泄漏
横断面/矢状面/双平面  跟随b超机切换

# v1.6.2
修复了矢状面截取错误的问题 由于矢状面ROI不确定 仍然有偏差
配准前 允许由键盘手动调整MASK位置

# v1.6.3(尚未打tag)
为GraphicsView2D读取us图片时 添加gama变换 使得能够清楚看到图像边缘





