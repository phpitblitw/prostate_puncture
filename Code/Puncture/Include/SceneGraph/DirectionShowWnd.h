#pragma once
#include "scene3dwnd.h"

class AFX_EXT_CLASS DirectionShowWnd : public Scene3DWnd
{
	DECLARE_DYNAMIC(DirectionShowWnd)
public:
	DirectionShowWnd(void);
	virtual ~DirectionShowWnd(void);

	void	SetRotation(float  angle, float  nx, float  ny, float  nz);

protected:
	void	InitRoot();
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

};
