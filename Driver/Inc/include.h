#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

//system
#include "option.h"
#include "def.h"
#include "44b.h"

#ifdef  ENM_VER_TARGET
#include "44blib.h"
#include "rtc.h"
#endif

#include "lcdlib.h"
#include "glib.h"
#include "TEXT.h"
#include "Glib.h"
#include "slib.h"
#include "zk.h"

//Hard Ware
#include "key&motor.h"


typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned long  INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   long  INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point  */
  
typedef	char			MCHAR;

typedef unsigned char  BOOLEAN;

//App
/*
#include "Main_Menu_Pro.h"
#include "Test_Method_Set_Menu_Pro.h"
#include "Measure_Method_Set_Menu_Pro.h"
#include "Single_Wave_Length_Measure_Pro.h"
#include "Double_Wave_Length_Measure_Pro.h"		
#include "Double_Time_Measure_Pro.h"
#include "Power_Method_Measure_Pro.h"		
#include "Blank_Method_Set_Menu_Pro.h"
#include "No_Blank_Pro.h"
#include "Multi_Blank_Pro.h"
#include "Row_Blank_Pro.h"
#include "Col_Average_Blank_Pro.h"		
#include "Col_Blank_Pro.h"
#include "Row_Average_Blank_Pro.h"
#include "Cal_Method_Set_Menu_Pro.h"
#include "Plate_In_Mode_Set_Pro.h"
#include "Shake_Plate_Mode_Set_Pro.h"
#include "Board_Analyze_Mode_Set_Pro.h"
	

#include "Test_Result_Pro_Menu_Pro.h"
#include "System_Set_Menu_Pro.h"
#include "Test_Program_Manage_Menu_Pro.h"

#include   "Par_Define.h"

*/

#define TRUE 	1
#define FALSE 	0


#endif  // __INCLUDE_H__
