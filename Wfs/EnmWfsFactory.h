// EnmWfsFactory.h: interface for the CEnmWfsFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMWFSFACTORY_H__7D307864_CA4F_4FF8_A6AF_021C121B1B06__INCLUDED_)
#define AFX_ENMWFSFACTORY_H__7D307864_CA4F_4FF8_A6AF_021C121B1B06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EnmWfo.h"
#include "EnmWfoSelecter.h"

class CEnmWfsFactory  
{
public:
	CEnmWfoSelector* CreateWfs();

private:
	BOOLEAN		CreateProgramModeTree( CEnmWfoSelector& root );
	BOOLEAN		CreateFastModeTree( CEnmWfoSelector& root );
	BOOLEAN		CreateProgrammingTree( CEnmWfoSelector& root );
	BOOLEAN		CreateProgramListTree( CEnmWfoSelector& root );
	BOOLEAN		CreateDataListTree( CEnmWfoSelector& root );
	BOOLEAN		CreateSysConfigTree( CEnmWfoSelector& root );
	BOOLEAN		CreateHostConsoleTree( CEnmWfoSelector& root );

};

#endif // !defined(AFX_ENMWFSFACTORY_H__7D307864_CA4F_4FF8_A6AF_021C121B1B06__INCLUDED_)
