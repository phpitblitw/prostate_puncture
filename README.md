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

# v1.2.2(尚未提交该版本)
添加了3个SurfaceFile的路径。
添加了3个objFile的路径
//TODO 把surgicalPlan同步到include目录下

# v1.3
修正了Calibrate项目的中间目录
添加了.gitignore，不再跟踪不需要的文件



