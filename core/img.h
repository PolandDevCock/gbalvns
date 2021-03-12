#ifndef __IMG_H__
#define __IMG_H__
#ifdef __cplusplus
extern "C" {
#endif


#include "libgba/gba.h"

//---------------------------------------------------------------------------
#define IMG_MAX_FILE_LEN					14

#define IMG_BG_STR							"MAX_S%02d.img"
#define IMG_CHR_STR							"MAX_C%02X.img"
#define IMG_HVS_STR							"HVS%02d.img"
#define IMG_VIS_STR							"VIS%02d.img"


enum {
	IMG_EFFECT_FADE_BLACK,					// 0.Black �t�F�[�h
	IMG_EFFECT_GURUGURU,					// 1.���邮��
	IMG_EFFECT_SLANTTILE,					// 2.�΂��ꉡ���U�C�N�X���C�h
	IMG_EFFECT_FADE_SQUARE,					// 3.�Ђ��`���t�F�[�h
	IMG_EFFECT_WIPE_SQUARE_LTOR,			// 4.�Ђ��`�������C�v
	IMG_EFFECT_FADE_MASK,					// 5.�}�X�N�t�F�[�h
	IMG_EFFECT_WIPE_TTOB,					// 6.�ォ�烏�C�v
	IMG_EFFECT_WIPE_LTOR, 					// 7.�����烏�C�v
	IMG_EFFECT_WIPE_MASK_LTOR,				// 8.�����烏�C�v(�}�X�N)
	IMG_EFFECT_VERTCOMPOSITION,				// 9.�c�����ł͂��݂���
	IMG_EFFECT_SLIDE_LTOR,					// 10.������c�X���C�h
	IMG_EFFECT_NORMAL,						// 11.�P���\��
	IMG_EFFECT_FADE_MASK2,					// 12.�}�X�N�t�F�[�h
	IMG_EFFECT_FADE_GUNYARI,				// 13.���ɂ��
	IMG_EFFECT_FADE_GUNYARI2,				// 14.���ɂ��2
	IMG_EFFECT_SHAKE,						// 15.��ʂ�h�炷

	// �ǉ�
	IMG_EFFECT_FADE_WHITE,					// 16.White �t�F�[�h
	IMG_EFFECT_WAVE,						// 17.Wave �^�C�g�����S�p
	IMG_EFFECT_TEXT,						// 18.���b�Z�[�W �t�F�[�h
};

enum {
	IMG_CHR_TYPE_LEFT,
	IMG_CHR_TYPE_RIGHT,
	IMG_CHR_TYPE_MID,
	IMG_CHR_TYPE_ALL,
};

enum {
	IMG_BG_TYPE_BACK,
	IMG_BG_TYPE_VISUAL,
	IMG_BG_TYPE_HCG,

	IMG_BG_TYPE_FILL,						// ���F
	IMG_BG_TYPE_LOGO,						// ���S�\��
	IMG_BG_TYPE_CLEAR,						// �����Ȃ��i���F�j
};

//---------------------------------------------------------------------------

// �摜�t�@�C���̃w�b�_
typedef struct {
	u16  cx;
	u16  cy;
} __PACKED ST_IMG_HEADER;


// �摜�Ǘ��p
typedef struct {
	bool isDraw;
	char fname[IMG_MAX_FILE_LEN];
	u16  type;
	u16  cx;
	u16  cy;
	u16* pDat;
} ST_CONTROL;


typedef struct {
	u16  step;

	// �w�i�{�L�����N�^�Ǘ�
	ST_CONTROL bg;
	ST_CONTROL chr[3];

	// ��ʑJ�ځiClear�j
	bool isClear;
	u16  clearType;
	u16  clearNum;

	// ��ʑJ�ځiUpdate�j
	bool isUpdate;
	u16  updateType;
	u16  updateNum;

	// ��ʑJ�ځi�e�L�X�g�j
	bool isTextClear;
	bool isText;							// �e�L�X�g�̕\���L��
	u16  textFadeNum;						// �\�����̃t�F�[�h
	u16  textWaitNum;						// �e�L�X�g�\�����̃E�F�C�g

} ST_IMG;


//---------------------------------------------------------------------------
EWRAM_CODE void ImgInit(void);
EWRAM_CODE void ImgExec(void);
EWRAM_CODE void ImgExecClear(void);
EWRAM_CODE void ImgExecUpdate(void);
EWRAM_CODE void ImgExecTextClear(void);
EWRAM_CODE void ImgExecBg(void);
EWRAM_CODE void ImgExecChr(void);

EWRAM_CODE void ImgSetBg(char* pStr);
EWRAM_CODE void ImgSetBg2(u16 no);
EWRAM_CODE void ImgSetBgV(u16 no);
EWRAM_CODE void ImgSetBgH(u16 no);
EWRAM_CODE void ImgSetBgFill(void);
EWRAM_CODE void ImgSetBgLogo(void);
EWRAM_CODE void ImgSetBgClear(void);

EWRAM_CODE void ImgSetChr(char* pStr, u16 type);
EWRAM_CODE void ImgSetChr2(u16 no, u16 type);
EWRAM_CODE void ImgSetChrClear(u16 type);

EWRAM_CODE void ImgSetEffectClear(u16 type);
EWRAM_CODE void ImgSetEffectClear2(u16 type, u16 num);
EWRAM_CODE void ImgSetEffectUpdate(u16 type);
EWRAM_CODE void ImgSetEffectUpdate2(u16 type, u16 num);

EWRAM_CODE u16  ImgGetTextFadeNum(void);
EWRAM_CODE u16  ImgGetTextWaitNum(void);
EWRAM_CODE void ImgSetTextFadeNum(u16 num);
EWRAM_CODE void ImgSetTextWaitNum(u16 num);

EWRAM_CODE bool ImgIsEnd(void);
EWRAM_CODE bool ImgIsText(void);


#ifdef __cplusplus
}
#endif
#endif
