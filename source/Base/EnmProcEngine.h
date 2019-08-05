#ifndef __ENM_PROC_ENGINE_H
#define __ENM_PROC_ENGINE_H

#include "EnmBase.hpp"
#include "EnmBasePage.hpp"

enum  enProcState
{
	PROC_IDLE,
	PROC_BUSY,
	PROC_SUSPEND,
	PROC_DELAY,
};

class  IProcEngine
{
public:
	IProcEngine();
	virtual ~IProcEngine();

public:
	virtual enProcState IEnterProc( ) = 0;
	virtual BOOLEAN		InitEngine(){ mStartTick = 0; return TRUE; }
	virtual enProcState ISuspend( S32 timeout = -1, BOOLEAN bForced = FALSE );
	virtual enProcState IResume( );

public:
	enProcState			GetCurState() const;
	CEnmBasePage*		GetAttchedPage() const;
	void				Attach( CEnmBasePage& page );
protected:
	void				Init();

//////////////////////////////////////////////////////////////////////////
///  member variables
//////////////////////////////////////////////////////////////////////////
protected:
	enProcState			mState;
	CEnmBasePage		*mpPage;
	S32					mElipse;  /// suspend overtime , -1 indicate always
	U32					mStartTick;
};


//////////////////////////////////////////////////////////////////////////
///  inline routines  section
//////////////////////////////////////////////////////////////////////////

inline  enProcState	IProcEngine::GetCurState() const
{
	return mState;
}

inline	CEnmBasePage*  IProcEngine::GetAttchedPage() const
{
	return mpPage;
}

inline  void  IProcEngine::Attach( CEnmBasePage& page )
{
	mpPage = &page;
}


#endif ///< __ENM_PROC_ENGINE_H
