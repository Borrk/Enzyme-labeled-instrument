#include <string.h>
#include "EnmFs.h"


#ifndef ENM_VER_TARGET
void EnableIrq()
{
}

void DisableIrq()
{
}

#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////    FAT  struct
typedef struct
{
	U16  year;
	U8    month;
	U8   day;
	U8   hour;
	U8  min;
	U8  sec;
}tagFatTime;

#define   ENM_FILE_NAME_LEN	16
typedef struct 
{
	U16   uID;			/// file id
	U16   uStartPage;	/// frist storage page
	char  Name[ENM_FILE_NAME_LEN];	/// file name
	tagFatTime  stTime;  ///  Create time
	U32  uLength;		/// file length
	U8    uFlag;		/// flag byte; bit0 : 0 --not used, 1---used
}tagEnmFAT;

typedef struct 
{
	U16   uID;			/// file id
	U16   uOpenFlag;	/// Open Flag
	U32   uCurPos;		/// current pointer position
	U8     bOpened;		/// flag byte;  0 --not opened, 1---opened
}tagEnmFileInfo;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///   Manage variables
static  tagEnmFAT  nFatTable[NF_FILE_MAX];
static  tagEnmFileInfo  nOpenFileTable[NF_FILE_MAX];
static  U16  nExistFileCnt = 0;
static  U32  nFatChangeCount = 0;
static  U8   nFsCreated = 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// FileId mapping to index in FAT table
/// here id equal to index
#define  ID2INDEX( id ) (id)
#define  INDEX2PAGE( idx ) ((idx) / 16 + NF_FAT_START )



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Load FAT from Flash
U16  nLoadFAT()
{
	U16 i, aExistCount;
	U8   aTableData[NF_FAT_COUNT *( NF_PAGE_DATA_LEN+NF_PAGE_SPARE_LEN)] = { 0 };
	U8  *ap = aTableData;
   
	/// read page manage data
	for ( i = NF_FAT_START; i < (NF_FAT_START + NF_FAT_COUNT); i++)
	{
	    _RdPage( 1, i, ap ); 
	    ap += NF_PAGE_DATA_LEN;
	}
	/// set to page manage table
	memcpy( nFatTable, aTableData, sizeof(nFatTable) );
	
	/// calculate exist files count
	aExistCount = 0;
	for ( i = 0; i < NF_FILE_MAX; i++)
	{
		if ( nFatTable[i].uFlag == 1 )
			aExistCount ++;
	}
		
	return aExistCount;
}

void  nSaveFatTable( )
{
	U16 i;
	U8  *ap = (U8*)nFatTable;
   
   	if ( 0 == nFatChangeCount )
   		return;
	
	_EraseBlock( 1 );
	
	/// Write page manage data
	for ( i = NF_FAT_START; i < (NF_FAT_START + NF_FAT_COUNT); i++)
	{
	    _WrPage( 1, i, ap ); 
	    ap += NF_PAGE_DATA_LEN;
	}
}

U16  nFindFileByName( char * pName )
{
	U16 i;
	U16 aFileId = NF_INVALID_FILE_ID; /// file's ID start from 1, 0 is a invalid ID
	
	if ( !pName )
		return aFileId;
		
	for ( i = 0; i < NF_FILE_MAX; i++ )
	{
		if ( nFatTable[i].uFlag == 1 )
		{
			if ( 0 == strcmp( nFatTable[i].Name, pName) )
			{
				aFileId = nFatTable[i].uID;
				break;
			}			
		}		
	}
		
	return aFileId;		
}

U16  nFindFreeFileEntry( )
{
	U16 i;
	U16 aFileId = NF_INVALID_FILE_ID; 
			
	for ( i = 0; i < NF_FILE_MAX; i++ )
	{
		if ( nFatTable[i].uFlag == 0 )
		{
			aFileId = i; //nFatTable[i].uID;
			break;
		}		
	}
		
	return aFileId;		
}

U8  IsValidID( U16 id )
{
	if ( id != NF_INVALID_FILE_ID && id <= NF_FILE_MAX )
		return 1;
	else
		return 0;
}

U16  nGetPageListNext( U16 page )
{
	U16 aNextPage;
	U8 aTempBuf[NF_PAGE_SPARE_LEN];

	Nf_ReadPageSpare( page, aTempBuf );

	aNextPage = (aTempBuf[14] << 8 ) | aTempBuf[15];
	if ( aNextPage > NF_PAGE_MAX )
		aNextPage = NF_INVALID_PAGE_ID;

	return aNextPage;
}

U8  nSetpageListNext( U16 prePage, U16 nextPage )
{
	U8 aTempBuf[NF_PAGE_SPARE_LEN];

	if ( prePage > NF_PAGE_MAX )
		return 0;

	Nf_ReadPageSpare( prePage, aTempBuf );
	/// set next page's id
	aTempBuf[14] = (nextPage >> 8) & 0xFF;
	aTempBuf[15] = nextPage & 0xFF;
	Nf_WritePageSpare( prePage, aTempBuf );

	return 1;
}

void  nSetNextPageFlag( U8* buffer, U16 page )
{
	buffer[14] = (page >> 8) & 0xFF;
	buffer[15] = page & 0xFF;
}

/// release page, return released page count
U16  nReleasePageList( U16 startPage )
{
	U16 aCount = 0;
	U16 aPage = startPage;
	
	while ( NF_INVALID_FILE_ID != aPage )
	{
		Fm_FreePage( aPage );
		aPage = nGetPageListNext( aPage );		
		aCount++;
	}
	
	return aCount;
}

U16  nSearchFileByExt( char* pName, char **pFileID, U16 size )
{
	U16  i, aCnt = 0;
	char *ap;

	if ( !pName || !pFileID || size < 1 )
		return 0;

	for (  i = 0; i < NF_FILE_MAX; i++ )
	{
		if ( nFatTable[i].uFlag == 1 )
		{
			ap = strstr( nFatTable[i].Name, "." );
			if ( ap && 0 == strcmp( ap+1, pName) )
			{
				pFileID[aCnt++] = nFatTable[i].Name;
			}
		}		
	}

	return aCnt;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Init  FAT table, must be called firstly
U8  efInit()
{
	/// Init FAT table
	memset( nFatTable, 0, sizeof(nFatTable) );
	nExistFileCnt = 0;

	nFatChangeCount = 0;
	
	/// Load Fat table
	nExistFileCnt = nLoadFAT();

	/// Init open file list
	memset( nOpenFileTable	, 0, sizeof(nOpenFileTable) );
	
	return 1;
}


/// Open a file,  return file ID
/// openFlag:  EF_OPEN_CREATE, EF_OPEN_READ, EF_OPEN_WRITE
/// return -1 ---failed
U16 efOpen( char * szName, U16 openFlag )
{
	U16 aFileId = NF_INVALID_FILE_ID;

	aFileId = nFindFileByName( szName );

	/// file exist
	if ( NF_INVALID_FILE_ID != aFileId )
	{
		if ( nOpenFileTable[aFileId].bOpened ==1 )
			return NF_INVALID_FILE_ID;
			
		nOpenFileTable[aFileId].bOpened = 1;	
		nOpenFileTable[aFileId].uCurPos = 0;
		nOpenFileTable[aFileId].uID = aFileId;
		nOpenFileTable[aFileId].uOpenFlag = openFlag;
	}
	else if ( (openFlag&EF_OPEN_CREATE) )
	{
		aFileId = nFindFreeFileEntry();		

		/// Set Fat Entry
		nFatTable[aFileId].uID = aFileId;
		nFatTable[aFileId].uFlag = 1;
		nFatTable[aFileId].uLength = 0;
		nFatTable[aFileId].uStartPage = NF_INVALID_PAGE_ID;
		// Set Create Time. Add later
		//nFatTable[aFileId].stTime = ;

		strncpy( nFatTable[aFileId].Name, szName, ENM_FILE_NAME_LEN - 1 );
		nFatTable[aFileId].Name[ENM_FILE_NAME_LEN - 1] = '\0';

		/// Save to Flash
		/// Hyq: not write to flash on time, 
		//aFileEntryIndex = ID2INDEX(aFileId);
		//aPage = INDEX2PAGE(aFileEntryIndex);
		// _WrPage( 0, aPage, (U8*)&nFatTable[aPage / 16 * 16] );

		/// Set Open table
		nOpenFileTable[aFileId].bOpened = 1;	
		nOpenFileTable[aFileId].uCurPos = 0;
		nOpenFileTable[aFileId].uID = aFileId;
		nOpenFileTable[aFileId].uOpenFlag = openFlag;
		
		nFatChangeCount++;
		
		nSaveFatTable(); ///< save fat table immediately
	}

	return aFileId;
}

/// Close a file
void efClose( U16 fileID )
{
	/// file exist
	if ( IsValidID(fileID) )
	{
		nOpenFileTable[fileID].bOpened = 0;	
		nOpenFileTable[fileID].uCurPos = 0;
		nOpenFileTable[fileID].uID = NF_INVALID_FILE_ID;
		nOpenFileTable[fileID].uOpenFlag = 0;
	}	
}

///  read file
/// return read size
U32 efRead( U8* buffer, U32 size, U16 fileID )
{
	U8  aTempBuf[EF_SECTOR_SIZE];
	U16 i;
	U32 aSize;
	U32 aCurPos, aBufferPos;
	U16 aCurPage, aPageCount;

	if ( nOpenFileTable[fileID].bOpened == 0 )
		return 0;	
	
	aCurPos = nOpenFileTable[fileID].uCurPos;

	/// left size less than requested size
	if ( (aCurPos + size) > nFatTable[fileID].uLength )
	{
		aSize = nFatTable[fileID].uLength - aCurPos;
	}
	else
	{
		aSize = size;
	}

	/// Read data
	aBufferPos = 0;
	aCurPage = nFatTable[fileID].uStartPage ;

	i = 0;
	aPageCount = aCurPos / EF_SECTOR_SIZE;
	while ( i < aPageCount )
	{
		aCurPage = nGetPageListNext( aCurPage );
		if ( NF_INVALID_PAGE_ID == aCurPage )
			return 0;

		i++;
	}

	if ( NF_INVALID_PAGE_ID == aCurPage )
		return 0;

	while ( aCurPos < nFatTable[fileID].uLength )
	{
		if ( (aCurPos % EF_SECTOR_SIZE + aSize) <= EF_SECTOR_SIZE )
		{
			Nf_ReadPage( aCurPage, 1, aTempBuf );

			/// copy to dest buffer
			memcpy( (void*)&buffer[aBufferPos], (void*)&aTempBuf[aCurPos % EF_SECTOR_SIZE], aSize );
			//aCurPos = nFatTable[fileID].uLength;
			aCurPos += aSize; 
			break;
		}
		else
		{
			Nf_ReadPage( aCurPage, 1, aTempBuf );

			/// copy to dest buffer
			memcpy( &buffer[aBufferPos], &aTempBuf[aCurPos % EF_SECTOR_SIZE], EF_SECTOR_SIZE - aCurPos % EF_SECTOR_SIZE );
			aBufferPos += EF_SECTOR_SIZE - aCurPos % EF_SECTOR_SIZE;
			aSize -= EF_SECTOR_SIZE - aCurPos % EF_SECTOR_SIZE;
			aCurPos += EF_SECTOR_SIZE - aCurPos % EF_SECTOR_SIZE;
		
			aCurPage = nGetPageListNext( aCurPage );
			if ( NF_INVALID_PAGE_ID == aCurPage )
				break;
		}
	}
	
	aBufferPos = aCurPos - nOpenFileTable[fileID].uCurPos;
	nOpenFileTable[fileID].uCurPos = aCurPos;

	return aBufferPos;
}

///  seek file
void efSeek( U16 fileID, U32 pos, U16 from )
{
	if ( !IsValidID(fileID))
		return;

	if ( nOpenFileTable[fileID].bOpened == 0 )
		return;

	if ( nFatTable[fileID].uLength == 0 )
	{
		nOpenFileTable[fileID].uCurPos;
		return;
	}
	
	switch( from )
	{
	case ESEEK_SET:
		nOpenFileTable[fileID].uCurPos = pos% nFatTable[fileID].uLength;
		break;

	case ESEEK_CUR:
		nOpenFileTable[fileID].uCurPos = (nOpenFileTable[fileID].uCurPos + pos) % nFatTable[fileID].uLength;
		break;

	case ESEEK_END:
		if ( nFatTable[fileID].uLength > pos )
		{
			nOpenFileTable[fileID].uCurPos = nFatTable[fileID].uLength - pos;
		}
		else
		{
			nOpenFileTable[fileID].uCurPos = 0;
		}
		break;
	default:
		break;
	}
}

///  write file
/// return read size
U32 efWrite( U8* buffer, U32 size, U16 fileID )
{
	U16 i;
	U8  aTempBuf[EF_SECTOR_SIZE];
	S32 aSize, aWrittenSize;
	U32 aCurPos, aBufferPos;
	U16 aCurPage, aNextPage, aPageCount;

	if ( !buffer || size == 0 )
		return 0;

	if ( nOpenFileTable[fileID].bOpened == 0 )
		return 0;	
	
	DisableIrq();
	
	aWrittenSize = 0;
	aBufferPos = 0;
	aSize = size;
	aCurPos = nOpenFileTable[fileID].uCurPos;
	aCurPage = nFatTable[fileID].uStartPage;
	aNextPage = aCurPage;

	/// write position in file page list , overwrite them
	if ( aCurPos < nFatTable[fileID].uLength )
	{
		i = 0;
		aPageCount = aCurPos / EF_SECTOR_SIZE;
		aNextPage = aCurPage;
		while ( (i < aPageCount) && (aCurPage != NF_INVALID_PAGE_ID) )
		{
			aNextPage = nGetPageListNext( aCurPage );
			aCurPage = aNextPage;
			i++;
		}

		/// invalid page
		if ( aNextPage == NF_INVALID_PAGE_ID )
			return 0;

		/// write to page
		while ( aCurPos < nFatTable[fileID].uLength )
		{
			if ( (aCurPos % EF_SECTOR_SIZE + aSize) >= EF_SECTOR_SIZE )
			{
				aWrittenSize = EF_SECTOR_SIZE - (aCurPos % EF_SECTOR_SIZE);
			}
			else
			{
				aWrittenSize = aSize;
			}
		
			if ( aWrittenSize < EF_SECTOR_SIZE )
				Nf_ReadPage( aCurPage, 1, aTempBuf );

			memcpy( (void*)&aTempBuf[aCurPos%EF_SECTOR_SIZE], (void*)&buffer[aBufferPos], aWrittenSize );
			Nf_WritePage( aNextPage, 1, aTempBuf );
			
			/// Postion forward
			aCurPos += aWrittenSize;
			aSize -= aWrittenSize;
			aBufferPos += aWrittenSize;

			/// written all data
			if ( aSize <= 0 )
			{
				nOpenFileTable[fileID].uCurPos = aCurPos;
				return aBufferPos;
			}

			if ( nGetPageListNext( aNextPage ) == NF_INVALID_PAGE_ID )
			{
				nOpenFileTable[fileID].uCurPos = aCurPos;
				break;
			}
			else
				aNextPage = nGetPageListNext( aNextPage );
		}
	}
	else if ( 0 != nFatTable[fileID].uLength )	/// not a empty file
	{
		nOpenFileTable[fileID].uCurPos = nFatTable[fileID].uLength;
		aCurPos = nFatTable[fileID].uLength;

		///find the file's last page
		i = 0;
		aPageCount = aCurPos / EF_SECTOR_SIZE;
		//aNextPage = aCurPage;
		while ( (i < aPageCount) && (aCurPage != NF_INVALID_PAGE_ID) )
		{
			aNextPage = nGetPageListNext( aCurPage );
			aCurPage = aNextPage;
			i++;
		}
		
		/// if the last page has free memory, write data to it's free memory
		if ( aCurPos % EF_SECTOR_SIZE != 0 )
		{		
			if ( (EF_SECTOR_SIZE - aCurPos % EF_SECTOR_SIZE) <= aSize )
			{
				aWrittenSize = EF_SECTOR_SIZE - (aCurPos % EF_SECTOR_SIZE);
			}
			else
			{
				aWrittenSize = aSize;
			}

			Nf_ReadPage( aCurPage, 1, aTempBuf );

			memcpy( (void*)&aTempBuf[aCurPos%EF_SECTOR_SIZE], (void*)&buffer[aBufferPos], aWrittenSize );
			Nf_WritePage( aCurPage, 1, aTempBuf );

			/// Postion forward
			aCurPos += aWrittenSize;
			aSize -= aWrittenSize;
			aBufferPos += aWrittenSize;
		}

		aNextPage = aCurPage;
	}

//	if ( aSize <= 0 )
//	if ( (nOpenFileTable[fileID].uCurPos + aBufferPos) > nFatTable[fileID].uLength )
//	{
//		nFatTable[fileID].uLength = nOpenFileTable[fileID].uCurPos + aBufferPos;
//		return aBufferPos;
//	}

	/// Write data to new page
	while ( aBufferPos < size )	
	{
		/// Allocate a page
		if ( NF_INVALID_PAGE_ID != (aCurPage = Fm_GetFreePage()) )		
		{
			if ( (aCurPos % EF_SECTOR_SIZE + aSize) >= EF_SECTOR_SIZE )
			{
				aWrittenSize = EF_SECTOR_SIZE - (aCurPos % EF_SECTOR_SIZE);
			}
			else
			{
				aWrittenSize = aSize;
			}

			if ( aWrittenSize < EF_SECTOR_SIZE )
				Nf_ReadPage( aCurPage, 1, aTempBuf );

			memcpy( (void*)&aTempBuf[aCurPos%EF_SECTOR_SIZE], (void*)&buffer[aBufferPos], aWrittenSize );
			Nf_WritePage( aCurPage, 1, aTempBuf );

			/// Set page to used
			Fm_UsePage( aCurPage );

			/// Append to file page list
			if ( aNextPage == NF_INVALID_PAGE_ID )	///< the first page in list
			{
				nFatTable[fileID].uStartPage = aCurPage;
			}
			else
			{
				nSetpageListNext( aNextPage, aCurPage );
			}
			
			nFatChangeCount++;
			
			/// Postion forward
			aCurPos += aWrittenSize;
			aSize -= aWrittenSize;
			aBufferPos += aWrittenSize;
			aNextPage = aCurPage;		

			if ( aSize <= 0 )
			{				
				break;
			}
		}
		else
		{
			break;
		}
	}


	if ( (nOpenFileTable[fileID].uCurPos + aBufferPos) > nFatTable[fileID].uLength )
	{
		nFatTable[fileID].uLength = nOpenFileTable[fileID].uCurPos + aBufferPos;
		nSetpageListNext( aNextPage, NF_INVALID_PAGE_ID );
	}
	
	/// Update FAT entry
	/// nSaveFatTable( fileID );
	
	/// Update manage table
	nOpenFileTable[fileID].uCurPos = aCurPos;

	EnableIrq();
	
	return aBufferPos;	
}

U8  efUnmount( )
{
	Fm_Flush();
	efFlushFAT();

	Nf_FlushBlockBuffer();

	return 1;
}	

U8  efSaveFS( )
{
	DisableIrq();

	Fm_Flush();
	efFlushFAT();

	Nf_FlushBlockBuffer();

	EnableIrq();
	
	return 1;
}	

void  efFlushFAT()
{
	/// Update FAT entry
	nSaveFatTable( );
}

void efFlush( U16 fileID )
{
	DisableIrq();
	
	efFlushFAT();
	Nf_FlushBlockBuffer();
	
	EnableIrq();
}

/// Get File size.
/// invlaid file return 0
U32 efGetSize( U16 fileID )
{
	U32 aSize = 0;
	
	if ( IsValidID(fileID) )
	{
		if ( nFatTable[fileID].uFlag == 1 )
		{
			aSize = nFatTable[fileID].uLength;
		}
	}
	
	return aSize;
}

///  Set file's size
///  return 1---ok, 0 ---failed or invalid file
U8  efSetSize( U16 fileID )
{
	
	return 1;
}

/// Delete file by ID
/// return 1 ---ok, 0 ---failed or invalid file
U8 efDeleteByID( U16 fileID )
{	
	U8 aRet = 0;
	
	if ( !IsValidID(fileID) )
	{
		return 0;
	}
	
	if ( (nFatTable[fileID].uFlag == 1) && (nOpenFileTable[fileID].bOpened == 0) )
	{
		/// Release page 		
		nReleasePageList( nFatTable[fileID].uStartPage );
		
		/// reset fat table
		nFatTable[fileID].uFlag = 0;
		nFatTable[fileID].uID = NF_INVALID_FILE_ID;
		nFatTable[fileID].uLength = 0;
		nFatTable[fileID].uStartPage = NF_INVALID_PAGE_ID;

		nFatChangeCount++;		
		nSaveFatTable(); ///< save fat table immediately

		aRet = 1;
	}
	
	return aRet;	
}

/// Delete file by name
/// return 1 ---ok, 0 ---failed or invalid file
U8 efDelete( char *name )
{
	U8 aRet = 0;
	U16 aFileID = nFindFileByName( name );

	if ( (aFileID != NF_INVALID_FILE_ID) && (nOpenFileTable[aFileID].bOpened == 0) )
	{
		/// Release page 		
		nReleasePageList( aFileID );

		nFatTable[aFileID].uFlag = 0;
		nFatTable[aFileID].uID = NF_INVALID_FILE_ID;
		nFatTable[aFileID].uLength = 0;
		nFatTable[aFileID].uStartPage = NF_INVALID_PAGE_ID;

		nFatChangeCount++;
		aRet = 1;
	}
	
	return aRet;	
}

U16  efSearchFileByExt( char* pName, char **pFileID, U16 size )
{
	return nSearchFileByExt( pName, pFileID, size );
}

U8 efRenameByID( U16 fileID, const char* pszDst )
{
	U16 i;
	U16 aFileId = NF_INVALID_FILE_ID;

	if ( !pszDst )
		return 0;
	
	for ( i = 0; i < NF_FILE_MAX; i++ )
	{
		if ( nFatTable[i].uFlag == 1 )
		{
			if ( fileID == nFatTable[i].uID )
			{
				strcpy( nFatTable[i].Name, pszDst );
				break;
			}			
		}		
	}
		
	return 1;
}

U8 efRename( char* pszSrc, const char* pszDst )
{
	U16 i;
	U16 aFileId = NF_INVALID_FILE_ID;

	if ( !pszDst )
		return 0;
	
	for ( i = 0; i < NF_FILE_MAX; i++ )
	{
		if ( nFatTable[i].uFlag == 1 )
		{
			if ( 0 == strcmp( nFatTable[i].Name, pszSrc ) )
			{
				strcpy( nFatTable[i].Name, pszDst );
				break;
			}			
		}		
	}
		
	return 1;
}


/// Format FAT table
U8 efFromatFAT()
{
	U16 i;
	
	DisableIrq();
	
	memset( nOpenFileTable, 0, sizeof(nOpenFileTable) );	
	
	/// set to page manage table
	nExistFileCnt = 0;
	memset( nFatTable, 0, sizeof(nFatTable) );		
	for ( i = 0; i < NF_FILE_MAX; i++ )
	{
		nFatTable[i].uID = NF_INVALID_FILE_ID;
		nFatTable[i].uStartPage = NF_INVALID_PAGE_ID;
	}
	
	nFatChangeCount = 100;
	nSaveFatTable( );
	
	EnableIrq();	
	return 1;
}

/// Create File system, including Format Flash, Create FAT, etc, must be called firstly
/// bRecreate: NF_ERASE --Erase flash, NF_FORMAT ---Format flash, NF_NORMAL -- only create Fs, all data will be lost.
U8  efMount( U8 bFormat )
{
	U8 aRet = 1;

	DisableIrq();

	if ( NF_ERASE == bFormat )
	{
		/// Erase flush
		EraseFlush();		
	}

	/// Must Format flush when Erase flush 
	if ( NF_FORMAT == bFormat || NF_ERASE == bFormat )
	{
		/// Format Flash
		Nf_Format( NF_PAGE_MAX );
	}

	Nf_Open();
	
	if ( NF_NORMAL != bFormat )
	{
		/// Create page manage table
		Fm_FormatPageTable();
		/// Create FS
		efFromatFAT();
	}
	
	Fm_Open();
	efInit();

	EnableIrq();

	return aRet;
}

