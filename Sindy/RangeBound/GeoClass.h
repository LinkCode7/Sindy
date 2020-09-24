#pragma once



#define DEFAULT_DOUBLE 0.0

#ifdef _WIN64
typedef __int64             SindyLong;
typedef unsigned __int64    SindyULong;
typedef __int64             SindyInt;
typedef unsigned __int64    SindyUInt;
#else
// Using __w64 let's us catch potential errors at compile time
// when /Wp64 is enabled.  Also, we use long, instead of int,
// in the 32-bit build.  That's for compatibility with the Int32
// and UInt32 types.
//
typedef __w64 long          SindyLong;
typedef __w64 unsigned long SindyULong;
//
typedef __w64 int           SindyInt;
typedef __w64 unsigned int  SindyUInt;
#endif



#if PMVCCUR_VER < PMVC2005_VER
typedef long REGIONID;
#else
typedef SindyInt REGIONID;
#endif





class AcGePoint3d
{
public:
	AcGePoint3d()
	{
		x = DEFAULT_DOUBLE;
		y = DEFAULT_DOUBLE;
		z = DEFAULT_DOUBLE;
	}
	AcGePoint3d(const AcGePoint3d& pt)
	{
		x = pt.x;
		y = pt.y;
		z = pt.z;
	}
	AcGePoint3d(double dX, double dY, double dZ)
	{
		x = dX;
		y = dY;
		z = dZ;
	}

	double x, y, z;
};

class AcDbExtents
{
public:
	//AcDbExtents();                            
	//AcDbExtents(const AcDbExtents& src);
	//AcDbExtents(const AcGePoint3d& min, const AcGePoint3d& max);

	AcGePoint3d       minPoint() const { return mMinPoint; }
	AcGePoint3d       maxPoint() const { return mMaxPoint; }
	//Acad::ErrorStatus set(const AcGePoint3d& min, const AcGePoint3d& max);

	//void              addPoint (const AcGePoint3d& pt);
	//void              addExt   (const AcDbExtents& src);
	//Acad::ErrorStatus addBlockExt(AcDbBlockTableRecord* pBTR);

	//void expandBy(const AcGeVector3d& vec);
	//void transformBy(const AcGeMatrix3d& mat);

	void ResetPoint(const AcGePoint3d& min, const AcGePoint3d& max){ mMinPoint = min; mMaxPoint = max; }

private:
	AcGePoint3d    mMinPoint;
	AcGePoint3d    mMaxPoint;
};



class IPmBoundItem
{
public:
	virtual bool GetId(REGIONID& id);
	virtual bool GetExtents(double& dMinX, double& dMinY, double& dMaxX, double& dMaxY);
};
