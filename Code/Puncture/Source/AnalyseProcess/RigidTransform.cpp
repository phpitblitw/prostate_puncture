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
#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkGlyph3DMapper.h>
#include <vtkLandmarkTransform.h>
#include <vtkLine.h>
#include <vtkMath.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkVertexGlyphFilter.h>
#include "pch.h"
#include "RigidTransform.h"
#include "ErrorManager/ErrorCodeDefine.h"

using namespace ANALYSEPROCESS;

///////////////////////////////////////////////////////
////////////////////////define/////////////////////////
///////////////////////////////////////////////////////
#define MAX_ITERATION 30
/*****************************************************************
Global Variables
*****************************************************************/

/*****************************************************************
Function
*****************************************************************/
#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

// For compatibility with new VTK generic data arrays
#ifdef vtkGenericDataArray_h
#define InsertNextTupleValue InsertNextTypedTuple
#endif

RigidTransform::RigidTransform()
{
	m_bHasSrcAttitude = false;
	m_bHasDstAttitude = false;
}


RigidTransform::~RigidTransform()
{
}

void ANALYSEPROCESS::RigidTransform::SetSrcAttitude(const fsutility::Attitude & attitude)
{
	this->m_SrcAttitude = attitude;
	this->m_bHasSrcAttitude = true;
}

void ANALYSEPROCESS::RigidTransform::SetDstAttitude(const fsutility::Attitude & attitude)
{
	this->m_DstAttitude = attitude;
	this->m_bHasDstAttitude = true;
}

void ANALYSEPROCESS::RigidTransform::SetSrcPts(const vector<fsutility::Coordinate>& pts)
{
	this->m_SrcPts = pts;
}

void ANALYSEPROCESS::RigidTransform::SetDstPts(const vector<fsutility::Coordinate>& pts)
{
	this->m_DstPts = pts;
}

void ANALYSEPROCESS::RigidTransform::Reset()
{
	this->m_bHasSrcAttitude = false;
	this->m_bHasDstAttitude = false;
	m_SrcPts.clear();
	m_DstPts.clear();
}

fsutility::Matrix ANALYSEPROCESS::RigidTransform::CalTransformMatrix()
{
	double defaultElems[16]{ 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	fsutility::Matrix defaultTransform=fsutility::Matrix(defaultElems);  //特殊情况下 输出单位矩阵
	fsutility::Matrix result = defaultTransform;
	vtkNew<vtkPolyData> src;
	vtkNew<vtkPolyData> dst;
	vtkNew<vtkIterativeClosestPointTransform> icp;

	//根据一组对应的位姿，直接计算刚体变换矩阵
	if (m_bHasSrcAttitude && m_bHasDstAttitude) 
	{
		Matrix srcPosition(m_SrcAttitude.m_ScanCenter, m_SrcAttitude.m_RightDir, m_SrcAttitude.m_UpDir, m_SrcAttitude.m_MoveDir);
		Matrix dstPosition(m_DstAttitude.m_ScanCenter, m_DstAttitude.m_RightDir, m_DstAttitude.m_UpDir, m_DstAttitude.m_MoveDir);
		if(abs(srcPosition.GetDeterminant())!=0)  //行列式为0的特殊情况下，无法计算逆矩阵
			result = dstPosition.GetMultiply(srcPosition.GetInverse());
	}
	//根据一组对应的点云，使用icp算法迭代计算刚体变换矩阵
	if (m_SrcPts.size() > 4 && m_DstPts.size() > 4)
	{
		Coordinates2Polydata(m_SrcPts, src);
		Coordinates2Polydata(m_DstPts, dst);
		icp->SetSource(src);
		icp->SetTarget(dst);
		icp->GetLandmarkTransform()->SetModeToRigidBody();  //设置为刚体变换
		icp->SetMaximumNumberOfIterations(MAX_ITERATION);  //设置迭代次数
		//将位姿计算出的变换矩阵 作为icp算法的初始值
		if (m_bHasSrcAttitude && m_bHasDstAttitude)
		{
			vtkSmartPointer<vtkMatrix4x4> m = icp->GetMatrix();  //获取指向icp对象矩阵的指针，用于赋值其中元素(内部默认调用一次icp->update() 此为没必要的操作 但无法避免)
			std::cout << "The resulting matrix is: " << *m << std::endl;
			for (int y = 0; y < 4; y++)
				for (int x = 0; x < 4; x++)
					m->SetElement(y, x, result.GetValue(y, x));
		}
		icp->Modified();  //更新本对象的修改时间(在本例中实际上用不到)
		icp->Update();
		vtkSmartPointer<vtkMatrix4x4> n = icp->GetMatrix();
		VtkMatrix2Matrix(n, result);
	}
	return result;
}

// 将Coordinate形式的点集，转为vtkPolyData数据类型
void ANALYSEPROCESS::RigidTransform::Coordinates2Polydata(vector<Coordinate>& src, vtkSmartPointer<vtkPolyData> res)
{
	int i, j;
	float pt[3];

	//分别添加各个点
	vtkNew<vtkPoints> points;
	for (i = 0; i < src.size(); i++) {
		pt[0] = src[i].x;
		pt[1] = src[i].y;
		pt[2] = src[i].z;
		points->InsertNextPoint(pt);
	}
	//创建模型
	vtkNew<vtkPolyData> obj;
	obj->SetPoints(points);
	//点集转换为顶点
	vtkNew<vtkVertexGlyphFilter> vertexFilter;
	vertexFilter->SetInputData(obj);
	vertexFilter->Update();
	res->ShallowCopy(vertexFilter->GetOutput());
}

// 将vtk定义的4*4矩阵，赋值给本项目定义的4*4矩阵
void ANALYSEPROCESS::RigidTransform::VtkMatrix2Matrix(vtkSmartPointer<vtkMatrix4x4> src, Matrix & dst)
{
	int y, x;
	double elememts[16];
	for (y = 0; y < 4; y++)
		for (x = 0; x < 4; x++)
			elememts[y * 4 + x] = src->GetElement(y, x);
	dst.SetValue(elememts);
}

// 将本项目定义的4*4矩阵，赋值给vtk定义的4*4矩阵
void ANALYSEPROCESS::RigidTransform::Matrix2VtkMatrix(Matrix & src, vtkSmartPointer<vtkMatrix4x4> dst)
{
	int y, x;
	for (y = 0; y < 4; y++)
		for (x = 0; x < 4; x++)
			dst->SetElement(y, x, src.GetValue(y, x));
}
