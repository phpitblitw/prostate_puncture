#pragma once
#include "GeometryNode.h"
#include "VectorFile.h"

class AFX_EXT_CLASS KnifeNode :public GeometryNode
{
public:
	KnifeNode(void);
	virtual ~KnifeNode(void);

	virtual void Draw();
	virtual	int getType(){return KNIFE_NODE;}

	void SetDirection(VECTOR direction);
	void SetDirection(float directionx, float directiony, float directionz);
	void SetKnifePoint(VECTOR knifepoint);
	void SetKnifePoint(float knifepointx, float knifepointy, float knifepointz);

	void GetDirection(VECTOR *direction);
	void GetDirection(float *directionx,float *directiony,float *directionz);
	void GetKnifePoint(VECTOR *knifepoint);
	void GetKnifePoint(float *knifepointx,float *knifepointy,float *knifepointz);

	void SetKnifeLength(float length);
	void GetKnifeLength(float *length);

	void SetKnifeColor(float red, float green, float blue);
	void GetKnifeColor(float *red, float *green,float *blue);

	void SetKnifeMoveStep(float step);
	void GetKnifeMoveStep(float *step);

	void KnifeForward();
	void KnifeBackward();

	int  LoadKnifeFile(const char *name);

protected:
	void DrawKnife();
	void DrawSolidCylinder(float radius, float height);
	void DrawSolidCone(float radius, float height);
    void DrawSphere(float radius, int numMajor, int numMinor);

protected:
	VECTOR m_vDirection; //手术刀的指向，归一化的
	VECTOR m_vKnifePoint; //手术刀刀尖位置

	float m_fMoveStep;  //手术刀移动步长
	float m_fKnifeLength; //手术刀长短
	VECTOR m_vColor; //手术刀颜色

	VECTOR m_vOrginalDir; //手术刀原始方向
	VECTOR m_vOrginalPoint; //手术刀原始刀尖位置
	float  m_fOrginalLength; //手术刀原始长度

};
