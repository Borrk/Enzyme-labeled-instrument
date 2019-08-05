#ifndef    __FLASH_MNG_H__
#define   __FLASH_MNG_H__

#include "km29ux.h"

/// Flash parameter define
#define   NF_BLOCK_MAX			1024
#define   NF_PAGE_PER_BLOCK	32
#define   NF_PAGE_DATA_LEN		512
#define   NF_PAGE_SPARE_LEN	16

#define   NF_PAGE_SIZE			(NF_PAGE_DATA_LEN +NF_PAGE_SPARE_LEN )
#define   NF_PAGE_MAX			(NF_PAGE_PER_BLOCK * NF_BLOCK_MAX)

/// FAT is in block 1, 
/// first page is FAT flag: "Enm File system"
/// others are for fat data
#define   NF_FAT_START			0
#define   NF_FAT_COUNT			(NF_PAGE_PER_BLOCK - 1)


#define  NF_REVERSED_BLOCK	(2)
#define  NF_USER_BLOCK_START	(2)
#define  NF_FREE_BLOCK		(NF_BLOCK_MAX - NF_USER_BLOCK_START)


#ifdef __cplusplus
extern "C" {
#endif

void EraseFlush( );	///< Erase Flush
U16 Nf_Format(int page32);  //returns the allocated good block excluding BIT. 
U16 Nf_Open(void);  //returns the last page #
void  Nf_FlushBlockBuffer();

U16 Nf_EraseBlock(unsigned int page32);

U16 Nf_WritePage( U16 startPage, U16 pageCount, U8 *buffer);
U16 Nf_ReadPage( U16 startPage, U16 pageCount, U8 *buffer);
U16 Nf_WritePageSpare( U16 page, U8 *buffer );
U16 Nf_ReadPageSpare( U16 page, U8 *buffer );

int Nf_Add1Block(void); //returns the last page #
int Nf_Replace1Block(int page32); //returns the last page #
U16 Nf_GetPageCount( );
U16 Nf_Page2Block( U16 page );



/// Init  Flash, must be called firstly
U8  Fm_InitFlash();

///  Format page manage table
U16  Fm_FormatPageTable( );

/// Lookup a free page
U16 Fm_GetFreePage( void );

///  Use page. Flash Manager will set used flag
///  Return: 1 ---ok, 0 ---failed
U8 Fm_UsePage( U16 pageNum );

///  free page. Flash Manager will set free flag
///  Return: 1 ---ok, 0 ---failed
U8 Fm_FreePage( U16  page );

///  free all page. Flash Manager will set free flag
///  Return: 1 ---ok, 0 ---failed
U8 Fm_FreeAllPage( void );

///  Quary free page count
U16 Fm_GetFreePageCount(void);

///  Flush manage table to flush
U8  Fm_Flush();

U16 Fm_Open();


#ifdef __cplusplus
}
#endif


#endif /*__FLASH_MNG_H__*/