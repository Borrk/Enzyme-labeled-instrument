
#include <math.h>
#include "EnmData.h"
#include "EnmCfg.h"
#include "EnmFs.h"


/**************************************************************************
 *     Data Structure:
 *
 *        Enm default  config
 *        Sys  default  config
 *        User Setting config
 *
 */

static U16 gfp = NF_INVALID_FILE_ID;

static  tagEnmDataArr	nstTestDataList;
tagEnmData				gstEnMData;
static  BYTE       		nNewData;
static  BYTE       		nHaveScanData;


//////////////////////////////////////////////////////////////////////////
/// Inner routines
static void nSaveData()
{	
	if ( NF_INVALID_FILE_ID == gfp )
		return;

	/// write data count
	efSeek( gfp, 0, ESEEK_SET );
	efWrite( (BYTE*)&nstTestDataList.sDataCnt,  sizeof(nstTestDataList.sDataCnt), gfp );	
	
	/// write all the data
	efWrite( (BYTE*)&nstTestDataList.stDataList, sizeof(tagEnmLoggedData) * nstTestDataList.sDataCnt, gfp );	
	//efWrite( (BYTE*)&nstTestDataList.stData, sizeof(nstTestDataList.stData), gfp );

	efFlush( gfp );
	
	efSaveFS();
}

BYTE  EnmLoadData( void )
{	
	short i;
	BYTE  aRet = 1;
	
	/// Init 
	memset( &gstEnMData, 0, sizeof(gstEnMData) );
	memset( &nstTestDataList, 0, sizeof(nstTestDataList) );
	
	nNewData = 0;
	nHaveScanData = 0;

	if ( (gfp = efOpen( "EnmData.dat", EF_OPEN_WRITE )) == NF_INVALID_FILE_ID )
	{
		if ( (gfp = efOpen( "EnmData.dat", EF_OPEN_CREATE| EF_OPEN_WRITE )) == NF_INVALID_FILE_ID )
		{
			return 0;
		}
		else
		{
			aRet = 0;
		}
	}
	else
	{
		if ( efGetSize( gfp )  < sizeof(nstTestDataList.sDataCnt) )
		{	
			aRet = 0;
		}
	}

	if ( aRet == 0 )
	{
		nstTestDataList.sDataCnt = 0;
		efWrite( (BYTE*)&nstTestDataList.sDataCnt,  sizeof(nstTestDataList.sDataCnt), gfp );	
		efWrite( (BYTE*)nstTestDataList.stDataList, sizeof(nstTestDataList.stDataList), gfp );	
		//efWrite( (BYTE*)nstTestDataList.stData, sizeof(tagEnmData) * 1, gfp );	

		efSaveFS();
	}
	/// Load cfg
	else
	{
		efSeek( gfp, 0, ESEEK_SET );
		efRead( (BYTE*)&nstTestDataList.sDataCnt,  sizeof(nstTestDataList.sDataCnt), gfp );	
		
		/// read all the data
		//efRead( (BYTE*)&nstTestDataList.stData, sizeof(tagEnmData) * nstTestDataList.sDataCnt, gfp );	
		efRead( (BYTE*)nstTestDataList.stDataList, sizeof(nstTestDataList.stDataList), gfp );	
	}

	return 1;
}

#define		MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define		GetSub( val1, val2 )  MAX( ((val1) - (val2)), FP_ZERO )

double	 RangeData( double data )
{
	double aData;
	aData = (long)(data * 10000) / 10000.0;
	
	if ( aData > 10000.0 )
	{
		aData = 10000.0;
	}
	else if ( aData < 0.0001 )
		aData = 0.0001;
		
	return aData;	
}

double   nCalResult( tagEnmData *pData, short ln, short col )
{
	double aVal;
 	double aRange;
	double aRet;

	double a = pData->dbRefVal[col];
	double b = pData->dbMinVal[col];
	//pData->dbRefVal[col] = 1000;
	//pData->dbMinVal[col] = 20;

	aRange =  pData->dbRefVal[ln] - pData->dbMinVal[ln];
	aVal = pData->dbData[col][ln] - pData->dbMinVal[ln];

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

	pData->dbRefVal[col] = a;
	pData->dbMinVal[col] = b;

	return aRet;
}

BYTE	 LogResult( char reagentType, tagEnmData *pData, tagEnmData* pDstData )
{
	const double  FP_ZERO = 0.000001;
	unsigned short i, j;
	double aVal;
 	double aRange;
	
	if ( !pData || !pDstData )
		return 0;
	
	switch ( reagentType )
	{
	case ENM_REAGENT_LN:
		for ( i = 0; i < ENM_REAGENT_COL_MAX; i++ )
		{
			aRange = GetSub( pData->dbRefVal[i], pData->dbMinVal[i] );

			for ( j = 0; j < ENM_REAGENT_LN_MAX; j++ )
			{
				/// T = (Tmeas - Tmin) / (Tmax - Tmin)
				/// OD = log( 1 / T )				
				
				pDstData->dbData[i][j] = nCalResult( pData, j, i );
				//aRange = GetSub( pData->dbRefVal[j], pData->dbMinVal[j] );
				//aVal = GetSub( pData->dbData[i][j], pData->dbMinVal[j] );
				//aRange = RangeData( aRange/aVal );
				//pDstData->dbData[i][j] = log10( aRange );
			}
		}		
		break;

	case ENM_REAGENT_NO:
	case ENM_REAGENT_MULTI:
	case ENM_REAGENT_COL_AVE:
	case ENM_REAGENT_COL:
	case ENM_REAGENT_LN_AVE:
	default:
		for ( i = 0; i < ENM_REAGENT_COL_MAX; i++ )
		{
			for ( j = 0; j < ENM_REAGENT_LN_MAX; j++ )
			{
				/// T = (Tmeas - Tmin) / (Tmax - Tmin)
				/// OD = log( 1 / T )
				pDstData->dbData[i][j] = nCalResult( pData, j, i );
				
				//aRange = GetSub( pData->dbRefVal[j], pData->dbMinVal[j] );
				//aVal = GetSub( pData->dbData[i][j], pData->dbMinVal[j] );
				//aRange = RangeData( aRange / aVal );
				//pDstData->dbData[i][j] = log10( aRange );
			}
		}		
	}

	return 1;
}

BYTE 	EnmGetData( unsigned short idx , tagEnmData *pDst  )
{
	BYTE	aRet = 1;
	
	if ( !pDst  )
	{
		return 0;
	}
	
	if ( idx >= nstTestDataList.sDataCnt )
	{
		*pDst = gstEnMData;
	}
	else
	{
		*pDst = nstTestDataList.stDataList[idx].stData;
	}
	
	return 1;
}

BYTE    EnmGetDataContextCfg( unsigned short idx , tagEnMarkerCfg *pCfg, tagSysCfg* pSysCfg )
{
	BYTE aRet = 1;
	if ( !pCfg || !pSysCfg )
		return 0;

	if ( idx < nstTestDataList.sDataCnt )
	{
		*pCfg = nstTestDataList.stDataList[idx].stCfg;
		*pSysCfg = nstTestDataList.stDataList[idx].stSysCfg;
	}
	else
	{
		aRet = 0;
	}

	return aRet;
}


BYTE 	EnmIsNewData( void  )
{
	return 	nNewData;
}


BYTE    EnmAddData( void )
{
	char  aTitle[32] = { 0 };
	
	if ( !EnmIsNewData() )
		return 0;
	
	if (  nstTestDataList.sDataCnt >= ENM_DATA_MAX )
		return  0;
	
	sprintf( aTitle, "½á¹û-%02d", nstTestDataList.sDataCnt + 1 );		
	nstTestDataList.stDataList[nstTestDataList.sDataCnt].stData  = gstEnMData;
	nstTestDataList.stDataList[nstTestDataList.sDataCnt].stCfg   = gstCurEnMCfg;
	nstTestDataList.stDataList[nstTestDataList.sDataCnt].stSysCfg   = gstSysCfg;
	nstTestDataList.stDataList[nstTestDataList.sDataCnt].stData.stDate  = EnmGetDate();
	nstTestDataList.stDataList[nstTestDataList.sDataCnt].stData.stTime  = EnmGetTime();	
	strcpy( nstTestDataList.stDataList[nstTestDataList.sDataCnt].stData.szTitle, aTitle  );
	nstTestDataList.sDataCnt++;	
	
	/// Save to Flash
	nSaveData();

	// data has been save, so not new data any more	
	nNewData = 0;

	return 1;
}

void 	 EnmDeleteData( unsigned short  cfgId )
{
	if (  cfgId >= nstTestDataList.sDataCnt  )
		return;
	
	if ( cfgId < nstTestDataList.sDataCnt - 1 )
	{
		memcpy( &nstTestDataList.stDataList[cfgId], &nstTestDataList.stDataList[cfgId+1], 
		                   sizeof(tagEnmLoggedData) * (nstTestDataList.sDataCnt - cfgId - 1)  );
	}
	
	memset( &nstTestDataList.stDataList[nstTestDataList.sDataCnt - 1], 0, sizeof(tagEnmLoggedData) );
	nstTestDataList.sDataCnt--;		

	/// Save to Flush
	nSaveData();
}

 void 	EnmSetNewDataFlag( void  )
{
	nNewData = 1;
}

BYTE 	EnmHaveScanData( void  )
{
	return 	nHaveScanData;
}

 void 	EnmSetHaveDataFlag( void  )
{
	nHaveScanData = 1;
}

unsigned short  EnmGetDataCount( void )
{
	return nstTestDataList.sDataCnt;
}

void	 EnmSetCurDataID( unsigned short  id  )
{
	
}

unsigned short  EnmGetCurDataID( void )
{
	return -1;
}


BYTE	EnmPickDataLn( tagEnmData src, double *pDst, BYTE ln  )
{
	short i, idx;
	if (  !pDst || ln > ENM_DATA_COUNT -1 )
	{
		return 0;
	}
	
	idx = 0;
	for ( i = 0; i < ENM_DATA_GROUP; i++ )
	{
		pDst[idx++] = src.dbData[i][ln];
	}
	
	return 1;
}
