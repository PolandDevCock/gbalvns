#ifndef __TEXT_H__
#define __TEXT_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------
#define TEXT_IMG_MAX_FONT_CNT			1853
#define TEXT_IMG_CX						12
#define TEXT_IMG_CY						10
#define TEXT_IMG_BLANK_CY				2

#define TEXT_SCREEN_CX					17
#define TEXT_SCREEN_CY					10

#define TEXT_CURSOR_SX					18
#define TEXT_CURSOR_SY					22
#define TEXT_CURSOR_WAIT_CNT			10				// vblank�Ɠ������Ă��܂�

#define TEXT_SIORI_CX					12

//---------------------------------------------------------------------------

typedef struct {
	u16* pImg;											// �t�H���g�f�[�^
	u16* pSjis2Leaf;									// �ϊ��e�[�u��
	u16  x;												// �������݈ʒu
	u16  y;

	bool isNewPage;										// ���y�[�W������t���O
	bool isDraw;										// �e�L�X�g��`�悷�邩�̃t���O
	u16  drawCnt;										// �`��J�E���g
	u16  waitCnt;										// �`�掞�̃E�F�C�g

	// �J�[�\��
	bool isCurDraw;
	bool isCurBlink;
	u16  curWaitCnt;

	// �֑��������[�N�o�b�t�@
	u16  workBuf[TEXT_SCREEN_CX * TEXT_SCREEN_CY] ALIGN(4);
	u16  workCnt;

	// �ޔ�p�̕����R�[�h�o�b�t�@
	u16  outBuf[TEXT_SCREEN_CX * TEXT_SCREEN_CY] ALIGN(4);

	// ������p�o�b�t�@
	u16  sioriBuf[TEXT_SIORI_CX] ALIGN(4);
} ST_TEXT;


//---------------------------------------------------------------------------
EWRAM_CODE void TextInit(void);
EWRAM_CODE void TextExec(void);
EWRAM_CODE void TextExecChr(void);
EWRAM_CODE void TextExecCur(void);

EWRAM_CODE void TextSetImgWhite(void);
EWRAM_CODE void TextSetImgGray(void);
EWRAM_CODE void TextSetDisp(void);
EWRAM_CODE void TextSetDispCur(void);
EWRAM_CODE void TextSetUnDisp(void);
EWRAM_CODE void TextSetUnDispCur(void);
EWRAM_CODE void TextSetClear(void);
EWRAM_CODE void TextSetClearNoBuf(void);
EWRAM_CODE void TextSetClearXY(void);

EWRAM_CODE void TextSetNewPage(void);
EWRAM_CODE void TextSetBuf(u16 x, u16 y, u16 code);
IWRAM_CODE void TextSetBufSiori(u16* pStr, u16 len);
IWRAM_CODE void TextSetBufWork(u16* pStr, u16 len);
IWRAM_CODE void TextSetBufWorkNoWait(u16* pStr, u16 len, bool isBuf);
IWRAM_CODE void TextSetRestore(void);

IWRAM_CODE void TextSetDat(u16 x, u16 y, u16 code);
IWRAM_CODE void TextSetDatSub1(u16 x, u16 y, u16 code);
IWRAM_CODE void TextSetDatSub2(u16 x, u16 y, u16 code);
IWRAM_CODE void TextSetDatSub3(u16 x, u16 y, u16 code);
IWRAM_CODE void TextSetDatSub4(u16 x, u16 y, u16 code);

IWRAM_CODE void TextSetLf(void);
IWRAM_CODE void TextSetChr(u16 code, bool isBuf);
IWRAM_CODE void TextSetStr(u16* pStr, u16 len, bool isBuf);
IWRAM_CODE void TextSetStrSjis(u16 x, u16 y, u16* pStr);
IWRAM_CODE void TextSetStrFull(u16* pStr);

IWRAM_CODE u16  TextGetLeafCode(u16 code);
IWRAM_CODE u16  TextGetSjisCode(u16 code);
EWRAM_CODE u16* TextGetSioriStr(void);

IWRAM_CODE bool TextCalcStrBoundary(u16* pSrc, u16 len);
IWRAM_CODE bool TextIsChrBoundaryBefore(u16 code);
IWRAM_CODE bool TextIsChrBoundaryAfter(u16 code);

EWRAM_CODE bool TextIsEnd(void);

EWRAM_CODE void TextDebug(u16 code);
EWRAM_CODE void TextDebugLf(void);


#ifdef __cplusplus
}
#endif
#endif
