
#ifndef __EnmSysCfgLib_H
#define __EnmSysCfgLib_H

#include "EnmBase.hpp"
#include "EnmDef.h"

class CEnmSysCfgLib  
{
public:
	enum  enSysCfgItem
	{
		CFG_FILTER			= 0x00000001, 
		CFG_DEV_NAME		= 0x00000002,
		CFG_COM_BAUD		= 0x00000004,
		CFG_COM_PROTOCOL	= 0x00000008,
		CFG_PRINTER			= 0x00000010,
		CFG_PLATE_CHECK		= 0x00000040,
		CFG_DEV_LANGUAGE	= 0x00000080,
	};

public:
	static CEnmSysCfgLib& Instance();
	virtual ~CEnmSysCfgLib();

public:
	BOOLEAN		Load();
	void		Unload();
	
	void		SyncSysCfg();
	void		SyncDevOpt();

	tagSystemCfg&		GetSysCfg();
	tagDeviceOption&	GetDevOpt();

public:	
	BOOLEAN		IsSysCfgChanged() const;
	BOOLEAN		IsDevOptChanged() const;
	void		SysCfgChanged();
	void		DevOptChanged();

	void		SetDefaultSysCfg( tagSystemCfg& cfg );
	void		SetDefaultDevOpt( tagDeviceOption& opt );

protected:
	CEnmSysCfgLib();
	void		Init();
	BOOLEAN		LoadSysCfg();
	BOOLEAN		LoadDevOpt();
	void		UnloadSysCfg();
	void		UnloadDevOpt();

protected:
	S32				mfSysCfg;
	S32				mfDevOpt;
	BOOLEAN			mbSysCfgChanged;
	BOOLEAN			mbDevOptChanged;

	tagSystemCfg	mSysCfg;
	tagDeviceOption	mDevOpt;

};


inline tagSystemCfg& CEnmSysCfgLib::GetSysCfg()
{
	return mSysCfg;
}

inline tagDeviceOption& CEnmSysCfgLib::GetDevOpt()
{
	return mDevOpt;
}

inline BOOLEAN CEnmSysCfgLib::IsSysCfgChanged() const
{
	return mbSysCfgChanged;
}

inline BOOLEAN CEnmSysCfgLib::IsDevOptChanged() const
{
	return mbDevOptChanged;
}

inline
void CEnmSysCfgLib::SysCfgChanged()
{
	mbSysCfgChanged = TRUE;
}

inline
void CEnmSysCfgLib::DevOptChanged()
{
	mbDevOptChanged = TRUE;
}


#endif ///< __EnmSysCfgLib_H
