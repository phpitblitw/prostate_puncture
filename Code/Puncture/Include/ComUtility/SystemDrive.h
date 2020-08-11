#pragma once
#include "singleton.h"
#include <vector>

namespace fsutility
{
	class AFX_EXT_CLASS SystemDrive:public Singleton<SystemDrive>
	{
		friend class Singleton<SystemDrive>;

	private:
		SystemDrive(void);
		virtual ~SystemDrive(void);

	public:
		BOOL    IsCDDriver(CString strDriver);
		CString GetCDDriver();
		BOOL    IsCDInDriver(CString strDriver);


	protected:
		std::vector<CString> DriverList;
	};


}