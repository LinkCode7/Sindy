#include "stdafx.h"
#include "UtilityClass.h"



namespace Sindy
{

	RunTime::RunTime() :
	m_dwTimeBegin(::GetTickCount()),
	m_dwTimeEnd(0)
	{}

	DWORD RunTime::GetRunTime(const CString& strCatalog)
	{
		DWORD dwCurTime = ::GetTickCount();

		DWORD dwDiff = 0;
		if (m_dwTimeEnd == 0)
		{
			dwDiff = dwCurTime - m_dwTimeBegin;
		}
		else
		{
			dwDiff = dwCurTime - m_dwTimeEnd;
		}

		m_dwTimeEnd = dwCurTime;

		_AddRunTime(strCatalog, dwDiff);

		return dwDiff;
	}

	void RunTime::Reset()
	{
		m_dwTimeBegin = ::GetTickCount();
		m_dwTimeEnd = 0;
		m_vecLog2Time.clear();
	}

	void RunTime::_AddRunTime(const CString& strCatalog, DWORD dwDiff)
	{
		if (strCatalog.IsEmpty())
			return;

		std::vector<std::pair<CString, DWORD>>::iterator iter = m_vecLog2Time.begin();
		for (; iter != m_vecLog2Time.end(); ++iter)
		{
			if (iter->first == strCatalog)
			{
				iter->second = iter->second + dwDiff;
				return;
			}
		}

		m_vecLog2Time.push_back(std::make_pair(strCatalog, dwDiff));
	}

	DWORD RunTime::GetTime(const CString& strBeginCatalog, const CString& strEndCatalog)
	{
		DWORD dwCurTime = ::GetTickCount();

		DWORD dwDiff = 0;
		if (!strBeginCatalog.IsEmpty())
		{
			std::vector<std::pair<CString, DWORD>>::iterator iter = m_vecLog2Time.begin();
			for (; iter != m_vecLog2Time.end(); ++iter)
			{
				if (iter->first == strBeginCatalog)
				{
					dwDiff = dwCurTime - iter->second;
					break;
				}
			}
		}
		else
		{
			dwDiff = dwCurTime - m_dwTimeBegin;
		}

		m_dwTimeEnd = dwCurTime;

		_AddRunTime(strEndCatalog, dwDiff);

		return dwDiff;
	}

	DWORD RunTime::GetAllTime(const CString& strCatalog)
	{
		m_dwTimeEnd = ::GetTickCount();

		DWORD dwDiff = m_dwTimeEnd - m_dwTimeBegin;

		_AddRunTime(strCatalog, dwDiff);

		return dwDiff;
	}

	void RunTime::Show(const CString& strInput)
	{
		if (m_dwTimeEnd == 0)
			m_dwTimeEnd = ::GetTickCount();

		CString strMsg(strInput);
		strMsg += _T("\n");

		if (m_vecLog2Time.empty())
		{
			strMsg.AppendFormat(_T("%ldms"), m_dwTimeEnd - m_dwTimeBegin);
		}
		else
		{
			std::vector<std::pair<CString, DWORD>>::iterator iter = m_vecLog2Time.begin();
			for (; iter != m_vecLog2Time.end(); ++iter)
			{
				strMsg.AppendFormat(_T("%s: %ld\n"), iter->first, iter->second);
			}
		}

		MessageBox(NULL, strMsg, CString(), MB_OK);
	}

	void RunTime::Write(const CString& strFileFullPath, const CString& strTitle)
	{
		m_dwTimeEnd = ::GetTickCount();

		using namespace std;
		// 将全局locale设为本地语言
		locale& loc = locale::global(locale(locale(), "", LC_CTYPE));

		std::wofstream ofs(strFileFullPath, std::ios::app);

		// 将全局locale恢复
		locale::global(loc);


		ofs << _T("\n");
		if (strTitle.IsEmpty())
			ofs << _T("总计：") << m_dwTimeEnd - m_dwTimeBegin << _T("ms") << _T("\n");
		else
			ofs << PmTString(strTitle) << _T(" 总计：") << m_dwTimeEnd - m_dwTimeBegin << _T("ms") << _T("\n"); // 直接输出CString是地址

		std::vector<std::pair<CString, DWORD>>::iterator iter = m_vecLog2Time.begin();
		for (; iter != m_vecLog2Time.end(); ++iter)
		{
			ofs << PmTString(iter->first) << _T("：") << iter->second << _T("ms") << _T("\n");
		}
	}

} // namespace Sindy