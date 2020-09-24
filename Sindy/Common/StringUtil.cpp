#include "stdafx.h"
#include "StringUtil.h"





namespace Sindy
{
	LPWSTR _A2W(LPCSTR pAInfo)
	{
		int iLength = (int)strlen(pAInfo);
		int iWLength = (iLength + sizeof(CHAR));
		LPWSTR pWInfo = new WCHAR[iWLength];

		ZeroMemory(pWInfo,iWLength * sizeof(WCHAR));
		ATLA2WHELPER(pWInfo, pAInfo, iLength + sizeof(CHAR), CP_ACP);
		return pWInfo;
	}

	LPSTR _W2A(LPCWSTR pWInfo)
	{
		int iLength = lstrlenW(pWInfo);
		int iALength = (iLength + 1) * sizeof(WCHAR);
		LPSTR pAInfo = new CHAR[iALength];

		ZeroMemory(pAInfo,iALength);
		ATLW2AHELPER(pAInfo, pWInfo, iALength, CP_ACP);
		return pAInfo;
	}

	LPWSTR _T2W(LPCTSTR pTInfo)
	{//将可能是Unicode或Acsii字符串 转为Unicode字符串
		LPWSTR pWInfo = NULL;
#ifdef UNICODE
		int iLength = (int)_tcslen(pTInfo);
		int iWLength = (iLength + 1);	/// 加结束符
		pWInfo = new WCHAR[iWLength];	/// 开辟内存

		ZeroMemory(pWInfo,iWLength * sizeof(WCHAR) );	/// 第二个参数是字节的长度
		CopyMemory(pWInfo,pTInfo,(iWLength - 1) * sizeof(WCHAR) ); /// 最后一个参数是字节的长度
#else
		pWInfo = PM_A2W(pTInfo);
#endif
		return pWInfo;
	}

	LPSTR _T2A(LPCTSTR pTInfo)
	{
		LPSTR pAInfo = NULL;
#ifdef UNICODE
		pAInfo = _W2A(pTInfo);
#else
		int iLength = _tcslen(pTInfo);
		int iALength = (iLength + sizeof(CHAR));
		pAInfo = new CHAR[iALength];

		ZeroMemory(pAInfo,iALength);
		CopyMemory(pAInfo,pTInfo,iALength - sizeof(CHAR));

#endif
		return pAInfo;
	}

	LPTSTR _A2T(LPCSTR pAInfo)
	{
		LPTSTR pTInfo = NULL;
#ifdef UNICODE
		pTInfo = _A2W(pAInfo);
#else
		int iAInfoSize = strlen(pAInfo);
		pTInfo = new char[iAInfoSize + 1];

		ZeroMemory(pTInfo,iAInfoSize + 1);
		CopyMemory(pTInfo,pAInfo,iAInfoSize);
#endif
		return pTInfo;
	}

	LPTSTR _W2T(LPCWSTR pWInfo)
	{
		LPTSTR pTInfo = NULL;
#ifdef UNICODE
		int iWLength = (int)_tcslen(pWInfo);
		pTInfo = new WCHAR[iWLength + 1];

		ZeroMemory(pTInfo,(iWLength + 1) * sizeof(WCHAR));
		CopyMemory(pTInfo,pWInfo,iWLength * sizeof(WCHAR));
#else
		pTInfo = PM_W2A(pWInfo);
#endif
		return pTInfo;
	}

	int CompareDbl(double src1, double src2, double dTol)
	{
		double dblSub = src1 - src2;
		// 相同
		if(GetAbs(dblSub) <=  dTol )
			return 0;
		else if(dblSub > dTol )
			return 1;
		else
			return -1;
	}


	CString GetDesktopPath()
	{
		// CSIDL_PERSONAL,CSIDL_DESKTOPDIRECTORY
		TCHAR sFloderPath[1024];
		if (SHGetSpecialFolderPath(NULL, sFloderPath, CSIDL_DESKTOPDIRECTORY, FALSE))
			return sFloderPath;

		return CString();
	}

	CString GetCurPath()
	{
		TCHAR tszModule[MAX_PATH + 1] = { 0 };
		::GetCurrentDirectory(MAX_PATH, tszModule);

		return tszModule;
	}

	CString GetCurFilePath(const CString& strFileName)
	{
		TCHAR tszModule[MAX_PATH + 1] = { 0 };
		::GetCurrentDirectory(MAX_PATH, tszModule);

		CString strLogPath = tszModule;
		strLogPath.AppendFormat(_T("\\%s"), strFileName);

		return strLogPath;
	}

} // namespace Sindy