#pragma once
namespace fsutility
{
	class AFX_EXT_CLASS CMySqlService
	{
	public:
		CMySqlService(void);
		~CMySqlService(void);
		static BOOL StartMySqlService(CString strMysqlName=_T("MySQL"));
	};

}