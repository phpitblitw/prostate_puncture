#pragma once
#include "ComUtility/singleton.h"
#include "ComUtility/IniFile.h"
#include <map>

class AFX_EXT_CLASS FYErrorCodeTextInfo: public fsutility::Singleton<FYErrorCodeTextInfo> 
{
	friend class fsutility::Singleton<FYErrorCodeTextInfo>;
private:
	FYErrorCodeTextInfo(void);
	virtual ~FYErrorCodeTextInfo(void);

public:
	BOOL    LoadErrorCodeFile(CString strFileName);
	CString GetCodeErrorInfo(DWORD nErrorCode);

protected:
	fsutility::CIniFile m_ErrorCodeConfig;
};
