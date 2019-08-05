#include <math.h>
#include "EnmConcDataUtil.h"
#include "EnmScanContext.h"

FP64 CEnmConcDataUtil::RangeData( FP64 data )
{
	FP64 aData;
	aData = (S32)(data * 10000) / 10000.0;
	
	if ( aData > 10000.0 )
	{
		aData = 10000.0;
	}
	else if ( aData < 0.0001 )
		aData = 0.0001;
		
	return aData;	
}

FP64  CEnmConcDataUtil::CalcConc( FP64& fConc, FP64 fBase, FP64 fBlank, U8 prec )
{
	FP64 aVal;
 	FP64 aRange;
	FP64 aRet;

	aRange = fBlank - fBase;
	aVal   = fConc - fBase;

	if ( aRange < 1.0 )
	{
		aRet = 0.0;
	}
	else if ( aVal < 1.0 )
	{
		aRet = 0.0;
	}
	else
	{
		
		aVal = RangeData( aRange / aVal );
		aRet = log10( aVal );
	}

	/// cut off precision
	aRet = ((U32)(aRet * pow( 10.0, (FP64)prec))) / (FP64)pow( 10.0, (FP64)prec );
	return aRet;

}

BOOLEAN  CEnmConcDataUtil::PickConcData( tagScanData& data, INT16U idx, FP64& fConc, FP64& fMin, FP64& fMax )
{
	if ( idx >= GETPLATEMAX() )	
		return FALSE;
	
	U16 aCol = GETPLATECOLUMN( idx ); 
	U16 aRow = GETPLATEROW(idx );

	fConc = data.dbData[aCol][aRow];
	fMin  = data.dbMinVal[aRow];
	fMax  = data.dbRefVal[aRow];

	return TRUE;
}
			
BOOLEAN  CEnmConcDataUtil::PutConcData( tagScanData& data, INT16U idx, FP64 fConc, FP64 fMin, FP64 fMax )
{
	if ( idx >= GETPLATEMAX() )	
		return FALSE;
	
	U16 aCol = GETPLATECOLUMN( idx ); 
	U16 aRow = GETPLATEROW(idx );

	data.dbData[aCol][aRow]	= fConc;
	data.dbMinVal[aRow]		= fMin;
	data.dbRefVal[aRow]		= fMax;

	return TRUE;	
}

BOOLEAN  CEnmConcDataUtil::PickConcData( FP64 *pData, INT16U idx, FP64& fConc )
{
	if ( idx >= GETPLATEMAX() )	
		return FALSE;
	
	if ( !pData )
		return FALSE;

	U16 aCol = GETPLATECOLUMN( idx ); 
	U16 aRow = GETPLATEROW(idx );

	fConc = pData[aCol * ENM_DATA_COUNT + aRow];
	return TRUE;
}

BOOLEAN  CEnmConcDataUtil::PutConcData( FP64 *pData, INT16U idx, FP64 fConc )
{
	if ( idx >= GETPLATEMAX() )	
		return FALSE;
	
	U16 aCol = GETPLATECOLUMN( idx ); 
	U16 aRow = GETPLATEROW(idx );

	pData[aCol * ENM_DATA_COUNT + aRow]  = fConc;	
	return TRUE;	
}
		
BOOLEAN CEnmConcDataUtil::CalcPcNc( CEnmScanContext& rScanContex )
{	
	if ( rScanContex.mProtocol.stMeasure.uMeasMode != MEASURE_QUALITATIVE )
		return FALSE;

	FP64 aPc, aNc ;
	aPc = aNc = 0.0;

#if 1
	FP64 *apConc = rScanContex.mCalcData.GetConc( );
	if ( apConc )
	{			
		U16 i;
		FP64  aConc, aMin, aMax;
	
		/// Calculate NC 
		U16 aStart = rScanContex.GetConcStartPos( CONC_NC );
		U16 aCnt = rScanContex.GetConcCount( CONC_NC );

		for ( i = aStart; i < (aStart + aCnt); i++ )
		{
			if ( CEnmConcDataUtil::PickConcData( apConc, i, aConc ) ) 
			{
				aNc += aConc;
			}
		}
		if ( aCnt )
			aNc /= aCnt;

		/// Calculate PC
		aStart = rScanContex.GetConcStartPos( CONC_PC );
		aCnt = rScanContex.GetConcCount( CONC_PC );

		for ( i = aStart; i < (aStart + aCnt); i++ )
		{
			if ( CEnmConcDataUtil::PickConcData( apConc, i, aConc ) ) 
			{
				aPc += aConc;
			}
		}
		if ( aCnt )
			aPc /= aCnt;

		rScanContex.mCalcData.SetPNc( aPc, aNc );
	}

#else
	if ( rScanContex.HasScanData() )
	{
		tagScanData *apData = rScanContex.GetScanData();
		if ( !apData )
			return FALSE;
			
		U16 i;
		FP64  aConc, aMin, aMax;
	
		/// Calculate NC 
		U16 aStart = rScanContex.GetConcStartPos( CONC_NC );
		U16 aCnt = rScanContex.GetConcCount( CONC_NC );

		for ( i = aStart; i < (aStart + aCnt); i++ )
		{
			if ( CEnmConcDataUtil::PickConcData( *apData, i, aConc, aMin, aMax ) ) 
			{
				aNc += aConc;
			}
		}
		if ( aCnt )
			aNc /= aCnt;

		/// Calculate PC
		aStart = rScanContex.GetConcStartPos( CONC_PC );
		aCnt = rScanContex.GetConcCount( CONC_PC );

		for ( i = aStart; i < (aStart + aCnt); i++ )
		{
			if ( CEnmConcDataUtil::PickConcData( *apData, i, aConc, aMin, aMax ) ) 
			{
				aPc += aConc;
			}
		}
		if ( aCnt )
			aPc /= aCnt;

		rScanContex.mCalcData.SetPNc( aPc, aNc );
	}
#endif

	return TRUE;
}

