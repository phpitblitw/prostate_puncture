#pragma once
namespace fsutility
{
	class AFX_EXT_CLASS CmdProxy
	{
	public:
		CmdProxy(void);
		virtual ~CmdProxy(void);

		//运行cmd命令，并返回输出结果
		static BOOL RunCmd(CString strCmdText, CString &strResult);
	};
}