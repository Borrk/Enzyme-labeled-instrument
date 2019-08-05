#ifndef  __EnmPgEtQualityPage_H
#define  __EnmPgEtQualityPage_H


#include "EnmBasePage.hpp"
#include "EnmPgEtSampleList.h"

class CEnmPgEtQualityPage : public CEnmBasePage  
{
	typedef CEnmBasePage  inherited;

public:
	CEnmPgEtQualityPage();

	BOOLEAN		OnForward();
	BOOLEAN		OnExit();
	
protected:
	void  CreateControl();
	void  CreateStdListControl();
	void  SetStdList();

protected:
	void		FocusOn();	

	void		OnKeyOK();
	void		OnKeyUp();	
	void		OnKeyDown();	

protected:
	void		SyncProtocol2UI();
	
protected:
	MigEditBox		*mpBlankBox;
	MigEditBox		*mpStdBox;	
	MigEditBox		*mpStdReptBox;	
	MigEditBox		*mpSampleReptBox;	

	MigUpDn			*mpUnit;
	CEnmSampleList  *mStdList;
};

#endif ///< __EnmPgEtQualityPage_H
