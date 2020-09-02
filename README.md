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
与mevislab预处理软件mr_processor v2.1同步调试通过。可以实现对应读取ini配置文件以及raw数据文件。由于SurgicalPlan.ini记录的是数据的绝对路径，如果移动了数据文件夹，仍然需要手动进入ini文件设置绝对路径。

