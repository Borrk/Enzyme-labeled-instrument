// EnmModeLib.cpp: implementation of the CEnmModeLib class.
//
//////////////////////////////////////////////////////////////////////
#include "MIG_OS_Base.hpp"
#include "EnmModeLib.h"
#include "MIG_Base.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnmModeLib::CEnmModeLib()
{
	Init();
}

CEnmModeLib::~CEnmModeLib()
{
}

CEnmModeLib& CEnmModeLib::Instance()
{
	static CEnmModeLib aInst;
	return aInst;
}

BOOLEAN	 CEnmModeLib::Load(void)
{
	BOOLEAN  bOK = FALSE;

	MCHAR  *apCfgFileName[256] = {NULL };
	MCHAR  aszName[256];

	U16 aCnt =  efSearchFileByExt( "cfg", apCfgFileName, 256 );

	tagProtocol aProt;
	U16  aFileId;
	for ( U16 i = 0; i < aCnt; i++ )
	{
		aFileId = efOpen( apCfgFileName[i], EF_OPEN_WRITE | EF_OPEN_READ );
		if ( NF_INVALID_FILE_ID != aFileId )
		{
			if ( sizeof(aProt) != efGetSize( aFileId ) )
				continue;
			
			if( sizeof(aProt) == efRead( (U8*)&aProt, sizeof(aProt), aFileId ) )
			{
				mModeList[mCount].mProtocol = aProt;
				mModeList[mCount].mFileID = aFileId;
				mModeList[mCount].mbChanged = false;
				strcpy( aszName, apCfgFileName[i] );
				//char* p = strchr( aszName, '.' );
				//*p = '\0';
				strcpy( mModeList[mCount].mName, aszName );
				if ( ++mCount > PROT_MAX )
					break;
			}
		}
	}
	
	return bOK;
}

U16	 CEnmModeLib::GetCount() const
{
	return mCount;
}

BOOLEAN	 CEnmModeLib::GetAt( U16 idx, tagProtocol& prot )
{
	if ( idx < GetCount() )
	{
		memcpy( &prot, &mModeList[idx].mProtocol, sizeof(prot) );
		return TRUE;
	}
	else
		return FALSE;
}

BYTE CEnmModeLib::SetAt( U16 idx, const tagProtocol& prot ) 
{
	if ( idx >= GetCount() )
		return 0;

	MCHAR *aszOldName = (MCHAR*)mModeList[idx].mProtocol.stMeasure.szName;
	MCHAR *aszNewName = (MCHAR*)prot.stMeasure.szName;
	BOOLEAN bNameChanged = strcmp( aszOldName, aszNewName ) == 0 ? FALSE : TRUE;

	if ( 0 == nCompare( prot, mModeList[idx].mProtocol ) )
	{
		return 2;
	}
	else
	{
		/// Notify
		if ( bNameChanged )
		{
			CEnmNotify aNotify;
			aNotify.mReason = RProtRename;
			aNotify.mContent = idx;
			Notify( aNotify );
		}

		memcpy( &mModeList[idx].mProtocol, &prot, sizeof(prot) );
		mModeList[idx].mbChanged = TRUE;

		return 1;
	}
}

BYTE CEnmModeLib::UpdateByID( U16 id, const tagProtocol& prot ) 
{
	tagProtocol aProt;
	for ( U16 i = 0; i < GetCount(); i++ )
	{
		if ( GetAt( i, aProt) && aProt.stMeasure.uID == id )
		{
			return SetAt( i, prot );
		}
	}

	return 0;
}

BOOLEAN	 CEnmModeLib::DeleteAt( U16 idx, BOOLEAN bSave )
{
	if ( idx >= mCount )
		return TRUE;

	/// Notify
	CEnmNotify aNotify;
	aNotify.mReason = RProtDelete;
	aNotify.mContent = idx;
	Notify( aNotify );

	/// Delete file
	MCHAR *apName = mModeList[idx].mName;
	efClose( mModeList[idx].mFileID );
	if( 1 == efDelete( apName ) )
	{
		memcpy( &mModeList[idx], &mModeList[idx+1], sizeof(CModeInfo) * (mCount - idx) );
		mCount--;

		if ( bSave )
			Sync();
	}

	return TRUE;
}

BOOLEAN	 CEnmModeLib::Add( const tagProtocol& prot )
{
	BOOLEAN  bOK = FALSE;
	
	if ( GetCount() >= PROT_MAX )
		return FALSE;

	MCHAR aszFileName[64] = {0};

	if ( !nFormatProtName( aszFileName ) )
		return FALSE;

	/// Write to FLash
	U16  aFile = efOpen( aszFileName, EF_OPEN_CREATE | EF_OPEN_WRITE );
	if ( NF_INVALID_FILE_ID == aFile )
		return FALSE;

	if ( sizeof(prot) == efWrite( (U8*)&prot, sizeof(prot), aFile ) )
		bOK = TRUE;

	/// Save to List
	if ( bOK )
	{
		memcpy( &mModeList[GetCount()].mProtocol, &prot, sizeof(prot) );
		mModeList[GetCount()].mbChanged = TRUE;
		mModeList[GetCount()].mFileID   = aFile;
		strcpy( mModeList[GetCount()].mName, aszFileName );

		/// Notify
		CEnmNotify aNotify;
		aNotify.mReason = RProtAdd;
		aNotify.mContent = GetCount();
		Notify( aNotify );
		
		mCount++;
	}

	return bOK;
}

MCHAR*		CEnmModeLib::GetProtocolName( U16 idx ) const
{
	if ( idx >= GetCount() )
		return NULL;
	else
		return (MCHAR*)mModeList[idx].mName;
		//return (MCHAR*)mModeList[idx].mProtocol.stMeasure.szName;
}

BOOLEAN	 CEnmModeLib::RenameProtocol( U16 idx, const MCHAR* pName )
{
	if ( !pName || idx >= GetCount() )
		return TRUE;

	BOOLEAN  bOK = FALSE;

	MCHAR aszFileName[64] = {0};
	strcpy( aszFileName, pName );

	strcpy( mModeList[idx].mProtocol.stMeasure.szName, aszFileName );
	mModeList[idx].mbChanged = TRUE;

	/// Notify
	{
		CEnmNotify aNotify;
		aNotify.mReason = RProtRename;
		aNotify.mContent = idx;
		Notify( aNotify );
	}

	
	bOK = TRUE;

	return bOK;
}

void CEnmModeLib::Sync()
{
	/// if changed, save to Flash
	for ( U16 i = 0; i < GetCount(); i++ )
	{
		if ( !mModeList[i].mbChanged )
			continue;

		efSeek( mModeList[i].mFileID, 0, ESEEK_SET );
		efWrite( (U8*)&mModeList[i].mProtocol, sizeof(mModeList[i].mProtocol), mModeList[i].mFileID );
		mModeList[i].mbChanged = FALSE;
	}

	efSaveFS();
	efFlushFAT();
}

void CEnmModeLib::Unload()
{
	Sync();
	/// if changed, save to Flash
	for ( U16 i = 0; i < GetCount(); i++ )
	{
		efClose( mModeList[i].mFileID );
	}

	Init();
}

void  CEnmModeLib::Init()
{
	mCount = 0;

	for ( U16 i = 0; i < PROT_MAX; i++ )
	{
		mModeList[i].mFileID = NF_INVALID_FILE_ID;
		mModeList[i].mbChanged = FALSE;
		memset( mModeList[i].mName, 0, 32 );
	}
}

S32	  CEnmModeLib::nCompare( const tagProtocol& first, const tagProtocol& second ) const
{
	S32 aRet = memcmp( &first, &second, sizeof(first) );

	return aRet;
}

S16  CEnmModeLib::nFind( const MCHAR* pszName )
{
	U16 aIdx = -1;

	for ( S16 i = 0; i < GetCount(); i++ )
	{
		if ( 0 == strcmp( mModeList[i].mName, pszName ) )
		{
			aIdx = i;
			break;
		}
	}

	return aIdx;
}

BOOLEAN	CEnmModeLib::nFormatProtName( MCHAR szName[] )
{
	BOOLEAN bCreated = FALSE;
	S16 aId = 0;
	
	while ( aId++ <= PROT_MAX*2 )
	{
		sprintf( szName, "Ml%d.cfg", aId );
		if ( -1 == nFind( szName ) )
		{
			bCreated = TRUE;
			break;
		}
	}

	return bCreated;
}
