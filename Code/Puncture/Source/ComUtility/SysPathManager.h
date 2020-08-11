#pragma once
#include "singleton.h"


namespace fsutility
{
	class AFX_EXT_CLASS CSysPathManager: public Singleton<CSysPathManager> 
	{
		 friend class fsutility::Singleton<CSysPathManager>;

	protected:
		CSysPathManager(void);
		virtual ~CSysPathManager(void);

	public:
		CString  GetExePath();
		CString  GetConfigPath();
		CString  GetLogPath();
		CString  GetAffineFilePath();

		CString  GetSVMPath();

	private:
		CString  m_strExePath;  
		CString  m_strConfigPath;
		CString  m_strLogPath;
		CString  m_strAffineFilePath;

		CString  m_strSVMPath;
	};
}

