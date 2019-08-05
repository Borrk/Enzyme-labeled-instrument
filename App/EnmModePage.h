// EnmModePage.h: interface for the CEnmModePage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMMODEPAGE_H__042A68B6_A02D_46A7_BA08_9E7DF5D57296__INCLUDED_)
#define AFX_ENMMODEPAGE_H__042A68B6_A02D_46A7_BA08_9E7DF5D57296__INCLUDED_


#include "EnmModeBasePage.hpp"
#include "Mig_Editbox.hpp"

class CEnmModePage : public CEnmModeBasePage  
{
	enum
	{
		MODE_CTRL_SAMPLE_LABEL, MODE_CTRL_SAMPLE, MODE_CTRL_MIXMODE, 
		MODE_CTRL_NEWCUTOFF_LABEL, MODE_CTRL_CUTOFF
	};

public:
	CEnmModePage( );
	CEnmModePage( MCHAR *pTitle, U32 id );
	virtual ~CEnmModePage();

protected:
	void		CreateSpecialControl();
	void		ShowTips( const tagTipInfo& info );
	void		DrawTips();
	void		SyncUI();
	
	void		FocusOn();

	void		OnKeyOK();


protected:
	void		FillProtoNo();

//////////////////////////////////////////////////////////////////////////
/// member variables
protected:
	MigEditBox		*mpSampleBox;
	MigUpDn			*mpCutoffBox;
	tagTipInfo		mTipInfo;
};

#endif // !defined(AFX_ENMMODEPAGE_H__042A68B6_A02D_46A7_BA08_9E7DF5D57296__INCLUDED_)
