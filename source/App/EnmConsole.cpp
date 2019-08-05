#include "EnmConsole.h"

CEnmConsole::CEnmConsole()
{
	Init();
}
	
CEnmConsole::~CEnmConsole()
{
}	

#include "Mig_Page.hpp"
enConsoleCommand  CEnmConsole::SetConsoleCmdLine( INT8U* pCmdLine, INT16U size )
{
	INT8S aFrame[32];

	mCurCmd = ENM_CONSOLE_NONE;

	mParser.Push( (INT8U*)pCmdLine, size );

	INT16U aLen = mParser.FindeFrame( (INT8U*)aFrame );
	if ( aLen <= 0 )
		return mCurCmd;

	/// parse command
	MCHAR ch1, ch = aFrame[0];
	switch ( ch )
	{
	case 'R':
		mCurCmd = (aLen == 1) ? ENM_CONSOLE_ENTER : ENM_CONSOLE_NONE;
		break;

	case 'Q':
		mCurCmd = (aLen == 1) ? ENM_CONSOLE_QUIT : ENM_CONSOLE_NONE;
		break;

	case 'P':
		ch = aFrame[1];
		if ( aLen == 1 )
		{
			mScanOption.scanCount = 1;
			mCurCmd = ENM_CONSOLE_SCAN;
		}
		else if (aLen == 2 && ch == 'M' )
		{
			mScanOption.scanCount = 1;
				/// do nothing, now ignore PM command
		}
		else
		{
			if ( '0' <= ch && ch <= '9' )
			{
				ch1 = aFrame[2];

				if ( aLen == 2 )
				{
					mScanOption.scanCount = ch - '0';
					mCurCmd = ENM_CONSOLE_SCAN;
				}
				else if ( aLen == 3 && ('0' <= ch1 && ch1 <= '9') )
				{
					mScanOption.scanCount = (ch - '0') * 10 + (ch1 - '0');
					mCurCmd = ENM_CONSOLE_SCAN;
				}
			}
		}

		break;

	case 'M':
		ch = aFrame[1];
		ch1 = aFrame[2];
		if ( aLen == 2 && ('1' <= ch && ch <= '9') )
		{
			mScanOption.startColumn = ch - '0' - 1;
			mCurCmd = ENM_CONSOLE_SCAN;
		}
		else if ( aLen == 3 && ('0' <= ch1 && ch1 <= '9') )
		{
			mScanOption.startColumn = (ch - '0') * 10 + (ch1 - '0') - 1;
			if ( mScanOption.startColumn < 12 )
			{
				mCurCmd = ENM_CONSOLE_SCAN;
			}
			else
			{
				mScanOption.startColumn = 0;
				mCurCmd = ENM_CONSOLE_NONE;				
			}
		}
		break;

	case 'F':
		/// now ignore FMn command
		if ( '1' <= aFrame[1] && aFrame[1] <= '8' )
		{
			mScanOption.wave = aFrame[1] - '0' - 1;
			mCurCmd = ENM_CONSOLE_CFG;
		}
		break;

	case 'E':
		ch = aFrame[1];
		if ( ch == '0' )
		{
			mScanOption.stepMode = ENM_FORWARD_ON;
			mCurCmd = ENM_CONSOLE_CFG;
		}
		else if ( ch == '1' )
		{
			mScanOption.stepMode = ENM_FORWARD_STEP;
			mCurCmd = ENM_CONSOLE_CFG;
		}
		break;

		/// plate shake 
	case 'X':
		if ( aLen == 2 )
		{
			ch = aFrame[1];
			if ( ch == '1' )
			{
				mScanOption.shakeSpeed = ENM_LIBRATE_SPEED_FAST;
				mCurCmd = ENM_CONSOLE_CFG;
			}
			else if ( ch == '2' )
			{
				mScanOption.shakeSpeed = ENM_LIBRATE_SPEED_MIDDLE;
				mCurCmd = ENM_CONSOLE_CFG;
			}
			else if ( ch == '3' )
			{
				mScanOption.shakeSpeed = ENM_LIBRATE_SPEED_SLOW;
				mCurCmd = ENM_CONSOLE_CFG;
			}
		}
		break;

	case 'Z':
		ch = aFrame[1];
		ch1 = aFrame[2];
		if ( aLen == 2 && ('0' <= ch && ch <= '9') )
		{
			mScanOption.shakeTime = ch - '0';
			mCurCmd = ENM_CONSOLE_SCAN;
		}
		else if ( aLen == 3 && ('0' <= ch1 && ch1 <= '9') )
		{
			mScanOption.shakeTime = (ch - '0') * 10 + (ch1 - '0');
			if ( mScanOption.startColumn <= 60 )
				mCurCmd = ENM_CONSOLE_CFG;
			else
			{
				mScanOption.shakeTime = 0;
				mCurCmd = ENM_CONSOLE_NONE;				
			}
		}
		break;

	case 'A':
		if (aLen == 1) 
		{
			mScanOption.reagent = ENM_REAGENT_NO;
			mScanOption.reagentCnt = 0;
			mCurCmd = ENM_CONSOLE_CFG;
		}
		break;

	case 'B':
		if ( mScanOption.reagentCnt >= 12 )
			break;

		if (aLen == 2 ) 
		{
			if( '1' <= aFrame[1] && aFrame[1] <= '9' )
			{
				mScanOption.reagent = ENM_REAGENT_LN;
				mScanOption.reagentArr[mScanOption.reagentCnt++] = aFrame[1] - '0' - 1;
				mCurCmd = ENM_CONSOLE_CFG;
			}
		}
		else if ( aLen == 3 )
		{
			ch = aFrame[1];
			ch1 = aFrame[2];

			if ( '1' <= aFrame[1] && aFrame[1] <= '9' && '1' <= aFrame[1] && aFrame[1] <= '9')
			{
				mScanOption.reagentArr[mScanOption.reagentCnt] = (ch - '0' - 1) * 10 + (ch1 - '0' - 1);
				if ( mScanOption.reagentArr[mScanOption.reagentCnt] <= 12 )
				{
					mScanOption.reagent = ENM_REAGENT_LN;
					mScanOption.reagentCnt++;
					mCurCmd = ENM_CONSOLE_CFG;
				}
			}
		}
		break;

	/// Quary commandsw
	case 'T':
		if (aLen == 1) 
		{
			mCurCmd = ENM_CONSOLE_QUAEY_DATE;
		}
		break;

	case 'N':
		if (aLen == 1) 
		{
			mCurCmd = ENM_CONSOLE_QUAEY_SN;
		}
		break;

	case 'V':
		if (aLen == 1) 
		{
			mCurCmd = ENM_CONSOLE_QUAEY_VER;
		}
		break;
	}

	if ( mCurCmd == ENM_CONSOLE_SCAN )
		mScanOption.columnCnt = 12 - mScanOption.startColumn;

	/// temp for debug
	if ( mCurCmd != ENM_CONSOLE_NONE )
	{
		PutStr( 10, 30, (MCHAR*)aFrame, 0 );
	}

	return mCurCmd;
}

void 	CEnmConsole::Init()
{
	mParser.SetBufferLen( 128 );
	mCurCmd = ENM_CONSOLE_NONE;
	mScanOption.Reset();
}	
