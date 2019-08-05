// ScanContext.h: interface for the CScanContext class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCANCONTEXT_H__81F5003D_BF68_480D_8002_A182A3623C58__INCLUDED_)
#define AFX_SCANCONTEXT_H__81F5003D_BF68_480D_8002_A182A3623C58__INCLUDED_


#include "EnmData.h"
#include "EnmBase.hpp"

enum { HAS_CONC = 1, HAS_PCNC = 2 };
class CEnmProcessedData
{
public:
	CEnmProcessedData() : flag(0), fPc(0), fNc(0){ ResetConc(); }
	void ResetConc()
	{
		flag &= ~HAS_CONC;
		for ( U16 i = 0; i < ENM_DATA_GROUP; i++ )
		{
			for ( U16 j = 0; j < ENM_DATA_COUNT; j++ )
				fConc[i][j] = 0.0;
		}
	}
	void ResetPcNc(){ flag &= ~HAS_PCNC;	fPc = fNc = 0.0; }
	void ResetData() {  ResetConc(); ResetPcNc(); }
	
	BOOLEAN HasPNc() const { return (flag&HAS_PCNC) ? TRUE: FALSE; }
	BOOLEAN Hasconc() const { return (flag&HAS_CONC) ? TRUE: FALSE; }

	BOOLEAN GetPNc( FP64& pc, FP64& nc )
	{
		if ( HasPNc() ) { pc = fPc; nc = fNc; return TRUE; }
		else { return FALSE; }
	}
	
	void SetPNc( FP64 pc, FP64 nc )
	{
		fPc = pc; fNc = nc;
		flag = flag | HAS_PCNC;
	}

	void  SetConcFlag( BOOLEAN bHas = TRUE )
	{
		if ( bHas )
			flag |= HAS_CONC;
		else
			flag &= ~HAS_CONC;
	}

	FP64* GetConc( BOOLEAN bForce = FALSE )
	{
		if ( Hasconc() || bForce ) { return (FP64*)fConc; }
		else { return NULL; }
	}


public:
	U32			flag;
	FP64		fPc;
	FP64		fNc;
	FP64		fConc[ENM_DATA_GROUP][ENM_DATA_COUNT];
};


enum  
{
	CONC_BLANK, CONC_SAMPLE, CONC_PC, CONC_NC, CONC_CO, CONC_LP,  CONC_HP, QUALITATIVE_CONC_UNKNOW
};

enum
{
	CONC_STD = CONC_SAMPLE + 1, QUALITY_CONC_UNKNOW
};

enum { SCAN_REPEAT = -1 };

enum 
{
	/// Scan Flags
	SC_SCAN_NORMAL = 0x01, SC_SCAN_FULL = 0x02, SC_SCAN_CM = 0x04, SC_SCAN_KINETIC = 0x08,
	SC_SCAN_MAINTENANCE = 0x10,

	/// View/Edit Flags
	SC_PROT_EDIT = 0x1000, SC_PROT_NEW = 0x2000, 
};

class CEnmScanContext  
{
	enum { SLAVE_MAX = 2 };

public:
	CEnmScanContext();
	virtual ~CEnmScanContext();

public:
	BOOLEAN			IsInUse() const ;
	BOOLEAN			Use( BOOLEAN bForce = FALSE );
	BOOLEAN			Release( BOOLEAN bForce = TRUE );

	BOOLEAN			HasScanData() const;
	BOOLEAN			IsConcCalculated();
	BOOLEAN			IsPNcCalculated();
	void			ClearScanData();
	tagScanData	    *GetScanData( BOOLEAN bForce = FALSE );
	void			SetScanData( tagScanData& data );
	
	tagScanData*	GetSlaveScanData( U16 idx );
	BOOLEAN			SetSlaveScanData( tagScanData& rData, U16 idx );
	U16				GetSlaveDataMax() const;

	S16				GetRefProtID() const;
	void			SetRefProtID( S16 idx );
	U16				GetSampleCount( BOOLEAN bIncludeCtrl = TRUE );
	S16				GetConcStartPos( U32 type );
	U16				GetConcCount( U32 type );
	U16				GetScanColumns( BOOLEAN bForce = FALSE );

	void			SetScanPeriod( U16 sec );
	U16				GetScanPeriod();
	void			SetScanCount( S16 cnt );
	S16				GetScanCount();

	U16				Reset();

	U32				GetFlag() const;
	void			SetFlag( U32 flag );

protected:
	void			Init();

public:
	tagProtocol		mProtocol;
	CEnmProcessedData mCalcData;
	
protected:
	S16				mRefProtID;
	BOOLEAN			mbInUse;
	BOOLEAN			mbHasNewData;
	tagScanData	    mData;
	tagScanData		mSlaveData[SLAVE_MAX];
	U16				mPeriod;
	S16				mRepeatCnt;

protected:
	U32				mFlag;
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
inline S16  CEnmScanContext::GetRefProtID() const
{
	return mRefProtID;
}

inline void	CEnmScanContext::SetRefProtID( S16 idx )
{
	mRefProtID = idx;
}

inline void	CEnmScanContext::SetScanPeriod( U16 sec )
{
	mPeriod = sec;
}

inline U16	CEnmScanContext::GetScanPeriod( )
{
	return mPeriod;
}

inline void	CEnmScanContext::SetScanCount( S16 cnt )
{
	mRepeatCnt = cnt;
}

inline S16	CEnmScanContext::GetScanCount( )
{
	return mRepeatCnt;
}

inline BOOLEAN	CEnmScanContext::IsInUse() const
{
	return mbInUse;
}

inline BOOLEAN CEnmScanContext::IsConcCalculated()
{
	return mCalcData.Hasconc();
}

inline BOOLEAN CEnmScanContext::IsPNcCalculated()
{
	return mCalcData.HasPNc();
}

inline BOOLEAN	CEnmScanContext::Use( BOOLEAN bForce )
{
	if ( IsInUse() && !bForce )
		return FALSE;
	else
	{
		mbInUse = TRUE;
		return  mbInUse;
	}
}

inline BOOLEAN	CEnmScanContext::Release( BOOLEAN bForce )
{
	if ( IsInUse() && bForce )
	    mbInUse = FALSE;
		
	return mbInUse;
}

inline  tagScanData*  CEnmScanContext::GetScanData( BOOLEAN bForce )
{

	return bForce ? &mData : (HasScanData() ? &mData : NULL);
}

inline  void  CEnmScanContext::ClearScanData()
{
	mbHasNewData = FALSE;
}

inline BOOLEAN	CEnmScanContext::HasScanData() const
{
	return mbHasNewData;
}

inline U16	CEnmScanContext::Reset()
{
	if ( !IsInUse() )
	{
		Init();
		return 1;
	}
	else
		return 0;
}

inline U32	CEnmScanContext::GetFlag() const
{
	return mFlag;
}

inline void	CEnmScanContext::SetFlag( U32 flag )
{
	mFlag = flag;
}

inline tagScanData*	CEnmScanContext::GetSlaveScanData( U16 idx )
{
	if ( idx < SLAVE_MAX )
		return &mSlaveData[idx];
	else
		return NULL;
}

inline BOOLEAN	 CEnmScanContext::SetSlaveScanData( tagScanData& rData, U16 idx )
{
	if ( idx < SLAVE_MAX )
	{
		mSlaveData[idx] = rData;
		return TRUE;
	}
	else
		return FALSE;
}

inline U16	CEnmScanContext::GetSlaveDataMax() const
{
	return SLAVE_MAX;
}

#endif // !defined(AFX_SCANCONTEXT_H__81F5003D_BF68_480D_8002_A182A3623C58__INCLUDED_)
