#include "stdafx.h"
#include "GlobalWndFunction.h"

using namespace  fsutility;

CGlobalWndFunction::CGlobalWndFunction(void)
{
}


CGlobalWndFunction::~CGlobalWndFunction(void)
{
}



void CGlobalWndFunction::SetForegroundWindowInternal(HWND hWnd)
{
	if(!::IsWindow(hWnd)) return;

	BYTE keyState[256] = {0};
	//to unlock SetForegroundWindow we need to imitate Alt pressing
	if(::GetKeyboardState((LPBYTE)&keyState))
	{
		if(!(keyState[VK_MENU] & 0x80))
		{
			::keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
		}
	}

	::SetForegroundWindow(hWnd);

	if(::GetKeyboardState((LPBYTE)&keyState))
	{
		if(!(keyState[VK_MENU] & 0x80))
		{
			::keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
		}
	}
}


void CGlobalWndFunction::SetForegroundWindowInternal2(HWND hWnd)
{
	if(!::IsWindow(hWnd)) return;

	HWND  hCurWnd = nullptr;   
	DWORD dwMyID;   
	DWORD dwCurID;   
	hCurWnd = ::GetForegroundWindow();   
	dwMyID  = ::GetCurrentThreadId();   
	dwCurID = ::GetWindowThreadProcessId(hCurWnd, nullptr);   
	::AttachThreadInput(dwCurID, dwMyID, TRUE);   
	::ShowWindow( hWnd, SW_SHOWNORMAL ); 
	::SetWindowPos( hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE ); 
	::SetWindowPos( hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE ); 
	::SetForegroundWindow(hWnd);   
	::AttachThreadInput(dwCurID, dwMyID, FALSE);
	//SetActiveWindow();

	//relation time of SetForegroundWindow lock
	//DWORD lockTimeOut = 0;
	//HWND  hCurrWnd = ::GetForegroundWindow();
	//DWORD dwThisTID = ::GetCurrentThreadId(),
	//	dwCurrTID = ::GetWindowThreadProcessId(hCurrWnd,0);

	////we need to bypass some limitations from Microsoft :)
	//if(dwThisTID != dwCurrTID)
	//{
	//	::AttachThreadInput(dwThisTID, dwCurrTID, TRUE);

	//	::SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT,0,&lockTimeOut,0);
	//	::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT,0,0,SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);

	//	::AllowSetForegroundWindow(ASFW_ANY);
	//}

	//::SetForegroundWindow(hWnd);

	//if(dwThisTID != dwCurrTID)
	//{
	//	::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT,0,(PVOID)lockTimeOut,SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);
	//	::AttachThreadInput(dwThisTID, dwCurrTID, FALSE);
	//}
}

void CGlobalWndFunction::SetForegroundWindowInternal3(HWND hWnd)
{
	HWND  pWnd1     = GetForegroundWindow();
	HWND  pWnd2     = GetFocus();
	HWND  pDeskWnd  = GetDesktopWindow();
	BOOL  bNeedToActive1=TRUE;
	BOOL  bNeedToActive2=TRUE;

	while(pWnd1!=nullptr && pWnd1!=pDeskWnd)
	{
		if(pWnd1==hWnd)
		{
			bNeedToActive1=FALSE;
			break;
		}
		pWnd1 = GetParent(pWnd1);
	}

	while(pWnd2!=nullptr && pWnd2!=pDeskWnd)
	{
		if(pWnd2==hWnd)
		{
			bNeedToActive2=FALSE;
			break;
		}
		pWnd2 = GetParent(pWnd2);
	}

	if(bNeedToActive1 || bNeedToActive2)
	{
		HWND  hCurWnd = nullptr;   
		DWORD dwMyID;   
		DWORD dwCurID;   
		hCurWnd = ::GetForegroundWindow();   
		dwMyID  = ::GetCurrentThreadId();   
		dwCurID = ::GetWindowThreadProcessId(hCurWnd, nullptr);   
		::AttachThreadInput(dwCurID, dwMyID, TRUE);   
		::SetForegroundWindow(hWnd);   
		::AttachThreadInput(dwCurID, dwMyID, FALSE);
		SetActiveWindow(hWnd);
		SetFocus(hWnd);
	}
}

