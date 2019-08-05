
#ifndef __EnmFmCmPage_H
#define __EnmFmCmPage_H

#include "EnmFmModePage.h"
#include "MIG_EditBox.Hpp"


class CEnmFmCmModePage : public CEnmFmModePage  
{
	typedef CEnmFmModePage  inherited;

public:
	CEnmFmCmModePage( );

protected:
	void		FocusOff();

protected:
	void		CreateControls();

	BOOLEAN		GetCfgFromUI( );

};

class CEnmFmKnModePage : public CEnmFmModePage  
{
	typedef CEnmFmModePage  inherited;
public:
	CEnmFmKnModePage( );

protected:
	void		FocusOff();
	void		OnKeyOK();

protected:
	void		CreateControls();

	BOOLEAN		GetCfgFromUI( );
};

class CEnmFmKnMeasPage : public CEnmBasePage  
{
	typedef CEnmBasePage  inherited;
public:
	CEnmFmKnMeasPage();

protected:
	void		Draw();
	void		DrawHints();
	void		StartupScan( BOOLEAN bFirst );
	void		FocusOn();
	void		FocusOff();
	void		OnUserMsg( const tagMigMSG& stMsg );
	BOOLEAN		OnExit();

	BOOLEAN		GetCfgFromUI( );

protected:
	U32			mTotalTime;
};


#endif ///< __EnmFmCmPage_H
