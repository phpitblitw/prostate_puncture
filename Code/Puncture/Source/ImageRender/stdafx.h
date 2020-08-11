// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once
#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <atlimage.h>
#include<gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#ifdef _DEBUG
#pragma comment(lib, "log4cplusUD.lib")
#else
#pragma comment(lib, "log4cplusU.lib")
#endif

using namespace log4cplus;

//#define DISABLE_DEBUGLOG
#ifndef FS_DEBUGLOG
#ifdef DISABLE_DEBUGLOG
#define FS_DEBUGLOG(logfunction, logevent) {}
#else 
#define FS_DEBUGLOG(logfunction, logevent) {\
	log4cplus::Logger _logger = log4cplus::Logger::getInstance(_T("ImageRender")); \
	logfunction(_logger, logevent);  };
#endif

#define FS_DEBUG(x) FS_DEBUGLOG(LOG4CPLUS_DEBUG, x)
#define FS_TRACE(x) FS_DEBUGLOG(LOG4CPLUS_TRACE, x)
#define FS_ERROR(x) FS_DEBUGLOG(LOG4CPLUS_ERROR, x)
#define FS_FATAL(x) FS_DEBUGLOG(LOG4CPLUS_FATAL, x)

#endif
