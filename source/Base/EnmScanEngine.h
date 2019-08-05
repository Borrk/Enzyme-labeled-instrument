// EnmScanEngine.h: interface for the CEnmScanEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMSCANENGINE_H__E3F55576_CCCD_4B20_B58E_3738C3178EF3__INCLUDED_)
#define AFX_ENMSCANENGINE_H__E3F55576_CCCD_4B20_B58E_3738C3178EF3__INCLUDED_


#include "EnmProcEngine.h"

class  CEnmScanContext;

class CEnmScanEngine : public IProcEngine  
{
public:
	enum enScanState{ SCAN_OFF, SCAN_ON, SCAN_WARM, SCAN_SHAKE, /*SCAN_MOTOR_ON, SCAN_LIGHT_ON, SCAN_RESET_FILTER, SCAN_RESET_PLATE, */
		                SCAN_SET_FILTER, SCAN_READ_BASENOISE, SCAN_READ_ABSORB, SCAN_READ, SCAN_FW_ONE, SCAN_READONECOLUMN, SCAN_DONE  };
	enum enScanDataType { DATA_MAIN, DATA_SLAVE, DATA_REF };
	
	typedef U16 (*pStepProc)(void);
	class CSeProcMatrix
	{
	public:
		U32			mState;
		U32			mNextState;
		pStepProc	mpProc;

	public:
		CSeProcMatrix()
		{
			mState = mNextState = SCAN_OFF;
			mpProc = NULL;
		}
		
		CSeProcMatrix( U32 state, U32 nextState, pStepProc pProc )
		{
			mState = state;  mNextState = nextState;
			mpProc = pProc;
		}
	};

public:
	CEnmScanEngine();
	virtual ~CEnmScanEngine();

public:
	enProcState IEnterProc( ) ;

	enScanState GetScanState() const;
	virtual BOOLEAN     StartScan( CEnmScanContext	*pScanContext, BOOLEAN bForceExec = FALSE );
	void		StopScan( BOOLEAN bReset = TRUE );
		
protected:
	void		Init();
	virtual     enProcState ScanProc();

	BOOLEAN		StartNewScan( CEnmScanContext *pScanContext );
	void		SetStateHints( );

protected:
	void		PreScan();
	U16			WarmPlate();
	U16 		ShakePlate();
	U16			ReadConc();
	U16 		PostScan();

	U16			LightEnable( BOOLEAN bEnable = TRUE );
	U16			MoterEnable( BOOLEAN bEnable = TRUE );
	U16			ResetFilter();
	U16			ResetPlate();
	U16			ReadBaseNoise();
	U16			ShakePlate( INT8S speed, U16 sec );
	U16			ReadAbsorbency();
	U16			SetFilter( );


protected:
	void		SampleOnce( ENMADC *pData );
	void		SamplingData( ENMADC *pData, U32 uColumnMask );

	void		SetScanDataPool( U16 type );
	void		nShakePlate( INT8S speed, U16 sec );
	
protected:
#ifndef ENM_VER_TARGET
	void		FillSimuData( tagScanData& rData );
#endif

//////////////////////////////////////////////////////////////////////////
///  member variables
//////////////////////////////////////////////////////////////////////////
protected:
	CEnmScanContext		*mpScanContext;
	enScanState			mScanState;
	U16							mDataType;
	tagScanData			*mpCurDataPool;
	S16					mCurPlatePos;
	static CSeProcMatrix		mStateMatrix[];
};

inline  CEnmScanEngine::enScanState CEnmScanEngine::GetScanState() const
{
	return mScanState;
}



#endif // !defined(AFX_ENMSCANENGINE_H__E3F55576_CCCD_4B20_B58E_3738C3178EF3__INCLUDED_)
