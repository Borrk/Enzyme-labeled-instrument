
#ifndef __EnmControlConcPage_H
#define __EnmControlConcPage_H

#include "EnmBasePage.hpp"
#include "MIG_EditBox.Hpp"

class CEnmControlConcPage : public CEnmBasePage  
{
	typedef CEnmBasePage inherited;
public:
	CEnmControlConcPage();
	virtual ~CEnmControlConcPage();

protected:
	void		CreateControls();
	void		FocusOn();
	void		FocusOff();

	void		OnKeyOK();
	void		OnKeyUp( void );
	void		OnKeyDown( void );
	void		OnKeyLeft( void );
	void		OnKeyRight( void );

protected:
	BOOLEAN	UpdateControlConc( BOOLEAN bToUI );
	BOOLEAN	Sync2UIConc();
	BOOLEAN	Sync2DataConc();

//////////////////////////////////////////////////////////////////////////
protected:
	MigGrid		*mpCtrlPc, *mpCtrlNc;
};

#endif ///< __EnmControlConcPage_H
