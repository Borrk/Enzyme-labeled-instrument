// EnmModeLib.h: interface for the CEnmModeLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMMODELIB_H__4EC260D3_D607_4665_B760_5AFDA44739B1__INCLUDED_)
#define AFX_ENMMODELIB_H__4EC260D3_D607_4665_B760_5AFDA44739B1__INCLUDED_


#include "EnmDef.h"
#include "EnmFs.h"
#include "EnmSubject.h"

#define FILE_NAME_LEN  32;

class CEnmModeLib  : public CEnmSubject
{
	struct CModeInfo
	{
		tagProtocol		mProtocol;
		U16				mFileID;
		char			mName[32];
		BOOLEAN			mbChanged;
	};
	
public:
	enum  { PROT_MAX = 256 };

	enum { RProtAdd, RProtModify, RProtDelete, RProtRename, RUnknow };

public:
	static CEnmModeLib& Instance();
	virtual ~CEnmModeLib();

private:
	CEnmModeLib();

public:
	BOOLEAN		Load(void);	
	void		Unload();
	U16			GetCount() const;
	BOOLEAN		GetAt( U16 idx, tagProtocol& prot );
	///< 0 --failed, 1 --- saved, 2 --- same
	BYTE		SetAt( U16 idx, const tagProtocol& prot ); 
	BYTE		UpdateByID( U16 id, const tagProtocol& prot ); 
	BOOLEAN		DeleteAt( U16 idx, BOOLEAN bSave = FALSE );
	BOOLEAN		Add( const tagProtocol& prot );
	MCHAR*		GetProtocolName( U16 idx ) const;
	BOOLEAN		RenameProtocol( U16 idx, const MCHAR* pName );
	
	void		Sync( );

protected:
	void		Init();
	S32			nCompare( const tagProtocol& first, const tagProtocol& second ) const;
	S16			nFind( const MCHAR* pszName );
	BOOLEAN		nFormatProtName( MCHAR szName[] );

/// member variables
	CModeInfo	mModeList[PROT_MAX];
	U16			mCount;
};

#endif // !defined(AFX_ENMMODELIB_H__4EC260D3_D607_4665_B760_5AFDA44739B1__INCLUDED_)
