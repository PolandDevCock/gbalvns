#ifndef __SIORI_H__
#define __SIORI_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------
#define SIORI_MAX_CNT				(8+1)				// +1�́A�I�����̂P�O�ɖ߂�p
#define SIORI_MAX_MENU_CNT			(8)


//---------------------------------------------------------------------------

typedef struct {
	u16 size;
} ST_SIORI;



//---------------------------------------------------------------------------
EWRAM_CODE void  SioriInit(void);

EWRAM_CODE void  SioriSaveSig(void);
EWRAM_CODE void  SioriSaveFlag(void);
EWRAM_CODE void  SioriLoadFlag(void);
EWRAM_CODE void  SioriSave(u16 no);
EWRAM_CODE bool  SioriLoad(u16 no);
EWRAM_CODE void  SioriSavePrev(void);
EWRAM_CODE void  SioriLoadPrev(void);
EWRAM_CODE char* SioriGetName(u16 no);

EWRAM_CODE bool  SioriIsSram(void);


#ifdef __cplusplus
}
#endif
#endif
