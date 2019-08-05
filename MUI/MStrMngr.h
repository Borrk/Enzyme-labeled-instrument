#ifndef __MIG_STRING_MANAGER_H
#define __MIG_STRING_MANAGER_H

#include "MIG_Base.hpp"

enum
{
	LANGUE_CHINESE,
    LANGUE_ENGLISH,

	LANGUE_MAX
};

class MStrMngr
{
public:
	static MStrMngr& Instance()
	{
		static MStrMngr aInst;
		return aInst;
	}
	
	MCHAR*	GetString(INT32U strId ) const
	{
		if ( strId >= mStrCnt )
			return NULL;

		return mStrTable[strId * LANGUE_MAX + mLangue];
	}

	void	SetStrTable( MCHAR** pTbl, INT32U cnt )
	{
		mStrTable = pTbl;	mStrCnt = cnt;
	}

	enMigLangue	SetLangue( enMigLangue langue )
	{
		enMigLangue aPre = mLangue;
		mLangue = langue;
		return aPre;
	}
	
protected:
	MStrMngr(): mLangue(M_CHINESE), mStrTable(NULL), mStrCnt(0){}
	
protected:
	enMigLangue     mLangue;
	MCHAR**			mStrTable;
	INT32U			mStrCnt;
};


#endif ///< __MIG_STRING_MANAGER_H
