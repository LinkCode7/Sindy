#pragma once

#include "sqlite3.h"

struct sqlite3;
struct sqlite3_stmt;



namespace Sindy
{
	class SINDY_API SQLite
	{
	public:
		explicit SQLite(const CString& strDbPath = _T(":memory:"));
		explicit SQLite(const SQLite& opened);
		~SQLite();

		int Open(const CString& strSqliteDbPath);
		int Close();

	public:

		// ��ͬһʱ��ʹ�ö�����ݿ��е�һ��
		// ATTACH DATABASE���ʹ�ú����е���佫�ڸ��ӵ����ݿ���ִ��
		int Attach(const CString& strDBPath, const CString& strDBName);
		int Detach(const CString& strDBName);


		// ��SQL������Ϊsqlite3_stmt���ýṹ������˽�Ҫִ�е�SQL�����Ϣ
		int Prepare(const CString& strSql);

		// ִ����䣬SQLITE_DONE��SQLITE_ROW���Ǳ�ʾִ�гɹ���SQLITE_DONE��ʾû�в�ѯ���
		// ÿ��Stepֻ����һ�����ݣ�SQLITE_OK��ʾִ�н������в�ѯ���
		int Step();

		// �ͷ���������SQL���ʹ����֮���ڴ���Prepare�����
		int Finalize();

		// �������������ĳ�ʼ״̬��׼��������ִ�У����SQL������Ҫ�ظ�ʹ�ã�
		int ResetSyntax();


		// ʹ�ó����������ؽ�����ĵ�һ���
		int Execute(const CString& strSql);

	public:

		// �����
		CString GetValueText(int iCol);
		bool GetValueText(const CString& strColName, CString& strColValue);

		bool GetValueDouble(const CString& strColName, double& dblColValue);
		double GetValueDouble(int iCol);

		bool GetValueInt(const CString& strColName, int& iColValue);
		int GetValueInt(int iCol);

		// ���ֶ�
		long LargeFieldSize(int iCol);
		const void* GetValueLargeField(int iCol);
		bool GetValueLargeField(const CString& strColName, const void*& pBuffer);
		bool GetValueLargeField(const CString& strColName, const void*& pBuffer, long& lBufferSize);

		// ��ȡ��¼���ֶθ���,ֻ����������л�α�ſ��Թ���
		int GetFieldCount();
		// ��ȡ�е�ֵ����
		int GetFieldType(const CString& strColName);

		void InitColName2ColIndex();
		CString ColumnName(int iCol);



		// ������
		int BindText(const CString& strColName, const CString& strValue);
		int BindText(int iIndex, const CString& strValue);

		int BindInt(int iIndex, int iValue);
		int BindInt(const CString& strColName, int iValue);
		int BindInt64(int iIndex, __int64 llValue);
		int BindInt64(const CString& strColName, __int64 llValue);

		int BindDouble(int iIndex, double dblValue);
		int BindDouble(const CString& strColName, double dblValue);

		int BindLargeField(int iIndex, const void* pBuffer, int iByte);
		int BindLargeField(const CString& strColName, const void* pBuffer, int iByte);

		int BindParameterIndex(const CString& strColName);

		// ��հ�ֵ
		int ClearBindings();

	public:

		// ɾ��
		int DropTable(const CString& strTableName, const CString& strDbName = _T("main"));
		int DropView(const CString& strTableName, const CString& strDbName = _T("main"));
		int ClearTable(const CString& strTableName, const CString& strDbName = _T("main"));

		// �Ƿ����
		bool IsTableExist(const CString& strTableName, const CString& strDbName = _T("main"));
		bool IsViewExist(const CString& strTableName, const CString& strDbName = _T("main"));
		bool IsFieldExist(const CString& strTableName, const CString& strFieldName, const CString& strDbName = _T("main"));

	public:

		// ������������Ҫ���ִ��SQL���ʱ���ȿ���������������ܴ�Ч��
		// SQLiteȱʡΪÿ����������һ������
		void BeginTransaction();

		// �ع�
		void RollBack();

		// �ύ
		void Commit();

		// ��ȡ���һ�δ�����Ϣ
		CString ErrorMessage();

	private:

		sqlite3* m_pDb;
		sqlite3_stmt* m_pStmt;

		bool m_isNeedDeleteDb;
		std::map<CString, int> m_mapFieldName2Index;
	};

} // namespace Sindy
