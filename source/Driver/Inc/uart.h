#ifndef __UART_H__
#define __UART_H__

//#ifdef __cplusplus
//extern "C" {
//#endif

///////////////uart0/////////////////////
extern unsigned char Uart0OneMessageBuff[]; //300 bytes
extern unsigned long ToSendFlag;
extern unsigned long RecCmdFlag;  
#define TAB0  0x1           // bit0=tab0 0x53
#define TAB1  0x2             // bit1=tab1 0x54
#define TAB2  0x4             // bit2=tab2 0x55
#define TAB3  0x8             // bit3=tab3 0x56
#define RDSN  0x10             // bit4=read s/n  0x52
#define WROEM 0x20             // bit5=send speed to oem  0x83
#define RDREG 0x40             // bit6=read reginfo 0x59
#define WRREG 0x80            // bit7=write reginfo 0x58
//#define   0x100            // bit8= 

//extern char  haveUart0Error;

void Uart0_Init(long baud);
char HaveUart0Message(void);
char Uart0ProtocolDecode(unsigned char *Sou_Buffer);
void Uart0CmdSwitch(unsigned char *pSource);
void Uart0_SendStringLength(char *pt,short length);
//int  Protocol_Add_Shift0(unsigned char lenght,unsigned char command,unsigned char *Sou_Buffer,unsigned char *tempbuffer);
void CheckCmdToSend(void);
void ToCmdSN(void);
void ToCmdRdReg(void);
void ToCmdOEM(void);
void ToCmdWrReg(void);
void ToCmdWrRegErase(void);

///////////////uart1/////////////////////

extern short Uart1TxWptr;
extern short Uart1TxRptr;
extern unsigned char Uart1TxBuff[];
void Uart1_Init(long baud);
void Uart1_Tx_AddStr( char *pSource,short bytes);

void Uart1_SendByte(unsigned char data);
void Uart1_SendString(char *pt);
extern unsigned char Uart1OneMessageBuff[];
char HaveUart1Message(void);  ////return 0=no 1=have messge and in Uart1OneMessgeBuff[] 
void Uart1CmdSwitch(unsigned char *pSource);

//#ifdef __cplusplus
//}
//#endif

#endif /*__UART_H__*/
