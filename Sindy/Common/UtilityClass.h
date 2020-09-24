#pragma once




namespace Sindy
{
	class SINDY_API RunTime
	{
		DWORD m_dwTimeBegin;
		DWORD m_dwTimeEnd;

		std::vector<std::pair<CString, DWORD>> m_vecLog2Time;

		void _AddRunTime(const CString& strCatalog, DWORD dwDiff);

	public:
		RunTime();

		// 获取上一个Catalog到当前Catalog流逝的时间
		DWORD GetRunTime(const CString& strCatalog = CString());

		// 获取某个Catalog到当前流逝的时间
		DWORD GetTime(const CString& strBeginCatalog, const CString& strEndCatalog = CString());
		DWORD GetAllTime(const CString& strCatalog = _T("_RunTime_"));

		void Reset();

		void Show(const CString& strInput = CString());

		void Write(const CString& strFileFullPath, const CString& strTitle = CString());
	};

} // namespace Sindy
