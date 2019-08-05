#ifndef __ENM_PRINTER_H__
#define __ENM_PRINTER_H__

#include "EnmData.h"
#include "EnmFormater.hpp"
#include "MIG_Base.hpp"	// Added by ClassView

/**
 *		¥Ú”°
 *
 */ 	

class CEnmScanContext;

class CEnmPrinter
{
public:
	CEnmPrinter();
		
	BOOLEAN Print( void );
	BOOLEAN DetectPrinter();
	void SetPrintContext( CEnmScanContext *pContext ); 
	
protected:
	void Init();
	void SetPrinter();
	
	void PrintHeader( void );
	void PrintTestInfo( void );
	void PrintTestMode( void );

	virtual void PrintMainData( void );
	virtual void PrintAttachData( void );
	
	void FormatBlankPos( MCHAR *pszDst, INT8U mode );
	void FormatRawData( MCHAR *pszDst, INT8U idx );
	INT16U FormatData( FP64 *pData, MCHAR *pszDst, INT8U idx);

	virtual U16		FormatModeInfo( MCHAR *pszInfo ); ///< seperated by '\0', the last one is two '\0'.
	static MCHAR*	nGetMeasureModeStr( U8 mode );

	static	U16 PickRowConcData( FP64 *pData, U16 row, FP64* pfConc, U16 uMax );	

/// memeber variabls
	CEnmFormater* mpCurFormater;	
	INT8U		mLeftAlign;	
	CEnmScanContext	*mpScanContext;
};

class CEnmEmbedPrinter : public CEnmPrinter
{
public:
	CEnmEmbedPrinter()
	{
		mLeftAlign = 0;
	}

protected:
	virtual void PrintMainData( void );
	virtual void PrintAttachData( void );
};
	
#endif	// __ENM_PRINTER_H__
