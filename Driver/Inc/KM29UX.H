#ifndef __KM29UX_H__
#define __KM29UX_H__
#include "def.h"

#define	NF_INVALIDE_BLOCK_ID		((U16)-1)

#ifdef __cplusplus
extern "C" {
#endif

U16 _EraseBlock(unsigned int block); 
U16 _WrPage(unsigned int block,unsigned int page,U8 *pPage);
U16 _RdPageAll(unsigned int block,unsigned int page,U8 *pPage);  
U16 _WrPageAll(unsigned int block,unsigned int page,U8 *pPage);
U16 _RdPage(unsigned int block,unsigned int page,U8 *pPage);  
U16 _RdPageSpare(unsigned int block,unsigned int page,U8 *pPage);
U16 _WrPageSpare(unsigned int block,unsigned int page,U8 *pPage);
U16 _CheckId(void);

U8     _ZeroBlock( U32 block );

#ifdef __cplusplus
}
#endif

#endif /*__KM29UX_H__*/