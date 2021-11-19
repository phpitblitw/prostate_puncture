//*****************************************************************
// 文件名 :						RigidTransform.h
// 版本	 :						1.0
// 目的及主要功能 :				解算刚体变换
// 创建日期 :					2021.11.05
// 修改日期 :					2021.11.05
// 作者 :						SunYizhe
// 修改者 :						
// 联系方式 :					fiki@seu.edu.cn
//*****************************************************************/
#pragma once

/*****************************************************************
Library Files Included
*****************************************************************/
#include <vtkMatrix4x4.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vector>
#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkGlyph3DMapper.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkLandmarkTransform.h>
#include <vtkLine.h>
#include <vtkMath.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkVertexGlyphFilter.h>
#include "ComUtility/Attitude.h"
#include "ComUtility/Coordinate.h"
#include "ComUtility/Matrix.h"

using namespace fsutility;
using namespace std;

/*****************************************************************
Defines
*****************************************************************/
#ifdef ANALYSEPROCESS_EXPORT
#define ANALYSEPROCESS_API _declspec(dllexport)
#else
#define ANALYSEPROCESS_API _declspec(dllimport)
#endif


/*****************************************************************
						Routine Definitions
解算刚体变换矩阵RigidTransform类
	i.	可根据一组对应的位姿，直接数值计算刚体变换参数
	ii.	如有一组对应的点云，则使用vtk封装的icp算法迭代计算缸体变换参数
	iii.如果以上两种数据都存在，则在i计算出的刚体变换参数基础上 使用icp算法迭代优化
	iv.	一般的，source指US，dstination指MRI
*****************************************************************/
namespace ANALYSEPROCESS
{
	//位置处理类
	class RigidTransform;
	typedef std::shared_ptr <RigidTransform> RigidTransformPtr;

	class ANALYSEPROCESS_API RigidTransform
	{
	public:
		RigidTransform();
		~RigidTransform();

		void SetSrcAttitude(const fsutility::Attitude& attitude);
		void SetDstAttitude(const fsutility::Attitude& attitude);
		void SetSrcPts(const vector<fsutility::Coordinate>& pts);
		void SetDstPts(const vector<fsutility::Coordinate>& pts);
		void Reset();  //清除所有已输入的数据
		fsutility::Matrix CalTransformMatrix();  //根据现有数据，解算刚体变换参数
	public:
	private:
		void Coordinates2Polydata(vector<Coordinate>& src, vtkSmartPointer<vtkPolyData> res);  //将Coordinate形式的点集，转为vtkPolyData数据类型
		void VtkMatrix2Matrix(vtkSmartPointer<vtkMatrix4x4> src, Matrix & dst);  //将vtk定义的4*4矩阵，赋值给本项目定义的4*4矩阵
		void Matrix2VtkMatrix(Matrix & src, vtkSmartPointer<vtkMatrix4x4> dst);  //将本项目定义的4*4矩阵，赋值给vtk定义的4*4矩阵
	private:
		bool m_bHasSrcAttitude;  //源位姿参数是否准备就绪 标志位
		bool m_bHasDstAttitude;  //目的位姿参数是否准备就绪 标志位

		// 一组对应的位姿，可用于直接计算刚体变换矩阵
		fsutility::Attitude m_SrcAttitude;  
		fsutility::Attitude m_DstAttitude;  

		//一组对应的点云，可参与icp算法迭代计算刚体变换矩阵
		vector<fsutility::Coordinate> m_SrcPts; 
		vector<fsutility::Coordinate> m_DstPts;
	};
}


