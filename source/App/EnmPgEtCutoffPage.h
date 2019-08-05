// EnmPgEtCutoffPage.h: interface for the CEnmPgEtCutoffPage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __EnmPgEtCutoffPage_H
#define __EnmPgEtCutoffPage_H

#include "EnmBasePage.hpp"

class CEnmPgEtCutoffPage : public CEnmBasePage  
{
	typedef  CEnmBasePage  inherited;

public:
	CEnmPgEtCutoffPage();

	BOOLEAN		OnForward();
	BOOLEAN		OnExit();

protected:
	void		CreateControl();
	void		FocusOn();	
	void		OnKeyOK();

protected:
	void		SyncProtocol2UI();
	void		SelectFormat( INT8U type, BOOLEAN bSel );
	
protected:
	MigEditBox		*mpCutoff;
	MigEditBox		*mpGrayP;	
	MigEditBox		*mpGrayN;	
	MigRadioBox		*mpFormat1, *mpFormat2, *mpFormat3;

};

#endif ///< __EnmPgEtCutoffPage_H
