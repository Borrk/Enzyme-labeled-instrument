#ifndef __EnmCutoffLib_H
#define __EnmCutoffLib_H

#include "EnmObserver.h"

#pragma pack(1)
struct tagCutoffEntry
{
	U32		mDate;
	U32		mTime;
	MCHAR   mProtName[PROTOCOL_NAME];
	MCHAR   mEquation[CUTOFF_MAX];
	FP32	mPC;
	FP32	mNC;
};
#pragma pack()

class CEnmCutoffLib : public CEnmObserver  
{
public:
	enum { CUTOFF_COUNT = 128 };

public:
	static CEnmCutoffLib& Instance();
	virtual ~CEnmCutoffLib();

public:
	BOOLEAN		Load();
	void		Unload();
	
public:
	BOOLEAN		Add( tagCutoffEntry& entry );
	BOOLEAN		Delete( U32 date );
	void		Update( CEnmSubject& subject, CEnmNotify& reson );
	
	BOOLEAN		IsChanged() const;
	
	void		Sync( BOOLEAN bAll );
	S32			Find( U32 date ) const;
	U32			Find( MCHAR* pProtName, tagCutoffEntry* pList, U16 count ) const;

protected:
	CEnmCutoffLib();


protected:
	tagCutoffEntry		mCutoffList[CUTOFF_COUNT];
	U32					mCount;
	S32					mFileID;
	BOOLEAN				mbChanged;
};

inline BOOLEAN CEnmCutoffLib::IsChanged() const
{
	return mbChanged;
}

#endif ///< __EnmCutoffLib_H
