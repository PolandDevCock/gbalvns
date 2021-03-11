#ifndef __ANIME_H__
#define __ANIME_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------
#define ANIME_MAX_DB_CNT				6
#define ANIME_MAX_TABLE_CNT				21
#define ANIME_MAX_STR_LEN				13

enum {
	ANIME_TYPE_JINGLE = 0x00,
	ANIME_TYPE_OPENING,
	ANIME_TYPE_TITLE,
	ANIME_TYPE_NAMIDA,
	ANIME_TYPE_ENDING,
	ANIME_TYPE_CLEAR,
};

enum {
	ANIME_ACT_OP = 0x01,						// �I�y�R�[�h���擾��
	ANIME_ACT_FUNC,								// �@�\���s��
	ANIME_ACT_END,								// �I��
};

//---------------------------------------------------------------------------

typedef struct {
	u16   act;									// ���
	void  (*pFunc)(void);						// ���߂̊֐�
	u16   funcStep;								// ���߂̎��s�X�e�b�v

	u16   type;									// �A�j���[�V�����̃^�C�v
	bool  isStop;								// �L�[���͂̒��f�L��
	bool  isKey;								// �L�[�������ꂽ�ꍇ�̃t���O

	// ��Ɨp�ϐ�
	u16   val;
	u16   wait;
	u16   waitCnt;
	char  str[ANIME_MAX_STR_LEN];
	char* pCur;
} ST_ANIME;


typedef struct {
	char* pDat;									// �A�j���[�V�����t�@�C���̊i�[�ʒu
} ST_ANIME_DB;

typedef struct {
	char* pStr;									// ���߂̕�����
	void  (*pFunc)(void);						// ���߂̊֐�
} ST_ANIME_TABLE;

//---------------------------------------------------------------------------
EWRAM_CODE void AnimeInit(void);
EWRAM_CODE void AnimeSetType(u16 type, bool isStop);
EWRAM_CODE void AnimeSetFunc(void);
EWRAM_CODE void AnimeSetKeyOn(bool is);
EWRAM_CODE void AnimeExec(void);

EWRAM_CODE void AnimeExecImage(void);
EWRAM_CODE void AnimeExecImage2(void);
EWRAM_CODE void AnimeExecImage3(void);
EWRAM_CODE void AnimeExecUpdate(void);
EWRAM_CODE void AnimeExecPrint(void);
EWRAM_CODE void AnimeExecPrintCls(void);
EWRAM_CODE void AnimeExecFill(void);
EWRAM_CODE void AnimeExecBlackIn(void);
EWRAM_CODE void AnimeExecBlackOut(void);
EWRAM_CODE void AnimeExecWhiteIn(void);
EWRAM_CODE void AnimeExecWhiteOut(void);
EWRAM_CODE void AnimeExecWait(void);
EWRAM_CODE void AnimeExecWaitKey(void);
EWRAM_CODE void AnimeExecWave(void);
EWRAM_CODE void AnimeExecBgm(void);
EWRAM_CODE void AnimeExecSetVal(void);
EWRAM_CODE void AnimeExecSetStr(void);
EWRAM_CODE void AnimeExecSetPos(void);
EWRAM_CODE void AnimeExecInc(void);
EWRAM_CODE void AnimeExecJb(void);
EWRAM_CODE void AnimeExecEnd(void);

EWRAM_CODE bool AnimeIsKeyOn(void);
EWRAM_CODE bool AnimeIsEnd(void);

EWRAM_CODE void AnimeCalcRemoveStrQuart(char* pDst, char* pSrc);



#ifdef __cplusplus
}
#endif
#endif
