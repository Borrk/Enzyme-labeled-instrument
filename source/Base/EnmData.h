// Date declare file

#ifndef __ENM_DATA_H__
#define __ENM_DATA_H__

// include section
#include "EnmDef.h"
#include "EnmApi.h"


#define	ENM_DATA_GROUP 	ENM_REAGENT_COL_MAX  
#define	ENM_DATA_COUNT		ENM_REAGENT_LN_MAX
#define	ENM_DATA_MAX	24

#define GETPLATEMAX() 				(ENM_DATA_GROUP * ENM_DATA_COUNT)
#define GETPLATECOLUMN( cnt ) ((cnt)/ENM_DATA_COUNT)
#define GETPLATEROW( cnt ) 		((cnt)%ENM_DATA_COUNT)


/// strcut defination section
#pragma  pack( 1 )


typedef struct
{
	double a, b, c, d, e;	///< curve coefficient
}tagCurveApproach;

typedef struct
{
	double reagent[ENM_REAGENT_COL_MAX];
}tagReagentValue;

typedef struct
{
	char 				calMethod;	//	计算公式	
	union eFormul
	{
		tagFactorCal		stFactorCal;
		tagCompareCal		stCompareCal; 
		tagLineCal			stLineCal;
		tagThresholdCal		stThresholdCal;
		tagBiThresholdCal	stBiThresholdCal;
		tagRangeCal			stRangeCal;
		tagSubtractCal		stSubtractCal;
		tagTwoPointsCal		stTwoPointCal;
		tagPNCal			stPNCal;	
	}unFormul;

	tagCurveApproach	stCoefficent;
}tagDateFormulate;
	
typedef struct 
{
	tagWaveLength		stWaveLen;
	tagCheckMethod		stCheckMethod;	// 测量方式
	char   			curReagent;
	long   			reagent[ENM_REAGENT_LN_MAX];
	tagDateFormulate	stCalFormula;	
	tagReagentValue		stReagentOD;
	tagDataOutType   	outputType;
	tagLibrate			stLibrate;		// 振板方式
	tagPlateStep		stPlateStep;	// 进板方式
}tagDataOption;

typedef struct 
{
	char		szTitle[17];
	tagDate		stDate;
	tagTime		stTime;
	double      dbRefVal[ENM_DATA_GROUP];
	double      dbMinVal[ENM_DATA_GROUP];
	double 		dbData[ENM_DATA_GROUP][ENM_DATA_COUNT];
}tagEnmData;

/// data manage info 
#define  ENM_DATA_TITLE_LEN 		20
typedef struct
{
	unsigned short	 iID;
	char	 szTitle[ENM_DATA_TITLE_LEN+4];
	tagDate	 stDate;  ///< create date
	tagTime	 stTime;  ///< create time	
}tagDataInfo;

typedef struct
{
	unsigned short	iID;
	char 	szFileName[128];
	unsigned short	fHandle;
}tagDataHandleInfo;


#if 1
typedef struct 
{
	tagDataOption	stOption;
	tagEnMarkerCfg	stCfg;	///< add temp for E20, 
	tagSysCfg		stSysCfg;	///< add temp for E20, 
	tagEnmData		stData;
}tagEnmLoggedData;

#else
typedef struct 
{
	tagEnmData		stData;
	tagEnMarkerCfg	stCfg;
	tagSysCfg		stSysCfg;
}tagEnmLoggedData;

#endif


typedef struct 
{
	short				sDataCnt;
	tagEnmLoggedData	stDataList[ENM_DATA_MAX];
}tagEnmDataArr;



//////////////////////////////////////////////////////////////////////////
/// E20 Add
//////////////////////////////////////////////////////////////////////////
typedef struct 
{
	char		szTitle[17];
	tagDate		stDate;
	tagTime		stTime;
	double      dbRefVal[ENM_DATA_GROUP];
	double      dbMinVal[ENM_DATA_GROUP];
	double 		dbData[ENM_DATA_GROUP][ENM_DATA_COUNT];
	U32			uColumnMask; 
}tagScanData;



#pragma pack()




#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif  // __ENM_DATA_H__
