// EnmFastModePage.h: interface for the CEnmFastModePage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMFASTMODEPAGE_H__D6161A3B_CF6F_4435_8C95_EC218445820C__INCLUDED_)
#define AFX_ENMFASTMODEPAGE_H__D6161A3B_CF6F_4435_8C95_EC218445820C__INCLUDED_

#include "EnmBasePage.hpp"

class CEnmFastModePage : public CEnmBasePage  
{
	enum 
	{
		ENM_FAST_START = 0, ENM_FAST_WHOLE = ENM_FAST_START, 
		ENM_FAST_COLUMN, ENM_FAST_MULTIFILTER,  ENM_FAST_KINETIC, 
		ENM_FAST_END
	};

public:
	CEnmFastModePage();
	virtual ~CEnmFastModePage();

protected:
	void CreateControls( void);
	void CreateChildPage(void);

};

#endif // !defined(AFX_ENMFASTMODEPAGE_H__D6161A3B_CF6F_4435_8C95_EC218445820C__INCLUDED_)
