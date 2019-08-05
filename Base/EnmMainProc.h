// EnmMainProc.h: interface for the CEnmMainProc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMMAINPROC_H__FA950049_8E09_4100_9275_3D4DCA3FE3DB__INCLUDED_)
#define AFX_ENMMAINPROC_H__FA950049_8E09_4100_9275_3D4DCA3FE3DB__INCLUDED_


#include "EnmProcEngine.h"

class CEnmWfoSelector;

class CEnmMainProc : public IProcEngine  
{
	typedef IProcEngine inherited;
	
public:
	CEnmMainProc();
	virtual ~CEnmMainProc();

public:
	enProcState IEnterProc() ;
	BOOLEAN     InitEngine();
	enProcState IResume();
  
protected:
	void  OnMessage( tagMigMSG& msg );

protected:
	BOOLEAN		OnSysCfgChange( tagMigMSG& msg );
	
protected:
	CEnmWfoSelector*	mpWfRoot;
};

#endif // !defined(AFX_ENMMAINPROC_H__FA950049_8E09_4100_9275_3D4DCA3FE3DB__INCLUDED_)
