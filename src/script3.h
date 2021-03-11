#ifndef __SCRIPT3_H__
#define __SCRIPT3_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------
#define SCRIPT_MAX_MSG_CNT					21


//---------------------------------------------------------------------------

// ���b�Z�[�W��̓e�[�u��
typedef struct {
	char  op;							// ���߂̃I�y�R�[�h
	void  (*pFunc)(void);				// ���߂̊֐�
} ST_SCRIPT_MSG_TABLE;


//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsg(void);

EWRAM_CODE void ScriptExecMsgEnd(void);
EWRAM_CODE void ScriptExecMsgLf(void);
EWRAM_CODE void ScriptExecMsgWaitPage(void);
EWRAM_CODE void ScriptExecMsgWaitKey(void);
EWRAM_CODE void ScriptExecMsgSkip1(void);
EWRAM_CODE void ScriptExecMsgSetChr(void);
EWRAM_CODE void ScriptExecMsgSetBg(void);
EWRAM_CODE void ScriptExecMsgSetBgAndChr(void);
EWRAM_CODE void ScriptExecMsgSetChrCls(void);
EWRAM_CODE void ScriptExecMsgSetChrAll(void);
EWRAM_CODE void ScriptExecMsgSetEffect(void);
EWRAM_CODE void ScriptExecMsgSetBg2(void);
EWRAM_CODE void ScriptExecMsgSetFlash(void);
EWRAM_CODE void ScriptExecMsgSetVisual(void);
EWRAM_CODE void ScriptExecMsgSetHscene(void);
EWRAM_CODE void ScriptExecMsgBgm(void);
EWRAM_CODE void ScriptExecMsgPcm(void);
EWRAM_CODE void ScriptExecMsgOffset(void);
EWRAM_CODE void ScriptExecMsgSpeed(void);

IWRAM_CODE void ScriptExecMsgText(void);

#ifdef __cplusplus
}
#endif
#endif

