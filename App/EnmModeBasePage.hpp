#ifndef  __ENM_MODE_BASE_H
#define  __ENM_MODE_BASE_H

#include "EnmBasePage.hpp"
#include "MIG_EditBox.Hpp"


class  CEnmApp;

class CEnmModeBasePage : public CEnmBasePage
{
	typedef CEnmBasePage  inherited;

public:
	enum{ MODE_CTRL_GO = 0x10001, MODE_CTRL_STOP, MODE_CTRL_HCOPY };

public:
	CEnmModeBasePage(  MCHAR* pTitle, U32 id );
	virtual ~CEnmModeBasePage();

public:
// 	BOOLEAN			IsShowFuncKey() const;
//	void			ShowFuncKey( BOOLEAN bShow );
	BOOLEAN			SetScanProtocol( U16 idx );
	void			SaveScanProtocol();
	BOOLEAN			IsCanActive() const;
	BOOLEAN			IsCanForward();

protected:
	virtual void	Draw(void);
	void			DrawModeInfo(void);
// 	void			DrawFuncKey(void);
	
	void			OnKeyUp( );
	void			OnKeyDown();

	void			FocusOn();

	virtual  void	SyncUI();
	virtual  void   SyncWfs();

protected:
	void			Init();
	void			CreateControl();
	void			UpdateModeInfoRect(  U8 from, U8 to );

	virtual U16		FormatModeInfo( MCHAR *pszInfo ); ///< seperated by '\0', the last one is two '\0'.
	virtual void	FillProtoNo();
	
/// Internal API
protected:
	static MCHAR*	nGetMeasureModeStr( U8 mode );

//////////////////////////////////////////////////////////////////////////
///  member variables
//////////////////////////////////////////////////////////////////////////
protected:
	U16			mTopMargin;
	U16			mLineStep;
//	BOOLEAN		mbShowFuncKey;

	MigUpDn		*mpProtoNo;
	MigControl  *mpProtoName;
	
	CEnmApp		&mApp;
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//inline  BOOLEAN CEnmModeBasePage::IsShowFuncKey() const
//{
//	return mbShowFuncKey;
//}

#endif ///< __ENM_MODE_BASE_H
