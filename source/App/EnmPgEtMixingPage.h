// EnmPgEtMixingPage.h: interface for the CEnmPgEtMixingPage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef  __EnmPgEtMixingPage_H
#define  __EnmPgEtMixingPage_H


#include "EnmBasePage.hpp"

class CEnmPgEtMixingPage : public CEnmBasePage  
{
	typedef CEnmBasePage  inherited;

	enum { CTRL_TEXT_K = 1, CTRL_TEXT_MIXTIME, CTRL_TEXT_MIXMODE, CTRL_TEXT_TEMP };
public:
	CEnmPgEtMixingPage();

public:	
	BOOLEAN		OnForward();
	BOOLEAN		OnExit();

protected:
	void		CreateControl();
	void		FocusOn();
	void		OnKeyOK();
	
protected:
	void		SyncProtocol2UI();
	void		SyncTempratureState( BOOLEAN bEnable );
	
protected:	
	MigEditBox		*mpKfactorBox;	
	MigEditBox		*mpMixTimeBox;
	MigEditBox		*mpTempratureBox;	

	MigRadioBox		*mpChkTemp;
	
	MigUpDn			*mpMixMode;
};

#endif ///< __EnmPgEtMixingPage_H
