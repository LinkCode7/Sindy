#pragma once

#include ".\GeoClass.h"





namespace Sindy
{
#define ZERO 0.000001
#define UNKNOWN_REGION_ID -999999

	// <
	struct SINDY_API DoubleLess: public std::binary_function<double, double, bool>
	{
		inline bool operator()(const double& _Left, const double& _Right) const
		{
			if (fabs(_Left - _Right) < ZERO)
				return false;

			return _Left < _Right;
		}
	};

	typedef bool (*SrcDestFunction)(bool isLeftSrc, bool isRightSrc);

	// ����
	inline bool CompareAll(bool, bool)
	{
		return true;
	}
	// ������һ����Src
	inline bool CompareSrc(bool isLeftSrc, bool isRightSrc)
	{
		if(isLeftSrc || isRightSrc)
			return true;
		return false;
	}
	// Src-Dest��Dest-Src
	inline bool CompareSrcDest(bool isLeftSrc, bool isRightSrc)
	{
		if(isLeftSrc != isRightSrc)
			return true;
		return false;
	}




	class IBoundItem
	{
	public:
		virtual bool GetId(REGIONID& id);
		virtual bool GetExtents(double& dMinX, double& dMinY, double& dMaxX, double& dMaxY);
	};

	// ���class����IBoundItem��ʵ��
	class SINDY_API BoundItem
	{
	public:
		BoundItem(IBoundItem* ipItem);
		virtual ~BoundItem();

		// ����Ƕ�ķ�ʽ����IBoundItem�����������ת��
		IBoundItem* m_ipItem;
		double m_dMinX;
		double m_dMinY;
		double m_dMaxX;
		double m_dMaxY;
	};

	class SINDY_API RangeItem: public BoundItem
	{
	public:
		// �������Ϊ����ͬһRange�����ˡ��յ�˹���ͬһ��Ranges
		class Ranges
		{
		public:
			// ���ﲻ�ͷ�
			std::vector<RangeItem*> m_items;
		};

		RangeItem(IBoundItem* ipItem, Ranges* pRange, bool isMin, bool isSrc);
		~RangeItem();

		bool m_isMin; // ���˱�־
		bool m_isSrc; // Դ �ı�־

		// ��Χ�ڵ�����Bound
		Ranges* m_pItems;

	public:

		// �򻯿ͻ�����
		std::vector<RangeItem*>::iterator Begin(){ return m_pItems->m_items.begin(); }
		std::vector<RangeItem*>::iterator End(){ return m_pItems->m_items.end(); }

		int Size(){ return int(m_pItems->m_items.size()); }
		//std::vector<RangeItem*>::iterator At(int iPos){ return m_pItems->m_items.at(iPos); }
	};


	// ��ȡĳ����Χ�ڵ�ʵ��
	class SINDY_API Range2d
	{
	public:
		~Range2d();

		void Reset();

		// ������߱�֤ItemΨһ��
		bool SetItemMin(IBoundItem* ipItem, double dTol=0.0, bool isNeedCheckRepeat=false);
		// ��ȡĳ����Χ�ڵ�Item
		void GetSameItem(const AcGePoint3d& ptMin, const AcGePoint3d& ptMax, std::set<IBoundItem*>& setRepeat, double dTol=1000)const;
		void GetSameItem(const AcGePoint3d& ptInsert, std::set<IBoundItem*>& setRepeat, double radius, double dTol=1000)const;

		// ���ã��˺����д�����20200220
		void GetSameItems(std::vector<RangeItem*>& vecIntersect);


		///////////////////////////////////////////////////////////////////////////////////////////////


		// ������߱�֤ItemΨһ��
		bool SetItemMax(IBoundItem* ipItem, double dTol=0.0, bool isNeedCheckRepeat=false);
		void GetIntersectItem(const AcGePoint3d& ptMin, const AcGePoint3d& ptMax, std::set<IBoundItem*>& setRepeat, double dTol)const;
		void GetIntersectItem(const AcGePoint3d& ptInsert, std::set<IBoundItem*>& setRepeat, double radius, double dTol=1000)const;



		///////////////////////////////////////////////////////////////////////////////////////////////



		// ֻ���Դʵ����ص�Bound��������߱�֤ItemΨһ��
		bool SetItems(IBoundItem* ipItem, bool isSrc=true, double dTol=0.0, bool isNeedCheckRepeat=false);
		// ��ȡ�ཻ��Item���������ǵ�����������߲�Ҫ�ͷŴ���������
		void GetIntersectItems(std::vector<RangeItem*>& vecIntersect, SrcDestFunction function=CompareSrc);

	private:

		std::multimap<double,BoundItem*,DoubleLess> m_mapDouble2Item;

		std::set<REGIONID> m_setCheck;
	};




	template<typename Array>
	void SetRangeItems(Range2d& range, const Array& arr, bool isSrc, double dTol)
	{
		int iSize = PmAzTemplate::GetSize(arr);
		for(int i = 0; i < iSize; ++i)
		{
			range.SetItems(arr.at(i), isSrc, dTol);
		}
	}

} // namespace Sindy