// Date declare file

#ifndef __ENM_DATA_PROCESS_H__
#define __ENM_DATA_PROCESS_H__

// include section
#include "EnmData.h"


#ifdef __cplusplus
//extern "C" {
#endif

typedef BYTE   (*ResultProcessor)( double*, double*, unsigned short, unsigned short );

///   ����������
BYTE    EnmFactorCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln );

///  ��׼Ũ�ȷ�����
BYTE   EnmCompareCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );
 
////  ��׼ֱ�߷�����
BYTE   EnmLinearCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );

 ////  ��ֵ������
BYTE   EnmRangeCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );

 ////  ˫��ֵ������
BYTE   EnmBiRangeCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );

 ////  ��Χ������
BYTE   EnmSegmentCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );

 ////  �м�������
 BYTE   EnmDifferCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );
 
////  ���㷨����
BYTE   EnmTwoPointCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );

 ////  �ٽ�ֵ������
BYTE   EnmPLNCal( tagDateFormulate *pFormul, double *pSrc, double *pDst , unsigned short col, unsigned short ln  );
BYTE   EnmPLNCalCutoff( tagPNCal cal, double *pSrc, unsigned short col, unsigned short ln, double* pCutoff, tagPNRefSample& pnl );
 
BYTE  CalculateResult(  tagDateFormulate *formul, double *pSrc, double *pDst  );
 
#ifdef __cplusplus
//}
#endif

#endif  // __ENM_DATA_PROCESS_H__
