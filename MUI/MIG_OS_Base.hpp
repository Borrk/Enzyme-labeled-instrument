// Type defination file

#ifndef __MIG_OS_BASE_H__
#define __MIG_OS_BASE_H__

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define GetKeyValue(key)	((key) & 0xFF)
#define GetKeyCount(key)	(((key) >> 8)& 0xFF )


#endif 	// __MIG_OS_BASE_H__