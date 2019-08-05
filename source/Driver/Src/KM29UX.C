#include <string.h>
#include "..\inc\44b.h"
//#include "..\inc\44blib.h"
//#include "..\inc\def.h"
#include "..\inc\km29ux.h"

#ifndef  ENM_VER_TARGET
void Delay( int tm )
{
	
}
#endif


//KM29U128T

/****************** CAUTION ***********************
 1) PA7,4,2 has to be configured as an output port
 2) PA3 has to be configured as an input port
 3) GCS3 has to be configured as 8-bit, 
    tcos=1,tacc=4 @ 60Mhz, tcos has to exist!!!
 4) caching has to be disabled on nGCS3
 5) write buffer has to be turned off.
 **************************************************/

//The code is made for bi-endian mode

/*
 1999.5 Wireless Div. Circuit for KS32C41000 
 H/W connection.
 F_nSE= L
 F_nRE= nGCS1 | nOE
 F_nWE= nGCS1 | nWE
 F_CLE= PA4
 F_ALE= PA7
 F_RB = PA3
 F_nCE = PA2
*/

// blk0:page0 : reserved
// blk0:page1,2,3,4: Block Information Table :  
//	 good block:used block:logical block number
//	            unused block:FFFF
//       bad block:  F0F0

#define WRITEVERIFY  (1)  //verifing is enable at writing.


//#define ALE	    (0x80)	
//#define CLE	    (0x10)	
//#define CE	    (0x04)     
//#define RB	    (0x08)
//#define FC_CMD()	    {rPDATA|=CLE;rPDATA&=~(ALE|CE);}
//#define FC_ADDR()     {rPDATA|=ALE;rPDATA&=~(CLE|CE);}
//#define FC_DATA()     {rPDATA&=~(ALE|CLE|CE);}
//#define FC_INACTIVE() {rPDATA|=CE;rPDATA&=~(ALE|CLE);}
//#define WAITRB()	    while(!(rPDATA&RB)) //	    //wait tWB and check R/B pin.   

#define rKM29UXDATA	   (*(volatile U8 *)0x2000000)
#define Nf_CONTROL_Port	   (*(volatile U8 *)0x4000000)
unsigned char Nf_CONTROL_Port_Mirror=0;


// 1block=528bytes x 32pages
// 1024block

// A[23:14][13:9]
//  block   page

/// delay
/// times --- loop times
void nDelay( int times )
{
    int i;
	for (i = 0; i < times; i++ )
	{
		/// do nothing
	}
}

void Nf_Set_ALE(void)
{
	unsigned char temp=4;
	Nf_CONTROL_Port_Mirror=Nf_CONTROL_Port_Mirror | temp | 0x08;	///< hardware bug fix. flash and lcd use the same control port , bit3 is lcd on/off bit
	Nf_CONTROL_Port=Nf_CONTROL_Port_Mirror;		
}

void Nf_Clr_ALE(void)
{
	unsigned char temp=4;
	Nf_CONTROL_Port_Mirror=Nf_CONTROL_Port_Mirror & (~temp)  | 0x08;
	Nf_CONTROL_Port=Nf_CONTROL_Port_Mirror;			
}

void Nf_Set_CLE(void)
{
	unsigned char temp=2;
	Nf_CONTROL_Port_Mirror=Nf_CONTROL_Port_Mirror | temp  | 0x08;
	Nf_CONTROL_Port=Nf_CONTROL_Port_Mirror;		
	
}

void Nf_Clr_CLE(void)
{
	unsigned char temp=2;
	Nf_CONTROL_Port_Mirror=Nf_CONTROL_Port_Mirror & (~temp)  | 0x08;
	Nf_CONTROL_Port=Nf_CONTROL_Port_Mirror;			
	
}

void Nf_Set_CE(void)
{
	unsigned char temp=1;
	Nf_CONTROL_Port_Mirror=Nf_CONTROL_Port_Mirror | temp  | 0x08;
	Nf_CONTROL_Port=Nf_CONTROL_Port_Mirror;			
}

void Nf_Clr_CE(void)
{
	unsigned char temp=1;
	Nf_CONTROL_Port_Mirror=Nf_CONTROL_Port_Mirror & (~temp)  | 0x08;
	Nf_CONTROL_Port=Nf_CONTROL_Port_Mirror;				
}

//#define FC_CMD()	    {rPDATA|=CLE;rPDATA&=~(ALE|CE);}
void FC_CMD(void)
{
	Nf_Set_CLE();
	Nf_Clr_CE();	
	Nf_Clr_ALE();
}
//#define FC_ADDR()     {rPDATA|=ALE;rPDATA&=~(CLE|CE);}
void FC_ADDR(void)
{
	Nf_Set_ALE();	
	Nf_Clr_CLE();
	Nf_Clr_CE();
}
//#define FC_DATA()     {rPDATA&=~(ALE|CLE|CE);}
void FC_DATA(void)
{
	Nf_Clr_ALE();
	Nf_Clr_CLE();	
	Nf_Clr_CE();
}
//#define FC_INACTIVE() {rPDATA|=CE;rPDATA&=~(ALE|CLE);}
void FC_INACTIVE(void)
{
	Nf_Set_CE();
	Nf_Clr_ALE();
	Nf_Clr_CLE();
}
//#define WAITRB()	    while(!(rPDATA&RB)) 
void WAITRB(void)
{
	Delay(100);
}

U16 _EraseBlock(unsigned int block) 
{
	
    unsigned int blockPage=(block<<5);
    int i;
     
    FC_CMD();
    rKM29UXDATA=0x60;

    FC_ADDR();	      
    rKM29UXDATA=blockPage&0xff;
    rKM29UXDATA=(blockPage>>8)&0xff;
    rKM29UXDATA=(blockPage>>16)&0xff;

    for(i=0;i<10;i++);//tWC  50ns        	
	
    FC_CMD();
    rKM29UXDATA=0xd0;    
    for(i=0;i<10;i++);       	
    
    FC_DATA();
    WAITRB();	//wait max 3ms
    FC_CMD();
    rKM29UXDATA=0x70;
 
    for(i=0;i<10;i++);       	
	
    FC_DATA();
    if (rKM29UXDATA&0x1) //erase error
    {	
	FC_INACTIVE();
	//Uart_Printf("[ERASE_ERROR:PB=%d]",block);
	return 0;
    }
    else 
    {
	FC_INACTIVE();   
        return 1;
    }
}


U8 _ZeroBlock( U32 block )
{
	U32 i;
	U8  aBuf[512] ;
	memset( aBuf, 0, 512 );
	
	for ( i = 0; i < 32; i++)
	{
		_WrPageSpare( block,  i, aBuf );
	}
	
	return 1;
} 

U16  _VerifyPage(U32 block,U32 page,U8 *buffer)
{
    U8 buf[528];
    _RdPage(block,page,buf);
    if(strncmp((char *)buf,(char *)buffer,528)==0)
    	return 1;
    else 
    	return 0;
}

#define DELAY_TIMES  	100
#define RD_DELAY_TIMES  10
    
U16 _WrPage(unsigned int block,unsigned int page,U8 *pPage)  
{
    unsigned int blockPage=(block<<5)+page;
    int i;
    int ii;
    U8 aData[528];

   _RdPageAll( block, page,  aData );
    memcpy( aData, pPage, 512 );
    
    FC_CMD();
    rKM29UXDATA=0x80;
    ////nDelay( DELAY_TIMES );//tWC  50ns  
   	
    FC_ADDR();    
    rKM29UXDATA=0;
    rKM29UXDATA=blockPage&0xff;
    rKM29UXDATA=(blockPage>>8)&0xff;
    rKM29UXDATA=0;
    ////nDelay( DELAY_TIMES );//tWC  50ns  
   	
    FC_DATA();
    for(i=0;i<528;i++)
    {
    	rKM29UXDATA=aData[i]; 
   	////nDelay( RD_DELAY_TIMES );//tWC  50ns  
    }  
    ////nDelay( DELAY_TIMES );//tWC  50ns  
   	
    FC_CMD();
    rKM29UXDATA=0x10;
    for(i=0;i<10;i++);  //twb=100ns. why is it 10? spec is false?  
                        //No. It's because of LED of PE1.
    WAITRB();	//wait max 500us;  	
    rKM29UXDATA=0x70;
    	
    FC_DATA();
    for(i=0;i<3;i++);  //twhr=60ns
    if((rKM29UXDATA&0x1))
    {
	FC_INACTIVE();	
        return 0;
    }
    else
    {
	FC_INACTIVE();	    
	return 1;
    }
}

U16 _WrPageAll(unsigned int block,unsigned int page,U8 *pPage)  
{
    unsigned int blockPage=(block<<5)+page;
    int i;
    
    U8* apData = pPage;
	rKM29UXDATA=0xff;
	nDelay( DELAY_TIMES );//tWC  50ns  

    
    FC_CMD();
    rKM29UXDATA=0x80;

    FC_ADDR();
    rKM29UXDATA=0;
    rKM29UXDATA=blockPage&0xff;
    rKM29UXDATA=(blockPage>>8)&0xff;
    rKM29UXDATA=0;

    FC_DATA();
    for(i=0;i<528;i++)
    {
    	rKM29UXDATA= *pPage++;
    	////nDelay( RD_DELAY_TIMES );
    }  
    //__WrPage528(pPage);

    FC_CMD();
    rKM29UXDATA=0x10;
    for(i=0;i<10;i++);  //twb=100ns. why is it 10? spec is false?  
                        //No. It's because of LED of PE1.
    WAITRB();	//wait max 500us;
    rKM29UXDATA=0x70;
 
    FC_DATA();
    for(i=0;i<3;i++);  //twhr=60ns
    if((rKM29UXDATA&0x1))
    {
	FC_INACTIVE();	
        return 0;
    }
    else
    {
	FC_INACTIVE();	    
	return 1;
    }
}

U16 _RdPage(unsigned int block,unsigned int page,U8 *pPage)  
{
    int i;
    int ii;
    U8 aData;
    unsigned int blockPage=(block<<5)+page;

    FC_CMD();
    rKM29UXDATA=0x00;
    
    FC_ADDR();
    rKM29UXDATA=0;
    rKM29UXDATA=blockPage&0xff;
    rKM29UXDATA=(blockPage>>8)&0xff;
    rKM29UXDATA=0;
    for(i=0;i<3;i++);

    FC_DATA();
    WAITRB();	//wait max. 10us 
    for(i=0;i<512;i++)
    {
    	*pPage++=rKM29UXDATA;
    }

    FC_INACTIVE();    
 
    return 1;
}

U16 _RdPageAll(unsigned int block,unsigned int page,U8 *pPage)  
{
    int i;
    U8 aData;
    unsigned int blockPage=(block<<5)+page;

    FC_CMD();
    rKM29UXDATA=0x00;

    FC_ADDR();
    rKM29UXDATA=0;
    rKM29UXDATA=blockPage&0xff;
    rKM29UXDATA=(blockPage>>8)&0xff;
    rKM29UXDATA=0;

    FC_DATA();
    WAITRB();	//wait max. 10us 

    for(i=0;i<528;i++)
    {
   	*pPage++=rKM29UXDATA;
    }
    //__RdPage528(pPage);
  
    FC_INACTIVE();
    
    return 1;
}

U16 _RdPageSpare(unsigned int block,unsigned int page,U8 *pPage)
{
    int i;
    unsigned int blockPage=(block<<5)+page;
   
    rKM29UXDATA=0xff;
    WAITRB();	//wait max. 10us 

    FC_CMD();
    rKM29UXDATA=0x00;
 
    FC_ADDR();
    rKM29UXDATA=0;
    rKM29UXDATA=blockPage&0xff;
    rKM29UXDATA=(blockPage>>8)&0xff;
    rKM29UXDATA=0;
    for(i=0;i<3;i++);
 
    FC_DATA();
    WAITRB();	//wait max. 10us 
    for(i=0;i<512;i++)
    {
    	*pPage  = rKM29UXDATA;
    }
    
    for(i=0;i<16;i++)
    {
	   *pPage++ = rKM29UXDATA;
    }
    //__RdPage528(pPage);
    
    FC_INACTIVE();
    
    return 1;
}

U16 _WrPageSpare(unsigned int block,unsigned int page,U8 *pPage)
{
    unsigned int blockPage=(block<<5)+page;
    int i;
    U8  aBuf[528];
    U8* apData = aBuf;

    rKM29UXDATA=0xff;
    for(i=0;i<10;i++);
    
    /// read data
    _RdPage( block, page, aBuf );
    memcpy( &aBuf[512], pPage, 16 );
    
    FC_CMD();
    rKM29UXDATA=0x80;

    FC_ADDR();
    rKM29UXDATA=0;
    rKM29UXDATA=blockPage&0xff;
    rKM29UXDATA=(blockPage>>8)&0xff;
    rKM29UXDATA=0;

    FC_DATA();    
    for(i=0;i<528;i++)
    {
    	rKM29UXDATA = *apData++;
    }  
    //__WrPage528(pPage);

    FC_CMD();
    rKM29UXDATA=0x10;
    for(i=0;i<10;i++);  //twb=100ns. why is it 10? spec is false?  
                        //No. It's because of LED of PE1.
    WAITRB();	//wait max 500us;
    rKM29UXDATA=0x70;

    FC_DATA();
    for(i=0;i<3;i++);  //twhr=60ns
    if((rKM29UXDATA&0x1))
    {
	FC_INACTIVE();	
	rSYSCFG=SYSCFG_8KB;
        return 0;
    }
    else
    {
	FC_INACTIVE();	    
	rSYSCFG=SYSCFG_8KB;
#if (WRITEVERIFY==1)
		return _VerifyPage(block,page,apData );	
#else
		return 1;
#endif
	}		
}


U16 _CheckId(void)
{
    U16 i;
    U16 id;
    U8 test;

    rKM29UXDATA=0xff;
    WAITRB();	//wait max 3ms
  
    FC_CMD();
    rKM29UXDATA=0x90;
  
    FC_ADDR();
    rKM29UXDATA=0;
    
 
    FC_DATA();
    for(i=0;i<5;i++); //wait 100ns
    id=rKM29UXDATA<<8;
    id+=rKM29UXDATA;
    FC_INACTIVE();
 
    return id;
}


