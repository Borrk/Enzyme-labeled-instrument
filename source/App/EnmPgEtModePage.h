// EnmPgEtModePage.h: interface for the CEnmPgEtModePage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __EnmPgEtModePage_H
#define __EnmPgEtModePage_H


#include "EnmBasePage.hpp"
#include "MIG_EditBox.Hpp"


class CEnmPgEtModePage : public CEnmBasePage  
{
	typedef  CEnmBasePage inherited;

public:
	enum  enPgEditMode { MODE_NEW, MODE_EDIT };
public:
	CEnmPgEtModePage();

	void		SetEditMode( enPgEditMode mode = MODE_NEW );
	U32			GetEditMode() const;

	BOOLEAN		OnForward();
	BOOLEAN		OnExit();

protected:
	void		CreateControl();
	void		FocusOn(void);
	void		OnKeyOK(void);
	void		OnKeyUp(void);
	void		OnKeyDown(void);

protected:
	void		SyncProtocol2UI();
	void		InitUI();

protected:
	enPgEditMode	mCurMode;
	
	MigEditBox		*mpNameBox;	

	MigUpDn			*mpProgramID;
	MigUpDn			*mpMainWl;
	MigUpDn			*mpRefWl;
	MigUpDn			*mpSlaveWl;
	MigUpDn			*mpMeasMode;
};


inline 	U32	CEnmPgEtModePage::GetEditMode() const
{
	return mCurMode;
}

#endif ///< __EnmPgEtModePage_H
 