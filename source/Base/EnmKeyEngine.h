// EnmKeyEngine.h: interface for the CEnmKeyEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMKEYENGINE_H__46446F6D_B873_41E7_8721_329FA700A830__INCLUDED_)
#define AFX_ENMKEYENGINE_H__46446F6D_B873_41E7_8721_329FA700A830__INCLUDED_


#include "EnmProcEngine.h"

class CEnmKeyEngine : public IProcEngine  
{
public:
	CEnmKeyEngine();
	virtual ~CEnmKeyEngine();

public:
	virtual enProcState IEnterProc( );

protected:
	void	DispathMsg( );
};

#endif // !defined(AFX_ENMKEYENGINE_H__46446F6D_B873_41E7_8721_329FA700A830__INCLUDED_)
