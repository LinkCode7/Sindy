#pragma once

#include "..\RangeBound\RangeBound.h"



namespace TestPerformance
{
	SINDY_API void TestRangeBound(const CString& strDbPath, const CString& strLogPath);


	const _TCHAR table_name[] = _T("Line");

	const _TCHAR handle[] = _T("handle");

	const _TCHAR bulge[] = _T("bulge");

	const _TCHAR from_x[] = _T("from_x");
	const _TCHAR from_y[] = _T("from_y");
	const _TCHAR to_x[] = _T("to_x");
	const _TCHAR to_y[] = _T("to_y");

	const _TCHAR min_x[] = _T("min_x");
	const _TCHAR min_y[] = _T("min_y");
	const _TCHAR max_x[] = _T("max_x");
	const _TCHAR max_y[] = _T("max_y");



	class TestLineData : public Sindy::IBoundItem
	{
	public:
		AcGePoint3d m_ptBegin;
		AcGePoint3d m_ptEnd;
		double m_dBulge;

		AcDbExtents m_extents;

		bool GetExtents(double& dMinX, double& dMinY, double& dMaxX, double& dMaxY)override
		{
			AcGePoint3d ptMin = m_extents.minPoint();
			dMinX = ptMin.x;
			dMinY = ptMin.y;

			AcGePoint3d ptMax = m_extents.maxPoint();
			dMaxX = ptMax.x;
			dMaxY = ptMax.y;
			return true;
		}
	};



	void UnSerializePoints(const CString& strDbPath, std::vector<TestLineData*>& vecLineData);

	bool _IsSamePt(const AcGePoint3d& ptSrc, const AcGePoint3d& ptDes, double dTol = 1.0);

} // TestPerformance