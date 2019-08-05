// Type defination file

#ifndef __ENM_DEF_H__
#define __ENM_DEF_H__

#include <stdlib.h>
#include <string.h>

// embed type
typedef   unsigned char  BYTE;
typedef   unsigned short WORD;
//typedef   unsigned long	DWORD;

typedef   short  ENMADC;

#ifndef NULL
#define	NULL	(0)
#endif	 

#pragma  pack( 1 )

typedef struct 
{
	short	year;
	char	month;
	char	day;
}tagDate;

typedef struct
{
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
}tagTime;


#if 1
//////// 测量方式 //////////////
typedef struct 
{
	char  wave;	//	短波长测试	
}tagWave;

typedef struct 
{
	char wave1;	//	双波长测试	第一波长
	char wave2;	//		第二波长
}tagBiWave;

typedef struct 
{
	char 		wave;
	tagTime 	stTime;
}tagBiTime;

typedef struct 
{
	char		wave;
	tagTime intervalTime;	//	酶标动力学法	间歇时间
	tagTime delayTime;	//		延迟时间
	tagTime totalTime;	//		总时间
}tagEnMark;

enum enENM_METHOD
{
	ENM_METHOD_WAVE  , 
	ENM_METHOD_BIWAVE,
	ENM_METHOD_BITIME,
	ENM_METHOD_ENMARK
};

typedef struct 
{
	char curMethod;
	tagWave		stWave;
	tagBiWave	stBiWave;
	tagBiTime 	stBiTime;
	tagEnMark	stEnMark;
}tagCheckMethod;


//////// 空白方式 //////////////
enum  enENM_REAGENT
{
	ENM_REAGENT_NO,
	ENM_REAGENT_MULTI,
	ENM_REAGENT_COL_AVE,
	ENM_REAGENT_COL,
	ENM_REAGENT_LN_AVE,
	ENM_REAGENT_LN
};

#define	ENM_REAGENT_LN_MAX			8
#define	ENM_REAGENT_COL_MAX			12
typedef struct
{
	char   curReagent;
#if 1
	long   reagent[6][ENM_REAGENT_LN_MAX];
#else	
	long   noReagent[ENM_REAGENT_LN_MAX];
	long   multiReagent[ENM_REAGENT_LN_MAX];
	long   colAve[ENM_REAGENT_LN_MAX];
	long   col[ENM_REAGENT_LN_MAX];
	long   cowAve[ENM_REAGENT_LN_MAX];
	long   cow[ENM_REAGENT_LN_MAX];
#endif	
}tagReagent;


//////// 计算方式 //////////////

typedef struct 
{
	double  factor;	//	因数计算	因数
	char  unit;	//		单位
}tagFactorCal;


//	标准浓度法计算	
#define		ENM_SAMPLE_MAX				8
#define		ENM_SAMPLE_MAX_PER_SAMPLE	3
#define		ENM_SAMPLE_TOTAL	(ENM_SAMPLE_MAX_PER_SAMPLE * ENM_SAMPLE_MAX)

#define		ENM_AXIS_LIN				0
#define		ENM_AXIS_LOG			1

#define		ENM_CAL_ALL				0
#define		ENM_CAL_ONE			1

typedef struct 
{
	short		xPos, yPos;
	double         	density;
}tagSampleData;

typedef struct 
{
	unsigned char culFormula;	///< [0, 3]
	unsigned char xUnit, yUnit;	///< LIN,LOG
	unsigned char unit;			///< [0, 12]
	unsigned char curStandard;	///< one standard, one per plate
	unsigned short count;
	unsigned short sampleCnt;	
	tagSampleData  stSampleData[ENM_SAMPLE_TOTAL];
	
}tagCompareCal;

typedef struct 
{
	double  ratio;	//	标准直线法计算	斜率（A）
	double dist;	//		截距（B）
}tagLineCal;

typedef struct 
{
	double  threshold;	//	限值法计算	限值
}tagThresholdCal;

typedef struct 
{
	double thresholdLo;	//	双限值法计算	低限值
	double thresholdHi;	//	高限值
}tagBiThresholdCal;

typedef struct 
{
	double minVal;	
	double maxVal;		
}	
tagRangeCal;

typedef struct 
{
	char  subRule;	//	列减法计算	
}tagSubtractCal;

#if 1
typedef 	tagCompareCal   tagTwoPointsCal;
#else
typedef struct 
{
	unsigned char culFormula;	///< Fixed equal to 0 always
	unsigned char   xUnit, yUnit;	///< LIN,LOG
	unsigned char   unit;			///< [0, 12]
	unsigned char   curStandard;	///< one standard, one per plate
	unsigned short  count;
	unsigned short  sampleCnt;	
	tagSampleData  stSampleData[ENM_SAMPLE_TOTAL];

}tagTwoPointsCal;
#endif

//	临界值法计算
enum  enCutoffRefer
{
	ENM_CUTOFF_P,
	ENM_CUTOFF_N,
	ENM_CUTOFF_B,	///< blank
	ENM_CUTOFF_L,
	ENM_CUTOFF_Q,
	ENM_CUTOFF_O,	///< sample
};
	
enum  enCutoff
{
	ENM_CUTOFF_STANDARD,
	ENM_CUTOFF_USER
};

enum enCutOffOutMode
{
	ENM_CUTOFF_PRINT_ASEND,
	ENM_CUTOFF_PRINT_DEASEND,
	ENM_CUTOFF_PRINT_CUTOFF
};
	
typedef struct 
{
	char   curCutoff; 
	char   printType;
	double K, C;
	char   szEquation[128];
	double grayAreaP, grayAreaN;
	char   sampleMatrix[ENM_REAGENT_LN_MAX][ENM_REAGENT_COL_MAX];
}tagPNCal;


enum  enENM_CALFORMULA
{
	ENM_FORMULA_NO,
	ENM_FORMULA_FACTOR,
	ENM_FORMULA_COMPARE,
	ENM_FORMULA_LINE,
	ENM_FORMULA_THRESHOLD,
	ENM_FORMULA_BI_THRESHOLD,
	ENM_FORMULA_SEGMENT,
	ENM_FORMULA_SUBTRACT,
	ENM_FORMULA_TWOPOINT,
	ENM_FORMULA_CRITICAL_VAL		
};

typedef struct 
{
	char 				calMethod;	//	计算公式	
	tagFactorCal		stFactorCal;
	tagCompareCal		stCompareCal; 
	tagLineCal			stLineCal;
	tagThresholdCal		stThresholdCal;
	tagBiThresholdCal	stBiThresholdCal;
	tagRangeCal			stRangeCal;
	tagSubtractCal		stSubtractCal;
	tagTwoPointsCal		stTwoPointCal;
	tagPNCal		stPNCal;
}tagCalFormula;

// 振板方式
enum enENM_LIBRATE
{
	ENM_LIBRATE_SPEED_FAST,
	ENM_LIBRATE_SPEED_MIDDLE,
	ENM_LIBRATE_SPEED_SLOW
	
};

typedef struct 
{
	char		bOn;				//振板方式
	char		LibrateSpeed;     //振板速度
	tagTime		LibrateTime;      //振板时间
}tagLibrate;

// 进板方式
enum enENM_FORWARD
{
	ENM_FORWARD_ON,
	ENM_FORWARD_STEP
};

typedef struct  
{
	char  stepMode;
}tagPlateStep;

typedef struct 
{
	tagCheckMethod		stCheckMethod;	// 测量方式
	tagReagent			stReagent;		// 空白方式
	tagCalFormula		stCalFormula;	// 计算方式
	tagLibrate			stLibrate;		// 振板方式
	tagPlateStep		stPlateStep;	// 进板方式
	
}tagEnMarkerCfg;

#define		ENMARKER_CFG_SUM	(16)
typedef struct 
{
	tagEnMarkerCfg	cfg;
	unsigned short		usID;
	char				szName[16];	
	tagDate			date;
	
}tagEnMCfgItem;

typedef struct 
{
	unsigned short		usCnt;	///< current saved cfg count
	unsigned short		usCurCfgID;	///< current selected cfg's id
	tagEnMCfgItem		stEnMCfg[ENMARKER_CFG_SUM];
	
}tagEnMCfgData;

// System settings item
#define		ENM_WAVE_MAX	8
typedef struct 
{
	char szWaveLength[8][6];
}tagWaveLength;

#define		ENM_DATAOUT_TYPE_FINAL	0
#define		ENM_DATAOUT_TYPE_ALL	1
typedef struct 
{
	unsigned short dataType;
}tagDataOutType;

/// printer type
#define		ENM_PRINTER_INTERNAL   		0
#define		ENM_PRINTER_EXT				1

#define     EMN_PRINTER_MODE_MR			0
//#define     EMN_PRINTER_MODE_			1

typedef struct 
{
	unsigned short type;
	unsigned char  bAutoPrint;
	unsigned char  mode;
}tagPrinterType;

//	板分析方式			
enum enENM_ANYLIZE
{
	ENM_ANYLIZE_ON,
	ENM_ANYLIZE_OFF
};
typedef struct  
{
	char  anylizeOn;
}tagPlateAnylize;


typedef struct
{
	 double fPreDist;	///<  distance before the laser 
	 double fPostDist;	///<  distance after the laser, Pre + Post = Total 
	 double fStepDist;  ///<  motor mm/step
	 double fHoleDist;	///<  distance between 2 hole.
     double fHolePre;   ///<  distance from the head to the 1st hole.
     short  iHoleCount; ///<  Hole count
     short  iType;      ///<  Motor type, Circle or plate.
}tagMotorInfo;
#define MOTOR_PLATE   0
#define MOTOR_CIRCLE  1

typedef struct
{
	tagMotorInfo	stPlateInfo;
	tagMotorInfo	stLightInfo;
	double  fPlateMotorDist;
	double  fLightMotorDist;
}tagMotorOpt;

// 系统设置项
typedef struct  
{
	tagWaveLength	stWaveLen;
	tagDataOutType	stDataType;
	tagPlateAnylize stPlateAnylize; // 板分析方式
	tagPrinterType	stPrinter;
	tagMotorOpt 	stMotorOpt;
	tagDate		stDate;
	tagTime		stTime;
}tagSysCfg;


 ////  临界值法计算
typedef struct 
{
	double  P, N, L, Q, B;
	long    flag;  ///< bit0~4: P, N, L, B, Q calculated, set to 1, else to 0
}tagPNRefSample;


#endif

//////////////////////////////////////////////////////////////////////////
///  E20 project 
//////////////////////////////////////////////////////////////////////////

typedef unsigned char  U8;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  S8;                    /* Signed    8 bit quantity                           */
typedef unsigned short U16;                   /* Unsigned 16 bit quantity                           */
typedef signed   short S16;                   /* Signed   16 bit quantity                           */
typedef unsigned int  U32;                   /* Unsigned 32 bit quantity                           */
typedef signed   long  S32;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point  */
  
typedef	char			MCHAR;

typedef unsigned char  BOOLEAN;


#define	PROTOCOL_NAME  16

enum	enMeasureMode
{
	MEASURE_QUALITITIVE,
	MEASURE_QUALITATIVE,
	MEASURE_COMMON
};

enum	enMixingMode
{
	MIX_NONE,
	MIX_SLOW,
	MIX_MIDDLE,
	EMIX_FAST
};

enum	enTemprature
{
	TEMPRATURE_OFF,
	TEMPRATURE_ON
};

#define  PROT_ID_INVALID	((U32)-1)
typedef struct  
{
	U32						uID;
	char					szName[PROTOCOL_NAME];
	U8						uMainFilter;
	U8						uReferFilter;
	U8						uSlaveFilter;
	U8						uMeasMode;
	U8						uMixingMode;
	U16						uMixingTime;
	U8						uTempratureCtrl;
	FP32					fTemprature;
	FP32					kFactor;
	U32						uColumnMask;
	U32						uPeriod;
}tagMeasure;

/// 定量
#define		STD_MAX			8
#define		BLANK_MAX		8
#define		SAMPLE_MAX		96

enum  enEnmCurve
{
	CURVE_LINE,
	CURVE_SEGMENT
};

enum enEnmAxis
{
	AXIS_LN,
	AXIS_LOG
};

typedef struct 
{
	U8						uUnit;
	U8						uSample;
	U8						uBlank;
	U8						uStd;
	U8						uStdRepeats;
	//FP64					fStdConc[STD_MAX];
	tagSampleData stStdDensity[STD_MAX];
	U8						uCurve;
	U8						uAxisX;
	U8						uAxisY;
}tagQualititive;


/// 定性
#define		CUTOFF_MAX		64
typedef struct 
{
	U8						uSample;
	U8						uBlank;
	U8						uNC;
	U8						uPC;
	U8						uLP;
	U8						uHP;
	U8						uCO;
	U8						uPrintFormat;
	FP64					fGrayN;
	FP64					fGrayP;
	char					szCutoff[CUTOFF_MAX];
}tagQualitative;


struct  tagProtocol
{
	tagMeasure			stMeasure;
	tagQualititive		stQuality;
	tagQualitative		stQualitative;

};


//////////////////////////////////////////////////////////////////////////
///  System Config
//////////////////////////////////////////////////////////////////////////

#define		FILTER_MAX				8
#define		FILTER_NAME_MAX			8
#define		DEVICE_NAME_MAX			32

typedef  struct 
{
	U32		uBandRate;
	U8		uDataBit;
	U8		uStopBit;
	U8		uCheck;
}tagSerialPortOption;


//typedef struct
//{
//	 double fPreDist;		///<  distance before the laser 
//	 double fPostDist;	///<  distance after the sample plate
//	 double fStepDist;  ///<  motor mm/step
//	 double fHoleDist;	///<  distance between 2 hole.
//}tagMotorInfo;
//
//typedef struct
//{
//	tagMotorInfo	stPlateInfo;
//	tagMotorInfo	stLightInfo;
//	double  fPlateMotorDist;
//	double  fLightMotorDist;
//}tagMotorOpt;

typedef  struct 
{
	char					szDeviceName[DEVICE_NAME_MAX];
	char					szFilter[FILTER_MAX][FILTER_NAME_MAX];
	tagDate					stDate;
	tagTime					stTime;
	U16						uLanguage;
	U8						bPlateCheck;
	tagSerialPortOption		stSerialCfg;
	tagPrinterType			stPrinter;
}tagSystemCfg;

typedef  struct 
{
	FP64					fFilterRatio[FILTER_MAX];
	tagMotorOpt				stMotorOpt;
}tagDeviceOption;


#pragma pack()



#endif 	// __ENM_DEF_H__