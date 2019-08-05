#ifndef __ENM_FAT_H__
#define __ENM_FAT_H__

#include "FlashMng.h"

#define      EF_SECTOR_SIZE			NF_PAGE_DATA_LEN
#define      EF_SECTOR_SPARE_SIZE	NF_PAGE_SPARE_LEN



#define   	NF_FILE_MAX				(NF_FAT_COUNT * 16 - 1)

#define  	NF_INVALID_FILE_ID		((U16)-1)
#define  	NF_INVALID_PAGE_ID		((U16)-1)

#define   	NF_NORMAL				0		///< create fs only
#define   	NF_ERASE				1		///< Erase flash
#define   	NF_FORMAT				2		///< Format flash
 

#define  	EF_OPEN_READ			0x0001
#define 	EF_OPEN_WRITE			0x0002
#define 	EF_OPEN_CREATE			0x0004

#define	ESEEK_SET				0
#define	ESEEK_CUR				1
#define	ESEEK_END				2


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

/// Create File system, including Format Flash, Create FAT, etc, must be called firstly
/// bRecreate: 1 --Format flash 0 -- only create Fs, all data will be lost.
U8  efMount( U8 bFormat );

/// flush data to flash , must be called before exit
U8  efUnmount( );

/// Init  FAT table, must be called firstly
U8  efInit();

///   Rebuild FAT, all files will be deleted.
U8  efFormatFAT();

U8  efSaveFS( );

///   Rebuild FAT, including page manage table, all files will be deleted.
U8  efMount( U8 bFormat );

/// Save FAT table to flush. must be called by user
void  efFlushFAT();

/// Open a file,  return file ID
/// openFlag:  EF_OPEN_CREATE, EF_OPEN_READ, EF_OPEN_WRITE
/// return -1 ---failed
U16 efOpen( char * szName, U16 openFlag );

/// Close a file
void efClose( U16 fileID );

///  read file
/// return read size
U32 efRead( U8* buffer, U32 size, U16 fileID );

///  write file
/// return read size
U32 efWrite( U8* buffer, U32 size, U16 fileID );

///  flush file
void efFlush( U16 fileID );

///  seek file
void efSeek( U16 fileID, U32 pos, U16 from );

/// Get File size.
/// invlaid file return 0
U32 efGetSize( U16 fileID );

///  Set file's size
///  return 1---ok, 0 ---failed or invalid file
U8  efSetSize( U16 fileID );

/// Delete file by ID
/// return 1 ---ok, 0 ---failed or invalid file
U8 efDeleteByID( U16 fileID );

/// Delete file by name
/// return 1 ---ok, 0 ---failed or invalid file
U8 efDelete( char *name );

U8 efRenameByID( U16 fileID, const char* pszDst );
U8 efRename( char* pszSrc, const char* pszDst );

/// find files by its' ext-name
/// return found files count
U16  efSearchFileByExt( char* pName, char **pFileID, U16 size );

#ifdef __cplusplus
}
#endif

#endif /*__ENM_FAT_H__*/