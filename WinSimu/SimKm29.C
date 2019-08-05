#include <string.h>

#include "km29ux.h"

#define PAGE2BLOCKID(page32)	(page32>>5)


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/// Simulate Flash

typedef struct  
{
	U8   buf[528];
}tagPage;

typedef struct 
{
	tagPage  page[32];
}tagBlock;

typedef struct 
{
	tagBlock  block[1024];
}tagDevice;

tagDevice  nDevice;

#include "stdio.h"
FILE *file;

void InitFlash()
{

	memset( &nDevice, 0xff, sizeof(nDevice) );

	if ( file = fopen( "FlashTest.dat", "rb" ) )
	{	
		fread( &nDevice, sizeof(nDevice), 1, file );
	}
	else
	{
		//memset( &nDevice, 0xff, sizeof(nDevice) );
		file = fopen( "FlashTest.dat", "wb" );
		fwrite( &nDevice, sizeof(nDevice), 1, file );
	}
	fclose( file );
}

void CloseFlash()
{
	if ( file = fopen( "FlashTest.dat", "wb" ) )
	{	
		fwrite( &nDevice, sizeof(nDevice),1, file );
		fclose( file );
	}
}

U16 _EraseBlock(unsigned int block)
{
    U16 i;
	tagBlock *ap;

	if ( block > 1023 )
		return 0;

	ap = &nDevice.block[block];

	for ( i = 0; i < 32; i++ )
	{
		memset( (void*)&ap->page[i], 0xff, 528 );
	}

	return 1;
}

U16 _WrPage(unsigned int block,unsigned int page,U8 *pPage)  
{
	if ( block > 1023 || page > 31 )
		return 0;

	memcpy( (void*)&nDevice.block[block].page[page], (void*)pPage, 512 );

	return 1;

}

U16 _RdPage(unsigned int block,unsigned int page,U8 *pPage)  
{
	if ( block > 1023 )
		return 0;

	memcpy( (void*)pPage, (void*)&nDevice.block[block].page[page], 512 );

	return 1;
}

U16 _WrPageSpare(unsigned int block,unsigned int page,U8 *pPage)  
{
	if ( block > 1023 || page > 31 )
		return 0;

	memcpy( (void*)&nDevice.block[block].page[page].buf[512], (void*)pPage, 16 );

	return 1;

}

U16 _RdPageSpare(unsigned int block,unsigned int page,U8 *pPage)  
{
	if ( block > 1023 )
		return 0;

	memcpy( (void*)pPage, (void*)&nDevice.block[block].page[page].buf[512], 16 );

	return 1;
}

U16 _WrPageAll(unsigned int block,unsigned int page,U8 *pPage)  
{
	if ( block > 1023 || page > 31 )
		return 0;

	memcpy( (void*)&nDevice.block[block].page[page], (void*)pPage, 528 );

	return 1;

}

U16 _RdPageAll(unsigned int block,unsigned int page,U8 *pPage)  
{
	if ( block > 1023 )
		return 0;

	memcpy( (void*)pPage, (void*)&nDevice.block[block].page[page], 528 );

	return 1;
}


U16 _CheckId(void)
{
	return 0xec76;
}

