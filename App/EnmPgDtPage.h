#ifndef __EnmPgDtPage_H
#define __EnmPgDtPage_H

#include "EnmBasePage.hpp"
#include "Mig_EditBox.hpp"


class CEnmPgDtPage : public CEnmBasePage  
{
	typedef CEnmBasePage  inherited;

public:
	CEnmPgDtPage();

	BOOLEAN		OnForward();

protected:
	void		CreateControl();

	void		FocusOn();	
	void		OnKeyUp();
	void		OnKeyDown();

	void		SyncProtName( S16 protId );
	void		ShowDeleteTips( MCHAR *pName, BOOLEAN bPreDelete = TRUE );
	
protected:
	MigUpDn		*mpProgram;
	MigControl  *mpPgName;

};

#endif ///< __EnmPgDtPage_H
