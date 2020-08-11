/*_____________________________________________________
 |
 |	Intra3D Layer1 :  Algebra 构件组成之一
 |                                          
 |  文件：Trackball.h
 |
 |  功能：鼠标-跟踪球旋转
 |
 |  开发：林锐 ，1999/02/01
 |
 |	源程序测试：进行了单步跟踪测试
 |_____________________________________________________*/

/*_____________________________________________________
 |  
 |	基于 OpenGL 的交互式三维图形软件开发工具
 |
 |	**   Intra3D 2.0 for Windows 9X/NT  **
 |
 |	          著作权人：林锐 
 |	
 |	浙江大学 CAD&CG 国家重点实验室 (310027)
 |_____________________________________________________*/

//鼠标跟踪球算法
//作用：计算出旋转轴矢量和旋转角度
#if !defined(TRACKBALL_H)
#define		 TRACKBALL_H

#include "Vector.h"

class Trackball
{
public:
	Trackball(void);
   ~Trackball(void);

public:
	// 跟踪球的中心为窗口的中心
	void SetTrackWindow(int iWidth, int iHeight);

	// 跟踪球的中心为窗口坐标(centerX, centerY) 
	void SetTrackWindow(int iWidth, int iHeight, int iCenterX, int iCenterY);

	// 输入刚按下鼠标时的坐标 (mouseX, mouseY)
	void Start(int iMouseX, int iMouseY);

	// 鼠标移动 (dx,dy), 计算旋转轴 axis 与角度 angle 
	void Tracking(int iDx, int iDy, VECTOR *axis, float *fAngle);

protected:

	// 将鼠标(mouseX, mouseY)投影到球面一点 P
	// 函数值即为球心到P的矢量
	VECTOR MouseToVector(int iMouseX, int iMouseY);

	int    m_iWidth,   m_iHeight;	// 窗口的宽高
	int    m_iCenterX, m_iCenterY;	// 跟踪球中心
	int    m_iRadius;				// 跟踪球半径
	int    m_iMouseX,  m_iMouseY;	// 鼠标位置

	VECTOR m_start;	// 刚按下时的鼠标投影矢量
	VECTOR m_stop;	// 移动(dx,dy)时的鼠标投影矢量

};

#endif // TRACKBALL_H
