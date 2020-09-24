#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // �Ƴ��Ի����е� MFC �ؼ�֧��

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include <afx.h>
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
// Windows ͷ�ļ�: 
#include <windows.h>













// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�


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
