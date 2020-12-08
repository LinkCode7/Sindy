#include "stdafx.h"
#include "SQLiteUtility.h"




namespace Sindy
{
	SQLite::SQLite(const CString& strDbPath) :
	m_pDb(NULL),
	m_pStmt(NULL),
	m_pUseCount(new unsigned int(1))
	{
		Open(strDbPath);
	}

	SQLite::SQLite(const SQLite& opened) :
	m_pDb(opened.m_pDb),
	m_pStmt(NULL),
	m_pUseCount(opened.m_pUseCount)
	{
		(*m_pUseCount)++;
	}

	SQLite::~SQLite()
	{
		--(*m_pUseCount);
		if ((*m_pUseCount) == 0)
		{
			Close();
			delete m_pUseCount;
		}
	}

	int SQLite::Open(const CString& strSqliteDbPath)
	{
#ifdef UNICODE
		int rc = sqlite3_open16(strSqliteDbPath, &m_pDb);
#else
		USES_CONVERSION;
		int rc = sqlite3_open16(A2W(strSqliteDbPath), &m_pDb);
#endif

		if (SQLITE_OK != rc)
		{
			Close();
			m_pDb = NULL;
		}
		return rc;
	}

	int SQLite::Close()
	{
		if (!m_pDb)
			return SQLITE_OK;

		// 释放附加在连接上的语句
		sqlite3_stmt* pStmt = sqlite3_next_stmt(m_pDb, NULL);
		sqlite3_stmt* pStmtNext = NULL;
		while (NULL != pStmt)
		{
			pStmtNext = sqlite3_next_stmt(m_pDb, pStmt);
			sqlite3_finalize(pStmt);
			pStmt = pStmtNext;
		}

		// 关闭数据库
		int rc = SQLITE_OK;
		if ((*m_pUseCount) == 0)
			rc = sqlite3_close(m_pDb);
		m_pDb = NULL;
		return SQLITE_OK;
	}

	int SQLite::Attach(const CString& strDBPath, const CString& strDBName)
	{
		// 附加数据库
		CString strSql;
		strSql.Format(_T("ATTACH :DBPath AS \"%s\""), strDBName);

		int rc = Prepare(strSql);
		if (SQLITE_OK != rc)
			return rc;

		BindText(_T(":DBPath"), strDBPath);

		// 执行SQL语句
		rc = Step();

		// 完成SQL语句
		m_mapFieldName2Index.clear();
		Finalize();

		return rc;
	}

	int SQLite::Detach(const CString& strDBName)
	{
		// 分离附加的数据库
		CString strSql;
		strSql.Format(_T("DETACH \"%s\""), strDBName);

		return Execute(strSql);
	}

	int SQLite::Prepare(const CString& strSql)
	{
		if (NULL != m_pStmt)
		{
			m_mapFieldName2Index.clear();
			Finalize();
		}

		// 准备编译SQL语句，这里要用v2版本
#ifdef UNICODE
		int rc = sqlite3_prepare16_v2(m_pDb, strSql, -1, &m_pStmt, NULL);
#else
		USES_CONVERSION;
		int rc = sqlite3_prepare16_v2(m_pDb, A2W(strSql), -1, &m_pStmt, NULL);
#endif

		if (SQLITE_OK != rc)
		{
			m_pStmt = NULL;
		}
		return rc;
	}

	int SQLite::Execute(const CString& strSql)
	{
		// 编译SQL语句
		int rc = Prepare(strSql);
		if (SQLITE_OK != rc)
		{
			// 准备语句失败
			return rc;
		}

		// 执行SQL语句
		rc = Step();

		// 完成SQL语句
		m_mapFieldName2Index.clear();
		Finalize();

		return rc;
	}

	// 执行语句
	int SQLite::Step()
	{
		return sqlite3_step(m_pStmt);
	}

	int SQLite::Finalize()
	{
		int rc = sqlite3_finalize(m_pStmt);
		m_pStmt = NULL;
		return rc;
	}

	// 重置语句对象到它的初始状态，准备被重新执行
	int SQLite::ResetSyntax()
	{
		return sqlite3_reset(m_pStmt);
	}

	void SQLite::InitColName2ColIndex()
	{
		if (!m_mapFieldName2Index.empty())
		{
			return;
		}

		// 保存列名和其对应的索引
		int iCount = sqlite3_column_count(m_pStmt);
		for (int i = 0; i < iCount; i++)
		{
			CString strColName = ColumnName(i);
			m_mapFieldName2Index[strColName] = i;
		}
	}

	CString SQLite::ColumnName(int iCol)
	{
		// 获取列名
		const void* pColumnName = sqlite3_column_name16(m_pStmt, iCol);
		LPCWSTR lpColumnName = static_cast<LPCWSTR>(pColumnName);

#ifdef UNICODE
		CString strColName = lpColumnName;
#else
		USES_CONVERSION;
		CString strColName = W2A(lpColumnName);
#endif
		return strColName;
	}

	bool SQLite::GetValueText(const CString& strColName, CString& strColValue)
	{
		InitColName2ColIndex();

		std::map<CString, int>::iterator it = m_mapFieldName2Index.find(strColName);
		if (m_mapFieldName2Index.end() == it)
		{
			strColValue.Empty();
			return false;
		}

		strColValue = GetValueText(it->second);
		return true;
	}

	CString SQLite::GetValueText(int iCol)
	{
		const void* pColumnName = sqlite3_column_text16(m_pStmt, iCol);
		LPCWSTR lpColumnName = static_cast<LPCWSTR>(pColumnName);

#ifdef UNICODE
		CString strColName = lpColumnName;
#else
		USES_CONVERSION;
		CString strColName = W2A(lpColumnName);
#endif
		return strColName;
	}

	bool SQLite::GetValueDouble(const CString& strColName, double& dblColValue)
	{
		InitColName2ColIndex();

		std::map<CString, int>::iterator it = m_mapFieldName2Index.find(strColName);
		if (m_mapFieldName2Index.end() == it)
		{
			return false;
		}

		dblColValue = GetValueDouble(it->second);
		return true;
	}

	double SQLite::GetValueDouble(int iCol)
	{
		return sqlite3_column_double(m_pStmt, iCol);
	}

	bool SQLite::GetValueInt(const CString& strColName, int& iColValue)
	{
		InitColName2ColIndex();

		std::map<CString, int>::iterator it = m_mapFieldName2Index.find(strColName);
		if (m_mapFieldName2Index.end() == it)
		{
			iColValue = 0;
			return false;
		}

		iColValue = GetValueInt(it->second);
		return true;
	}

	int SQLite::GetValueInt(int iCol)
	{
		return sqlite3_column_int(m_pStmt, iCol);
	}

	const void* SQLite::GetValueLargeField(int iCol)
	{
		// 获取列的值
		return sqlite3_column_blob(m_pStmt, iCol);
	}

	bool SQLite::GetValueLargeField(const CString& strColName, const void*& pBuffer)
	{
		InitColName2ColIndex();

		std::map<CString, int>::iterator it = m_mapFieldName2Index.find(strColName);
		if (m_mapFieldName2Index.end() == it)
		{
			pBuffer = NULL;
			return false;
		}
		pBuffer = GetValueLargeField(it->second);
		return true;
	}

	bool SQLite::GetValueLargeField(const CString& strColName, const void*& pBuffer, long& lBufferSize)
	{
		InitColName2ColIndex();

		std::map<CString, int>::iterator it = m_mapFieldName2Index.find(strColName);
		if (m_mapFieldName2Index.end() == it)
		{
			pBuffer = NULL;
			lBufferSize = 0;
			return false;
		}
		pBuffer = GetValueLargeField(it->second);
		lBufferSize = LargeFieldSize(it->second);

		return true;
	}

	long SQLite::LargeFieldSize(int iCol)
	{
		return sqlite3_column_bytes(m_pStmt, iCol);
	}

	int SQLite::GetFieldType(const CString& strColName)
	{
		InitColName2ColIndex();

		std::map<CString, int>::iterator it = m_mapFieldName2Index.find(strColName);
		if (m_mapFieldName2Index.end() == it)
			return 0;

		return sqlite3_column_type(m_pStmt, it->second);
	}

	int SQLite::GetFieldCount()
	{
		return sqlite3_data_count(m_pStmt);
	}

	int SQLite::BindText(const CString& strColName, const CString& strValue)
	{
		int iIndex = BindParameterIndex(strColName);
		return BindText(iIndex, strValue);
	}

	int SQLite::BindText(int iIndex, const CString& strValue)
	{
#ifdef UNICODE
		return sqlite3_bind_text16(m_pStmt, iIndex, strValue, -1, SQLITE_TRANSIENT);
#else
		USES_CONVERSION;
		return sqlite3_bind_text16(m_pStmt, iIndex, A2W(strValue), -1, SQLITE_TRANSIENT);
#endif
	}

	int SQLite::BindInt(int iIndex, int iValue)
	{
		return	sqlite3_bind_int(m_pStmt, iIndex, iValue);
	}

	int SQLite::BindInt(const CString& strColName, int iValue)
	{
		int iIndex = BindParameterIndex(strColName);
		return BindInt(iIndex, iValue);
	}

	int SQLite::BindInt64(int iIndex, __int64 llValue)
	{
		return	sqlite3_bind_int64(m_pStmt, iIndex, llValue);
	}

	int SQLite::BindInt64(const CString& strColName, __int64 llValue)
	{
		int iIndex = BindParameterIndex(strColName);
		return BindInt64(iIndex, llValue);
	}

	int SQLite::BindDouble(int iIndex, double dblValue)
	{
		return	sqlite3_bind_double(m_pStmt, iIndex, dblValue);
	}

	int SQLite::BindDouble(const CString& strColName, double dblValue)
	{
		int iIndex = BindParameterIndex(strColName);
		return BindDouble(iIndex, dblValue);
	}

	int SQLite::BindLargeField(int iIndex, const void* pBuffer, int iByte)
	{
		return sqlite3_bind_blob(m_pStmt, iIndex, pBuffer, iByte, SQLITE_TRANSIENT);
	}

	int SQLite::BindLargeField(const CString& strColName, const void* pBuffer, int iByte)
	{
		int iIndex = BindParameterIndex(strColName);
		return BindLargeField(iIndex, pBuffer, iByte);
	}

	int SQLite::BindParameterIndex(const CString& strColName)
	{
		// 获取绑定参数对应的索引号
		const char* pstrColName = NULL;
#ifdef UNICODE
		USES_CONVERSION;
		pstrColName = W2A(strColName);
#else
		pstrColName = strColName;
#endif
		return sqlite3_bind_parameter_index(m_pStmt, pstrColName);
	}

	int SQLite::ClearBindings()
	{
		return sqlite3_clear_bindings(m_pStmt);
	}

	void SQLite::BeginTransaction()
	{
		Execute(_T("BEGIN"));
	}

	void SQLite::RollBack()
	{
		Execute(_T("ROLLBACK"));
	}

	void SQLite::Commit()
	{
		Execute(_T("COMMIT"));
	}



	int SQLite::ClearTable(const CString& strTableName, const CString& strDbName)
	{
		CString strSql;
		strSql.Format(_T("DELETE TABLE IF EXISTS \"%s\".\"%s\""), strDbName, strTableName);

		return Execute(strSql);
	}

	int SQLite::DropTable(const CString& strTableName, const CString& strDbName /*= _T("main")*/)
	{
		CString strSql;
		//strSql.Format(_T("DROP TABLE IF EXISTS \"%s\".\"%s\""), strDbName, strTableName);
		strSql.Format(_T("DROP TABLE %s"), strTableName);

		return Execute(strSql);
	}

	int SQLite::DropView(const CString& strTableName, const CString& strDbName /*= _T("main")*/)
	{
		CString strSql;
		strSql.Format(_T("DROP VIEW IF EXISTS \"%s\".\"%s\""), strDbName, strTableName);

		return Execute(strSql);
	}

	bool SQLite::IsTableExist(const CString& strTableName, const CString& strDbName /*= _T("main")*/)
	{
		// 查询sqlite_master表
		CString strSql;
		if (0 == strDbName.CompareNoCase(_T("temp")))
		{
			strSql.Format(_T("SELECT * FROM sqlite_temp_master WHERE type = 'table' AND name = '%s'"), strTableName);
		}
		else
		{
			strSql.Format(_T("SELECT * FROM \"%s\".sqlite_master WHERE type = 'table' AND name = '%s'"), strDbName, strTableName);
		}


		int rc = Prepare(strSql);
		if (SQLITE_OK != rc)
		{
			return FALSE;
		}

		Step();

		// 如果没有记录,表不存在
		rc = GetFieldCount();

		// 释放
		m_mapFieldName2Index.clear();
		Finalize();
		if (0 == rc)
		{
			return false;
		}
		return true;
	}

	bool SQLite::IsViewExist(const CString& strTableName, const CString& strDbName /*= _T("main")*/)
	{
		// 查询sqlite_master表
		CString strSql;
		if (0 == strDbName.CompareNoCase(_T("temp")))
		{
			strSql.Format(_T("SELECT * FROM sqlite_temp_master WHERE type = 'view' AND name = '%s'"), strTableName);
		}
		else
		{
			strSql.Format(_T("SELECT * FROM \"%s\".sqlite_master WHERE type = 'view' AND name = '%s'"), strDbName, strTableName);
		}

		int rc = Prepare(strSql);
		if (SQLITE_OK != rc)
		{
			return false;
		}

		Step();

		// 如果没有记录,表不存在
		rc = GetFieldCount();

		m_mapFieldName2Index.clear();
		Finalize();

		return 0 == rc ? false : true;
	}

	bool SQLite::IsFieldExist(const CString& strTableName, const CString& strFieldName, const CString& strDbName /*= _T("main")*/)
	{
		// 获取表信息
		CString strSql;
		strSql.Format(_T("PRAGMA \"%s\".table_info(\"%s\")"), strDbName, strTableName);

		int rc = Prepare(strSql);
		if (SQLITE_OK != rc)
		{
			return false;
		}

		// 遍历字段
		rc = Step();

		bool isExist = false;
		while (SQLITE_ROW == rc)
		{
			CString strColName;
			GetValueText(_T("name"), strColName);

			if (strColName == strFieldName)
			{
				isExist = true;
				break;
			}
			rc = Step();
		}

		m_mapFieldName2Index.clear();
		Finalize();
		return isExist;
	}

	// 获取最近一次错误信息
	CString SQLite::ErrorMessage()
	{
		const void* pErrMsg = sqlite3_errmsg16(m_pDb);
		LPCWSTR lpErrMsg = static_cast<LPCWSTR>(pErrMsg);

#ifdef UNICODE
		CString strErrMsg = lpErrMsg;
#else
		USES_CONVERSION;
		CString strErrMsg = W2A(lpErrMsg);
#endif

		return strErrMsg;
	}

} // namespace Sindy

