// EnmProcPanelPage.h: interface for the CEnmProcPanelPage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMPROCPANELPAGE_H__39E0F23E_AA8A_464C_BDD1_4451197CA5B6__INCLUDED_)
#define AFX_ENMPROCPANELPAGE_H__39E0F23E_AA8A_464C_BDD1_4451197CA5B6__INCLUDED_

#include "EnmBasePage.hpp"
#include "EnmBasePage.hpp"

class CEnmProcPanelPage : public CEnmBasePage  
{

	enum 
	{
		ENM_PROCPANEL_START = 0, ENM_PROCPANEL_COMPUTE = ENM_PROCPANEL_START, 
		ENM_PROCPANEL_EDIT, ENM_PROCPANEL_DELETE, ENM_PROCPANEL_SAVE, 
		ENM_PROCPANEL_END
	};

public:
	CEnmProcPanelPage();
	virtual ~CEnmProcPanelPage();

protected:
	void CreateControls( void);
	void CreateChildPage(void);

};

#endif // !defined(AFX_ENMPROCPANELPAGE_H__39E0F23E_AA8A_464C_BDD1_4451197CA5B6__INCLUDED_)
