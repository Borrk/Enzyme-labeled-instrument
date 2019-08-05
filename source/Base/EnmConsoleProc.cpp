// EnmMainProc.cpp: implementation of the CEnmConsoleProc class.
//
//////////////////////////////////////////////////////////////////////

#include "EnmConsoleProc.h"
#include "EnmConsolePage.h"
#include "EnmConcDataUtil.h"
#include "MigEventMng.h"
#include "EnmFormater.hpp"
#include "EnmApp.hpp"
#include "EnmBeeper.hpp"

extern "C" char UartGetch( unsigned char *pData );
void SendByte( INT8U *pBuf, INT16U size )
{
#ifdef ENM_VER_TARGET

	INT16U i = 0;
	INT8U *p = pBuf;

	while ( i < size )
	{
		Uart_SendByte( *p );
		p++;
		i++;
	}
#endif
}

INT16U GetByte( INT8U *pBuf, INT16U size )
{
	INT16U i, aCnt = 0;
	INT8U *p = pBuf;

#ifdef ENM_VER_TARGET
	
	while ( UartGetch( p ) )
	{
		aCnt++;
		p++;
		if ( aCnt > size )
			break;
	}	

#endif

	return aCnt;
}


CEnmConsoleProc::CEnmConsoleProc()
{
	mState = PROC_SUSPEND;
	mpConsolePage = NULL; 
}

CEnmConsoleProc::~CEnmConsoleProc()
{
	if ( mpConsolePage )
	{
		delete mpConsolePage;
		mpConsolePage = NULL; 
	}
}

BOOLEAN  CEnmConsoleProc::InitEngine()
{
	mpConsolePage = new CEnmConsolePage;
	mpConsolePage->SetConsole( &mConsole );

	return TRUE;
}

enProcState  CEnmConsoleProc::IResume( )
{
	inherited::IResume();

	if ( mState == PROC_BUSY )
	{
		MigPage::SetCurFocusPage( mpConsolePage );
	}

	return mState;
}

enProcState CEnmConsoleProc::IEnterProc( )
{
	ProcConsoleCommand();

	if ( PROC_SUSPEND == mState )
		return mState;
	
	MigEventMng &aEventMng = MigEventMng::Instance();

	tagMigMSG *ap = aEventMng.GetMsg();
	while ( ap )
	{
		OnMessage( *ap );
		ap = aEventMng.GetMsg();
	}
	
	return mState;
}

void  CEnmConsoleProc::OnMessage( tagMigMSG& msg )
{
	U32 aKey;	
	tagScanData *apScanData;

	switch ( msg.msg )
	{
	case ENM_MSG_KEY:
		CEnmBeeper::Beep( BEEP_ALARM );		
		break;

	case ENM_MSG_CFG_CHG:
		/// do nothing
		break;

	case ENM_MSG_SCAN_OK:
		apScanData = CEnmApp::Instance().GetScanContext().GetScanData( TRUE );
		if ( apScanData )
		{
			SendScanData( apScanData );
		}

		if ( ++ mScanCount == mConsole.GetOption().scanCount )
		{
			mScanCount = 0;
			SendAckMsg();
		}
		break;

	default:
		break;
	}

}

void  CEnmConsoleProc::OnModeChange( enEnmSysMode mode )
{
	CEnmApp::Instance().ChangeMode( mode );

	if ( GetMode() == ENM_MODE_PC )
	{
		mConsole.ResetScanOption();
		mpConsolePage->SetTip( " " );
		MigPage::SetCurFocusPage( mpConsolePage );
	}
}

enEnmSysMode  CEnmConsoleProc::GetMode()
{
	return CEnmApp::Instance().GetSysMode();
}

void  CEnmConsoleProc::ProcConsoleCommand()
{
	INT16U	aCmdLen;
	INT16U  i;
	enConsoleCommand  aCmd = ENM_CONSOLE_NONE;
	enConsoleError    aErr = ENM_CONSOLE_ERR_NONE;
	static MCHAR aBuff[256];

	tagMigMSG msg;
	tagScanData  *apData;

	tagDate aDate;
	tagTime aTime;

	BOOLEAN bOk = FALSE;

	aCmdLen = GetByte( (INT8U*)aBuff,  sizeof(aBuff) );	
	if ( aCmdLen > 0 )
	{
		aCmd = mConsole.SetConsoleCmdLine( (INT8U*)aBuff, aCmdLen );
	}

	while ( aCmd != ENM_CONSOLE_NONE )
	{
		bOk = FALSE;
		aErr = ENM_CONSOLE_OK;
		switch ( aCmd )
		{
			case ENM_CONSOLE_ENTER:
				SendAckMsg();
				OnModeChange( ENM_MODE_PC );
				bOk = TRUE;
				break;

			case ENM_CONSOLE_QUIT:
				SendAckMsg();
				OnModeChange( ENM_MODE_ALONE );
				bOk = TRUE;
				break;

			case ENM_CONSOLE_CFG:
				SendAckMsg();
				/// do nothing now
				bOk = TRUE;
				break;

			case ENM_CONSOLE_SCAN:
				if ( GetMode() != ENM_MODE_PC )
				{
					aErr = ENM_CONSOLE_ERR3;
					break;
				}

				SendAckMsg();
//				for ( i = 0; i < mConsole.GetOption().scanCount; i++ )
//				{
					mScanCount = 0;
					/// notify to scan
					msg.msg = ENM_MSG_SCAN;
					msg.param1 = mConsole.GetOption().scanCount;
					mpConsolePage->OnMessage( msg );

//					/// send data to PC host
//					if ( apData = mpConsolePage->GetScanData() )
//					{
//						SendScanData( apData );
//					}
//				}
//
//				SendAckMsg();
				bOk = TRUE;
				aErr = ENM_CONSOLE_ERR_NONE;
				break;

			case ENM_CONSOLE_QUAEY_DATE:
				SendAckMsg();
			
				aDate = EnmGetDate( );
				aTime = EnmGetTime( );
				sprintf( aBuff, "%4d-%d-%d %d:%d:%d", aDate.year, aDate.month, aDate.day,
								aTime.hour, aTime.min, aTime.sec 
						);
				
				SendByte( (INT8U*)aBuff, strlen(aBuff) );
				//Uart_SendString( aBuff );
				break;

			case ENM_CONSOLE_QUAEY_SN:
				// add later
				SendAckMsg();
				bOk = TRUE;
				break;

			case ENM_CONSOLE_QUAEY_VER:
				// add later
				SendAckMsg();
				bOk = TRUE;
				break;
			
			default:
				aErr = ENM_CONSOLE_ERR3;
				break;
		}

		/// parse other command, not giving cmd chars
		aCmd = mConsole.SetConsoleCmdLine( (INT8U*)aBuff, 0 );
	}
}

void  CEnmConsoleProc::SendAckMsg()
{
	INT16U aLen = 4;
	INT8U aOKAck[4] = { 'O', 'K', 13, 10 };
	SendByte( (INT8U*)aOKAck, aLen );
}

BOOLEAN  CEnmConsoleProc::SendScanData( tagScanData* pData )
{
	INT8U	i,j;
	FP64	aVal[24];
	INT16U	aLen;
	MCHAR	aszTemp[16];
	MCHAR	aszData[128];

	CEnmFormater aFormater;
	FP64 fConc, fBase, fBlank;

	for ( i = 0; i < ENM_DATA_COUNT; i++ )
	{
		aszData[0] = '\0';
		for ( j = 0; j < ENM_DATA_GROUP; j++ )
		{
			if ( CEnmConcDataUtil::PickConcData( *pData, i, fConc, fBase, fBlank ) )

			aLen = strlen(aszData);
		
			FormatFP( aVal[j], aszTemp, 6, 6-2 );
			PadFormat( aszTemp, &aszData[aLen], 6, 0 );
		}
		strcat( aszData, "\r\n" );

		SendByte( (INT8U*)(aszData), strlen(aszData) );
 	}

	return TRUE;
}

