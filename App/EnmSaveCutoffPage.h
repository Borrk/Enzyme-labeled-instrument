
#ifndef __EnmSaveCutoffPage_H
#define __EnmSaveCutoffPage_H

#include "EnmBasePage.hpp"
#include "MIG_EditBox.Hpp"

class CEnmSaveCutoffPage : public CEnmBasePage  
{
	typedef CEnmBasePage inherited;

public:
	CEnmSaveCutoffPage();
	virtual ~CEnmSaveCutoffPage();

protected:
	void		CreateControls();
	void		FocusOn();
	BOOLEAN		OnForward();

protected:
	BOOLEAN		SaveCutoff();
	void		Convert2Int( tagDate& date, tagTime& time, U32& uDate, U32& uTm );

//////////////////////////////////////////////////////////////////////////
protected:
	MigControl	*mpSaveTip;
	MigUpDn		*mpSaveConfirm;
};

#endif ///< __EnmSaveCutoffPage_H
