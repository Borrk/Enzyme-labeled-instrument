#ifndef __EnmConcDataUtil_H
#define __EnmConcDataUtil_H

#include "EnmBase.hpp"
#include "EnmData.h"

class CEnmScanContext;

class  CEnmConcDataUtil
{
public:
	static	BOOLEAN PickConcData( tagScanData& data, INT16U idx, FP64& fConc, FP64& fMin, FP64& fMax );	
	static	BOOLEAN PutConcData( tagScanData& data, INT16U idx, FP64 fConc, FP64 fMin, FP64 fMax );	

	static	BOOLEAN PickConcData( FP64 *pData, INT16U idx, FP64& fConc );	
	static	BOOLEAN PutConcData(  FP64 *pData, INT16U idx, FP64 fConc );	

	static  FP64	CalcConc( FP64& fConc, FP64 fBase, FP64 fBlank, U8 prec );
	static  BOOLEAN CalcPcNc( CEnmScanContext& rScanContex );

protected:
	static FP64		RangeData( FP64 data );

};

#endif ///< __EnmConcDataUtil_H
