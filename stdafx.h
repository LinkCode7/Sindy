#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // 移除对话框中的 MFC 控件支持

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include <afx.h>
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
// Windows 头文件: 
#include <windows.h>













// TODO:  在此处引用程序需要的其他头文件


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <fstream>
#include <iostream>



#ifdef SINDY_EXPORTS
#define SINDY_API __declspec(dllexport)
#else
#define SINDY_API __declspec(dllimport)
#endif







#ifdef  UNICODE                    
#ifndef _PM_T_DEFINED
typedef	std::wstring PMTSTRING, PmTString;
typedef std::wostringstream PMTSTRINGSTREAM, PmTStringStream;
typedef std::wostream PMOSTREAM, PmOStream;
#define _PM_T_DEFINED
#endif
#else
#ifndef _PM_T_DEFINED
typedef	std::string PMTSTRING, PmTString;
typedef std::ostringstream PMTSTRINGSTREAM, PmTStringStream;
typedef std::ostream PMOSTREAM, PmOStream;
#define _PM_T_DEFINED
#endif /* !_TCHAR_DEFINED */
#endif /*UNICODE*/
