#pragma once

namespace fsutility
{
	class AFX_EXT_CLASS CPathStringFunction
	{
	public:
		CPathStringFunction(void);
		~CPathStringFunction(void);

		static CString TrimFileNameFromFullPath(CString strFullPath);

		static BOOL    CheckAndCreatePath(CString strDirectory);

		static BOOL    DeleteDirectory(CString strDir);
	};

}
