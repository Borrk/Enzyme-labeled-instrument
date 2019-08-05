#include <math.h>
#include <string.h>
#include "EnmDef.h"
#include "EnmDataProc.h"
#include "Matrix.h"
#include "EnmMathParser.hpp"

BYTE  nEnmCalParam(  double *pSrc, double *pParam, tagCompareCal cfg );
short  nEnmGetSampleData(  tagSampleData *pSampleData, double *pSrc, unsigned short sampleCnt, double *pData );
BYTE  nEnmLineParam( double *pSampleData,  short sampleCnt,  double *pParam );
BYTE  nEnmSquareParam(  double *pSampleData,  short sampleCnt,  double *pParam );
BYTE  nEnmCubicParam(  double *pSampleData,  short sampleCnt,  double *pParam );
BYTE  nEnmExpParam( double *pSampleData,  short sampleCnt,  double *pParam );
BYTE   nEnmCompareCal( U8 formula, double *pSrc, double *pDst , double *param, unsigned short col, unsigned short ln  );

///   因数法计算
 BYTE   EnmFactorCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  )
 {
 	unsigned short i; 
	tagFactorCal factorCal = pFormul->unFormul.stFactorCal;

	if ( !pSrc || !pDst )
 		return 0;
 		
	///  	
	for ( i = 0; i < col * ln; i++ )
	{
		*(pDst + i) = *( pSrc + i ) * factorCal.factor;
	}	
	
	return 1;
}

///	标准浓度法计算
 BYTE   EnmCompareCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  )
 {
	 unsigned short i;
	double  aParam[4] = { 0.0 };
	tagCompareCal stCompareCal = pFormul->unFormul.stCompareCal;
 	if ( !pSrc || !pDst )
 		return 0;
 	
 	/// Calculate formula's parameters	
	nEnmCalParam(  pSrc, aParam, stCompareCal );	 
	
	/// process sample	
	nEnmCompareCal( stCompareCal.culFormula, pSrc, pDst ,  aParam, col,   ln  );

	/// save curve coefficient
	pFormul->stCoefficent.a = aParam[0];
	pFormul->stCoefficent.b = aParam[1];
	pFormul->stCoefficent.c = aParam[2];
	pFormul->stCoefficent.d = aParam[3];

	return 1;
}

////  标准直线法计算
BYTE   EnmLinearCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  )
 {
	unsigned short i;
	tagLineCal lineCal = pFormul->unFormul.stLineCal;

 	if ( !pSrc || !pDst )
 		return 0;
 		
	///  	
	for ( i = 0; i < col * ln; i++ )
	{
		*(pDst + i) = *( pSrc + i ) * lineCal.ratio + lineCal.dist ;
	}	
	
	return 1;
}

 ////  限值法计算
BYTE   EnmRangeCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  )
 {
	unsigned short i;
	tagThresholdCal thresholdCal  = pFormul->unFormul.stThresholdCal;

 	if ( !pSrc || !pDst )
 		return 0;
 		
	///  	
	for ( i = 0; i < col * ln; i++ )
	{
		pDst[i] =  (pSrc[i] < thresholdCal.threshold) ? -1 : 1;
	}	
	
	return 1;
}

 ////  双限值法计算
BYTE   EnmBiRangeCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  )
{
	unsigned short i;
	tagBiThresholdCal cal = pFormul->unFormul.stBiThresholdCal;

 	if ( !pSrc || !pDst )
 		return 0;
 		
	for ( i = 0; i < col * ln; i++ )
	{
		if ( pSrc[i] > cal.thresholdHi ) 
		{
			pDst[i] =  1;
		}
		else if ( pSrc[i] < cal.thresholdLo )
		{
			pDst[i] =  -1;
		}
		else
		{
			pDst[i] =  0;
		}
	}	

	return 1;
}


 ////  范围法计算
BYTE   EnmSegmentCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  )
{
	 unsigned short i, j;
	 double aVal, aStep;
	tagRangeCal cal = pFormul->unFormul.stRangeCal;

 	if ( !pSrc || !pDst )
 		return 0;
 		
	aStep = (cal.maxVal - cal.minVal) / 10.0;

	for ( i = 0; i < col * ln; i++ )
	{
		aVal = pSrc[i];

		pDst[i] = -1;	/// invalid value
		for ( j = 0; j < 10; j++ )
		{
			if ( (cal.minVal + j * aStep <= aVal ) && (aVal < cal.minVal + (j+1) * aStep) ) 
			{
				pDst[i] =  j;
				break;
			}
		}
	}	

	return 1;
}

 ////  列减法计算
 BYTE   EnmDifferCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  )
 {
	short  i,  j;
	tagSubtractCal cal = pFormul->unFormul.stSubtractCal;

 	if ( !pSrc || !pDst )
 		return 0;
 		
	///  	
	if (   0 == cal.subRule )	///<  1-2, 3-4
	{
		for ( i = 0; i < col - 1; i+=2 )
		{
			for ( j = 0; j <  ln; j++ )
			{
				*(pDst + i * ln +j ) = *(pSrc + i * ln +j ) - *(pSrc + (i +1)* ln +j ) ;
			}	
		}	
	}
	else			///<  2-1, 4-3
	{
		for ( i = col - 1; i >= 1; i-=2 )
		{
			for ( j = 0; j <  ln; j++ )
			{
				*(pDst + i * ln +j ) = *(pSrc + i * ln +j ) - *(pSrc + (i -1)* ln +j ) ;
			}	
		}			
	}
	
	return 1;
}
 
////  两点法计算
BYTE   EnmTwoPointCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  )
{
	unsigned short i;
	double  aParam[4] = { 0.0 };
	tagTwoPointsCal cal = pFormul->unFormul.stTwoPointCal;

 	if ( !pSrc || !pDst )
 		return 0;
 	
 	/// Calculate formula's parameters	
	nEnmCalParam(  pSrc, aParam, cal );	 
	
	/// process sample	
	double a = aParam[0], b = aParam[1];
	for ( i = 0; i < col * ln; i++ )
	{
		*(pDst + i) = *( pSrc + i ) * a + b	;
	}	

	return 1;
}

static void   nPickPNValue( tagPNCal cal, double *pSrc, unsigned short col, unsigned short ln, tagPNRefSample *PNRef )
{
	unsigned short i, j;
	short aPCnt, aNCnt, aLCnt, aBCnt, aQCnt;
	aPCnt = aNCnt = aLCnt = aBCnt = aQCnt = 0;

	PNRef->P = PNRef->N = PNRef->L = PNRef->B = PNRef->Q = 0;
	PNRef->flag = 0;

	/// calculate reference values
	for ( i = 0; i < col; i++ )
	{
		for ( j = 0; j < ln; j++ )
		{
			switch ( cal.sampleMatrix[j][i] )
			{
			case ENM_CUTOFF_P:
				PNRef->P += *(pSrc + i * ln +j);
				aPCnt++;
				break;

			case ENM_CUTOFF_N:
				PNRef->N += *(pSrc + i * ln +j);
				aNCnt++;
				break;
		
			case ENM_CUTOFF_L:
				PNRef->L += *(pSrc + i * ln +j);
				aLCnt++;
				break;
		
			case ENM_CUTOFF_Q:
				PNRef->Q += *(pSrc + i * ln +j);
				aQCnt++;
				break;
		
			case ENM_CUTOFF_B:
				PNRef->B += *(pSrc + i * ln +j);
				aBCnt++;
				break;

			default:
				break;
			}
		}	
	}
	
	if ( aPCnt > 0 )
	{
		PNRef->P /= aPCnt;
		PNRef->flag |= 0x01;
	}

	if ( aNCnt > 0 )
	{
		PNRef->N /= aNCnt;
		PNRef->flag |= 0x02;
	}
	if ( aLCnt > 0 )
	{
		PNRef->L /= aLCnt;
		PNRef->flag |= 0x04;
	}
	if ( aQCnt > 0 )
	{
		PNRef->Q /= aQCnt;
		PNRef->flag |= 0x08;
	}
	if ( aBCnt > 0 )
	{
		PNRef->B /= aBCnt;
		PNRef->flag |= 0x10;
	}

}

static BYTE nCalPNCutoffStandard( tagPNCal cal, double p, double n, double* cutoff )
{
	BYTE aRet = 1;

	if ( n <= cal.C )
	{
		*cutoff = cal.K * cal.C;
	}
	else
	{
		*cutoff = cal.K * n;
	}

	return aRet;
}

static BYTE nCalPNCutoffUser( tagPNCal cal, tagPNRefSample PNRef, double* cutoff )
{
	BYTE	aRet = 1;
	double	aCutOff = 0;

	//strcpy( cal.szEquation, "(P+1.25)*0.35-N" );
	MCHAR   aszTemp[16];
	MCHAR   aExp[128] = { 0 };
	INT16U  aCnt = 0, aLen = strlen( cal.szEquation );

	for ( INT16U i = 0; i < aLen; i++ )
	{
		switch ( cal.szEquation[i] )
		{
		case 'P':
			FormatFP( PNRef.P, aszTemp, 6, 3 );
			strcat( &aExp[aCnt], aszTemp );
			aCnt += strlen( aszTemp );
			break;

		case 'N':
			FormatFP( PNRef.N, aszTemp, 6, 3 );
			strcat( &aExp[aCnt], aszTemp );
			aCnt += strlen( aszTemp );
			break;

		case 'L':
			FormatFP( PNRef.L, aszTemp, 6, 3 );
			strcat( &aExp[aCnt], aszTemp );
			aCnt += strlen( aszTemp );
			break;

		default:
			aExp[aCnt++] = cal.szEquation[i];
		}
	}

	aExp[aCnt] = '\0';

	CMathCalculater aCalculater;
	if ( aCalculater.Calculate(aExp, aCutOff ) == TRUE )
	{
		*cutoff = aCutOff;
	}
	else
	{
		aCutOff = 0.0;
		aRet = 0;
	}

	return aRet;
}

BYTE   EnmPLNCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  )
{
	BYTE aRet;
	double aCutoff;
	tagPNRefSample aPNRef;	
	tagPNCal cal = pFormul->unFormul.stPNCal;

	nPickPNValue( cal, pSrc, col, ln, &aPNRef );

	if ( cal.curCutoff == ENM_CUTOFF_STANDARD )
	{
		aRet = nCalPNCutoffStandard( cal, aPNRef.P, aPNRef.N, &aCutoff );
	}
	else
	{
		aRet = nCalPNCutoffUser( cal, aPNRef, &aCutoff );
	}

	if ( aRet == 1 )
	{
		pFormul->stCoefficent.a = aPNRef.P;
		pFormul->stCoefficent.b = aPNRef.N;
		pFormul->stCoefficent.c = aCutoff;
	}
	else
	{
		pFormul->stCoefficent.a = 0;
		pFormul->stCoefficent.b = 0;
		pFormul->stCoefficent.c = 0;
	}

	return aRet;
}

BYTE   EnmPLNCalCutoff( tagPNCal cal, double *pSrc , unsigned short col, unsigned short ln, double* pCutoff, tagPNRefSample& pnl )
{
	BYTE aRet;
	double aCutoff;

	nPickPNValue( cal, pSrc, col, ln, &pnl );

	if ( cal.curCutoff == ENM_CUTOFF_STANDARD )
	{
		aRet = nCalPNCutoffStandard( cal, pnl.P, pnl.N, &aCutoff );
	}
	else
	{
		aRet = nCalPNCutoffUser( cal, pnl, &aCutoff );
	}

	if ( pCutoff )
	{
		*pCutoff = aCutoff;
	}

	return aRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    internal routines
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct 
{
	double density;
	double absorbency;
	short   cnt;
}tagInfo;
 
 short  IsExist( tagInfo *arr, double val , short cnt )	///< check if val is in arr, is not , return -1
{
	short i = -1;
	for ( i = 0; i < cnt; i++ )
	{
		if ( arr[i].density == val )
		{
			break;
		}		
	}

	return (i<cnt )? i : -1;
}

#ifdef ENM_DEBUG
 double Func( double val )
 {
	 double a = val;
 	switch ( gCompareCal.culFormula )
	{
	///  "1. Y = CX+D",
	case 0:
		a  = (val - 0.4 ) / 2;
		break;
		
	/// "2. Y = BX2+CX+D"	
	case 1:
		a  = sqrt(val ) + 1.5;
		a  = pow(val, 2 ) + 1.5;
		break;
		
	/// 	"3. Y = AX3+BX2+CX+D"
	case 2:
		a  = sqrt(val ) + 1.5;
		break;
		
	///  	"4. Y = B*EXP(AX)"
	case 3:
		a  = log(val /2.3) / 1.5;
		break;
	}

	return a;
}
#endif

double funcs(double x )
{
	double a;
	a = -0.456 * x + 1.9807;
	//a = (4.00001 * x * x + 2.56 * x + 3.45);
	// a = (0.004 * pow( x, 3 ) + 1.234 * pow( x, 2) + 3.456 *x + 5.678);
	// a = 0.004 * expf( 0.024132 * x );
	return a;
}

 short  nEnmGetSampleData(  tagSampleData *pSampleData, double *pSrc, unsigned short sampleCnt, double *pData )
 { 	
 	short i, idx;
 	short  aCnt  = 0;
 	tagInfo aXY[ENM_SAMPLE_TOTAL];
 	memset( aXY, 0, sizeof(aXY) );
	
 	for ( i = 0; i < sampleCnt; i++ )
 	{
 		idx =  IsExist( aXY, pSampleData[i].density,  aCnt ) ;
 		if ( idx == -1 )  ///< not exist
 		{
 			aXY[aCnt].density = pSampleData[i].density;
 			aXY[aCnt].absorbency = pSrc[pSampleData[i].xPos * ENM_REAGENT_LN_MAX + pSampleData[i].yPos];
 			aXY[aCnt].cnt = 1;
 			aCnt++;
 		}
 		else
 		{
 			aXY[idx].absorbency += pSrc[pSampleData[i].xPos * ENM_REAGENT_LN_MAX + pSampleData[i].yPos];
 			aXY[idx].cnt++;
 		} 		 		
 	} 	
 	
	for ( i = 0; i < aCnt; i++ )
	{
		pData[i*2]   =  aXY[i].absorbency / aXY[i].cnt;
		pData[i*2+1] =  aXY[i].density;

#ifdef  ENM_DEBUG
		pData[i*2] =  i+1;
		pData[i*2+1]  =	Func( pData[i*2] );
	
		pData[i*2] =  i+1;
		pData[i*2+1]  =	funcs( pData[i*2] );
#endif
	} 		

 	return aCnt;
}

BYTE  nEnmCalParam(  double *pSrc, double *pParam , tagCompareCal cfg )
{
	short i;
	BYTE  aRet = 1;
	
	double  aSampleData[ENM_SAMPLE_MAX * 2]= { 0.0 };	///< Density, Sample Value....
	short     aSampleCnt ;
	
	if ( !pSrc || !pParam )
		return 0;
		
	/// Get Sample data
	aSampleCnt = nEnmGetSampleData( cfg.stSampleData, pSrc, cfg.sampleCnt, aSampleData );
	if (  0 == aSampleCnt )
		return 0;
		
	/// if x set to Log, cal x data
	if ( cfg.xUnit == ENM_AXIS_LOG )
	{
		for ( i = 0; i < aSampleCnt; i++ )
		{
			aSampleData[i*2] = log (  aSampleData[i*2]  );
		}
	}
	/// if y set to Log, cal y data
	if ( cfg.yUnit == ENM_AXIS_LOG )
	{
		for ( i = 0; i < aSampleCnt; i++ )
		{
			aSampleData[i*2+1] = log (  aSampleData[i*2+1]  );
		}
	}
			
	switch ( cfg.culFormula )
	{
	///  "1. Y = CX+D",
	case 0:
		aRet  = nEnmLineParam( aSampleData,  aSampleCnt,  pParam );
		break;
		
	/// "2. Y = BX2+CX+D"	
	case 1:
		aRet = nEnmSquareParam(  aSampleData,  aSampleCnt,  pParam );	
		break;
		
	/// 	"3. Y = AX3+BX2+CX+D"
	case 2:
		aRet = nEnmCubicParam(  aSampleData,  aSampleCnt,  pParam );	
		break;
		
	///  	"4. Y = B*EXP(AX)"
	case 3:
		aRet = nEnmExpParam( aSampleData,  aSampleCnt,  pParam );		
		break;
	default:
		aRet = 0;	
	}
		
	return aRet;	
}

BYTE  nEnmLineParam( double *pSampleData,  short sampleCnt,  double *pParam )
{
	/// 
	short i;
	double x, y;
	double  aSumX , aSumY, aXY,  aX2, aSumX2;
	
	aSumX = aSumY = aXY = aX2 = aSumX2 = 0;
	
	for ( i = 0; i < sampleCnt; i++ )
	{
		x = *(pSampleData + i * 2);
		y = *(pSampleData + i * 2 + 1);
		
		aSumX +=  x;
		aSumY += y;
		
		aXY 	+= x * y;
		aX2   += x * x;
	}
	
	*(pParam+1) = (aSumY * aX2 - aSumX * aXY) / (sampleCnt * aX2 - aSumX  * aSumX);
	*(pParam) = (sampleCnt * aXY - aSumX  * aSumY) / (sampleCnt * aX2 - aSumX * aSumX);

	return 1;
}

BYTE  nEnmSquareParam(  double *pSampleData,  short sampleCnt,  double *pParam )
{
	short 	i;
	double 	x, y;
	double  aX, aX2, aX3, aX4 ,aY, aXY, aX2Y  ;
	double 	a ,b, c, n ;

	n = sampleCnt;
	aX  = aX2 =  aX3 = aX4 = aY = aXY = aX2Y = 0;
	
	for ( i = 0; i < sampleCnt; i++ )
	{
		x = *(pSampleData + i * 2);
		y = *(pSampleData + i * 2 + 1);
		
		aX +=  x;
		aX2 += pow( x, 2.0 );
		aX3 += pow( x, 3.0 );
		aX4 += pow( x, 4.0 );
		
		aY += y;
		aXY += x * y;
		aX2Y += x * x * y;
	}
		
	a = ( (n*aXY - aX*aY) * (n*aX3 - aX*aX2) - (n*aX2Y - aX2*aY) * (n*aX2 - aX*aX) ) /
		( (n*aX3 - aX2*aX) * (n*aX3 - aX2*aX) - (n*aX4 - aX2*aX2) * (n*aX2 - aX*aX) ) ;
	b = ( n * (aXY - aX3 * a) - aX * (aY - aX2 * a) ) / ( n * aX2 - aX * aX);
	c = ( aY - aX2 * a - b * aX ) / n;

	*pParam = a;
	*(pParam + 1) = b;
	*(pParam + 2) = c;

	return 1;
}

BYTE  nEnmCubicParam(  double *pSampleData,  short sampleCnt,  double *pParam )
{
	short 	i;
	double 	x, y;
	double  aX, aX2, aX3, aX4 , aX5, aX6, aY, aXY, aX2Y, aX3Y  ;
	double 	n ;

	double aa[4*4] ;
	double ad[4*4] = { 0 };
	double ab[4] = { aY, aXY, aX2Y, aX3Y };
	double ac[4] = { 0 };
	double *aap[4*4] = 
	{ 
		&n,   &aX,  &aX2, &aX3, 
		&aX,  &aX2, &aX3, &aX4, 
		&aX2, &aX3, &aX4, &aX5,
		&aX3, &aX4, &aX5, &aX6
	};
	matrix m44 = { 4,4, aa };
	matrix inv44 = { 4,4, ad };
	matrix n41 = { 4,1, ab };
	matrix result = { 4,1, ac };


	n = sampleCnt;
	aX  = aX2 =  aX3 = aX4 = aX5 = aX6 = aY = aXY = aX2Y = aX3Y = 0;
	
	for ( i = 0; i < sampleCnt; i++ )
	{
		x = *(pSampleData + i * 2);
		y = *(pSampleData + i * 2 + 1);
		
		aX +=  x;
		aX2 += pow( x, 2 );
		aX3 += pow( x, 3 );
		aX4 += pow( x, 4 );
		aX5 += pow( x, 5 );
		aX6 += pow( x, 6 );
		
		aY += y;
		aXY += x * y;
		aX2Y += pow( x, 2 ) * y;
		aX3Y += pow( x, 3 ) * y;
	}
		
	for ( i = 0; i < 16; i++ )
	{
		aa[i] = *aap[i];
	}		

	ab[0] = aY;
	ab[1] = aXY;
	ab[2] = aX2Y;
	ab[3] = aX3Y;
	
	minv( &m44, &inv44 );
	mmult( &inv44, &n41, &result );

	*pParam = ac[3];
	*(pParam + 1) = ac[2];
	*(pParam + 2) = ac[1];
	*(pParam + 3) = ac[0];

	return 1;	
}

BYTE  nEnmExpParam( double *pSampleData,  short sampleCnt,  double *pParam )
{
	short 	i;
	double 	x, y;
	double  	aX, aX2, aXlnY, alnY ;
	double 	a ,b,  n ;
	
	n = sampleCnt;
	aX = aX2 = aXlnY = alnY  = 0;
	
	for ( i = 0; i < sampleCnt; i++ )
	{
		x = *(pSampleData + i * 2);
		y = *(pSampleData + i * 2 + 1);
		
		aX +=  x;
		aX2 += pow( x, 2 );
		aXlnY += x * log( y  );
		alnY += log( y );		
	}	
	
	a  = 	( alnY *aX2 - aX*aXlnY ) / (  n*aX2 - aX * aX ) ;
	b  = 	( n *aXlnY - aX*alnY ) / (  n*aX2 - aX * aX ) ;	
	a = exp( a );

	*pParam = a;
	*(pParam + 1) = b;

	return 1;
}

 BYTE   nEnmCompareCal( U8 formula, double *pSrc, double *pDst , double *param, unsigned short col, unsigned short ln  )
{
	unsigned short i; 
	double a,b,c,d,x;
	BYTE aRet = 1;

	if ( !pSrc || !pDst )
 		return 0;
 		
	switch ( formula )
	{
	///  "1. Y = CX+D",
	case 0:
		a = *(param); b = *(param+1);
		for ( i = 0; i < col * ln; i++ )
		{
			*(pDst + i) = *( pSrc + i ) * a + b	;
		}	
		break;
		
	/// "2. Y = BX2+CX+D"	
	case 1:
		a = *(param); b = *(param+1); c = *(param+2);
		for ( i = 0; i < col * ln; i++ )
		{
			x = *( pSrc + i );
			*(pDst + i) = a * pow( x, 2)  + b *  x + c;
		}	
		break;
		
	/// 	"3. Y = AX3+BX2+CX+D"
	case 2:
		a = *(param); b = *(param+1); c = *(param+2); d = *(param+3);
		for ( i = 0; i < col * ln; i++ )
		{
			x = *( pSrc + i );
			*(pDst + i) = a * pow( x, 3)  + b * pow( x, 2)  + c * x + d;
		}	
		break;
		
	///  	"4. Y = B*EXP(AX)"
	case 3:
		a = *(param); b = *(param+1); 
		for ( i = 0; i < col * ln; i++ )
		{
			x = *( pSrc + i );
			*(pDst + i) = b * exp( a * x) ;
		}	
		break;
	default:
		aRet = 0;	
	}


	return aRet;	
}

BYTE  CalculateResult( tagDateFormulate* formul, double *pSrc, double *pDst  )
{
	unsigned short i, j;
	unsigned short col = ENM_REAGENT_COL_MAX; 
	unsigned short ln  = ENM_REAGENT_LN_MAX;
	unsigned short type = formul->calMethod;

	switch( type )  
	{
		///   因数法计算
		case 	ENM_FORMULA_FACTOR:
			return  EnmFactorCal( formul, pSrc,  pDst ,   col,   ln );
		
		///  标准浓度法计算	
		case 	ENM_FORMULA_COMPARE:
			return EnmCompareCal( formul, pSrc,  pDst ,   col,   ln );
			
		////  标准直线法计算
		case 	ENM_FORMULA_LINE:
			return EnmLinearCal( formul, pSrc,  pDst ,   col,   ln );
			
		////  列减法计算	
		case    ENM_FORMULA_SUBTRACT:
			return  EnmDifferCal( formul, pSrc,  pDst ,   col,   ln );

		////  两点法计算 EnmTwoPointCal
		case 	ENM_FORMULA_TWOPOINT:
			return  EnmTwoPointCal( formul, pSrc, pDst, col, ln );
	
		 ////  临界值法计算
		case 	ENM_FORMULA_CRITICAL_VAL:
			return EnmPLNCal( formul, pSrc, pDst, col, ln );

		////  限值法计算
		case 	ENM_FORMULA_THRESHOLD:
			/// do nothing
			/// return  EnmRangeCal(  pSrc,  pDst ,   col,   ln );			
		 
		////  双限值法计算
		case 	ENM_FORMULA_BI_THRESHOLD:
			/// do nothing
			//return  EnmBiRangeCal(  pSrc,  pDst ,   col,   ln );
		
		////  范围法计算	
		case 	ENM_FORMULA_SEGMENT:
			/// do nothing
			//return  EnmSegmentCal(  pSrc,  pDst ,   col,   ln );
						
		case 	ENM_FORMULA_NO:
		default:
			if ( pSrc &&  pDst )
			{
				for ( i = 0; i < col * ln; i++ )
				{
					pDst[i] = pSrc[i ];
				}
			}
			return 1;
	}	
}

	
