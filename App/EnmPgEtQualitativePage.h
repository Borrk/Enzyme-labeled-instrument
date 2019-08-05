// EnmPgEtQualitativePage.h: interface for the CEnmPgEtQualitativePage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __EnmPgEtQualitativePage_H
#define __EnmPgEtQualitativePage_H

#include "EnmBasePage.hpp"

class CEnmPgEtQualitativePage  : public CEnmBasePage
{
	typedef CEnmBasePage  inherited;
	
public:
	CEnmPgEtQualitativePage();

public:	
	BOOLEAN		OnForward();
	BOOLEAN		OnExit();

protected:
	void		CreateControl();

	void		FocusOn();
	
protected:
	void		SyncProtocol2UI();
	
protected:
	MigEditBox		*mpBlank;
	MigEditBox		*mpNC;	
	MigEditBox		*mpCO;	
	MigEditBox		*mpLP;	
	MigEditBox		*mpPC;	
	MigEditBox		*mpHP;	

};

#endif ///< __EnmPgEtQualitativePage_H
