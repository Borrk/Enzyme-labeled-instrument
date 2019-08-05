// Date declare file

#ifndef __ENM_DATA_PROCESS_H__
#define __ENM_DATA_PROCESS_H__

// include section
#include "EnmData.h"


#ifdef __cplusplus
//extern "C" {
#endif

typedef BYTE   (*ResultProcessor)( double*, double*, unsigned short, unsigned short );

///   因数法计算
BYTE    EnmFactorCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln );

///  标准浓度法计算
BYTE   EnmCompareCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );
 
////  标准直线法计算
BYTE   EnmLinearCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );

 ////  限值法计算
BYTE   EnmRangeCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );

 ////  双限值法计算
BYTE   EnmBiRangeCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );

 ////  范围法计算
BYTE   EnmSegmentCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );

 ////  列减法计算
 BYTE   EnmDifferCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );
 
////  两点法计算
BYTE   EnmTwoPointCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );

 ////  临界值法计算
BYTE   EnmPLNCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );
BYTE   EnmPLNCalCutoff( tagPNCal cal, double *pSrc, unsigned short col, unsigned short ln, double* pCutoff, tagPNRefSample& pnl );
 
BYTE  CalculateResult(  tagDateFormulate *formul, double *pSrc, double *pDst  );
 
#ifdef __cplusplus
//}
#endif

#endif  // __ENM_DATA_PROCESS_H__
