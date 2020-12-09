#pragma once
/*****************************************************************
Defines
*****************************************************************/
#ifdef SURGICALPLAN_EXPORT
#define SURGICALPLAN_API __declspec(dllexport)
#else
#define SURGICALPLAN_API __declspec(dllimport)
#endif

#include <atlstr.h>
#include <string>
#include <vector>
#include "ErrorManager//ErrorCodeDefine.h"
#include "StructDef.h"
#include <ComUtility/Coordinate.h>

using namespace fsutility;

namespace SURGICALPLAN
{
	//class Coordinate;  //类Coordinate的前向声明。不引用该类的头文件，是为了避免头文件中的AFX_EXT_CLASS在其他项目中无法被解析

	class SURGICALPLAN_API ObjData
	{
	public:
		ObjData();
		~ObjData();

		BOOL LoadObjFile(CString strFileName);
		int GetVertexNum() { return m_nVertexNum; }
		int GetTriangleNum() { return m_nTriangleNum; }
		void GetBoundBox(float &x1, float &x2, float &y1, float &y2, float &z1, float &z2);
		float* GetVboArray();  //以float数组的形式，返回vbo数据(每个点包括：3*float 顶点x、y、z坐标;3*float 顶点法向量)
		unsigned int* GetEboArray(); //以unsigned int数组的形式 返回ebo数据

	private:
		void CalculateNormal();  //更新法线数组
	private:
		int m_nVertexNum;  //顶点数量
		int m_nTriangleNum;  //三角面片数量
		std::vector<Coordinate> m_vertices;  //模型顶点
		float* m_pVbo;  //顶点缓冲对象 vbo数组 (总数=顶点数量*3个分量+法向量数量*3个分量)
		std::vector<Coordinate> m_normal;  //各个顶点的法线数组
		std::vector<std::vector<int>> m_indices;  //模型下标
		unsigned int* m_pEbo;  //索引缓冲对象 ebo数组 (总数=三角面片数量数量*3个下标)

		ThreeD_BOUND_BOX m_BoundBox;
	};
	typedef std::shared_ptr<ObjData> ObjDataPtr;
}


