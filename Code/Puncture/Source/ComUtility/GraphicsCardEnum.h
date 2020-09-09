#pragma once

#include <vector>
#include <setupapi.h> 

namespace fsutility
{
	class AFX_EXT_CLASS CGraphicsCardEnum
	{
	public:
		CGraphicsCardEnum(void);
		~CGraphicsCardEnum(void);

	public:
		static BOOL GetGraphicsCardString(std::vector<CString> &CardList);

	protected:
		static BOOL IsDeviceDisabled(DWORD dwDevID, HDEVINFO hDevInfo);
	};
}
