#ifndef __ENMPROGRAMSCANPAGE_H
#define __ENMPROGRAMSCANPAGE_H

#include "EnmModeBasePage.hpp"

class CEnmProgramScanPage : public CEnmModeBasePage  
{

public:
	CEnmProgramScanPage( );
	CEnmProgramScanPage( MCHAR *pTitle, U32 id );
	virtual ~CEnmProgramScanPage();

	void		DrawTips();
	BOOLEAN		IsCanForward();

protected:
	void		ShowTips( const tagTipInfo& info );
	void		FocusOn();
	void		FocusOff();
	void		OnUserMsg( const tagMigMSG& stMsg );


//////////////////////////////////////////////////////////////////////////
/// member variables
protected:
	tagTipInfo		mTipInfo;
	BOOLEAN			mbInScan;
};


#endif ///< __ENMPROGRAMSCANPAGE_H
