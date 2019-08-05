#ifndef __ENM_FORMATER_H__
#define __ENM_FORMATER_H__

#include "EnmData.h"
#include "MIG_Base.hpp"	// Added by ClassView

/**
 *		Result formater
 *
 */ 	
class CEnmFormater
{
public:
	CEnmFormater();

	/// Format one line data, 
	/// ln  ---line's index
	/// pData --data to be formatted
	/// dataCnt---data count
	/// pBuff ---buf to keep the formatted string, seperated by '\0'
	/// return: string line's count
	virtual INT16U FormatDataIndex( INT16U dataCnt, MCHAR* pszDst );	
	virtual INT16U Format( INT16U ln, FP64 *pData, INT16U dataCnt, MCHAR* pszDst );	
	virtual void SetOption( const tagDataOption& rOption ) { mOption = rOption; mbOptionSet = TRUE; }	
	void ClearOption() { mbOptionSet = FALSE; }
	void SetUnitLength( INT16U len, INT16U interval ) { mUnitLen = len; mInterval = interval; }
	
protected:
	void Init();
	//static void PadFormat( MCHAR *pSrc, MCHAR *pDst, INT16U width, INT8U aligned = 0 );  ///< 0: center, 1: left, 2: right

/// memeber variabls
protected:
	INT16U		mUnitLen;
	INT16U 		mInterval;
	INT16U      mLeftAlign;

	tagDataOption	mOption;
	
	BOOLEAN		mbOptionSet;
};


/**
 *	CEnmFormaterSegment 
 *
 */
class CEnmFormaterSegment : public CEnmFormater
{
public:
	virtual INT16U Format( INT16U ln, FP64 *pData, INT16U dataCnt, MCHAR* pszDst );

/// memeber variabls
};

/**
 *	CEnmFormaterRange 
 *
 */
class CEnmFormaterRange : public CEnmFormater
{
public:
	CEnmFormaterRange():mSortType(0){}
	
	virtual INT16U Format( INT16U ln, FP64 *pData, INT16U dataCnt, MCHAR* pszDst );
	void	SetSortType( INT8U type = 0 ) { mSortType = type; }
	void	SetThreshold( FP64 fHigh, FP64 fLow ) { mHigh = fHigh; mLow = fLow; }
	
/// memeber variabls

protected:
	FP64	mLow, mHigh;	///< low and high threshold value
	INT8U  	mSortType;	///< 0: ascending, 1: deascending
};

/**
 *	CEnmFormaterSegment 
 *
 */
class CEnmFormaterCutoff : public CEnmFormaterRange
{
public:
	virtual INT16U Format( INT16U ln, FP64 *pData, INT16U dataCnt, MCHAR* pszDst );
	BOOLEAN PLNCalCutoff( tagPNCal& cal, tagEnmData& pData );
	void	SetCoefficent( const tagPNRefSample& pnl, FP64 cutoff );

private:
	void		Init();
	INT16U		FormatCutoff( INT16U ln, FP64 *pData, INT16U dataCnt, MCHAR* pszDst, FP64 zoneP, FP64 cutoff, FP64 zoneN );
	
/// memeber variabls
	tagPNRefSample	mPNL;
	FP64			mCutoff;
	BOOLEAN			mbValidCutoff;
};
	
#endif	// __ENM_FORMATER_H__
