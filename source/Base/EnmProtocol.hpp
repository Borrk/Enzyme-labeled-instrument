#ifndef __ENM_UART_PROTOCOL_
#define __ENM_UART_PROTOCOL_

#include "EnmBase.hpp"

class CEnmUartProtocol
{
/// member routines
public:
	
	enum  { SYN_A = '\n' };
	
	CEnmUartProtocol();
	~CEnmUartProtocol();

	BOOLEAN SetBufferLen( INT32U len );  ///< Set buffer len	
	void	Push( INT8U* pInData, INT32U size );	///< Save buffer to cache
	INT32U 	FindeFrame( INT8U* pFrameBuf );  ///< Search frame from the cache buffer, return frame's size	
	
	//static BOOLEAN	MakeFrame( tagUbxMessage& rMessage ); ///< generate UBX message
	
private:
	void 	Init();
		
/// member variables
private:
	enum { CACHE_SIZE = 128 };
	INT8U* 	mpCacheBuf;	
	INT32U		mCacheSize;
	INT32S		mStartPos;
	INT32U		mDataLen;
	
	INT32S		mFramePos;
};

#endif ///< __ENM_UART_PROTOCOL_
