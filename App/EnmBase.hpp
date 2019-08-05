#ifndef __ENK_BASE_H__
#define __ENK_BASE_H__

#include "Mig_OS_Base.hpp"
#include "Mig_Base.hpp"
#include "Mig_EditBox.hpp"
#include "Mig_Page.hpp"

#include "EnmApi.h"


#define  ENM_CHINESE     0
#define  ENM_T( tag, title1, title2)  (MCHAR*)((tag) ? title1 : title2)

enum  enEnmSysMode
{
	ENM_MODE_ALONE, ENM_MODE_PC
};


/// Page Id defination
enum	enmPageID
{
	ENM_PAGE_ID_MAIN		= 0x10000000,

	ENM_PAGE_ID_MODE		= 0x01000000,
	ENM_PAGE_ID_FASTMODE,
	ENM_PAGE_ID_PROGRAMMING,
	ENM_PAGE_ID_MODELIST,
	ENM_PAGE_ID_DMM,
	ENM_PAGE_ID_SYSCFG,

	/// Programming sub page (ENM_PAGE_ID_CHECKMODE)
	ENM_PAGE_ID_PLATE	= 0x00100000,
	ENM_PAGE_ID_SCANDATA,
	ENM_PAGE_ID_CUTOFFPROC,
	ENM_PAGE_ID_SHAKE,
	ENM_PAGE_ID_ANYLIZEMODE,

	ENM_PAGE_ID_CONTROLCONC,

	ENM_PAGE_ID_CUTOFF_DELETE,
	ENM_PAGE_ID_CUTOFF_SAVE,

	/// Fast Mode PID
	//  Whole plate 
	PID_FM_WP_SETUP,
	PID_FM_WP_MEASURE,
	PID_FM_WP_STATATISC,
	PID_FM_WP_OD,

	// Column monitor
	PID_FM_CM_MODE,
	PID_FM_CM_MEASURE,

	// Kinetic 
	PID_FM_KN_MODE,
	PID_FM_KN_MEASURE, 
	
	/**
	 *  Programming function
	 */
	PID_PG_LIST,
	PID_PG_EDIT_MODE,
	PID_PG_DELETE,
	
	PID_PG_EDIT_QUALITY,
	PID_PG_EDIT_QUALITITIVE,
	PID_PG_MIXING_MODE,
	PID_PG_CURVE_SELECT,
	
	PIG_PG_EDIT_QUALITATIVE,
	PIG_PG_EDIT_CUTOFF,
	
	/**
	 *  Reader Config
	 */
	PID_CFG_FILTER,
	PID_CFG_TIME,
	PID_CFG_SERIALPORT,
	PID_CFG_PRINTER,
	PID_CFG_MAINTENANCE,
	PID_CFG_CHECK_PLATE,

};

struct  tagTipInfo
{
	INT32S		mTipID;
	INT32S		mParam1;
	INT32S		mParam2;
	void*	    mpExt;
};

#endif // __ENK_BASE_H__