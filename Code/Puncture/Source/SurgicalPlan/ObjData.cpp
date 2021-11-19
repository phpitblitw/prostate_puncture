#include "stdafx.h"
#include "ObjData.h"
#include <fstream>

using namespace SURGICALPLAN;

ObjData::ObjData()
{
	m_pVbo = nullptr;
	m_pEbo = nullptr;
	m_BoundBox.x1 = m_BoundBox.y1 = m_BoundBox.z1 = 99999999;
	m_BoundBox.x2 = m_BoundBox.y2 = m_BoundBox.z2 = -1;
}


ObjData::~ObjData()
{
	if(m_pVbo!=nullptr)
		delete[] m_pVbo;
	if (m_pEbo != nullptr)
		delete[] m_pEbo;
}

BOOL ObjData::LoadObjFile(CString strFileName)
{
	int temp = 0;
	int i,j;
	char cType;  //类别 c-顶点 f-三角面片
	float x, y, z;
	std::fstream file;
	file.open(strFileName.GetBuffer());

	//读取顶点坐标
	file >> m_nVertexNum;
	if (m_nVertexNum <= 0)
		return ER_ImportObjDataError;
	//m_pVbo = new float[m_nVertexNum * 3];
	m_vertices = std::vector<Coordinate>(m_nVertexNum);
	for (i = 0; i < m_nVertexNum; i++)
	{
		file >> cType;
		if (cType == 'v')
		{
			file >> m_vertices[i].x >> m_vertices[i].y >> m_vertices[i].z;
			m_vertices[i].w = 1;  //w=1代表载入的是点坐标
			//统计boundbox
			m_BoundBox.x1 = min(m_BoundBox.x1, m_vertices[i].x);
			m_BoundBox.x2 = max(m_BoundBox.x2, m_vertices[i].x);
			m_BoundBox.y1 = min(m_BoundBox.y1, m_vertices[i].y);
			m_BoundBox.y2 = max(m_BoundBox.y2, m_vertices[i].y);
			m_BoundBox.z1 = min(m_BoundBox.z1, m_vertices[i].z);
			m_BoundBox.z2 = max(m_BoundBox.z2, m_vertices[i].z);
		}
	}

	//读取三角面片下标
	file >> m_nTriangleNum;
	if (m_nTriangleNum <= 0)
		return ER_ImportObjDataError;
	m_indices = std::vector<std::vector<int>>(m_nTriangleNum, std::vector<int>(3));  //(m_nTriangleNum*3)的数组
	for (i = 0; i < m_nTriangleNum; i++)
	{
		file >> cType;
		if (cType == 'f')
		{
			file >> m_indices[i][0] >> m_indices[i][1] >> m_indices[i][2];
		}
	}
	for (i = 0; i < m_nTriangleNum; i++)
	{
		for (j = 0; j < 3; j++)
		{
			m_indices[i][j] -= 1;  //opengl规定索引下标从0开始，而本obj文件的索引下标从1开始，故进行修正
		}
	}
	
	//计算法线
	this->CalculateNormal();
	return 0;
}

void SURGICALPLAN::ObjData::GetBoundBox(float & x1, float & x2, float & y1, float & y2, float & z1, float & z2)
{
	x1 = m_BoundBox.x1;
	x2 = m_BoundBox.x2;
	y1 = m_BoundBox.y1;
	y2 = m_BoundBox.y2;
	z1 = m_BoundBox.z1;
	z2 = m_BoundBox.z2;
}

std::vector<Coordinate> SURGICALPLAN::ObjData::GetVertices()
{
	return m_vertices;
}

float* ObjData::GetVboArray()
{
	//将顶点位置、顶点法向量数据存储到float数组中
	if (m_pVbo == nullptr)
	{
		int vertexIndex;
		m_pVbo = new float[m_nVertexNum*6];  //顶点位置 float*3 顶点法向量 float*3
		for (vertexIndex = 0; vertexIndex < m_nVertexNum; vertexIndex++)
		{
			//顶点位置数据
			m_pVbo[vertexIndex * 6 + 0] = float(m_vertices[vertexIndex].x);
			m_pVbo[vertexIndex * 6 + 1] = float(m_vertices[vertexIndex].y);
			m_pVbo[vertexIndex * 6 + 2] = float(m_vertices[vertexIndex].z);
			//顶点法线数据
			m_pVbo[vertexIndex * 6 + 3] = float(m_normal[vertexIndex].x);
			m_pVbo[vertexIndex * 6 + 4] = float(m_normal[vertexIndex].y);
			m_pVbo[vertexIndex * 6 + 5] = float(m_normal[vertexIndex].z);
		}
	}
	return m_pVbo;
}

unsigned int* ObjData::GetEboArray()
{
	//将三角面片的顶点下标数据 存储到unsigned int数组中
	if (m_pEbo == nullptr)
	{
		int triIndex;
		m_pEbo = new unsigned int[m_nTriangleNum*3];
		for (triIndex = 0; triIndex < m_nTriangleNum; triIndex++)
		{
			m_pEbo[triIndex*3 + 0] = unsigned int(m_indices[triIndex][0]);
			m_pEbo[triIndex*3 + 1] = unsigned int(m_indices[triIndex][1]);
			m_pEbo[triIndex*3 + 2] = unsigned int(m_indices[triIndex][2]);
		}
	}
	return m_pEbo;
}

void ObjData::CalculateNormal()
{
	int triIndex,vertexIndex;
	Coordinate ptA, ptB, ptC;  //当前三角面片的3个顶点
	Coordinate vAB, vBC;  //当前三角面片，AB,BC向量
	Coordinate vNormal;  //当前面片的法向量
	m_normal = std::vector<Coordinate>(m_nVertexNum,Coordinate(0,0,0,0));  //默认初始化为全0向量
	//每个顶点的法向量为其所在面的法向量之和
	for (triIndex = 0; triIndex < m_nTriangleNum; triIndex++)
	{
		ptA = m_vertices[m_indices[triIndex][0]];
		ptB = m_vertices[m_indices[triIndex][1]];
		ptC = m_vertices[m_indices[triIndex][2]];
		vAB = ptB - ptA;
		vBC = ptC - ptB;
		vNormal = vAB.GetCrossProduct(vBC);  //法向量由三角面片的两条边叉乘获得
		m_normal[m_indices[triIndex][0]] = m_normal[m_indices[triIndex][0]] + vNormal;  //为三角面片的每个顶点 叠加该三角面的法向量
		m_normal[m_indices[triIndex][1]] = m_normal[m_indices[triIndex][1]] + vNormal;
		m_normal[m_indices[triIndex][2]] = m_normal[m_indices[triIndex][2]] + vNormal;
	}
	//法向量归一化
	for (vertexIndex = 0; vertexIndex < m_nVertexNum; vertexIndex++)
	{
		m_normal[vertexIndex].Normalize();
	}
}
