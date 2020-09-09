#pragma once

namespace fsutility
{
	class AFX_EXT_CLASS CGlobalWndFunction
	{
	public:
		CGlobalWndFunction(void);
		virtual ~CGlobalWndFunction(void);

		//确保窗口处于最前面
		static void SetForegroundWindowInternal(HWND hWnd);

		static void SetForegroundWindowInternal2(HWND hWnd);


		static void SetForegroundWindowInternal3(HWND hWnd);

	};

}