#ifndef __ADRV_H__
#define __ADRV_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "../libgba/gba.h"


//---------------------------------------------------------------------------
#define ACRV_INTERUPT_TIMER			(65535-950)	// ���荞�݃^�C�~���O
												// �������߂ɂ��Ă܂��B�{����1097

#define ACRV_MAX_SCALE_CNT			12
#define ACRV_MAX_TRACK_CNT			4
#define ACRV_NO_DATA				0x0000


enum {
	C2 = 0,
	Cs2,
	D2,
	Ds2,
	E2,
	F2,
	Fs2,
	G2,
	Gs2,
	A2,
	As2,
	B2,
	C3,
	Cs3,
	D3,
	Ds3,
	E3,
	F3,
	Fs3,
	G3,
	Gs3,
	A3,
	As3,
	B3,
	C4,
	Cs4,
	D4,
	Ds4,
	E4,
	F4,
	Fs4,
	G4,
	Gs4,
	A4,
	As4,
	B4,
	C5,
	Cs5,
	D5,
	Ds5,
	E5,
	F5,
	Fs5,
	G5,
	Gs5,
	A5,
	As5,
	B5,
	C6,
	Cs6,
	D6,
	Ds6,
	E6,
	F6,
	Fs6,
	G6,
	Gs6,
	A6,
	As6,
	B6,
	C7,
	Cs7,
	D7,
	Ds7,
	E7,
	F7,
	Fs7,
	G7,
	Gs7,
	A7,
	As7,
	B7	,			// ����
	CMD_DUMMY,		// �_�~�[
	CMD_R,			// �x��
	CMD_Q,			// �Q�[�g
	CMD_P,			// �p���|�b�g
	CMD_W,			// �f���[�e�B
	CMD_S,			// �X�e�b�v�^�C��
	CMD_D,			// �f�`���[��(���g�����炵)
	CMD_ECHO,		// �G�R�[
	CMD_ECHOVOL,	// �G�R�[�{���E��
	CMD_AND,		// �^�C
	CMD_TAI,		// �^�C
	CMD_LOOPBEGIN,	// ���[�v�J�n
	CMD_LOOPEND,	// ���[�v�I��
	CMD_VOLUME,		// ����
	CMD_TONE,		// ���F
	CMD_TRACKLOOP,	// �g���b�N�擪�ɖ߂�
	CMD_TRACKEND,	// �g���b�N�I��

	CMD_MATUYOSI_SW,		// �X�B�[�v(ch1 only)
	CMD_MATUYOSI_PL,		// �p�^�[���E�����O�X(all channel)
	CMD_MATUYOSI_EV,		// �G���x���[�v(ch1,ch2 only)
	CMD_MATUYOSI_VL,		// �{�����[��(ch3 only)
	CMD_MATUYOSI_SP,		// �Đ��X�s�[�h

	CMD_DUMMY_END,
};


//---------------------------------------------------------------------------
// ���y�f�[�^�̃w�b�_
typedef struct {
	char id[8];				// 'ANDRIVER'
	u16  offset[4];			// �e�g���b�N�̐擪����̈ʒu,0�̏ꍇ�ɂ́A���̃g���b�N���g�p

} __PACKED ST_AC_HEADER;


// �g���b�N�̃X�e�[�^�X
typedef struct {
	// GBA
	u8  sweepShift;			// �X�E�B�[�v�̗�  �i0-7�j
	u8  sweepDir;			// �X�E�B�[�v�̕����i0=Inc, 1=Dec�j
	u8  sweepTime;			// �X�E�B�[�v�̎��ԁi0-7�j
	u8  duty;				// �f���[�e�B�[��i0-3�j
	u8  envelopStep;		// �G���x���[�v�̌����i0-7�j
	u8  envelopDir;			// �G���x���[�v�̕����i0=Dec, 1=Inc�j
	u8  envelopVol;			// �G���x���[�v�̃{�����[���i0-15�j
	u16 freq;				// ���g���i0: �x���j

	// MML
	bool isUse;				// �g�p�̗L��
	u8*  pTop;				// �擪�ʒu
	u8*  pCur;				// ��ƈʒu

	bool isAnd;				// �A���h
	bool isPan;				// �p��
	u8   pan;
	u8   gate;				// �Q�[�g
	u8   gateWork;
	u8   echo;				// �G�R�[
	u8   echoWork;
	u8   echoVol;
	s8   detune;			// �f�`���[��
	u8   note;				// ����
	u16  length;			// �����i�^�C�}���荞�ݖ��j

	// ONE �ǉ���
	u8   sp;

} ST_ADRV_STATUS;


// �e��h���C�o�̃X�e�[�^�X
typedef struct {
	bool isUse;
	ST_ADRV_STATUS status[4];

} ST_ADRV;


//---------------------------------------------------------------------------
EWRAM_CODE void AdrvInit(void);
EWRAM_CODE void AdrvSetData(u8* pData);

IWRAM_CODE void AdrvPlay(void);
IWRAM_CODE void AdrvStop(void);

IWRAM_CODE void AdrvExec(u16 num);
IWRAM_CODE void AdrvExecCh1(void);
IWRAM_CODE void AdrvExecCh2(void);
IWRAM_CODE void AdrvExecCh3(void);
IWRAM_CODE void AdrvExecCh4(void);
IWRAM_CODE void AdrvExecPan(u16 num);


IWRAM_CODE void AdrvIntr(void);
IWRAM_CODE void AdrvIntrSub(void);

IWRAM_CODE void AdrvNextCmd(u16 num);

IWRAM_CODE void AdrvDebug(u16 num);


#ifdef __cplusplus
}
#endif
#endif
