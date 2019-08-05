#ifndef __ENM_CONSOLE_H__
#define __ENM_CONSOLE_H__

#include "EnmBase.hpp"
#include "EnmProtocol.hpp"


class ConsoleOption
{
public:

	ConsoleOption()
	{
		Init();
	};

	void Reset()
	{
		Init();
	}

private:
	void Init()
	{
		wave		= 0;		///< filter wave
		stepMode	= ENM_FORWARD_ON;
		scanCount	= 1;	///< scan count
		startColumn	= 0;
		columnCnt	= 12;  ///< total column

	/// plane shake
		shakeSpeed	= ENM_LIBRATE_SPEED_FAST; 
		shakeTime	= 5;

		/// reagent
		reagent		= ENM_REAGENT_NO;
		reagentCnt	= 0;
		memset( reagentArr, 0, sizeof(reagentArr) );
	}

/// member variables
public:
	INT8S	wave;		///< filter wave
	INT8S	stepMode;
	INT8U	scanCount;	///< scan count
	INT8U   startColumn;
	INT8U	columnCnt;  ///< total column

	/// plane shake
	INT8S	shakeSpeed; 
	INT8U	shakeTime;

	/// blank mode 
	INT8U	reagent; 
	INT8U	reagentCnt;
	INT8U	reagentArr[12];
};

enum enConsoleCommand
{
	ENM_CONSOLE_NONE, ENM_CONSOLE_SCAN, ENM_CONSOLE_CFG, ENM_CONSOLE_ENTER, ENM_CONSOLE_QUIT,
	ENM_CONSOLE_QUAEY_SN, ENM_CONSOLE_QUAEY_DATE, ENM_CONSOLE_QUAEY_VER 
};

enum enConsoleError
{
	ENM_CONSOLE_ERR_NONE, ENM_CONSOLE_OK, ENM_CONSOLE_ERR1, ENM_CONSOLE_ERR3
};

class CEnmConsole
{
/// member routines
public:	
	CEnmConsole();
	~CEnmConsole();

	
	enConsoleCommand SetConsoleCmdLine( INT8U* pCmdLine, INT16U size );
	void ResetScanOption() { mScanOption.Reset(); }

	enConsoleCommand GetLastCmd() { return mCurCmd; }
	ConsoleOption& GetOption() { return mScanOption; }

private:
	void 	Init();
		
/// member variables
private:
	CEnmUartProtocol	mParser;
	enConsoleCommand	mCurCmd;
	ConsoleOption		mScanOption;
};

#endif ///< __ENM_CONSOLE_H__
