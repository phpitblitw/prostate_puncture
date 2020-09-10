#pragma once
#include "StructDef.h"

/*****************************************************************
Defines
*****************************************************************/
#ifdef SURGICALPLAN_EXPORT
#define SURGICALPLAN_API __declspec(dllexport)
#else
#define SURGICALPLAN_API __declspec(dllimport)
#endif

namespace SURGICALPLAN
{
	static const float DELTA = 1.0e-10f;	// 定义误差

	class SURGICALPLAN_API CPubFunc
	{
	public:
		CPubFunc();
		virtual ~CPubFunc();

		static int Round(double x);//对x四舍五入，并返回其值

		template <class T>
		static inline void Exchange(T &x, T &y)//交换x,y的值
		{
			T t = x;
			x = y;
			y = t;
		}

		//采用双线性插值算法进行二维图像缩放
		template <class Type>//add by zgc 3.30
		static BOOL LinearZoom(Type *pOutImg, int Tcx, int Tcy, Type *pSrcImg, int Scx, int Scy,
			double dx, double dy, int Sleft, int Stop, int Sright, int Sbottom)
		{
			if (dx <= img_EqualCriteria || dy <= img_EqualCriteria)
				return FALSE;
			if (pOutImg == NULL || pSrcImg == NULL)
				return FALSE;
			if (Sleft < 0)
				Sleft = 0;
			if (Stop < 0)
				Stop = 0;
			if (Sright > Scx)
				Sright = Scx;
			if (Sbottom > Scy)
				Sbottom = Scy;
			int idx, idy, i, j;
			double ddx, ddy;
			double p0, p1, p2, p3;
			Type *pOut = pOutImg;
			Type *pInTemp = NULL;

			for (i = 0; i < Tcy; i++)
			{
				ddy = (double)i / dy + Stop;
				idy = int(ddy);
				ddy -= (double)idy;

				for (j = 0; j < Tcx; j++)
				{

					ddx = (double)j / dx + Sleft;
					idx = int(ddx);
					ddx -= (double)idx;
					if (idx < Sright - 1 && idy < Sbottom - 1)
					{
						pInTemp = pSrcImg + idy * Scx + idx;
						p0 = (double)*pInTemp;
						p1 = (double)*(pInTemp + 1);
						p2 = (double)*(pInTemp + Scx);
						p3 = (double)*(pInTemp + Scx + 1);
						double  temp = (p0*(1.0 - ddx) + p1 * ddx)*(1.0 - ddy) + (p2*(1.0 - ddx) + p3 * ddx)*ddy;
						*(pOut + j) = (Type)temp;
					}
					else if (idx == Sright - 1 && idy < Sbottom - 1)
					{
						pInTemp = pSrcImg + idy * Scx + idx;
						p0 = (double)*pInTemp;
						p2 = (double)*(pInTemp + Scx);

						double  temp = p0 * (1.0 - ddy) + p2 * ddy;
						*(pOut + j) = (Type)temp;
					}
					else if (idx < Sright - 1 && idy == Sbottom - 1)
					{
						pInTemp = pSrcImg + idy * Scx + idx;
						p0 = (double)*pInTemp;
						p1 = (double)*(pInTemp + 1);

						double  temp = p0 * (1.0 - ddx) + p1 * ddx;
						*(pOut + j) = (Type)temp;

					}
					else if (idx == Sright - 1 && idy == Sbottom - 1)
					{
						*(pOut + j) = *(pSrcImg + idy * Scx + idx);
					}

				}

				pOut += Tcx;
			}

			return TRUE;//done
		}

		//彩色图像缩放
		static BYTE ColorZoom(img_COLOR *pOutImg, int Tcx, int Tcy, img_COLOR *pSrcImg, int Scx, int Scy, double dx, double dy,
			int Sleft, int Stop, int Sright, int Sbottom)
		{
			if (dx <= img_EqualCriteria || dy <= img_EqualCriteria)
				return FALSE;
			if (pOutImg == NULL || pSrcImg == NULL)
				return FALSE;

			int i, j;
			int SrcLength = Scx * Scy;
			int TarLength = Tcx * Tcy;
			BYTE *pSrcChannel = new BYTE[SrcLength];
			BYTE *pTarChannel = new BYTE[TarLength];
			BYTE *pSrc = (BYTE*)pSrcImg;
			BYTE *pTar = (BYTE*)pOutImg;

			for (j = 0; j < 3; j++)
			{
				for (i = 0; i < SrcLength; i++)
					*(pSrcChannel + i) = *(pSrc + i * 3 + j);

				LinearZoom(pTarChannel, Tcx, Tcy, pSrcChannel, Scx, Scy, dx, dy, Sleft, Stop, Sright, Sbottom);

				for (i = 0; i < TarLength; i++)
					*(pTar + i * 3 + j) = *(pTarChannel + i);
			}

			delete[]pSrcChannel;
			delete[]pTarChannel;

			return TRUE;//done
		}

		template <class Type>
		static BOOL Zoom1Dx4(Type *pOutImg, int Tcx, Type *pSrcImg, int Scx, int Sx0, int Sx1, double MinMargin = 0.0, double MaxMargin = 255.0)
		{
			if (pOutImg == NULL || pSrcImg == NULL)
				return FALSE;
			if (Sx1 - Sx0 < 1)
				return FALSE;
			memset(pOutImg, 0, Tcx * sizeof(Type));
			int Cnt = Sx1 - Sx0;
			Type *pBuf = new Type[Cnt + 3];
			memcpy(pBuf + 1, pSrcImg, Cnt * sizeof(Type));
			*pBuf = *(pBuf + 1);
			*(pBuf + Cnt + 1) = *(pBuf + Cnt + 2) = *(pBuf + Cnt);
			int xIn, xOut;
			int InSize = Tcx / 4;
			if (InSize > Cnt)
				InSize = Cnt;
			double Inter1[4] = { -0.0703125, 0.8671875, 0.2265625, -0.0234375 };
			double Inter2[4] = { -0.0625, 0.5625, 0.5625, -0.0625 };
			double Inter3[4] = { -0.0234375, 0.2265625, 0.8671875, -0.0703125 };
			double Buf;
			for (xIn = 0, xOut = 0; xIn < InSize; xIn++)
			{
				*(pOutImg + xOut++) = *(pBuf + xIn + 1);
				Buf = Inter1[0] * *(pBuf + xIn) + Inter1[1] * *(pBuf + xIn + 1) + Inter1[2] * *(pBuf + xIn + 2) + Inter1[3] * *(pBuf + xIn + 3);
				*(pOutImg + xOut++) = (Type)(Buf < MinMargin ? MinMargin : (Buf > MaxMargin ? MaxMargin : Buf));
				Buf = Inter2[0] * *(pBuf + xIn) + Inter2[1] * *(pBuf + xIn + 1) + Inter2[2] * *(pBuf + xIn + 2) + Inter2[3] * *(pBuf + xIn + 3);
				*(pOutImg + xOut++) = (Type)(Buf < MinMargin ? MinMargin : (Buf > MaxMargin ? MaxMargin : Buf));
				Buf = Inter3[0] * *(pBuf + xIn) + Inter3[1] * *(pBuf + xIn + 1) + Inter3[2] * *(pBuf + xIn + 2) + Inter3[3] * *(pBuf + xIn + 3);
				*(pOutImg + xOut++) = (Type)(Buf < MinMargin ? MinMargin : (Buf > MaxMargin ? MaxMargin : Buf));
			}
			delete[] pBuf;
			return TRUE;
		}

		template <class Type>
		static BOOL Zoomx4(Type *pOutImg, int Tcx, int Tcy, Type *pSrcImg, int Scx, int Scy, int Sleft, int Stop, int Sright, int Sbottom,
			double MinMargin = 0.0, double MaxMargin = 255.0)
		{
			if (pOutImg == NULL || pSrcImg == NULL)
				return FALSE;
			Type *pBufSrcY = new Type[Scy];
			Type *pBufTarY = new Type[Tcy];
			Type *pBufOut = new Type[Scy*Tcx];
			Type *pOut = pOutImg;
			Type *pIn = pSrcImg;
			int i, j;
			for (i = 0; i < Scy; i++, pIn += Scx, pBufOut += Tcx)
				Zoom1Dx4(pBufOut, Tcx, pIn, Scx, Sleft, Sright, MinMargin, MaxMargin);
			pBufOut -= Tcx * Scy;
			for (i = 0; i < Tcx; i++, pBufOut++, pOut++)
			{
				for (j = 0; j < Scy; j++, pBufOut += Tcx)
					*(pBufSrcY + j) = *pBufOut;
				pBufOut -= Tcx * Scy;
				Zoom1Dx4(pBufTarY, Tcy, pBufSrcY, Scy, Stop, Sbottom, MinMargin, MaxMargin);
				for (j = 0; j < Tcy; j++, pOut += Tcx)
					*pOut = *(pBufTarY + j);
				pOut -= Tcx * Tcy;
			}
			pBufOut -= Tcx;
			delete[] pBufOut;
			delete[] pBufSrcY;
			delete[] pBufTarY;
			return TRUE;//done
		}

		//彩色图像缩放
		static BOOL ColorZoomx4(img_COLOR *pOutImg, int Tcx, int Tcy, img_COLOR *pSrcImg, int Scx, int Scy,
			int Sleft, int Stop, int Sright, int Sbottom)
		{
			if (pOutImg == NULL || pSrcImg == NULL)
				return FALSE;

			int i, j;
			int SrcLength = Scx * Scy;
			int TarLength = Tcx * Tcy;
			BYTE *pSrcChannel = new BYTE[SrcLength];
			BYTE *pTarChannel = new BYTE[TarLength];
			BYTE *pSrc = (BYTE*)pSrcImg;
			BYTE *pTar = (BYTE*)pOutImg;

			for (j = 0; j < 3; j++)
			{
				for (i = 0; i < SrcLength; i++)
					*(pSrcChannel + i) = *(pSrc + i * 3 + j);

				Zoomx4(pTarChannel, Tcx, Tcy, pSrcChannel, Scx, Scy, Sleft, Stop, Sright, Sbottom);

				for (i = 0; i < TarLength; i++)
					*(pTar + i * 3 + j) = *(pTarChannel + i);
			}

			delete[]pSrcChannel;
			delete[]pTarChannel;

			return TRUE;//done
		}

		static void VectorNormalize(Point3D *A)
		{
			double magnitude = sqrt(A->x*A->x + A->y*A->y + A->z*A->z);

			if (magnitude >= DELTA)	// 浮点误差
			{
				A->x = A->x / magnitude;
				A->y = A->y / magnitude;
				A->z = A->z / magnitude;
			}

		}
		// 矢量叉积，返回值 = A x B
		static Point3D VectorCross(const Point3D A, const Point3D B)
		{
			Point3D V;
			V.x = A.y*B.z - A.z*B.y;
			V.y = A.z*B.x - A.x*B.z;
			V.z = A.x*B.y - A.y*B.x;
			return V;
		}

		static int ByteSum(byte *pData, int nSize)
		{
			int i, sum;
			for (i = 0, sum = 0; i < nSize; i++)
				sum += pData[i];
			return sum;
		}

	};

}


