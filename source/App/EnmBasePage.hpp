#ifndef __ENK_BASE_PAGE_H__
#define __ENK_BASE_PAGE_H__

#include "EnmBase.hpp"
#include "EnmStringTbl.hpp"

#include "EnmScanContext.h"


class  IWorkStep;
	
enum  enStepMode{ STEP_FW, STEP_BW, STEP_RETURN };
enum  enFuncKey{ ENM_FUNCKEY_GO = 0, ENM_FUNCKEY_EXIT, ENM_FUNCKEY_PCOPY };

class CFuncKeyCfg
{
public:
	S32		mTitleID;
	BOOLEAN mbShow;
	BOOLEAN	mbResponse;

	void Set( S32 id, BOOLEAN bShow, BOOLEAN bResponse )
	{
		mTitleID = id; mbShow = bShow; mbResponse = bResponse;
	}
};

/**
 *		EnMarker base page class
 *
 */ 
class CEnmBasePage : public MigPage
{
public:
	CEnmBasePage( MCHAR* pTitle, INT32U id );
	CEnmBasePage(const tagCS& cs ): MigPage( cs ), mpStep(0){ }

public:
	virtual	 void ShowTips( const tagTipInfo&  tipInfo ){}
	IWorkStep  *GetStep() { return mpStep; }
	void        SetStep( IWorkStep* pStep ) { mpStep = pStep; }

	CEnmScanContext& GetScanContext() const;
	BOOLEAN			 IsScanContextUsable() const;
	virtual BOOLEAN  IsCanActive() const { return TRUE; }

/// normal routines
protected:
	virtual void SetParam(void ){ }

	virtual void OnKeyOK( void );
	virtual void OnKeyGO( void );
	void OnNumKey( INT16U key );
	void SetLinkRange( INT32U minID, INT32U maxID );

protected:
	virtual  void DrawTips(){}
	virtual  void	Draw();

/// member variables
protected:
	INT32U		mMinID, mMaxID;
	INT32U		mLeftMargin, mTopMargin;

	IWorkStep   *mpStep;

	/// for operation tips
	enum { STEP_TIP_MAX = 4};
	//enum { STEP_GO_NEXT, STEP_EXIT_EXIT, STEP_P_HCOPY, STEP_P_PRINT, };
	enum { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT };
	U8			mStepTips[STEP_TIP_MAX];
	U32			mStepTipMode;

	enStepMode	mForwardMode;
	enStepMode	mEscMode;

/// Step routines
public:
	virtual BOOLEAN  IsCanForward();
	virtual BOOLEAN  IsCanExit();
	virtual BOOLEAN  OnForward();
	virtual BOOLEAN  OnExit();
	virtual BOOLEAN  OnPrint();

public:
	void			SetStepTipMode( U32 mode ) { mStepTipMode = mode; }
	U32				GetStepTipMode( ) { return mStepTipMode; }
	void			SetStepTips( U8 mode[] )
	{
	}

	void			SetForWardMode( enStepMode mode ) { mForwardMode = mode; }
	void			SetEscMode( enStepMode mode ) { mEscMode = mode; }

protected:
	void			InitStepMode() { mForwardMode = STEP_FW; mEscMode = STEP_RETURN; }

	BOOLEAN			 StepSwitch( enStepMode mode );
	BOOLEAN			 StepForward();
	BOOLEAN			 StepBackward();
	BOOLEAN			 StepReturn();


/// Function key 
public:
	BOOLEAN			SetFuncKey( U8 idx, BOOLEAN bVisible, BOOLEAN bResponse = TRUE, S32 titleId = STR_BLANK );
	BOOLEAN			IsShowFuncKey() const;
	void			ShowFuncKey( BOOLEAN bShow );
	
protected:
	void			InitFuncKey();
	void			DrawFuncKey();

protected:
	CFuncKeyCfg		mFuncKeyCfg[3];
	BOOLEAN			mbShowFuncKey;
	U32				mFuncKeyTop;
};


inline  BOOLEAN CEnmBasePage::IsShowFuncKey() const
{
	return mbShowFuncKey;
}

inline  void CEnmBasePage::ShowFuncKey( BOOLEAN bShow )
{
	mbShowFuncKey = bShow;
}


/**
 *
 *
 **/
class CEnTimeBox : public MigEditBoxGrp
{
/// member routines
public:
	CEnTimeBox( MigObj *pParent = NULL, INT8U format = 0 )
		:MigEditBoxGrp( pParent )
	{
		tagCS cs;
		SetBoxParam( cs, format );
	}
		
	CEnTimeBox( const tagCS& cs )
		: MigEditBoxGrp( cs )
	{
		SetBoxParam( cs, 0 );
	}
		
	~CEnTimeBox( ){ }
	
	void	SetTime( const tagTime& stTm )
	{

		mstTime = stTm;
		SetBoxInt( mstTime.hour, 0 );
		SetBoxInt( mstTime.min, 1 );
		SetBoxInt( mstTime.sec, 2 );

		Update();
	}

	tagTime	GetTime( void)
	{
		mstTime.hour = (INT8U)GetBoxInt( 0 );
		mstTime.min	 = (INT8U)GetBoxInt( 1 );
		mstTime.sec  = (INT8U)GetBoxInt( 2 );
	
		return mstTime;
	}

	void SetHourRange( INT8U min, INT8U max ) 
	{
		mItemSet[0].box.SetRangeInt(min, max );
	}
	void SetMinutesRange( INT8U min, INT8U max ) 
	{
		mItemSet[1].box.SetRangeInt(min, max );
	}
	void SetSecondRange( INT8U min, INT8U max ) 
	{
		mItemSet[2].box.SetRangeInt(min, max );
	}

protected:
	void	SetBoxParam( const tagCS& cs, INT8U format )
	{
		/// Set Box's param
		SetParent( (MigObj*)cs.hParent );
		SetStyle( cs.dwStyle );
		SetTitle( (MCHAR*)cs.pszTitle );
		SetX( cs.x );
		SetY( cs.y );
		mHeight = cs.h; 

		tagEditBoxCS ecs[3];
		
		ecs[0].maxVal = 23;
		ecs[0].minVal = 0;
		ecs[0].iVal   = 0;
		strcpy( ecs[0].pszTitle, ENM_T(ENM_CHINESE, "时", ": ") );
		ecs[0].type = ENM_EB_TYPE_INT;

		ecs[1].maxVal = 59;
		ecs[1].minVal = 0;
		ecs[1].iVal   = 0;
		strcpy( ecs[1].pszTitle, ENM_T(ENM_CHINESE, "分", ": ") );
		ecs[1].type = ENM_EB_TYPE_INT;

		ecs[2].maxVal = 59;
		ecs[2].minVal = 0;
		ecs[2].iVal   = 0;
		strcpy( ecs[2].pszTitle, ENM_T(ENM_CHINESE, "秒", "  ") );
		ecs[2].type = ENM_EB_TYPE_INT;

		SetBoxGrpParam( ecs, 3 );				
	}

//	void  OnKey( U16 key )
//	{
//		MigEditBoxGrp::OnKey( key );
//		
//		INT16U  aKey = GetKeyValue( key );
//
//		if ( aKey == KEY_OK && !mItemSet[mCurItem].box.IsInEdit() )
//		{
//			U32 aVal = mItemSet[mCurItem].box.GetInt();
//			MCHAR aTm[16];
//			sprintf( aTm, "%02d", aVal );
//			mItemSet[mCurItem].box.SetStr( (MCHAR*)aTm );
//		}
//	}

/// member variables
public:
protected:
	tagTime		mstTime;
};

/**
 *
 *
 **/
class CEnDateBox : public MigEditBoxGrp
{
/// member routines
public:
	CEnDateBox( MigObj *pParent = NULL, INT8U format = 0 )
		:MigEditBoxGrp( pParent )
	{
		tagCS cs;
		SetBoxParam( cs, format );
	}
		
	CEnDateBox( const tagCS& cs, INT8U format = 0 )
		: MigEditBoxGrp( cs )
	{
		SetBoxParam( cs, format );
	}
		
	~CEnDateBox( ){ }

	void	SetDate( const tagDate& date )
	{

		mstDate = date;
		SetBoxInt( mstDate.year, 0 );
		SetBoxInt( mstDate.month, 1 );
		SetBoxInt( mstDate.day, 2 );

		Update();
	}

	tagDate	GetDate( void)
	{
		mstDate.year  = (INT16U)GetBoxInt( 0 );
		mstDate.month = (INT8U)GetBoxInt( 1 );
		mstDate.day   = (INT8U)GetBoxInt( 2 );
	
		return mstDate;
	}
	
protected:
	void	SetBoxParam( const tagCS& cs, INT8U format )
	{
		/// Set Box's param
		SetParent( (MigObj*)cs.hParent );
		SetStyle( cs.dwStyle );
		SetTitle( (MCHAR*)cs.pszTitle );
		SetX( cs.x );
		SetY( cs.y );
		mHeight = cs.h; 

		MCHAR *aFormat[2][3] = 
		{
			//{ ENM_T(ENM_CHINESE, "年", "/"), ENM_T(ENM_CHINESE, "月", "/"), ENM_T(ENM_CHINESE, "日", " ") },
			{ "--", "--", "" },
			{ ".", ".", "" }
		};

		tagEditBoxCS ecs[3];
		
		ecs[0].maxVal = 2700;
		ecs[0].minVal = 1900;
		ecs[0].iVal   = 2004;
		strcpy( ecs[0].pszTitle, aFormat[format][0] );
		ecs[0].type = ENM_EB_TYPE_INT;

		ecs[1].maxVal = 12;
		ecs[1].minVal = 1;
		ecs[1].iVal   = 1;
		strcpy( ecs[1].pszTitle, aFormat[format][1] );
		ecs[1].type = ENM_EB_TYPE_INT;

		ecs[2].maxVal = 31;
		ecs[2].minVal = 1;
		ecs[2].iVal   = 1;
		strcpy( ecs[2].pszTitle, aFormat[format][2] );
		ecs[2].type = ENM_EB_TYPE_INT;

		SetBoxGrpParam( ecs, 3 );				
	}
	
/// member variables
public:	
protected:
	tagDate		mstDate;
};

 
#endif // __ENK_BASE_PAGE_H__
