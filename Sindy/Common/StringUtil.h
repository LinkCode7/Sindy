#pragma once






namespace Sindy
{
	SINDY_API LPWSTR _A2W(LPCSTR pAInfo);

	SINDY_API LPSTR _W2A(LPCWSTR pWInfo);

	SINDY_API LPWSTR _T2W(LPCTSTR pTInfo);

	SINDY_API LPSTR _T2A(LPCTSTR pTInfo);

	SINDY_API LPTSTR _A2T(LPCSTR pAInfo);

	SINDY_API LPTSTR _W2T(LPCWSTR pWInfo);



	inline double GetAbs(double dValue)
	{
		if (dValue > 0)
			return dValue;
		else
			return -dValue;
	}

	// 比较两个浮点数：0表示相同 1表示前面大 -1表示后面大
	SINDY_API int CompareDbl(double src1, double src2, double dTol=0.000001);

	SINDY_API CString GetDesktopPath();
	SINDY_API CString GetCurPath();
	SINDY_API CString GetCurFilePath(const CString& strFileName);

} // namespace Sindy