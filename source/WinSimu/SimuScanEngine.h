// SimuScanEngine.h: interface for the CSimuScanEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUSCANENGINE_H__BD9E14BA_9330_4E01_903B_8628AFFD67AE__INCLUDED_)
#define AFX_SIMUSCANENGINE_H__BD9E14BA_9330_4E01_903B_8628AFFD67AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EnmScanEngine.h"

class CSimuScanEngine : public CEnmScanEngine  
{
public:
	CSimuScanEngine();
	virtual BOOLEAN StartScan( CEnmScanContext	*pScanContext, BOOLEAN bForceExec = FALSE );

protected:
	enProcState ScanProc();

protected:
	void		FillSimuData( tagScanData& rData );
	
protected:
	int   mCnt;
};

#endif // !defined(AFX_SIMUSCANENGINE_H__BD9E14BA_9330_4E01_903B_8628AFFD67AE__INCLUDED_)
