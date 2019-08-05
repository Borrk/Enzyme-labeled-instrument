// EnmPgEtCurvePage.h: interface for the CEnmPgEtCurvePage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef  __EnmPgEtCurvePage_H
#define  __EnmPgEtCurvePage_H


#include "EnmBasePage.hpp"

class CEnmPgEtCurvePage : public CEnmBasePage  
{
	typedef  CEnmBasePage  inherited;

public:
	CEnmPgEtCurvePage();

	BOOLEAN		OnForward();
	BOOLEAN		OnExit();

protected:
	void		CreateControl();
	void		FocusOn();	

protected:
	void		SyncProtocol2UI();
	
	
protected:
	MigUpDn			*mpCurve;
	MigUpDn			*mpAxisX, *mpAxisY;

};

#endif ///< __EnmPgEtCurvePage_H
