
#include "EnmCutoffLib.h"
#include "EnmFs.h"

CEnmCutoffLib::CEnmCutoffLib()
{
	mbChanged = FALSE;
}

CEnmCutoffLib::~CEnmCutoffLib()
{
	if ( mFileID != NF_INVALID_FILE_ID )
		Unload();
}

CEnmCutoffLib& CEnmCutoffLib::Instance()
{
	static CEnmCutoffLib aInst;
	return aInst;
}

BOOLEAN	CEnmCutoffLib::Load(void)
{
	BOOLEAN  bOK = FALSE;

	U32  aSize = CUTOFF_COUNT * sizeof(tagCutoffEntry) + 4;
	U16  aFileId;

	mCount = 0;

	aFileId = efOpen( "E20.cut", EF_OPEN_WRITE | EF_OPEN_READ );

	if ( NF_INVALID_FILE_ID == aFileId )
	{
		aFileId = efOpen( "E20.cut", EF_OPEN_WRITE | EF_OPEN_CREATE );
	
		efWrite( (U8*)&mCount, 4, aFileId );
		efWrite( (U8*)mCutoffList, aSize-4, aFileId );
		efClose( aFileId );
	
		aFileId = efOpen( "E20.cut", EF_OPEN_WRITE | EF_OPEN_READ );
	}

	BOOLEAN bValid = TRUE;
	if ( aSize != efGetSize( aFileId ) )
	{
		bValid = FALSE;
	}
	
	if ( bValid)
	{
		efSeek( aFileId, 0, ESEEK_SET );
		if ( efRead( (U8*)&mCount, 4, aFileId) != 4 )
			bValid = FALSE;
	}

	aSize -= 4;
	if ( bValid )
	{
		if( aSize != efRead( (U8*)mCutoffList, aSize, aFileId ) )
			bValid = FALSE;
	}

	if ( !bValid )
	{
		mCount = 0;
		efSeek( aFileId, 0, ESEEK_SET );
		efWrite( (U8*)&mCount, 4, aFileId );
		efWrite( (U8*)mCutoffList, aSize, aFileId );
	}
	
	mFileID = aFileId;
	
	return bOK;
}

BOOLEAN	 CEnmCutoffLib::Add( tagCutoffEntry& entry )
{
	if ( mCount > CUTOFF_COUNT ) 
		return FALSE;

	mCutoffList[mCount] = entry;
	mCount++;

	mbChanged = TRUE;

	return TRUE;
}

BOOLEAN	 CEnmCutoffLib::Delete( U32 date )
{
	S32	aIdx = Find( date );
	if ( aIdx == -1 )
		return TRUE;

	BOOLEAN bRet = FALSE;

	for ( S32 i = aIdx; i < mCount - 1; i++ )
	{
		mCutoffList[i] = mCutoffList[i+1];
	}

	mCutoffList[mCount-1].mDate = 0;
	memset( mCutoffList[mCount-1].mProtName, 0, sizeof(mCutoffList[mCount-1].mProtName) );
	memset( mCutoffList[mCount-1].mEquation, 0, sizeof(mCutoffList[mCount-1].mEquation) );
	mCount--;

	return TRUE;
}

U32	 CEnmCutoffLib::Find( MCHAR* pProtName, tagCutoffEntry* pList, U16 count ) const
{
	U32 aCnt = 0;

	if ( !pProtName || !pList || count <= 0 )
		return 0;

	for ( U16 i = 0; i < mCount; i++ )
	{
		if ( strcmp( mCutoffList[i].mProtName, pProtName ) == 0 )
		{
			if ( aCnt < count )
				pList[aCnt] = mCutoffList[i];

			aCnt++;
		}
	}

	return aCnt;
}

S32  CEnmCutoffLib::Find( U32 date ) const
{
	S32 aIdx = -1;

	for ( S32 i = 0; i < mCount; i++ )
	{
		if ( mCutoffList[i].mDate == date )
		{
			aIdx = i;
			break;
		}
	}

	return aIdx;
}

void CEnmCutoffLib::Sync( BOOLEAN bAll )
{
	/// if changed, save to Flash
	if ( !mbChanged )
		return;

	U32 aSize = bAll ? sizeof(tagCutoffEntry) * CUTOFF_COUNT : sizeof(tagCutoffEntry) * mCount;
	efSeek( mFileID, 0, ESEEK_SET );
	efWrite( (U8*)&mCount, 4, mFileID );
	efWrite( (U8*)&mCutoffList, aSize, mFileID );

	efSaveFS();
	efFlushFAT();
}

void CEnmCutoffLib::Unload()
{
	if ( NF_INVALID_FILE_ID == mFileID )
		return;

	Sync( TRUE );
	efClose( mFileID );

	mFileID = NF_INVALID_FILE_ID;
	mCount = 0;
}

void CEnmCutoffLib::Update( CEnmSubject& subject, CEnmNotify& reson )
{

}
