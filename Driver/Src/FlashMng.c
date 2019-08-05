#include  "FlashMng.h"


#define   NF_BLOCK_MNG_START		0
#define   NF_BLOCK_MNG_COUNT		5
#define   NF_PAGE_MNG_START		(NF_BLOCK_MNG_START + NF_BLOCK_MNG_COUNT)
#define   NF_PAGE_MNG_COUNT		8


/// Page status define
#define	NF_BLOCK_FREE	0x03
#define	NF_BLOCK_USED	0x02
#define	NF_BLOCK_DIRTY	0x00
#define	NF_BLOCK_BAD	0x01

static const unsigned short FLASH_ID = 0xEC76;

///  Page used manage table,  2 bit ---> 1page
static U8   nPageLookupTable[NF_PAGE_MAX / 4]; 
static U16  nFreePageCount = 0;		///< free page count
static U16  nLookupTableChangeCount = 0;	///< page table change counter, for erase gate

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/////  Inner routines

U16 nCountFreePage( U8 byte )
{
	U16 aCount = 0;
	U8 i = 0;
	while ( i < 4 )
	{
		if ( NF_BLOCK_FREE == ((byte >> i*2 ) & 0x03) )
			aCount++;
			
		i++;
	}
	
	return aCount;
}

void nUsePage( U8* byte, U8 pos )
{
	*byte = *byte& ~(U8)(0x03 << (pos*2) );
	*byte = *byte | (U8)(NF_BLOCK_USED << (pos*2) );
}

void nFreePage( U8* byte, U8 pos )
{
	*byte = *byte & ~(U8)(0x03 << (pos*2) );
	*byte = *byte | (U8)(NF_BLOCK_FREE << (pos*2) );
}

U16 nFindFreePage( U8 byte )
{
	U16 i = 0;
	while ( i < 4 )
	{
		if ( ((byte >> (i*2)) & 0x03) == NF_BLOCK_FREE  )
			break;
		
		i++;	
	}
	
	if ( i >= 4 )
		i = (U16)-1;
	return i;
}

void  nResetPageLookupTable( )
{
	memset( nPageLookupTable, 0xFF, sizeof(nPageLookupTable) );
}

U16   Page2TablePage( U16 page )
{
	U16 i ;
					
	/// Find a free page
	i = page / 4;	
	i = (i) / NF_PAGE_DATA_LEN;
	
	return i + NF_PAGE_MNG_START;
}

static U16  Fm_LoadPageTable(  )
{
	U16 i, aPageCount;
	U8   aTableData[( NF_PAGE_DATA_LEN+NF_PAGE_SPARE_LEN)] = { 0 };
	U8  *apTable = (U8*)nPageLookupTable;
	
	/// read page manage data
	for ( i = NF_PAGE_MNG_START; i < (NF_PAGE_MNG_START + NF_PAGE_MNG_COUNT); i++)
	{
		_RdPage( 0, i, aTableData ); 
		/// set to page manage table
		memcpy( apTable, aTableData, NF_PAGE_DATA_LEN  );
		apTable += NF_PAGE_DATA_LEN;
	}
	
	/// calculate free page count
	aPageCount = 0;
	apTable = (U8*)nPageLookupTable;
	for ( i = 0; i < Nf_GetPageCount() / 4; i++)
	{
		aPageCount += nCountFreePage( *apTable) ;
		apTable++;
	}
	
	return aPageCount;
}	


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if    1

static U16 blockLookupTable[NF_BLOCK_MAX+8]; //logical blockId # ->physical block #
	                    	     //8 is spare area for loading 528bytes.
static U16 lastPage=0;
static U16 isOpen=0;

typedef struct 
{
	U8	nBlockCache[NF_PAGE_PER_BLOCK][(NF_PAGE_DATA_LEN+NF_PAGE_SPARE_LEN)];	///< Block cache buffer, for page write erase 
	U8  u8AccessType[NF_PAGE_PER_BLOCK];	///< 0 ---None, 1 --Read, 2 ---Write
		#define  NF_ACCESS_NONE		0
		#define  NF_ACCESS_READ		1
		#define  NF_ACCESS_WRITE	2

	U16 u16BlockId;			///< buffered block id
	U8  bChanged;
}tagBlockBuffer;
static tagBlockBuffer  nBlockBuffer;

#define PAGE2BLOCKID(page32)	(page32>>5)

/// Init block buffer
void nInitBlockBuffer()
{
	nBlockBuffer.bChanged = 0;
	nBlockBuffer.u16BlockId = NF_INVALIDE_BLOCK_ID;
	memset( nBlockBuffer.nBlockCache, 0, sizeof(nBlockBuffer.nBlockCache) );
	memset( nBlockBuffer.u8AccessType, 0, sizeof(nBlockBuffer.u8AccessType) );
}

U8  nIsBlockBuffered( U16 block )
{
	return (nBlockBuffer.u16BlockId == block ) ? 1 : 0;
}

U16	nFlushBlockBuffer( )
{
	U16 i;
	U16 aBlock;

	if ( 0 == nBlockBuffer.bChanged )
		return 0;

	if ( NF_INVALIDE_BLOCK_ID == nBlockBuffer.u16BlockId )
		return 0;
	
	/// Erase block
	aBlock = nBlockBuffer.u16BlockId;
	_EraseBlock( aBlock );

	for ( i = 0; i < NF_PAGE_PER_BLOCK; i++ )
	{
		_WrPageAll( aBlock, i, (U8*)nBlockBuffer.nBlockCache[i] );
		nBlockBuffer.u8AccessType[i] = NF_ACCESS_NONE;
	}
	
	nBlockBuffer.bChanged = 0;
	//nBlockBuffer.u16BlockId = NF_INVALIDE_BLOCK_ID;	
	
	return 1;
}

void nEmptyBlockBuffer()
{
	nFlushBlockBuffer();
	nBlockBuffer.u16BlockId = NF_INVALIDE_BLOCK_ID;	
}

U16 nLoadBlockData( U16 block )
{
	U16 i;
	
	if ( block != nBlockBuffer.u16BlockId )
		nFlushBlockBuffer();

	for ( i = 0; i < NF_PAGE_PER_BLOCK; i++ )
	{
		_RdPageAll( block, i, (U8*)nBlockBuffer.nBlockCache[i] );
		nBlockBuffer.u8AccessType[i] = NF_ACCESS_NONE;
	}
	
	nBlockBuffer.bChanged = 0;
	nBlockBuffer.u16BlockId = block;	
	
	return 1;
}

U16	nReadPageFromBuffer( U16 page, U8* pDst, U8 type )
{
	if (  1 == type )
	{
		memcpy( pDst, nBlockBuffer.nBlockCache[page], NF_PAGE_DATA_LEN );
	}
	else
	{
		memcpy( pDst, &nBlockBuffer.nBlockCache[page][NF_PAGE_DATA_LEN], NF_PAGE_SPARE_LEN );
	}

	nBlockBuffer.u8AccessType[page ] = NF_ACCESS_READ;

	return 1;
}

/// type: 1 --- data, 0 ---spare data
U16	nWritePageToBuffer( U16 page, U8* pSrc, U8 type  )
{
	if (  1 == type )
	{
		memcpy( nBlockBuffer.nBlockCache[page], pSrc, NF_PAGE_DATA_LEN );
	}
	else
	{
		memcpy( &nBlockBuffer.nBlockCache[page][NF_PAGE_DATA_LEN], pSrc, NF_PAGE_SPARE_LEN );
	}
	
	nBlockBuffer.u8AccessType[page] = NF_ACCESS_WRITE;

	nBlockBuffer.bChanged = 1;

	return 1;
}



U16 Nf_Page2Block( U16 page )
{
	return blockLookupTable[PAGE2BLOCKID(page)];
}

void  Nf_FlushBlockBuffer()
{
	nFlushBlockBuffer();
}

void EraseFlush( )
{
	int i;
	for ( i = 0; i < 1024; i++ )
		_EraseBlock( i );
		
}
	
/// Check block, and establish block mapping table.
/// this should be done before using Flash memory
/// param: page32---total pages in flash device
U16 Nf_Format(int page32)
//returns the allocated good block excluding BIT. 
{
    U16 i;
    U16 error=0,retry=0;
    U16 blockCnt;
    U16 blockId;
    U8 buffer[NF_PAGE_SIZE];
    unsigned int bp0,bp1;
    U8 *phyStTbl=(U8 *)blockLookupTable;

    if((page32%NF_PAGE_PER_BLOCK)!=0)
    	return 0;
    	
    blockCnt=page32/NF_PAGE_PER_BLOCK;
    
    if(_CheckId()!= FLASH_ID ) //check KM29U128T
    {
	return 0;
    }

    /// check if the flash had been formatted.
    _RdPage(0,0,buffer);
    if(buffer[0]=='N')
    	return 0;
    
    blockId=0;
 
    phyStTbl[0]=0xf5;  //For BIT
    phyStTbl[1]=0xf5;  
    
    for(i=NF_USER_BLOCK_START;i<NF_BLOCK_MAX;i++)
    {
		_RdPageSpare(i,0,buffer);bp0=buffer[6];
		_RdPageSpare(i,0,buffer);bp1=buffer[6];

		if(bp0==0xff && bp1==0xff)
		{  //good block
			
			if(blockId < blockCnt)
			{
			//good block have blcokId
			phyStTbl[2*i]=blockId &0xff;  
			phyStTbl[2*i+1]=(blockId>>8) &0xff;  
	    		blockId++; 
			}
			else
			{
			phyStTbl[2*i]=0xff;  //unused good block have 0xffff instead of blcokId
			phyStTbl[2*i+1]=0xff;  
			}
		}
        else 
		{   //bad block
			//bad block have 0xffff instead of blcokId
			phyStTbl[2*i]=0xf0; 
			phyStTbl[2*i+1]=0xf0; 
		}
    }

    while(1)
    {
		for(i=0;i<NF_PAGE_SIZE;i++)buffer[i]=0;
		memcpy(buffer,"NAND FLASH",sizeof("NAND FLASH") );	
		if(!_WrPage(0,0,buffer))error++;

		memcpy(buffer,phyStTbl+0x0, NF_PAGE_DATA_LEN);
		if(!_WrPage(0,1,buffer))error++;

		memcpy(buffer,phyStTbl+NF_PAGE_DATA_LEN, NF_PAGE_DATA_LEN);
		if(!_WrPage(0,2,buffer))error++;
    
		memcpy(buffer,phyStTbl+NF_PAGE_DATA_LEN*2, NF_PAGE_DATA_LEN);
		if(!_WrPage(0,3,buffer))error++;
    
		memcpy(buffer,phyStTbl+NF_PAGE_DATA_LEN*3, NF_PAGE_DATA_LEN);
		if(!_WrPage(0,4,buffer))error++;
		if(error==0)break;
		error=0;
		if(++retry==3)return 0; //retry error!!! unusable NAND flash

		_EraseBlock(0); //erase physical block #0 for retry
    }

    return blockId; //returns the allocated good block excluding BIT. 
}


U16 Nf_Open(void)
//returns the last page #
{
    U16 i,j;
    U8 buffer[NF_PAGE_DATA_LEN*4+NF_PAGE_SPARE_LEN];
    U16 lBlock=0;

	/// Init Block Buffer
	nInitBlockBuffer();

    if(_CheckId()!= FLASH_ID ) //check KM29U128T
    {
	return 0;
    }

    _RdPage(0,0,buffer+0); //check if the flash is formatted.
    if(buffer[0]!='N')return 0;
    
    _RdPage(0,1,buffer+0);
    _RdPage(0,2,buffer+NF_PAGE_DATA_LEN);
    _RdPage(0,3,buffer+NF_PAGE_DATA_LEN*2);
    _RdPage(0,4,buffer+NF_PAGE_DATA_LEN*3);
    
    for(i=0;i<NF_BLOCK_MAX;i++)
    	blockLookupTable[i]= NF_INVALIDE_BLOCK_ID;

    for(i=NF_USER_BLOCK_START;i<NF_BLOCK_MAX;i++)
    {
		j=(U8)buffer[i*2]+((U8)buffer[i*2+1])*0x100;
		if(j!=0xf0f0 && j!=0xffff)
		{blockLookupTable[j]=i;lBlock++;}
    }
    lastPage=(lBlock)*NF_PAGE_PER_BLOCK-1; //total page count.
    
    //check the Block Information Table Integrity.
    for(i=NF_USER_BLOCK_START;i<lBlock;i++)
    {
		if(blockLookupTable[i]==NF_INVALIDE_BLOCK_ID)
			return 0; //error!!!
    }

    isOpen=1;
    return lastPage;
}   

U16 Nf_GetPageCount( )
{
	return  isOpen ? lastPage : 0;
}

U16 Nf_EraseBlock(unsigned int page32)
{
    if((page32%NF_PAGE_PER_BLOCK)!=0)
		return 0; //page32[4:0] has to be 00000.

    return _EraseBlock( blockLookupTable[PAGE2BLOCKID(page32)] );
}

U16 Nf_ReadPage( U16 startPage, U16 pageCount, U8 *buffer)
{
    U16 page=startPage;
    U8 *pPage=buffer;
	U16 aBlockId = blockLookupTable[PAGE2BLOCKID(page)];
	U16 aPageId  = (page&0x1f);
	
	if ( NF_INVALIDE_BLOCK_ID == aBlockId )
		return 0;
	
	/// if block is not buffered, load the block
	if ( !nIsBlockBuffered( aBlockId) )
	{
		nLoadBlockData( aBlockId );
	}

	nReadPageFromBuffer( aPageId, buffer, 1 );


//    for(i=0;i<pageCount;i++)
//    {
//    	_RdPage(blockLookupTable[PAGE2BLOCKID(page)],(page&0x1f),pPage);
//		pPage+=NF_PAGE_DATA_LEN;
//		page++;
//    }

    return 1;
}

U16 Nf_WritePage( U16 startPage, U16 pageCount, U8 *buffer)
{
	U16 page=startPage;
	U8 *pPage=buffer;
	U16 aBlockId = blockLookupTable[PAGE2BLOCKID(page)];
	U16 aPageId  = (page&0x1f);
	
	if ( NF_INVALIDE_BLOCK_ID == aBlockId )
		return 0;
	
	/// if block is not buffered, load the block
	if ( !nIsBlockBuffered( aBlockId) )
	{
		nLoadBlockData( aBlockId );
	}

	nWritePageToBuffer( aPageId, buffer, 1 );

//    for(i=0;i<pageCount;i++)
//    {
//    	if(!_WrPage(blockLookupTable[PAGE2BLOCKID(page)],(page&0x1f),pPage))
//	    return 0;
//	pPage+=NF_PAGE_DATA_LEN;
//	page++;
//    }

    return 1;
}

U16 Nf_ReadPageSpare( U16 page,  U8 *buffer)
{
	U16 aBlockId = blockLookupTable[PAGE2BLOCKID(page)];
	U16 aPageId  = (page&0x1f);
	
	if ( NF_INVALIDE_BLOCK_ID == aBlockId )
		return 0;
	
	/// if block is not buffered, load the block
	if ( !nIsBlockBuffered( aBlockId) )
	{
		nLoadBlockData( aBlockId );
	}

	nReadPageFromBuffer( aPageId, buffer, 0 );

    return 1;
}

U16 Nf_WritePageSpare( U16 page, U8 *buffer)
{  
	U16 aBlockId = blockLookupTable[PAGE2BLOCKID(page)];
	U16 aPageId  = (page&0x1f);
	
	if ( NF_INVALIDE_BLOCK_ID == aBlockId )
		return 0;
	
	/// if block is not buffered, load the block
	if ( !nIsBlockBuffered( aBlockId) )
	{
		nLoadBlockData( aBlockId );
	}

	nWritePageToBuffer( aPageId, buffer, 0 );

	return 1;
}

int Nf_Add1Block(void)
{
    if(_RepAdd1Block(0)==0)return 0;
    return Nf_Open();
}

int Nf_Replace1Block(int page32)
{
    if(_RepAdd1Block(PAGE2BLOCKID(page32))==0)return 0;
    return Nf_Open();
}

int _RepAdd1Block(int badBlockId)
//if badBlockId==0, replacement doesn't occur.
//blockId is blockLookupTable entry. 0=BIT table, 1=page0-31, 2=page32-63
{
    int i;
    int error=0,retry=0;
    U8 buffer[NF_PAGE_SIZE];
    U8 reserved[NF_PAGE_SIZE];
    unsigned int phyBadBlock=0,phyNewBlock,newBlockId;
    U8 *phyStTbl=(U8 *)blockLookupTable;
    
    if(isOpen==0)return 0;

    if(badBlockId!=0) //must remove the bad block.
    {
		newBlockId=badBlockId;
		phyBadBlock=blockLookupTable[badBlockId];
		_RdPage(0,0,reserved);
		_RdPage(0,1,phyStTbl+0x0);
		_RdPage(0,2,phyStTbl+NF_PAGE_DATA_LEN);
		_RdPage(0,3,phyStTbl+NF_PAGE_DATA_LEN*2);
		_RdPage(0,4,phyStTbl+NF_PAGE_DATA_LEN*3);
		*((U16 *)phyStTbl+phyBadBlock)=0xf0f0;  //mark bad block
    }
    else
    {
		newBlockId=(lastPage+1)/NF_PAGE_PER_BLOCK;
		_RdPage(0,0,reserved);
		_RdPage(0,1,phyStTbl+0x0);
		_RdPage(0,2,phyStTbl+NF_PAGE_DATA_LEN);
		_RdPage(0,3,phyStTbl+NF_PAGE_DATA_LEN*2);
		_RdPage(0,4,phyStTbl+NF_PAGE_DATA_LEN*3);
    }

    for(i=1;i<NF_BLOCK_MAX;i++)
    {
        if(*((U16 *)phyStTbl+i)==0xffff)break;
    }

    if(i==NF_BLOCK_MAX)return 0;    //no more available empty good block.

    phyNewBlock=i;

    *((U8 *)((U16 *)phyStTbl+phyNewBlock))=0xff & newBlockId;  //replace mark bad block
    *((U8 *)((U16 *)phyStTbl+phyNewBlock)+1)=0xff & (newBlockId>>8);  
    
    while(1)
    {
		_EraseBlock(0); //erase physical block #0

		//buffer[NF_PAGE_SIZE] has the ROM writer maker's information.
		if(!_WrPage(0,0,reserved))error++;
    
		for(i=0;i<NF_PAGE_SIZE;i++)buffer[i]=0;   
 
		memcpy(buffer,phyStTbl+0x0,NF_PAGE_DATA_LEN);
		if(!_WrPage(0,1,buffer))error++;

		memcpy(buffer,phyStTbl+NF_PAGE_DATA_LEN,NF_PAGE_DATA_LEN);
		if(!_WrPage(0,2,buffer))error++;
    
		memcpy(buffer,phyStTbl+NF_PAGE_DATA_LEN*2,NF_PAGE_DATA_LEN);
		if(!_WrPage(0,3,buffer))error++;
    
		memcpy(buffer,phyStTbl+NF_PAGE_DATA_LEN*3,NF_PAGE_DATA_LEN);
		if(!_WrPage(0,4,buffer))error++;
		if(error==0)break;
		error=0;
		if(++retry==3)return 0; //retry error!!! unusable NAND flash
    }

    return 1; 
}

#endif 	/// #ifdef  ENM_VER_TARGET

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Lookup a free page
U16 Fm_GetFreePage( void )
{
	U16 i , j;
	
	/// Find a free page
	for ( i = 0; i <  nFreePageCount / 4; i++)
	{
		if ( (j  = nFindFreePage(nPageLookupTable[i]) ) != (U16)-1 )
		{
			break;
		}		
	}
	
	if ( i < nFreePageCount / 4 )
	{
		i = i * 4 + j;
	}
	else
	{
		i = -1;
	}
	
	return i;
}

///  Use page. Flash Manager will set used flag
///  Return: 1 ---ok, 0 ---failed
U8 Fm_UsePage( U16 pageNum )
{
	U16 i;
	U8  j;
	//U16 aPage;
	//U8  aBuf[NF_PAGE_SIZE];
	
	if (  pageNum >= NF_PAGE_MAX )
		return 0;
				
	/// Find a free page
	i = pageNum / 4;
	j = pageNum % 4;
	
	nUsePage( &nPageLookupTable[i], j );
		
	nLookupTableChangeCount++;

//// don't write to flush now, user can call Fm_Flush() to do it
/*		
	/// Save to block
	aPage = Page2TablePage( pageNum);
    	_RdPage( 0, aPage, aBuf ); 
    	memcpy( aBuf,  &nPageLookupTable[i/NF_PAGE_DATA_LEN], NF_PAGE_DATA_LEN );
	_WrPage( 0, aPage, &nPageLookupTable[i/NF_PAGE_DATA_LEN] ); 
*/		
	return 1;	
}

///  free page. Flash Manager will set free flag
///  Return: 1 ---ok, 0 ---failed
U8 Fm_FreePage( U16  pageNum )
{
	U16 i;
	U8  j;
	//U8  aBuf[NF_PAGE_SIZE];
	//U16 aPage;
	
	if (  pageNum >= NF_PAGE_MAX )
		return 0;
				
	/// Find a free page
	i = pageNum / 4;
	j = pageNum % 4;
	
	nFreePage( &nPageLookupTable[i], j );

	nLookupTableChangeCount++;

	return 1;	
}

U8  Fm_Flush()
{
	U16 i;
	U8  *apTable;
	U8  aBuf[NF_PAGE_SIZE * NF_PAGE_PER_BLOCK];

	if ( nLookupTableChangeCount )
	{
		/// _erase block 0
		apTable = aBuf;
		for ( i = 0; i < NF_PAGE_PER_BLOCK; i++)
		{
		    _RdPageAll( 0, i, apTable ); 
		    apTable += NF_PAGE_SIZE;
		}
		
		_EraseBlock( 0 );
		
		/// write page manage data
		apTable = aBuf;
		for ( i = 0; i < NF_PAGE_MNG_START; i++)
		{
		    _WrPageAll( 0, i, apTable ); 
		    apTable += NF_PAGE_SIZE;
		}

		for ( i = 0; i < NF_PAGE_MNG_COUNT; i++)
		{
		    memcpy( apTable, &nPageLookupTable[i*NF_PAGE_DATA_LEN],  NF_PAGE_DATA_LEN );
		    _WrPageAll( 0, i + NF_PAGE_MNG_START, apTable ); 
		    apTable += NF_PAGE_SIZE;
		}
		
		for ( i = (NF_PAGE_MNG_START + NF_PAGE_MNG_COUNT); i < NF_PAGE_PER_BLOCK; i++)
		{
		    _WrPageAll( 0, i, apTable ); 
		    apTable += NF_PAGE_SIZE;
		}

		nLookupTableChangeCount = 0;		
	}
	
	return 1;		
}

///  free all page. Flash Manager will set free flag
///  Return: 1 ---ok, 0 ---failed
U8 Fm_FreeAllPage( void )
{
	/// Init User black zone
	nResetPageLookupTable( );
	
	/// Load page manage table
	nFreePageCount = Nf_GetPageCount();
	
	nLookupTableChangeCount = nFreePageCount;
	Fm_Flush();
	
	return 1;
}

///  Format page manage table
U16  Fm_FormatPageTable( )
{
	/// Clear manage page
	Fm_FreeAllPage();
	
	return Nf_GetPageCount();
}

U16 Fm_GetFreePageCount(void)
{
	return 	nFreePageCount;
}

U16 Fm_Open()
{
	/// Init User black zone
	nResetPageLookupTable();
	
	/// Load page manage table
	nFreePageCount = Fm_LoadPageTable();

	return nFreePageCount;		
}
