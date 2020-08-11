// Camera.h: interface for the Camera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERA_H__A380E057_4627_463F_910B_8CD45B0F42A4__INCLUDED_)
#define AFX_CAMERA_H__A380E057_4627_463F_910B_8CD45B0F42A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Matrix.h"
#include "Enum.h"
#include "Vector.h"
#include "Rotation.h"

//相机，使用一个全局相机
class AFX_EXT_CLASS Camera  
{
public:
	Camera();
	virtual ~Camera();

	//使用相机所需参数
	/*矩阵变量及相应的简单函数*/
	SceneGraphMatrix m_matProj;   //投影矩阵
	SceneGraphMatrix m_matView;   //视景矩阵
	SceneGraphMatrix m_matOldView; //原始视景矩阵，为复员使用
	SceneGraphMatrix getProjMat(){return m_matProj;}
	SceneGraphMatrix getViewMat(){return m_matView;}

	/*投影变量及跟投影相关的函数*/
	float m_fFovY;  //y方向的视角范围，以度为单位
	float m_fAspect; //x方向决定视场的宽长比，比率为x（宽）：y（长）
	float m_fNearZ;  //观察者到近裁剪面的距离（大于零）
	float m_fFarZ;  //观察者到远裁剪面的距离（大于零）

	PROJ_MODE   m_eProjMode;
	void setProjMode(PROJ_MODE  mode) { m_eProjMode = mode;}

	void setAspect(float Aspect);
	void setProj(float FovY,float Aspect,float NearZ,float FarZ);
	void setProj(float left,float right,float bottom,float top,float nearZ,float farZ);

	/*－－－－－－－－－－－－－－－－
	View变量，主要用于相机的变换
	有绕固定轴旋转、平移等
	－－－－－－－－－－－－－－－－*/
	VECTOR m_vEyePt; //照相机（或人眼）位置
	VECTOR m_vLookatPt; //相机对准的方向（即场景的中心位置）
	VECTOR m_vUpVec;  //相机上升方向（即相机沿哪个坐标轴上升）
	
	VECTOR m_vOldEyePt;//照相机（或人眼）原来位置
	VECTOR m_vOldLookatPt;//相机对准的方向初始值（即场景的中心位置）
	VECTOR m_vOldUpVec;//相机上升方向初始值（即相机沿哪个坐标轴上升）

	void setView(VECTOR vEyePt, VECTOR vLookatPt, VECTOR vUpVec);
	void setView(float eyex,float eyey,float eyez,float centerx,float centery,float centerz,float upx,float upy,float upz);

	void Translate(float x,float y,float z); //相机移动（如果视点不移动，则看起来是场景旋转了）

	void Rotation (VECTOR vDirection, float Angle);//相机绕固定轴旋转
	void RotateX(float Angle); //相机绕世界坐标系的X轴旋转
	void RotateY(float Angle); //相机绕世界坐标系的Y轴旋转
	void RotateZ(float Angle); //相机绕世界坐标系的Z轴旋转
	void RotateIncameraX(float Angle);//相机绕自身坐标系的X轴旋转
	void RotateIncameraY(float Angle);//相机绕自身坐标系的Y轴旋转
	void RotateIncameraZ(float Angle);//相机绕自身坐标系的Z轴旋转
	void ReStoreView();          //恢复View的原始状态
	void SaveViewState();        //保存当前View状态

	void MultViewMatrix(const SceneGraphMatrix *M); //相机的View矩阵乘以一个变换矩阵

};

#endif // !defined(AFX_CAMERA_H__A380E057_4627_463F_910B_8CD45B0F42A4__INCLUDED_)
