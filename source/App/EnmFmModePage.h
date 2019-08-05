// EnmFmModePage.h: interface for the CEnmFmModePage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMFMMODEPAGE_H__7E8D60F0_F7C9_4128_9971_7079BCEB0000__INCLUDED_)
#define AFX_ENMFMMODEPAGE_H__7E8D60F0_F7C9_4128_9971_7079BCEB0000__INCLUDED_

#include <math.h>
#include "EnmBasePage.hpp"
#include "EnmDef.h"
#include "EnmSysCfgLib.h"

class CEnmFmModePage : public CEnmBasePage  
{
public:
	CEnmFmModePage( U32 id );
	virtual ~CEnmFmModePage();

protected:
	virtual void CreateControls() = 0;

protected:
	void		FocusOn();

protected:
	MigUpDn*	mpMeasFilterCtrl;
	MigUpDn*	mpRefFilterCtrl;
	MigUpDn*	mpMixModeCtrl;

	MigEditBox* mpColumnCtrl, *mpMixTimeCtrl, *mpPeriodCtrl;
};


#endif // !defined(AFX_ENMFMMODEPAGE_H__7E8D60F0_F7C9_4128_9971_7079BCEB0000__INCLUDED_)
