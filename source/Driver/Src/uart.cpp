#include "44b.h"
#include "uart.h"
#include "all.h"
#include "map.h"
#include "oem.h"
#include "class.h"


////////////Uart0 Rx //////////////////
//char  haveUart0Error=0; //test
unsigned char Uart0OneMessageBuff[300];
volatile unsigned char Uart0RxBuff[5000];
volatile short Uart0Wptr=0;
volatile short Uart0Rptr=0;
////////////Uart0 Tx//////////////
unsigned char Uart0TxBuff[200];



/*
void __irq Uart0_RxInt(void)
{
    rI_ISPC=BIT_URXD0;
    Uart0RxBuff[Uart0Wptr++]=RdURXH0();
    if(Uart0Wptr>=5000)Uart0Wptr=0;
}
*/


void __irq Uart0_RxFifoInt(void)
{
 //   char  errortemp;
    rI_ISPC=BIT_URXD0;
  //  errortemp=(char )rUERSTAT0;
  //  if( errortemp) haveUart0Error=errortemp;
    while( (rUFSTAT0&0xf) >0 ){
	     Uart0RxBuff[Uart0Wptr++]=rURXH0;
	     if(Uart0Wptr>=5000)Uart0Wptr=0;
    }
    rI_ISPC=BIT_URXD0;
}

void __irq Uart0_TxInt(void)
{
    rI_ISPC=BIT_UTXD0;
}

#define  MCLOCK 50000000 //66000000 //50000000
//*
void Uart0_Init(long baud)
{
    short i;
          //        8 bytes     //fifo enable
    rUFCON0=(0<<6)|(1<<4)|  (6)|1;
    rUMCON0=0x0;
    rULCON0=0x3;     //Normal,No parity,1 stop,8 bit
    rUCON0=0x2C5;//0x245;    //rx=edge,tx=level,disable timeout int.,enable rx error int.,normal,interrupt or polling
    rUBRDIV0=( (int)(MCLOCK/16./baud + 0.5) -1 );
    Uart0Wptr=Uart0Rptr=0;  
    for(i=0;i<100;i++);                               
    pISR_URXD0=(unsigned)Uart0_RxFifoInt;
    pISR_UTXD0=(unsigned)Uart0_TxInt;
    rINTMSK &= ~(BIT_URXD0);
}
//*/

/*
void Uart0_Init(long baud)
{
    short i;
    rUFCON0=0x0;     //FIFO disable
    rUMCON0=0x0;
    rULCON0=0x3;     //Normal,No parity,1 stop,8 bit
    rUCON0=0x245;//0x245;    //rx=edge,tx=level,disable timeout int.,enable rx error int.,normal,interrupt or polling
    rUBRDIV0=( (int)(MCLOCK/16./baud + 0.5) -1 );
    Uart0Wptr=Uart0Rptr=0;  
    for(i=0;i<100;i++);                               
    pISR_URXD0=(unsigned)Uart0_RxInt;
    pISR_UTXD0=(unsigned)Uart0_TxInt;
    rINTMSK &= ~(BIT_URXD0);
}
*/


void Uart0_SendByte(unsigned char data)
{
	while(!(rUTRSTAT0 & 0x2)); //not ok
	rUTXH0=data;
}		

/*
void Uart0_SendString(char *pt)
{
    while(*pt)Uart0_SendByte(*pt++);
}
*/

void Uart0_SendStringLength(char *pt,short length)
{
    short i;
    for( i=0; i<length; i++ ){
        Uart0_SendByte( pt[i] );
    }
    
}

const unsigned char invertTab[]={
 //  decode tab      encode tab       
   0x92,  //  0      0xC9,  //  0     
   0xB8,  //  1      0xEE,  //  1     
   0x94,  //  2      0xE0,  //  2     
   0xC8,  //  3      0x8D,  //  3     
   0x80,  //  4      0x46,  //  4     
   0x77,  //  5      0x6D,  //  5     
   0x7E,  //  6      0xEB,  //  6     
   0x5B,  //  7      0x33,  //  7     
   0xFE,  //  8      0x6A,  //  8     
   0x54,  //  9      0xB9,  //  9     
   0xFD,  // 10      0xF2,  // 10     
   0x89,  // 11      0xE6,  // 11     
   0x73,  // 12      0xF9,  // 12     
   0xBF,  // 13      0x35,  // 13     
   0xFC,  // 14      0x77,  // 14     
   0x9A,  // 15      0xDE,  // 15     
   0x49,  // 16      0x8B,  // 16     
   0x85,  // 17      0x1D,  // 17     
   0x13,  // 18      0xE2,  // 18     
   0x23,  // 19      0x12,  // 19     
   0x42,  // 20      0x3A,  // 20     
   0x3A,  // 21      0x26,  // 21     
   0xB2,  // 22      0xCC,  // 22     
   0x31,  // 23      0xFB,  // 23     
   0xC1,  // 24      0x90,  // 24     
   0x71,  // 25      0x57,  // 25     
   0x67,  // 26      0x8A,  // 26     
   0xA5,  // 27      0x87,  // 27     
   0xA7,  // 28      0xAF,  // 28     
   0x11,  // 29      0x27,  // 29     
   0x33,  // 30      0x5B,  // 30     
   0x6A,  // 31      0xC2,  // 31     
   0xC5,  // 32      0xD8,  // 32     
   0xBB,  // 33      0x4E,  // 33     
   0x24,  // 34      0xA4,  // 34     
   0x65,  // 35      0x13,  // 35     
   0x81,  // 36      0x22,  // 36     
   0xD4,  // 37      0xD0,  // 37     
   0x15,  // 38      0xC5,  // 38     
   0x1D,  // 39      0x84,  // 39     
   0xDB,  // 40      0x89,  // 40     
   0x9B,  // 41      0x39,  // 41     
   0xD3,  // 42      0x47,  // 42     
   0x40,  // 43      0x4A,  // 43     
   0x78,  // 44      0x76,  // 44     
   0xBE,  // 45      0xDA,  // 45     
   0xDC,  // 46      0xDD,  // 46     
   0xE9,  // 47      0x70,  // 47     
   0xB0,  // 48      0xDB,  // 48     
   0x4F,  // 49      0x17,  // 49     
   0x66,  // 50      0xBC,  // 50     
   0x07,  // 51      0x1E,  // 51     
   0xE0,  // 52      0x91,  // 52     
   0x0D,  // 53      0xB1,  // 53     
   0xC2,  // 54      0x96,  // 54     
   0xF5,  // 55      0xB2,  // 55     
   0xEF,  // 56      0x3D,  // 56     
   0x29,  // 57      0x8F,  // 57     
   0x14,  // 58      0x15,  // 58     
   0x57,  // 59      0xA1,  // 59     
   0x3E,  // 60      0xC8,  // 60     
   0x38,  // 61      0x65,  // 61     
   0xB3,  // 62      0x3C,  // 62     
   0xE6,  // 63      0xCD,  // 63     
   0x6C,  // 64      0x2B,  // 64     
   0x4B,  // 65      0xD3,  // 65     
   0x6E,  // 66      0x14,  // 66     
   0xE8,  // 67      0xE9,  // 67     
   0x68,  // 68      0xF7,  // 68     
   0xED,  // 69      0xF8,  // 69     
   0x04,  // 70      0xBF,  // 70     
   0x2A,  // 71      0xB7,  // 71     
   0xD7,  // 72      0xD9,  // 72     
   0xAB,  // 73      0x10,  // 73     
   0x2B,  // 74      0x69,  // 74     
   0xD0,  // 75      0x41,  // 75     
   0x64,  // 76      0xC0,  // 76     
   0x7A,  // 77      0x63,  // 77     
   0x21,  // 78      0x9B,  // 78     
   0x98,  // 79      0x31,  // 79     
   0x97,  // 80      0x92,  // 80     
   0x59,  // 81      0x7C,  // 81     
   0x83,  // 82      0xCB,  // 82     
   0xF8,  // 83      0x7F,  // 83     
   0x8C,  // 84      0x09,  // 84     
   0xFF,  // 85      0x9C,  // 85     
   0x62,  // 86      0xE4,  // 86     
   0x19,  // 87      0x3B,  // 87     
   0xB4,  // 88      0xA6,  // 88     
   0x70,  // 89      0x51,  // 89     
   0xE5,  // 90      0xF1,  // 90     
   0x1E,  // 91      0x07,  // 91     
   0x5E,  // 92      0xBB,  // 92     
   0xDA,  // 93      0x6B,  // 93     
   0xDF,  // 94      0x5C,  // 94     
   0xC4,  // 95      0xAA,  // 95     
   0xCD,  // 96      0x98,  // 96     
   0xCB,  // 97      0x88,  // 97     
   0x72,  // 98      0x56,  // 98     
   0x4D,  // 99      0xB0,  // 99     
   0xBA,  //100      0x4C,  //100     
   0x3D,  //101      0x23,  //101     
   0xE7,  //102      0x32,  //102     
   0xAC,  //103      0x1A,  //103     
   0xCF,  //104      0x44,  //104     
   0x4A,  //105      0xEF,  //105     
   0x08,  //106      0x1F,  //106     
   0x5D,  //107      0xA8,  //107     
   0xF0,  //108      0x40,  //108     
   0x05,  //109      0x7A,  //109     
   0xA0,  //110      0x42,  //110     
   0xB6,  //111      0xB3,  //111     
   0x2F,  //112      0x59,  //112     
   0xD5,  //113      0x19,  //113     
   0xEE,  //114      0x62,  //114     
   0xAF,  //115      0x0C,  //115     
   0xBD,  //116      0xD1,  //116     
   0xA9,  //117      0x82,  //117     
   0x2C,  //118      0xF5,  //118     
   0x0E,  //119      0x05,  //119     
   0xA3,  //120      0x2C,  //120     
   0xE2,  //121      0x83,  //121     
   0x6D,  //122      0x4D,  //122     
   0xF7,  //123      0xC6,  //123     
   0x51,  //124      0xB4,  //124     
   0x93,  //125      0x95,  //125     
   0xE4,  //126      0x06,  //126     
   0x53,  //127      0xFF,  //127     
   0xD8,  //128      0x04,  //128     
   0xD6,  //129      0x24,  //129     
   0x75,  //130      0xFC,  //130     
   0x79,  //131      0x52,  //131     
   0x27,  //132      0xEC,  //132     
   0xA4,  //133      0x11,  //133     
   0xF3,  //134      0xAD,  //134     
   0x1B,  //135      0xCF,  //135     
   0x61,  //136      0xF4,  //136     
   0x28,  //137      0x0B,  //137     
   0x1A,  //138      0xEA,  //138     
   0x10,  //139      0xCA,  //139     
   0xE3,  //140      0x54,  //140     
   0x03,  //141      0x94,  //141     
   0xC9,  //142      0xB8,  //142     
   0x39,  //143      0xE5,  //143     
   0x18,  //144      0xED,  //144     
   0x34,  //145      0xD6,  //145     
   0x50,  //146      0x00,  //146     
   0xA2,  //147      0x7D,  //147     
   0x8D,  //148      0x02,  //148     
   0x7D,  //149      0x9D,  //149     
   0x36,  //150      0xF6,  //150     
   0xFA,  //151      0x50,  //151     
   0x60,  //152      0x4F,  //152     
   0xFB,  //153      0xC4,  //153     
   0xF2,  //154      0x0F,  //154     
   0x4E,  //155      0x29,  //155     
   0x55,  //156      0xA2,  //156     
   0x95,  //157      0xE1,  //157     
   0xB5,  //158      0xD5,  //158     
   0xC7,  //159      0xBD,  //159     
   0xA6,  //160      0x6E,  //160     
   0x3B,  //161      0xBE,  //161     
   0x9C,  //162      0x93,  //162     
   0xC0,  //163      0x78,  //163     
   0x22,  //164      0x85,  //164     
   0xF4,  //165      0x1B,  //165     
   0x58,  //166      0xA0,  //166     
   0xDD,  //167      0x1C,  //167     
   0x6B,  //168      0xB6,  //168     
   0xEB,  //169      0x75,  //169     
   0x5F,  //170      0xAB,  //170     
   0xAA,  //171      0x49,  //171     
   0xB9,  //172      0x67,  //172     
   0x86,  //173      0xB5,  //173     
   0xEA,  //174      0xFA,  //174     
   0x1C,  //175      0x73,  //175     
   0x63,  //176      0x30,  //176     
   0x35,  //177      0xDF,  //177     
   0x37,  //178      0x16,  //178     
   0x6F,  //179      0x3E,  //179     
   0x7C,  //180      0x58,  //180     
   0xAD,  //181      0x9E,  //181     
   0xA8,  //182      0x6F,  //182     
   0x47,  //183      0xE8,  //183     
   0x8E,  //184      0x01,  //184     
   0x09,  //185      0xAC,  //185     
   0xF6,  //186      0x64,  //186     
   0x5C,  //187      0x21,  //187     
   0x32,  //188      0xDC,  //188     
   0x9F,  //189      0x74,  //189     
   0xA1,  //190      0x2D,  //190     
   0x46,  //191      0x0D,  //191     
   0x4C,  //192      0xA3,  //192     
   0xCE,  //193      0x18,  //193     
   0x1F,  //194      0x36,  //194     
   0xCA,  //195      0xCE,  //195     
   0x99,  //196      0x5F,  //196     
   0x26,  //197      0x20,  //197     
   0x7B,  //198      0xD2,  //198     
   0xEC,  //199      0x9F,  //199     
   0x3C,  //200      0x03,  //200     
   0x00,  //201      0x8E,  //201     
   0x8B,  //202      0xC3,  //202     
   0x52,  //203      0x61,  //203     
   0x16,  //204      0xE7,  //204     
   0x3F,  //205      0x60,  //205     
   0xC3,  //206      0xC1,  //206     
   0x87,  //207      0x68,  //207     
   0x25,  //208      0x4B,  //208     
   0x74,  //209      0xF3,  //209     
   0xC6,  //210      0xFE,  //210     
   0x41,  //211      0x2A,  //211     
   0xE1,  //212      0x25,  //212     
   0x9E,  //213      0x71,  //213     
   0x91,  //214      0x81,  //214     
   0xF9,  //215      0x48,  //215     
   0x20,  //216      0x80,  //216     
   0x48,  //217      0xE3,  //217     
   0x2D,  //218      0x5D,  //218     
   0x30,  //219      0x28,  //219     
   0xBC,  //220      0x2E,  //220     
   0x2E,  //221      0xA7,  //221     
   0x0F,  //222      0xFD,  //222     
   0xB1,  //223      0x5E,  //223     
   0x02,  //224      0x34,  //224     
   0x9D,  //225      0xD4,  //225     
   0x12,  //226      0x79,  //226     
   0xD9,  //227      0x8C,  //227     
   0x56,  //228      0x7E,  //228     
   0x8F,  //229      0x5A,  //229     
   0x0B,  //230      0x3F,  //230     
   0xCC,  //231      0x66,  //231     
   0xB7,  //232      0x43,  //232     
   0x43,  //233      0x2F,  //233     
   0x8A,  //234      0xAE,  //234     
   0x06,  //235      0xA9,  //235     
   0x84,  //236      0xC7,  //236     
   0x90,  //237      0x45,  //237     
   0x01,  //238      0x72,  //238     
   0x69,  //239      0x38,  //239     
   0xF1,  //240      0x6C,  //240     
   0x5A,  //241      0xF0,  //241     
   0x0A,  //242      0x9A,  //242     
   0xD1,  //243      0x86,  //243     
   0x88,  //244      0xA5,  //244     
   0x76,  //245      0x37,  //245     
   0x96,  //246      0xBA,  //246     
   0x44,  //247      0x7B,  //247     
   0x45,  //248      0x53,  //248     
   0x0C,  //249      0xD7,  //249     
   0xAE,  //250      0x97,  //250     
   0x17,  //251      0x99,  //251     
   0x82,  //252      0x0E,  //252     
   0xDE,  //253      0x0A,  //253     
   0xD2,  //254      0x08,  //254     
   0x7F  //255       0x55,  //255     
}; //invertTab end

// encode 
 char HaveUart0Message(void)
 {
     static short Uart0OneMessageCnt=0;
     static short Uart0DecodeMode=0;
     short i;
     unsigned char recchar;

     while(Uart0Wptr!=Uart0Rptr){
         recchar=Uart0RxBuff[Uart0Rptr++];
         if(Uart0Rptr>=5000)Uart0Rptr=0;
       //   Uart1_SendByte(recchar);  //for test
         switch(Uart0DecodeMode){
             case 0: // receive  no bytes
                 if(recchar==0x06){  //7e
                     Uart0DecodeMode=1;
                     Uart0OneMessageCnt=0;
                     Uart0OneMessageBuff[Uart0OneMessageCnt++]=recchar;
                 }
                 break;
             case 1: //have receive start byte (0x7e)
                 if(recchar==0xff){ //7f
                     Uart0DecodeMode=0;
                     Uart0OneMessageBuff[Uart0OneMessageCnt++]=recchar;
                     for( i=0; i<Uart0OneMessageCnt; i++ )
                                 Uart0OneMessageBuff[i]=invertTab[ Uart0OneMessageBuff[i] ];
                     return 1;
                 }
                 else if(recchar==0x06){ //7e
                     Uart0DecodeMode=1;
                     Uart0OneMessageCnt=0;
                     Uart0OneMessageBuff[Uart0OneMessageCnt++]=recchar;
                 }
                 else {
                     Uart0OneMessageBuff[Uart0OneMessageCnt++]=recchar;
                 }
                 break;
                     
         }//switch(SioDecodeMode){	
         
     }//while(SioInPtr!=SioOutPtr){
     return 0;
 }//function  

/*
//not encode 
 char HaveUart0Message(void)
 {
     static short Uart0OneMessageCnt=0;
     static short Uart0DecodeMode=0;
     unsigned char recchar;

     while(Uart0Wptr!=Uart0Rptr){
         recchar=Uart0RxBuff[Uart0Rptr++];
         if(Uart0Rptr>=5000)Uart0Rptr=0;
       //   Uart1_SendByte(recchar);  //for test
         switch(Uart0DecodeMode){
             case 0: // receive  no bytes
                 if(recchar==0x7e){
                     Uart0DecodeMode=1;
                     Uart0OneMessageCnt=0;
                     Uart0OneMessageBuff[Uart0OneMessageCnt++]=recchar;
                 }
                 break;
             case 1: //have receive start byte (0x7e)
                 if(recchar==0x7f){
                     Uart0DecodeMode=0;
                     Uart0OneMessageBuff[Uart0OneMessageCnt++]=recchar;
                     return 1;
                 }
                 else if(recchar==0x7e){
                     Uart0DecodeMode=1;
                     Uart0OneMessageCnt=0;
                     Uart0OneMessageBuff[Uart0OneMessageCnt++]=recchar;
                 }
                 else {
                     Uart0OneMessageBuff[Uart0OneMessageCnt++]=recchar;
                 }
                 break;
                     
         }//switch(SioDecodeMode){	
         
     }//while(SioInPtr!=SioOutPtr){
     return 0;
 }//function  
 
*/

/**********************************************************************************************************************
*	Function Name				: Uart0ProtocolDecode
*	Date						: 2003.06.02
*	Description					: Decode the Uart0  command
*	Return Value				: 1=have command ;0=no command
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	: 
*	Called by					:
*	Author						: TangXiuMing
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
**********************************************************************************************************************/

char Uart0ProtocolDecode(unsigned char *Sou_Buffer)
{ 
	unsigned char c;
	unsigned char state=0;
	unsigned short temppointer=0, i;
	unsigned short lenght,Des_pointer=0;
//	char HaveEnd=0;
	do{
		c=Sou_Buffer[temppointer++];
		if(temppointer>=500) return(0);
		switch(state){
			case 0x00:					//接收头
				if(c==0x7E){
				    state=1;
				    Des_pointer=0;
				    Sou_Buffer[Des_pointer++]=c;
				}
				break;		
			case 0x01:					//接收协议内容
				if(c==0x7D) state=2;
				else{
					if(c==0x7F){
						state=0;
					}
					else if( c==0x7e ){
					    Des_pointer=0;
					}
					Sou_Buffer[Des_pointer++]=c;
				}
				break;
			case 0x02:					//接收转移字符
				switch(c){
					case 0x01:
						Sou_Buffer[Des_pointer++]=0x7E;
						state=1;
						break;
					case 0x02:
						Sou_Buffer[Des_pointer++]=0x7F;
						state=1;
						break;
					case 0x03:
						Sou_Buffer[Des_pointer++]=0x7D;
						state=1;
						break;	
					default:
						state=0;			//协议出错回原状态
				}//switch
				break;
			default: state=0;		
		}//switch
	}while( c!=0x7F ); //
	//for(i=Des_pointer;i<temppointer;i++)
	Sou_Buffer[Des_pointer]=0;
	if((Sou_Buffer[0]!=0x7E) ||(Sou_Buffer[Des_pointer-1]!=0x7F))
		return(0);	//		头尾不对
	state=0; //Sou_Buffer[0];
	for(i=1;i<Des_pointer-2;i++) state^=Sou_Buffer[i];
	if(state!=Sou_Buffer[Des_pointer-2]) 
		return(0);	//校验码出错
	lenght=Sou_Buffer[1]*256+Sou_Buffer[2]+6;
	if(Des_pointer!=lenght) 
		return(0);//长度出错
	return(1);
}//function

int Protocol_Add_Shift0(unsigned char lenght,unsigned char command,unsigned char *Sou_Buffer,unsigned char *tempbuffer)
{
	unsigned int i,j=0;
	unsigned char Xr_v;
	Sou_Buffer[j++]=0x7E;
	Sou_Buffer[j]=lenght/256;
	Xr_v=Sou_Buffer[j++];
	Sou_Buffer[j]=lenght%256;
	Xr_v ^=Sou_Buffer[j++];
	Sou_Buffer[j]=command;
	Xr_v ^=Sou_Buffer[j++];
	for(i=0;i<lenght;j++,i++){
		Sou_Buffer[j]=tempbuffer[i];
		Xr_v ^=Sou_Buffer[j];
		switch(Sou_Buffer[j]){
			case 0x7E:
				Sou_Buffer[j++]=0x7D;
				Sou_Buffer[j]=0x01;
				break;	
			case 0x7D:
				Sou_Buffer[j++]=0x7D;
				Sou_Buffer[j]=0x03;
				break;
			case 0x7F:
				Sou_Buffer[j++]=0x7D;
				Sou_Buffer[j]=0x02;
				break;
			default:
				break;
		}//end switch()

	}//命令体拼凑
	Sou_Buffer[j]=Xr_v;
		switch(Sou_Buffer[j]){
			case 0x7E:
				Sou_Buffer[j++]=0x7D;
				Sou_Buffer[j]=0x01;
				break;	
			case 0x7D:
				Sou_Buffer[j++]=0x7D;
				Sou_Buffer[j]=0x03;
				break;
			case 0x7F:
				Sou_Buffer[j++]=0x7D;
				Sou_Buffer[j]=0x02;
				break;
			default:
				break;
		}//end switch()
		j++;
	Sou_Buffer[j++]=0x7F;

   return(j);
}


/**********************************************************************************************************************
*	Function Name				: SioCmdSwitch
*	Date						: 2003.06.02
*	Description					: dispatch sio port command
*	Return Value				: 
*	Global Variable				:
*	File Static Variable		:
*	Function Static Variable	: 
*	Called by					:
*	Author						: TangXiuMing
*
*----------------------------------------------------------------------------------------------------------------------
*	Revision History
*	No.		Date		Revised by			Modification
*
**********************************************************************************************************************/

void Uart0CmdSwitch(unsigned char *pSource)
{
     short i;
  //   short x,y;
  //   char tempstr[5];
     unsigned char testcheck;
     switch(pSource[3]){
         case 0x25:  //oem cmd
            OemCmd((char *)&pSource[4] );  //in oem.c file
            break;
         case 0x57:  // s/n
            RecCmdFlag|=RDSN;
            ToSendFlag&=~RDSN;
            pSource[12]=0x0;
            strncpy(SNnumber,(char *)&pSource[4],8);
            SNnumber[8]=0x0;
            TabInitFlag&=0xef;
            break;         
         case 0x63:  //first tab
            RecCmdFlag|=TAB0;
            ToSendFlag&=~TAB0;
            testcheck=pSource[4];
            for(i=1;i<64;i++)testcheck^=pSource[4+i];
            if(testcheck==0x01){
                 AddToTab(0,&pSource[4]);
                 TabInitFlag&=0xfe;
            }
            break;
         case 0x64: //second tab
            RecCmdFlag|=TAB1;
            ToSendFlag&=~TAB1;
            testcheck=pSource[4];
            for(i=1;i<64;i++)testcheck^=pSource[4+i];
            if(testcheck==0x02){
                 AddToTab(1,&pSource[4]);
                 TabInitFlag&=0xfd;
            }
            break;         
         case 0x65: //third tab
            RecCmdFlag|=TAB2;
            ToSendFlag&=~TAB2;
            testcheck=pSource[4];
            for(i=1;i<64;i++)testcheck^=pSource[4+i];
            if(testcheck==0xfd){
                 AddToTab(2,&pSource[4]);
                 TabInitFlag&=0xfb;
            }
            break;         
         case 0x66:  //fourth tab
            RecCmdFlag|=TAB3;
            ToSendFlag&=~TAB3;
            testcheck=pSource[4];
            for(i=1;i<64;i++)testcheck^=pSource[4+i];
            if(testcheck==0xfe){
                 AddToTab(3,&pSource[4]);
                 TabInitFlag&=0xf7;
            }
            break;         
         case 0x69:  //register information
            RecCmdFlag|=RDREG;
            ToSendFlag&=~RDREG;
            RegisterOld= * ((tagRegister *)&pSource[4]);
            break;         

         default:
            break;   
     
     }//switch(pSource[3])   
}//function    

unsigned char EncodeBuff[200];

unsigned long ToSendFlag=0x0000002f;//3f; 
             // bit0=tab0 0x53
             // bit1=tab1 0x54
             // bit2=tab2 0x55
             // bit3=tab3 0x56
             // bit4=read s/n  0x52
             // bit5=send speed to oem  0x83
             // bit6=read reginfo 0x59
             // bit7=write reginfo 0x58
             // bit8= 
             
unsigned long RecCmdFlag=0x00000000; 
void ToCmdTab0(void);
void ToCmdTab1(void);
void ToCmdTab2(void);
void ToCmdTab3(void);            
void CheckCmdToSend(void)
{
     short outcnt;
     for(outcnt=0;outcnt<10;outcnt++){   
        if( (ToSendFlag>>outcnt)& 0x1 ){
          switch(outcnt){
              case 0: 
                  ToCmdTab0(); //lpp=Protocol_Add_Shift1(8,0x53,Uart0TxBuff,EncodeBuff);
                  break;
              case 1: 
                  ToCmdTab1(); //lpp=Protocol_Add_Shift1(8,0x54,Uart0TxBuff,EncodeBuff);
                  break;
              case 2: 
                  ToCmdTab2(); //lpp=Protocol_Add_Shift1(8,0x55,Uart0TxBuff,EncodeBuff);
                  break;
              case 3: 
                  ToCmdTab3(); //lpp=Protocol_Add_Shift1(8,0x56,Uart0TxBuff,EncodeBuff);
                  break;
              case 4: 
                  ToCmdSN(); //lpp=Protocol_Add_Shift1(5,0x59,Uart0TxBuff,EncodeBuff);
                  break;
              case 5: 
                  ToCmdOEM(); 
                  break;
              case 6: 
                  ToCmdRdReg(); 
                  break;
                  
          }//switch
        }//if  
      }//for 
}//function

void ToCmdTab0(void)
{
     short lpp;
     sprintf( (char *)&EncodeBuff[0], (char *)"12345678" );
     lpp=Protocol_Add_Shift0(8,0x53,Uart0TxBuff,EncodeBuff);
     Uart0_SendStringLength( (char *)Uart0TxBuff,lpp );
}
void ToCmdTab1(void)
{
     short lpp;
     sprintf( (char *)&EncodeBuff[0], (char *)"12345678" );
     lpp=Protocol_Add_Shift0(8,0x54,Uart0TxBuff,EncodeBuff);
     Uart0_SendStringLength( (char *)Uart0TxBuff,lpp );
}
void ToCmdTab2(void)
{
     short lpp;
     sprintf( (char *)&EncodeBuff[0], (char *)"12345678" );
     lpp=Protocol_Add_Shift0(8,0x55,Uart0TxBuff,EncodeBuff);
     Uart0_SendStringLength( (char *)Uart0TxBuff,lpp );
}
void ToCmdTab3(void)
{
     short lpp;
     sprintf( (char *)&EncodeBuff[0], (char *)"12345678" );
     lpp=Protocol_Add_Shift0(8,0x56,Uart0TxBuff,EncodeBuff);
     Uart0_SendStringLength( (char *)Uart0TxBuff,lpp );
}

void ToCmdSN(void)
{
     short lpp;
     sprintf( (char *)&EncodeBuff[0], (char *)"12345678" );
     lpp=Protocol_Add_Shift0(8,0x52,Uart0TxBuff,EncodeBuff);
     Uart0_SendStringLength( (char *)Uart0TxBuff,lpp );
}

void ToCmdRdReg(void)
{
     short lpp;
     sprintf( (char *)&EncodeBuff[0], (char *)"12345678" );
     lpp=Protocol_Add_Shift0(5,0x59,Uart0TxBuff,EncodeBuff);
     Uart0_SendStringLength( (char *)Uart0TxBuff,lpp );
}

void ToCmdWrReg(void)
{
     short lpp;
     * (tagRegister *)&EncodeBuff[0]=RegisterOld;
     lpp=Protocol_Add_Shift0(5,0x58,Uart0TxBuff,EncodeBuff);
     Uart0_SendStringLength( (char *)Uart0TxBuff,lpp );
}

// ToCmdWrRegErase is only for test 
void ToCmdWrRegErase(void)
{
     short lpp;
     tagRegister test;
     test.RegType =0xff;
     test.RegEndYear = 0xffff;
     test.RegEndMonth = 0xff;
     test.RegEndDay = 0xff;
     * (tagRegister *)&EncodeBuff[0]=test;
     lpp=Protocol_Add_Shift0(5,0x58,Uart0TxBuff,EncodeBuff);
     Uart0_SendStringLength( (char *)Uart0TxBuff,lpp );
}


void ToCmdOEM(void)
{
     short lpp;
     char tempstr[50];
     unsigned char tempv;
     if( myConfig.VelocityFilterType < 2 )tempv=myConfig.VelocityFilterType;
     else tempv=myConfig.VelocityFilter;
     sprintf( tempstr,"%03d",tempv);
     tempstr[3]=0x0;
     sprintf( (char *)EncodeBuff, (char *)"$PGRMC,A,,,,,,,,,3,%s,,,*\r\n",tempstr );
     lpp=(short )strlen( (const char *)EncodeBuff );
     lpp=Protocol_Add_Shift0(lpp,0x83,Uart0TxBuff,EncodeBuff);
     Uart0_SendStringLength( (char *)Uart0TxBuff,lpp );
}


////////////////////////////uart1 /////////////////////////////////////////////////////


unsigned char Uart1OneMessageBuff[2000];
short Uart1RxWptr=0;
short Uart1RxRptr=0;
#define UARTRXMAX  2000
unsigned char Uart1RxBuff[ UARTRXMAX ];
short Uart1TxWptr=0;
short Uart1TxRptr=0;
unsigned char Uart1TxBuff[2000];

void __irq Uart1_RxInt(void);
void __irq Uart1_TxInt(void);


void __irq Uart1_RxInt(void)
{
    Uart1RxBuff[Uart1RxWptr++]=RdURXH1();
    if( Uart1RxWptr>=UARTRXMAX )Uart1RxWptr=0;
    rI_ISPC=BIT_URXD1;
}

void __irq Uart1_TxInt(void)
{
    short temp;
    temp=Uart1TxRptr+1;
    if( temp>=2000 )temp=0;
    if( temp==Uart1TxWptr ){
        rINTMSK |= BIT_UTXD1;    
    }
    else {
        WrUTXH1( Uart1TxBuff[temp] );
        Uart1TxRptr=temp;
    }   
    rI_ISPC=BIT_UTXD1;
}


void Uart1_Init(long baud)
{
    short i;
    rUFCON1=0x0;     //FIFO disable
    rUMCON1=0x0;
    rULCON1=0x3;     //Normal,No parity,1 stop,8 bit
    rUCON1=0x045;//0x245;    //rx=edge,tx=level,disable timeout int.,enable rx error int.,normal,interrupt or polling
    rUBRDIV1=( (int)(MCLOCK/16./baud + 0.5) -1 );
    for(i=0;i<100;i++);                               
    pISR_URXD1=(unsigned)Uart1_RxInt;
    pISR_UTXD1=(unsigned)Uart1_TxInt;
    Uart1RxWptr=0;
    Uart1RxRptr=0;
    Uart1TxWptr=1;
    Uart1TxRptr=0;
    rINTMSK &= ~(BIT_URXD1);
}

void Uart1_Tx_AddStr( char *pSource,short bytes)
{
     short i;
     short temp;
     for( i=0; i<bytes; i++ ){
         while( 1 ){  
             temp = Uart1TxWptr+1;
             if( temp >=2000 )temp = 0;
             if( temp != Uart1TxRptr )break;
             if( rINTMSK & BIT_UTXD1 ){ // if irq is off
                 WrUTXH1( Uart1TxBuff[Uart1TxRptr++] );
                 if( Uart1TxRptr >=2000 ) Uart1TxRptr = 0;
                 rINTMSK&= ~(BIT_UTXD1);  // irq turn on
             } 
         }//while
        // while( Uart1TxWptr==Uart1TxRptr );
         Uart1TxBuff[ Uart1TxWptr++ ]=pSource[i];
         if( Uart1TxWptr >= 2000 )Uart1TxWptr=0;
     }//for
}

void Uart1_SendByte(unsigned char data)
{
	while(!(rUTRSTAT1 & 0x2));  //Wait until THR is empty.
	rUTXH1=data;
}		

void Uart1_SendString(char *pt)
{
    while(*pt)Uart1_SendByte(*pt++);
}

//return 0=no 1=have messge and in Uart1OneMessgeBuff[] 
 char HaveUart1Message(void)
 {
     static short Uart1OneMessageCnt=0;
     static short Uart1DecodeMode=0;
     unsigned char recchar;

     while( Uart1RxWptr != Uart1RxRptr ){
         recchar=Uart1RxBuff[Uart1RxRptr++];
         if( Uart1RxRptr>=UARTRXMAX )Uart1RxRptr=0;
        // Uart1_SendByte( recchar ); // for test
         switch(Uart1DecodeMode){
             case 0: // receive  no bytes
                 if( recchar=='$' ){  //start
                     Uart1DecodeMode=1;
                     Uart1OneMessageCnt=0;
                     Uart1OneMessageBuff[Uart1OneMessageCnt++]=recchar;
                 }
                 break;
             case 1: //have receive start byte ')'
                 if( recchar== 0x0d )Uart1DecodeMode=2;
                 Uart1OneMessageBuff[Uart1OneMessageCnt++]=recchar;
                 if( Uart1OneMessageCnt > 1998 ){ //message too length and throw it 
                     Uart1DecodeMode = 0;
                 }
                 break;
             case 2: //have receive start byte ')'
                 Uart1DecodeMode = 0;
                 if( recchar== 0x0a ){
                      Uart1OneMessageBuff[Uart1OneMessageCnt++]=recchar;
                      Uart1OneMessageBuff[ Uart1OneMessageCnt ]= 0x0;
                      return 1;
                 }
                 break;
         }//switch(SioDecodeMode){	
     }//while
     return 0;
 }//function  

void SVRdec(char *pSource);
void WPTdec(char *pSource);
void ROUTEdec(char *pSource);
void TRACKdec(char *pSource);

void Uart1CmdSwitch(unsigned char *pSource)
{
     if(     strncmp((char *)pSource, (char *)"$TTSVR",6)==0 )SVRdec((char *)pSource); 
     else if( strncmp((char *)pSource,(char *)"$TTWPT",6)==0 )WPTdec((char *)pSource);
     else if( strncmp((char *)pSource,(char *)"$TTRUT",6)==0 )ROUTEdec((char *)pSource);
     else if( strncmp((char *)pSource,(char *)"$TTTRK",6)==0 )TRACKdec((char *)pSource);
}//function    


void SVRdec(char *pToken)
{
    
    short dotnub;
    char temp[40];
    short index;
    dotnub=0;
    while( (pToken=strchr( pToken , ',' ))!=0 ){
        dotnub++;
        pToken++;
        switch(dotnub){
            case 1: //index
	           if(*pToken==',')return ;
	           temp[0] = pToken[0];
	           temp[1] = 0x0;
		       index = (short)atoi( temp );
		       if( index < 1 || index >5 )return;
		       index--;
		       break;
	        case 2://name
	           if(*pToken==','){
	               myinfo[ index ].name[0] = 0x0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=35  )break;
	               }   
	               temp[i]=0;
		           sprintf( myinfo[ index ].name, "%s", temp );
		            myinfo[index].name[34]=0x0;
		       }    
	           break;
	        case 3://tel
	           if(*pToken==','){
	               myinfo[ index ].tel[0] = 0x0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=18  )break;
	               }   
	               temp[i]=0;
		           sprintf( myinfo[ index ].tel, "%s", temp );
		           myinfo[index].address[17]=0x0;
		       }    
		       break;
	        case 4://address
	           if(*pToken==','){
	               myinfo[ index ].address[0] = 0x0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== '*' || i>=35  )break;
	               }   
	               temp[i]=0;
		           sprintf( myinfo[ index ].address, "%s", temp );
		           myinfo[index].address[34]=0x0;
		       }    
	            break;
        }//switch
    }//while 
} 

//$TTWPT,0001,00,1875089,7230119*57
void WPTdec(char *pToken)
{
    
    short dotnub;
    char temp[40];
    char isend = 0;
    tagWayPoint TempWpt;    
    dotnub=0;
    while( (pToken=strchr( pToken , ',' ))!=0 ){
        dotnub++;
        pToken++;
        switch(dotnub){
            case 1:
               if( *pToken == '1' )isend = 1;
               else isend = 0;
               break;
            case 2: //name
	           if(*pToken==','){
	               TempWpt.name[0] = 0x0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=9  )break;
	               }   
	               temp[i]=0;
		           sprintf( (char *)TempWpt.name, "%s", temp );
		       }    
		       break;
	        case 3://icon
	           if(*pToken==','){
	               TempWpt.icon = 0x0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=2 )break;
	               }   
	               temp[i]=0;
	               TempWpt.icon = (unsigned char )atoi(temp );
		       }    
	           break;
	        case 4://lat
	           if(*pToken==','){
	               TempWpt.lat = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ','  )break;
	               }   
	               temp[i]=0;
		           TempWpt.lat = atoi(temp);
		       }    
		       break;
	        case 5://log
	           if(*pToken==','){
	               TempWpt.log = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== '*'  )break;
	               }   
	               temp[i]=0;
		           TempWpt.log = atoi(temp);
		       }    
		        //find the wpt
		        short tempindex;
		        tempindex = gWpt.FindWpt( (char *)TempWpt.name ); 
		        if( tempindex == -1 )gWpt.WriteNewWptNoFlash( TempWpt );
		        else gWpt.WriteWptNoFlash( TempWpt, tempindex );
		        if( isend == 1 )gWpt.FlashWriteWpt();
		        return;
	            break;
        }//switch
    }//while 
} 

       
void ROUTEdec(char *pToken)
{
    short dotnub;
    char temp[40];
    char isend = 0;
    short wptnub = 0;
    tagWayPoint TempWpt;    
    tagRoute TempRoute,ReadRoute;
    dotnub=0;
    while( (pToken=strchr( pToken , ',' ))!=0 ){
        dotnub++;
        if( dotnub > 6 )dotnub -=4;
        pToken++;
        switch(dotnub){
            case 1: // isend
               if( *pToken == '1' )isend = 1;
               else isend = 0;
               break;
            case 2: //totalwpt
	           if(*pToken==','){
	               TempRoute.totalwpt = 0x0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=2 )break;
	               }   
	               temp[i]=0;
	               TempRoute.totalwpt = (short )atoi(temp );
		       }   
		       wptnub = 0; 
		       break; 
            case 3: //name
	           if(*pToken==','){
	               TempWpt.name[0] = 0x0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=9  )break;
	               }   
	               temp[i]=0;
		           sprintf( (char *)TempWpt.name, "%s", temp );
		       }    
		       break;
	        case 4://icon
	           if(*pToken==','){
	               TempWpt.icon = 0x0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=2 )break;
	               }   
	               temp[i]=0;
	               TempWpt.icon = (unsigned char )atoi(temp );
		       }    
	           break;
	        case 5://lat
	           if(*pToken==','){
	               TempWpt.lat = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ','  )break;
	               }   
	               temp[i]=0;
		           TempWpt.lat = atoi(temp);
		       }    
		       break;
	        case 6://log
	           if(*pToken==','){
	               TempWpt.log = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== '*' || temp[i]== ',' )break;
	               }   
	               temp[i]=0;
		           TempWpt.log = atoi(temp);
		       }    
		        //find the wpt
		        short tempindex;
		        short temproutenub;
		        tempindex = gWpt.FindWpt( (char *)TempWpt.name ); 
		        if( tempindex == -1 ){
		             tempindex = gWpt.WriteNewWptNoFlash( TempWpt );
		             if( tempindex != -1 ){
		                 TempRoute.wpttab[ wptnub ] = tempindex;
		             }
		        }     
		        else TempRoute.wpttab[ wptnub ] = tempindex ; 
		        wptnub++;
		        if( wptnub == TempRoute.totalwpt ){
		             temproutenub = gRoute.FindFirstFreeRoute();
		             if( temproutenub != -1 ){
		               /////////if find same route ////////////////
		                 short havesame = 0;
		                 for( short i=0; i<gRoute.GetTotalRoute(); i++){
		                     if( havesame == 1 )break;
		                     if( gRoute.GetTabRoute( &ReadRoute, i ) == 1 ){
		                         if( ReadRoute.totalwpt != TempRoute.totalwpt )continue;
		                         havesame = 1;
		                         for( short j=0; j<ReadRoute.totalwpt; j++ ){
		                             if( ReadRoute.wpttab[j] != TempRoute.wpttab[j] ){
		                                 havesame = 0; break;
		                             }//if
		                         }//for
		                     }//if
		                 }//for( short i
		               /////////////////////////////////////////////  
		                 if( havesame == 0 ){
		                     TempRoute.isEnable = 1;
		                     gRoute.WriteTabRouteNoFlash( TempRoute, temproutenub );
		                 }    
		               //  Uart1_Tx_AddStr( (char *)"wroute", 6);  // for test
		             }//if( temproutenub != -1 ){
		             if( isend == 1 ){
		                 gWpt.FlashWriteWpt();
		                 gRoute.FlashWriteRoute();
		                 gRoute.CreateIndex();     
		             } 
		             return;
		        }
	            break;
        }//switch
    }//while 
}


//$TTTRK, 0, 3,18,   78,92556F0088AA1C00D3070A0A0B2B0C,EE546F00E4A91C00D3070A0A0B2B0E,4A546F0040A91C00D3070A0A0B2B10,A6536F009CA81C00D3070A0A0B2B12,02536F00F8A71C00D3070A0A0B2B14,6E526F0047A71C00D3070A0A0B2B16,50526F005DA61C00D3070A0A0B2B18,46526F0070A51C00D3070A0A0B2B1A,32526F0084A41C00D3070A0A0B2B1C,1E526F0098A31C00D3070A0A0B2B1E,0A526F00ACA21C00D3070A0A0B2B20,F6516F00C0A11C00D3070A0A0B2B22,E2516F00D4A01C00D3070A0A0B2B24,CE516F00E89F1C00D3070A0A0B2B26,BA516F00FC9E1C00D3070A0A0B2B28,A6516F00109E1C00D3070A0A0B2B2A,92516F00249D1C00D3070A0A0B2B2C,7E516F00389C1C00D3070A0A0B2B2E*16
void TRACKdec(char *pToken)
{
    short dotnub;
    char temp[50];
    TrackHead TempTrack;
    trackPoint TempTrackPoint;
    short routenub;
    short trackpoint;
    short TotalPoints,TotalTrackPoints;
    short curframe;
 //   short totalframe;
    dotnub=0;
    while( (pToken=strchr( pToken , ',' ))!=0 ){
        dotnub++;
        if( dotnub > 5 )dotnub -=1;
        pToken++;
        switch(dotnub){
            case 1: //route nub
	           if(*pToken==','){
	               routenub = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=2 )break;
	               }   
	               temp[i]=0;
	               routenub = (short )atoi(temp );
		       }   
		       TempTrack = gTrack.GetTrackHead( routenub ); 
		       break; 
            case 2: //curframe
	           if(*pToken==','){
	               curframe = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=2  )break;
	               }   
	               temp[i]=0;
		           curframe =(short )atoi( temp );
		       }  
		       if( curframe == 0){// clear track
		           TempTrack.CurrentPoint = 0;
		           TempTrack.HavePoints = 0;
		       }    
              break;

            case 3: //current frame trackpoints
	           if(*pToken==','){
	               TotalPoints = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=2  )break;
	               }   
	               temp[i]=0;
		           TotalPoints =(short )atoi( temp );
		       }  
		       trackpoint = 0;  
		       break;

            case 4: // route  total trackpoints
	           if(*pToken==','){
	               TotalTrackPoints = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=5  )break;
	               }   
	               temp[i]=0;
		           TotalTrackPoints =(short )atoi( temp );
		       }  
		       trackpoint = 0;  
		       break;

	        case 5://log
               short i;
               unsigned char uc;
               for(  i=0; ; i++ ){
                  temp[i]=pToken[i];
                  if( temp[i]== ',' || temp[i]=='*' )break;
               }   
               for( i=0; i<sizeof(trackPoint); i++){
                  if( temp[i*2] >= 'A' )uc= (temp[i*2]-'A'+10)<<4 ;
                  else uc= (temp[i*2]-'0')<<4 ;
                  if( temp[i*2+1] >= 'A' )uc += temp[i*2+1]-'A'+10 ;
                  else uc += temp[i*2+1]-'0' ;
                  temp[i]=(char )uc;
               }
	           TempTrackPoint = *(trackPoint* )temp;

		       TempTrack.pTrackSource[ TempTrack.CurrentPoint++ ] = TempTrackPoint;
		       if( TempTrack.CurrentPoint >= TempTrack.TotalTrackPoints )TempTrack.CurrentPoint = 0;
		       if( TempTrack.HavePoints < TempTrack.TotalTrackPoints )TempTrack.HavePoints++;
		       trackpoint ++;  
		       if( trackpoint >= TotalPoints ){
		           gTrack.WriteTrackHead( TempTrack, routenub );
		           // write to flash
                   return;		           
		       }
		       break;
        }//switch
    }//while 
}

/*
//$TTTRK, 0,   16, 7295656, 1876111,2003,10, 9,12,48,14, 7295656, 1876311,2003,10, 9,12,48,23, 7295656, 1876511,2003,10, 9,12,48,31, 7295656, 1876711,2003,10, 9,12,48,39, 7295656, 1876911,2003,10, 9,12,48,47, 7295656, 1877111,2003,10, 9,12,48,55, 7295670, 1877313,2003,10, 9,12,49, 2, 7295860, 1877464,2003,10, 9,12,49, 8, 7296064, 1877602,2003,10, 9,12,49,14, 7296268, 1877740,2003,10, 9,12,49,20, 7296472, 1877878,2003,10, 9,12,49,26, 7296597, 1878070,2003,10, 9,12,49,32, 7296409, 1878176,2003,10, 9,12,49,37, 7296209, 1878266,2003,10, 9,12,49,42, 7296009, 1878356,2003,10, 9,12,49,47, 7295809, 1878446,2003,10, 9,12,49,52,*53
void TRACKdec(char *pToken)
{
    short dotnub;
    char temp[40];
    TrackHead TempTrack;
    trackPoint TempTrackPoint;
    short routenub;
    short trackpoint;
    short TotalPoints;
    dotnub=0;
    while( (pToken=strchr( pToken , ',' ))!=0 ){
        dotnub++;
        if( dotnub > 10 )dotnub -=8;
        pToken++;
        switch(dotnub){
            case 1: //route nub
	           if(*pToken==','){
	               routenub = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=2 )break;
	               }   
	               temp[i]=0;
	               routenub = (short )atoi(temp );
		       }   
		       TempTrack = gTrack.GetTrackHead( routenub ); 
		       // clear track
		       TempTrack.CurrentPoint = 0;
		       TempTrack.HavePoints = 0;
		       break; 
            case 2: //trackpoints
	           if(*pToken==','){
	               TotalPoints = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=5  )break;
	               }   
	               temp[i]=0;
		           TotalPoints =(short )atoi( temp );
		       }  
		       trackpoint = 0;  
		       break;
	        case 3://log
	           if(*pToken==','){
	               TempTrackPoint.log = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=8 )break;
	               }   
	               temp[i]=0;
		           TempTrackPoint.log = atoi(temp);
		       }    
	           break;
	        case 4://lat
	           if(*pToken==','){
	               TempTrackPoint.lat = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=8  )break;
	               }   
	               temp[i]=0;
		           TempTrackPoint.lat = atoi(temp);
		       }    
		       break;
	        case 5://year
	           if(*pToken==','){
	               TempTrackPoint.year = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=4  )break;
	               }   
	               temp[i]=0;
		           TempTrackPoint.year =(unsigned short)atoi(temp);
		       }    
		       break;

	        case 6://m
	           if(*pToken==','){
	               TempTrackPoint.m = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=2  )break;
	               }   
	               temp[i]=0;
		           TempTrackPoint.m =(unsigned char)atoi(temp);
		       }    
		       break;
	        case 7://d
	           if(*pToken==','){
	               TempTrackPoint.d = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=2  )break;
	               }   
	               temp[i]=0;
		           TempTrackPoint.d =(unsigned char )atoi(temp);
		       }    
		       break;
	        case 8://hour
	           if(*pToken==','){
	               TempTrackPoint.hour = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=2  )break;
	               }   
	               temp[i]=0;
		           TempTrackPoint.hour =(unsigned char ) atoi(temp);
		       }    
		       break;
	        case 9://min
	           if(*pToken==','){
	               TempTrackPoint.min = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=2  )break;
	               }   
	               temp[i]=0;
		           TempTrackPoint.min = (unsigned char )atoi(temp);
		       }    
		       break;
	        case 10://sec
	           if(*pToken==','){
	               TempTrackPoint.sec = 0; 
	           }
	           else {
	               short i;
	               for(  i=0; ; i++ ){
	                  temp[i]=pToken[i];
	                  if( temp[i]== ',' || i>=2 || temp[i]=='*' )break;
	               }   
	               temp[i]=0;
		           TempTrackPoint.sec =(unsigned char )atoi(temp);
		       }    
		       TempTrack.pTrackSource[ TempTrack.CurrentPoint++ ] = TempTrackPoint;
		       if( TempTrack.CurrentPoint >= TempTrack.TotalTrackPoints )TempTrack.CurrentPoint = 0;
		       if( TempTrack.HavePoints < TempTrack.TotalTrackPoints )TempTrack.HavePoints++;
		       trackpoint ++;  
		       if( trackpoint >= TotalPoints ){
		           gTrack.WriteTrackHead( TempTrack, routenub );
		           // write to flash
		           
                   return;		           
		       }
		       break;
        }//switch
    }//while 
}
*/