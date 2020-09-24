#include "stdafx.h"
#include "TestRange.h"

#include ".\RangeBound.h"

#include "..\SQLite\SQLiteUtility.h"

#include "..\Common\UtilityClass.h"
#include "..\Common\template.h"





namespace TestPerformance
{
	void TestRangeBound()
	{
		std::cout << "正在运算..." << std::endl;
		TestDoubleArithmetic();
		system("cls");
	}




	void TestDoubleArithmetic()
	{
		TCHAR tszModule[MAX_PATH + 1] = { 0 };
		::GetCurrentDirectory(MAX_PATH, tszModule);


		// TEST_DATABASE_PATH
		CString strDbPath = tszModule;
		strDbPath += _T("\\_test.db3");

		CString strLogPath = tszModule;
		strLogPath += _T("\\_Test.log");


		std::vector<TestLineData*> vecLineData;
		UnSerializePoints(strDbPath, vecLineData);

		{
			Sindy::RunTime time;

			using namespace PmRangeBound;
			Range2d range;

			std::vector<TestLineData*>::iterator theIter = vecLineData.begin();
			for( ; theIter != vecLineData.end(); ++theIter)
			{
				range.SetItems(*theIter, true, 100);
			}

			std::vector<RangeItem*> vecItem;
			range.GetIntersectItems(vecItem);

			std::vector<RangeItem*>::iterator iter = vecItem.begin();
			for( ; iter != vecItem.end(); ++iter)
			{
				RangeItem* pItem = *iter;

				TestLineData* pSrcLineData = static_cast<TestLineData*>(pItem->m_ipItem);

				std::vector<RangeItem*>::iterator it = pItem->m_pItems->m_items.begin();
				for( ; it != pItem->m_pItems->m_items.end(); ++it)
				{
					RangeItem* pIntersectItem = *it;

					TestLineData* pDestLineData = static_cast<TestLineData*>(pIntersectItem->m_ipItem);


					bool isLink = false;
					if(_IsSamePt(pSrcLineData->m_ptBegin, pDestLineData->m_ptBegin))
					{
						isLink = true;
					}
					else if(_IsSamePt(pSrcLineData->m_ptBegin, pDestLineData->m_ptEnd))
					{
						isLink = true;
					}
					else if(_IsSamePt(pSrcLineData->m_ptEnd, pDestLineData->m_ptBegin))
					{
						isLink = true;
					}
					else if(_IsSamePt(pSrcLineData->m_ptEnd, pDestLineData->m_ptEnd))
					{
						isLink = true;
					}
				}
			}

			CString strText;
			strText.Format(_T("VS2005控制台程序-用PmRangeBound对%d个实体求交 "), vecLineData.size());

			// LOG_FILE_PATH
			time.Write(strLogPath, strText);
		}

		Sindy::ReleaseContainer(vecLineData);
	}

	void UnSerializePoints(const CString& strDbPath, std::vector<TestLineData*>& vecLineData)
	{
		Sindy::SQLite database(strDbPath);

		CString strSql;
		strSql.Format( _T("select * from %s"), PmAzTestTable::table_name);

		database.BeginTransaction();
		database.Prepare(strSql);

		while( SQLITE_ROW == database.Step() )
		{
			TestLineData* pLineData = new TestLineData;

			CString strHandle;
			database.GetValueText(PmAzTestTable::handle, strHandle);

			database.GetValueDouble(PmAzTestTable::bulge, pLineData->m_dBulge);

			database.GetValueDouble(PmAzTestTable::from_x, pLineData->m_ptBegin.x);
			database.GetValueDouble(PmAzTestTable::from_y, pLineData->m_ptBegin.y);
			database.GetValueDouble(PmAzTestTable::to_x, pLineData->m_ptEnd.x);
			database.GetValueDouble(PmAzTestTable::to_y, pLineData->m_ptEnd.y);

			double dMinX = 0.0;
			database.GetValueDouble(PmAzTestTable::min_x, dMinX);
			double dMinY = 0.0;
			database.GetValueDouble(PmAzTestTable::min_y, dMinY);
			double dMaxX = 0.0;
			database.GetValueDouble(PmAzTestTable::max_x, dMaxX);
			double dMaxY = 0.0;
			database.GetValueDouble(PmAzTestTable::max_y, dMaxY);

			pLineData->m_extents.ResetPoint(AcGePoint3d(dMinX,dMinY,0.0), AcGePoint3d(dMaxX,dMaxY,0.0));

			vecLineData.push_back(pLineData);
		}
	}

	void _UnSerializePoints(const CString& strDbPath, std::vector<TestLineData*>& vecLineData)
	{
		//CString strSql;
		//strSql.Format( _T("select * from %s"), PmAzTestTable::table_name);


		//PmSQLite database(strDbPath);

		//database.BeginTransaction();
		//database.Prepare(strSql);

		//while( SQLITE_ROW == database.Step() )
		//{
		//	TestLineData* pLineData = new TestLineData;

		//	CString strHandle;
		//	database.GetValueText(PmAzTestTable::handle, strHandle);

		//	database.GetValueDouble(PmAzTestTable::bulge, pLineData->m_dBulge);

		//	database.GetValueDouble(PmAzTestTable::from_x, pLineData->m_ptBegin.x);
		//	database.GetValueDouble(PmAzTestTable::from_y, pLineData->m_ptBegin.y);
		//	database.GetValueDouble(PmAzTestTable::to_x, pLineData->m_ptEnd.x);
		//	database.GetValueDouble(PmAzTestTable::to_y, pLineData->m_ptEnd.y);

		//	double dMinX = 0.0;
		//	database.GetValueDouble(PmAzTestTable::min_x, dMinX);
		//	double dMinY = 0.0;
		//	database.GetValueDouble(PmAzTestTable::min_y, dMinY);
		//	double dMaxX = 0.0;
		//	database.GetValueDouble(PmAzTestTable::max_x, dMaxX);
		//	double dMaxY = 0.0;
		//	database.GetValueDouble(PmAzTestTable::max_y, dMaxY);

		//	pLineData->m_extents.set(AcGePoint3d(dMinX,dMinY,0.0), AcGePoint3d(dMaxX,dMaxY,0.0));

		//	vecLineData.push_back(pLineData);
		//}
	}

	bool _IsSamePt(const AcGePoint3d& ptSrc, const AcGePoint3d& ptDes, double dTol)
	{
		if( fabs(ptSrc.x - ptDes.x) <= dTol && fabs(ptSrc.y - ptDes.y) <= dTol )
			return true;

		return false;
	}

} // TestPerformance