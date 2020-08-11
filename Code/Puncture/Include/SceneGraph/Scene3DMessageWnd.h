#pragma once
#include "Scene3DBaseWnd.h"
#include "MessageDef.h"

// Scene3DMessageWnd

class AFX_EXT_CLASS Scene3DMessageWnd : public Scene3DBaseWnd
{
	DECLARE_DYNAMIC(Scene3DMessageWnd)

public:
	Scene3DMessageWnd();
	virtual ~Scene3DMessageWnd();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	BOOL m_bSelect;
	BOOL m_bRotate;
	BOOL m_bTranslate;

public:
	afx_msg LRESULT OnSelectBegin(WPARAM w, LPARAM l); //w 代表CPoint的指针
	afx_msg LRESULT OnSelectEnd(WPARAM w, LPARAM l);

	afx_msg LRESULT OnNormalRotateBegin(WPARAM w, LPARAM l); //w 代表CPoint的指针
	afx_msg LRESULT OnNormalRotateProcess(WPARAM w, LPARAM l); //w 代表CPoint的指针
	afx_msg LRESULT OnNormalRotateEnd(WPARAM w, LPARAM l); //w 代表CPoint的指针

	afx_msg LRESULT OnBoxRotateBegin(WPARAM w, LPARAM l); //w 代表CPoint的指针
	afx_msg LRESULT OnBoxRotateProcess(WPARAM w, LPARAM l); //w 代表CPoint的指针
	afx_msg LRESULT OnBoxRotateEnd(WPARAM w, LPARAM l); //w 代表CPoint的指针

	afx_msg LRESULT OnTranslateBegin(WPARAM w, LPARAM l); //w 代表CPoint的指针, l代表平移步长
	afx_msg LRESULT OnTranslateProcess(WPARAM w, LPARAM l); //w 代表CPoint的指针
	afx_msg LRESULT OnTranslateEnd(WPARAM w, LPARAM l); //w 代表CPoint的指针

	afx_msg LRESULT OnScaleProcess(WPARAM w, LPARAM l); //w 代表zDelta, l代表CPoint的指针
};


