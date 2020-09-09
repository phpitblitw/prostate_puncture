//matrix.cpp
#include "pch.h"
#include "AnalyseProcess/Matrix.h"

using namespace ANALYSEPROCESS;

Matrix::Matrix()
{
	int y, x, i = 0;
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			m_dItem[y][x] = 0;
		}
	}
}

Matrix::Matrix(double m[16])
{
	int y, x, i = 0;
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			m_dItem[y][x] = m[i++];
		}
	}
}

Matrix::Matrix(Coordinate c0, Coordinate c1, Coordinate c2, Coordinate c3)
{
	m_dItem[0][0] = c0.x;
	m_dItem[0][1] = c1.x;
	m_dItem[0][2] = c2.x;
	m_dItem[0][3] = c3.x;
	m_dItem[1][0] = c0.y;
	m_dItem[1][1] = c1.y;
	m_dItem[1][2] = c2.y;
	m_dItem[1][3] = c3.y;
	m_dItem[2][0] = c0.z;
	m_dItem[2][1] = c1.z;
	m_dItem[2][2] = c2.z;
	m_dItem[2][3] = c3.z;
	m_dItem[3][0] = c0.w;
	m_dItem[3][1] = c1.w;
	m_dItem[3][2] = c2.w;
	m_dItem[3][3] = c3.w;
	return;
}

Matrix::~Matrix()
{}

void Matrix::SetValue(double m[16])
{
	int y, x, i = 0;
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			m_dItem[y][x] = m[i];
			i++;
		}
	}
	return;
}

void Matrix::SetValue(Coordinate c0, Coordinate c1, Coordinate c2, Coordinate c3)
{
	m_dItem[0][0] = c0.x;
	m_dItem[0][1] = c1.x;
	m_dItem[0][2] = c2.x;
	m_dItem[0][3] = c3.x;
	m_dItem[1][0] = c0.y;
	m_dItem[1][1] = c1.y;
	m_dItem[1][2] = c2.y;
	m_dItem[1][3] = c3.y;
	m_dItem[2][0] = c0.z;
	m_dItem[2][1] = c1.z;
	m_dItem[2][2] = c2.z;
	m_dItem[2][3] = c3.z;
	m_dItem[3][0] = c0.w;
	m_dItem[3][1] = c1.w;
	m_dItem[3][2] = c2.w;
	m_dItem[3][3] = c3.w;
	return;
}

Coordinate Matrix::GetMultiply(Coordinate c)
{
	Coordinate result;
	result.x = m_dItem[0][0] * c.x + m_dItem[0][1] * c.y + m_dItem[0][2] * c.z + m_dItem[0][3] * c.w;
	result.y = m_dItem[1][0] * c.x + m_dItem[1][1] * c.y + m_dItem[1][2] * c.z + m_dItem[1][3] * c.w;
	result.z = m_dItem[2][0] * c.x + m_dItem[2][1] * c.y + m_dItem[2][2] * c.z + m_dItem[2][3] * c.w;
	result.w = m_dItem[3][0] * c.x + m_dItem[3][1] * c.y + m_dItem[3][2] * c.z + m_dItem[3][3] * c.w;
	return result;
}

Matrix Matrix::GetMultiply(Matrix m)
{
	int y, x;
	Matrix result;
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			result.m_dItem[y][x] = this->m_dItem[y][0] * m.m_dItem[0][x] + this->m_dItem[y][1] * m.m_dItem[1][x]
				+ this->m_dItem[y][2] * m.m_dItem[2][x] + this->m_dItem[y][3] * m.m_dItem[3][x];
		}
	}
	return result;
}

Matrix Matrix::GetInverse()
{
	int y, x;
	double det;	//矩阵行列式
	Matrix result;

	//求矩阵行列式
	det = this->GetDeterminant();
	if (det == 0)
		return result;
	//求每个位置的代数余子式
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			result.m_dItem[y][x] = this->GetCofactor(y, x);
		}
	}
	//求伴随矩阵
	result = result.GetTranspose();
	//最终结果
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			result.m_dItem[y][x] /= det;
		}
	}

	return result;
}

double Matrix::GetDeterminant()
{
	double result;
	result = m_dItem[0][0] * GetCofactor(0, 0) + m_dItem[0][1] * GetCofactor(0, 1) + m_dItem[0][2] * GetCofactor(0, 2) + m_dItem[0][3] * GetCofactor(0, 3);
	return result;
}

Matrix Matrix::GetTranspose()
{
	int y, x;
	Matrix result;
	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			result.m_dItem[y][x] = m_dItem[x][y];
		}
	}
	return result;
}

double Matrix::GetMinor(int y, int x)
{
	int i, n, col[3], row[3];	//col[3]记录行列式对应哪几列，row[3]记录行列式对应的哪几行
	double result;
	for (i = 0, n = 0; i < 4; i++)
	{
		if (i == x)
			continue;
		col[n++] = i;
	}
	for (i = 0, n = 0; i < 4; i++)
	{
		if (i == y)
			continue;
		row[n++] = i;
	}
	result = m_dItem[row[0]][col[0]] * (m_dItem[row[1]][col[1]] * m_dItem[row[2]][col[2]] - m_dItem[row[1]][col[2]] * m_dItem[row[2]][col[1]])
		- m_dItem[row[0]][col[1]] * (m_dItem[row[1]][col[0]] * m_dItem[row[2]][col[2]] - m_dItem[row[1]][col[2]] * m_dItem[row[2]][col[0]])
		+ m_dItem[row[0]][col[2]] * (m_dItem[row[1]][col[0]] * m_dItem[row[2]][col[1]] - m_dItem[row[1]][col[1]] * m_dItem[row[2]][col[0]]);
	return result;
}

double Matrix::GetCofactor(int y, int x)
{
	double result;
	result = pow(-1, y + x)*GetMinor(y, x);
	return result;
}