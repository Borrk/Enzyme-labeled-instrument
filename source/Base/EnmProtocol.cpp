#include "EnmProtocol.hpp"

/**
 *  UBx Protocol parse class
 *
 *	Sail 
 *	2005-7-1 16:15
 */

#include <string.h>
#include "EnmProtocol.hpp"


CEnmUartProtocol::CEnmUartProtocol()
{
	Init();
}

CEnmUartProtocol::~CEnmUartProtocol()
{
	if ( mpCacheBuf )
	{
		delete []mpCacheBuf;
	}	
}
	
BOOLEAN CEnmUartProtocol::SetBufferLen( INT32U len )
{
	/// must larger than 1024
	if ( len < 16 )
		return FALSE;
	
	INT8U* ap = new INT8U[len];	
	if ( !ap )
		return FALSE;
		
	if ( mpCacheBuf )
	{
		delete []mpCacheBuf;
		mpCacheBuf = NULL;
		mCacheSize = 0;
	}
	
	mpCacheBuf = ap;		
	mCacheSize = len;

	return TRUE;
}

/*
BOOLEAN	CEnmUartProtocol::MakeFrame( tagUbxMessage& rMessage )
{
	if ( rMessage.pMsgBody == NULL && rMessage.len != 0 )
		return FALSE;

	if ( rMessage.pOutFrame == NULL )
		return FALSE;

	INT8U aFrameHead[] = { SYN_A, SYN_B, 0x00, 0x00, 0x00, 0x00 };
	aFrameHead[2] = rMessage.uClass;
	aFrameHead[3] = rMessage.uID;
	aFrameHead[4] = (INT8U)(rMessage.len);
	aFrameHead[5] = (INT8U)((rMessage.len /256 ));

	memcpy( rMessage.pOutFrame, aFrameHead, 6 );
	memcpy( rMessage.pOutFrame+6, rMessage.pMsgBody, rMessage.len );

	INT16U aChksum = CalChksum( &rMessage.pOutFrame[2], rMessage.len + 4 );
	rMessage.pOutFrame[6+rMessage.len]   = (INT8U)aChksum;
	rMessage.pOutFrame[6+rMessage.len+1] = (INT8U)(aChksum >> 8);

	rMessage.len += 8;	///< the whole frame's length

	return TRUE;
}
*/
	
void	CEnmUartProtocol::Push( INT8U* pInData, INT32U size )
{
	long	 aTempPos, aEndPos, aFree, aFrameLen;
	char	 abCover = 0;
	INT8U*		pData = pInData;

	/// copy to cache
	if ( mDataLen == 0 )
	{
		aTempPos = 0;
		//if ( (aTempPos = FindFrameHeader(pData, size)) != -1 )
		{
			mStartPos = 0;
			mFramePos = 0;

			/// if the input data > the cache, kick off the rest data
			if ( size - aTempPos > mCacheSize )
			{
				size = mCacheSize;
			}

			memcpy( mpCacheBuf, &pData[aTempPos], size - aTempPos );
			mDataLen = size - aTempPos;
		}
	}
	else
	{
		/// no enough space to push the data
		/// part of the input data will lost!
		if ( size > (mCacheSize - mDataLen) )
		{
			abCover = 1;
			size = (mCacheSize - mDataLen);
		}

		if ( (mStartPos + mDataLen + size) <= mCacheSize )
		{
			aEndPos = mStartPos + mDataLen;
			aFree = 0;
		}
		else
		{
			if ((mStartPos + mDataLen) <  mCacheSize )
			{
				aEndPos = mStartPos + mDataLen;
				aFree = mCacheSize - (mStartPos + mDataLen);
			}
			else
			{
				aEndPos = mStartPos + mDataLen - mCacheSize;
				aFree = 0;
			}
		}

		if ( aFree == 0 )
		{
			memcpy( &mpCacheBuf[aEndPos], pData, size );
		}
		else
		{
			memcpy( &mpCacheBuf[aEndPos], pData, aFree );
			memcpy( &mpCacheBuf[0], &pData[aFree], (size - aFree) );
		}

		mDataLen += size;
	}	
}

INT32U 	CEnmUartProtocol::FindeFrame( INT8U* pFrameBuf )  ///< Search frame from the cache buffer	
{
	INT32S aLen1, aLen2;
	INT8U* p = mpCacheBuf + mStartPos;

	if ( (mStartPos + mDataLen) <= mCacheSize )
	{
		aLen1 = mDataLen;
		aLen2 = 0;
	}
	else
	{
		aLen1 = mCacheSize - mStartPos - 1;
		aLen2 = (mStartPos + mDataLen) - mCacheSize + 1;
	}

	BOOLEAN bFind = FALSE;
	INT32S i = 0;
	INT32S aFrameLen = 0;
	while ( i < aLen1 )
	{
		if ( p[i] == SYN_A )
		{
			aFrameLen = i+1;
			
			/// check if has a frame
			if ( aFrameLen > 0 )
			{
				if ( aFrameLen <= mDataLen )
				{
					if ( aFrameLen <= aLen1 )
					{
						memcpy( pFrameBuf, p, aFrameLen );
						mStartPos = mStartPos+aFrameLen;
					}
					else
					{
						memcpy( pFrameBuf, p, aLen1 );
						memcpy( &pFrameBuf[aLen1], mpCacheBuf, (aFrameLen - aLen1) );
						mStartPos = aFrameLen - aLen1;
					}
			
					bFind = TRUE;
					mDataLen -= aFrameLen;
					break;
				}
				else
				{
					aFrameLen = 0;
					break;
				}
			}
			else	///< a invalid frame, kick it off
			{
				mStartPos = mStartPos + aFrameLen; ///< header length ignored
				mDataLen -= aFrameLen;
				break;
			}
		}

		i++;
	}

	/// remove space char and frame end char
	aLen1 = 0;
	if ( bFind )
	{
		for ( i = 0; i < aFrameLen - 1; i++ )
		{
			if ( pFrameBuf[i] != ' '  )
				pFrameBuf[aLen1++] = pFrameBuf[i];
		}

		pFrameBuf[aLen1] = '\0';
		aFrameLen = aLen1;
	}

	/// move the rest bytes to the header
#if 1
	if ( mDataLen < 2 )
	{
		memcpy( mpCacheBuf, &mpCacheBuf[mStartPos], mDataLen );
		mStartPos = 0;
	}
#endif

	return aFrameLen;
}


void 	CEnmUartProtocol::Init()
{
	mpCacheBuf = NULL;	
	mCacheSize = CACHE_SIZE;
	mStartPos  = 0;
	mFramePos  = -1;
	mDataLen   = 0;
		
	SetBufferLen( mCacheSize );
}

