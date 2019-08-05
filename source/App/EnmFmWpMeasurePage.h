// EnmFmWpMeasurePage.h: interface for the CEnmFmWpMeasurePage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMFMWPMEASUREPAGE_H__767BAA2E_2E91_4E36_A092_51B26414685C__INCLUDED_)
#define AFX_ENMFMWPMEASUREPAGE_H__767BAA2E_2E91_4E36_A092_51B26414685C__INCLUDED_

#include "EnmBasePage.hpp"

class CEnmFmWpMeasurePage : public CEnmBasePage  
{
	typedef  CEnmBasePage  inherited;
	
public:
	CEnmFmWpMeasurePage();

	BOOLEAN		IsCanForward();
	
protected:
	void   Draw();
	void   DrawHints();
	void   CreateControl();
	
protected:
	void		FocusOn();
	void		FocusOff();
	void		OnUserMsg( const tagMigMSG& stMsg );
	
protected:
	BOOLEAN		mbInScan;
};


class CEnmFmWpReportPage : public CEnmBasePage  
{
	typedef CEnmBasePage  inherited;

public:
	CEnmFmWpReportPage();

protected:
	void		Draw();
	void		DrawHints();

protected:
	void		FocusOn();

protected:
	void		FormatDate( tagDate& date, tagTime& time, MCHAR *pBuff, S32 langue );
	void		Reset();
	BOOLEAN		Evaluate();
	
protected:
	U16	 mCount;
	FP32 mMean; 
	FP32 mCV;   
	FP32 mSD;  
	
};

#endif // !defined(AFX_ENMFMWPMEASUREPAGE_H__767BAA2E_2E91_4E36_A092_51B26414685C__INCLUDED_)
