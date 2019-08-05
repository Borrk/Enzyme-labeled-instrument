
#ifndef __EnmFmWpModePage_H
#define __EnmFmWpModePage_H

#include "EnmFmModePage.h"
#include "MIG_EditBox.Hpp"

class CEnmFmWpModePage : public CEnmFmModePage  
{
	typedef  CEnmFmModePage  inherited;
	
public:
	CEnmFmWpModePage( );

public:
	BOOLEAN  OnForward();
	
protected:
	void		CreateControls();

protected:
	void		FocusOn();
	void		FocusOff();

protected:
	BOOLEAN		GetCfgFromUI( );
};

#endif ///< __EnmFmWpModePage_H
