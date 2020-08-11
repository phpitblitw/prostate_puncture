
#pragma once

#include "Scene3DMessageWnd.h"


// Scene3DWnd, 多个窗口共存的问题已经解决
class AFX_EXT_CLASS Scene3DWnd : public Scene3DMessageWnd
{
	DECLARE_DYNAMIC(Scene3DWnd)

public:
	Scene3DWnd();
	virtual ~Scene3DWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void   SetLBTInteractionMode(InteractionMode mode); //09-7-17，设置交互模式
	void   SetRBTInteractionMode(InteractionMode mode); //09-7-17，设置交互模式
	void   SetMBTInteractionMode(InteractionMode mode); //09-7-17，设置交互模式


protected:	
	InteractionMode m_eLBTInterationMode; //左键交互模式
	InteractionMode m_eRBTInterationMode; //右键交互模式
	InteractionMode m_eMBTInterationMode; //中间键交互模式

	BOOL m_bLBtDown;
	BOOL m_bRBtDown;
	BOOL m_bMBtDown;

};


