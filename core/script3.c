#include "script3.h"
#include "script.h"
#include "img.h"
#include "bgm.h"
#include "text.h"


// script3.c
// �u���v���b�Z�[�W�p�[�T�{��

//---------------------------------------------------------------------------
const ST_SCRIPT_MSG_TABLE ScriptMsgTable[SCRIPT_MAX_MSG_CNT] = {
	{ '$', (void*)ScriptExecMsgEnd         },
	{ 'r', (void*)ScriptExecMsgLf          },
	{ 'p', (void*)ScriptExecMsgWaitPage    },
	{ 'k', (void*)ScriptExecMsgWaitKey     },
	{ 'K', (void*)ScriptExecMsgWaitKey     },
	{ 'O', (void*)ScriptExecMsgSkip1       },
	{ 'C', (void*)ScriptExecMsgSetChr      },
	{ 'B', (void*)ScriptExecMsgSetBg       },
	{ 'S', (void*)ScriptExecMsgSetBgAndChr },
	{ 'D', (void*)ScriptExecMsgSetChrCls   },
	{ 'A', (void*)ScriptExecMsgSetChrAll   },
	{ 'a', (void*)ScriptExecMsgSetChrAll   },
	{ 'Q', (void*)ScriptExecMsgSetEffect   },
	{ 'E', (void*)ScriptExecMsgSetBg2      },
	{ 'F', (void*)ScriptExecMsgSetFlash    },
	{ 'V', (void*)ScriptExecMsgSetVisual   },
	{ 'H', (void*)ScriptExecMsgSetHscene   },
	{ 'M', (void*)ScriptExecMsgBgm         },
	{ 'P', (void*)ScriptExecMsgPcm         },
	{ 'X', (void*)ScriptExecMsgOffset      },
	{ 's', (void*)ScriptExecMsgSpeed       },
};

//---------------------------------------------------------------------------
extern ST_SCRIPT Script;


//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsg(void)
{
	u8  op;
	u16 i;

	Script.pMsgOld = Script.pMsgCur;
	Script.isLoop  = TRUE;

	do
	{
		op = *Script.pMsgCur++;

		if(op & 0x80)
		{
			Script.pMsgCur--;
			ScriptExecMsgText();

			continue;
		}

		for(i=0; i<SCRIPT_MAX_MSG_CNT ;i++)
		{
			if(op == ScriptMsgTable[i].op)
			{
				TRACEOUT("--------------------------------\n");
				TRACEOUT("[���b�Z�[�W: %0x02x]\n", (u32)op);

				ScriptMsgTable[i].pFunc();
				break;
			}
		}
		_ASSERT(i < SCRIPT_MAX_MSG_CNT);


	} while(Script.isLoop == TRUE);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgEnd(void)
{
	TRACEOUT("[���b�Z�[�W�I��]\n");

	ScriptSetAct(SCRIPT_ACT_EVENT);
	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgLf(void)
{
	TRACEOUT("[���s]\n");

	// ���s��PC98��Windows��̉�ʃT�C�Y�ɍ��킹�Ĕz�u���Ă���ׁA
	// GBA�ł͖������܂�
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgWaitPage(void)
{
	TRACEOUT("[�y�[�W�X�V�҂�]\n");

	ImgSetEffectUpdate(IMG_EFFECT_TEXT);
	ScriptSetAct(SCRIPT_ACT_WAIT_PAGE);

	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgWaitKey(void)
{
	TRACEOUT("[�L�[���͑҂�]\n");

	ImgSetEffectUpdate(IMG_EFFECT_TEXT);
	ScriptSetAct(SCRIPT_ACT_WAIT_KEY);

	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgSkip1(void)
{
	TRACEOUT("[��1]\n");
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgSetChr(void)
{
	u8* c = Script.pMsgCur;

	u8 pos   = ScriptGetMapChrPos(c[0]);
	u8 chrNo = ScriptGetMapHexToDig2(c[1], c[2]);

	TRACEOUT("[�L�����N�^����: %d, MAX_C%02X]\n", pos, chrNo);

	ImgSetChr2(chrNo, pos);
	ImgSetEffectUpdate(IMG_EFFECT_FADE_MASK);
	BgmPlayNext();

	Script.pMsgCur += 3;
	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgSetBg(void)
{
	u8* c = Script.pMsgCur;

	u8 num1 = ScriptGetMapBg2(c[0], c[1]);
	u8 num2 = ScriptGetMapEffect(c[2], c[3]);
	u8 num3 = ScriptGetMapEffect(c[4], c[5]);

	TRACEOUT("[�w�i���[�h: %d, %d, %d]\n", num1, num2, num3);

	ImgSetEffectClear(num2);
	ImgSetBg2(num1);
	ImgSetEffectUpdate(num3);
	BgmPlayNext();

	Script.pMsgCur += 6;
	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgSetBgAndChr(void)
{
	u8* c = Script.pMsgCur;

	u8 num1 = ScriptGetMapChrPos(c[0]);
	u8 num2 = ScriptGetMapHexToDig2(c[1], c[2]);
	u8 num3 = ScriptGetMapBg2(c[3], c[4]);
	u8 num4 = ScriptGetMapEffect(c[5], c[6]);
	u8 num5 = ScriptGetMapEffect(c[7], c[8]);

	TRACEOUT("[�w�i�t���L�����\\��: %d, MAX_C%02X, MAX_S%02d, %d, %d]\n", num1, num2, num3, num4, num5);

	ImgSetEffectClear(num4);
	ImgSetBg2(num3);
	ImgSetChr2(num2, num1);
	ImgSetEffectUpdate(num3);
	BgmPlayNext();

	Script.pMsgCur += 9;
	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgSetChrCls(void)
{
	u8* c = Script.pMsgCur;

	u8 pos = ScriptGetMapChrPos(c[0]);
	u8 no  = ScriptGetMapHexToDig2(c[1], c[2]);

	TRACEOUT("[�L�����S������\\��: %d, MAX_C%02X]\n", pos, no);

	ImgSetChrClear(IMG_CHR_TYPE_ALL);
	ImgSetChr2(no, pos);
	ImgSetEffectUpdate(IMG_EFFECT_FADE_MASK);
	BgmPlayNext();

	Script.pMsgCur += 3;
	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgSetChrAll(void)
{
	u8* c = Script.pMsgCur;

	u8 pos1 = ScriptGetMapChrPos(c[0]);
	u8 num1 = ScriptGetMapHexToDig2(c[1], c[2]);
	u8 pos2 = ScriptGetMapChrPos(c[3]);
	u8 num2 = ScriptGetMapHexToDig2(c[4], c[5]);
	u8 pos3 = ScriptGetMapChrPos(c[6]);
	u8 num3 = ScriptGetMapHexToDig2(c[7], c[8]);

	TRACEOUT("[�L����3�l�\\��: %d, %x, %d, %x, %d, %x]\n", pos1, num1, pos2, num2, pos3, num3);

	ImgSetChr2(num1, pos1);
	ImgSetChr2(num2, pos2);
	ImgSetChr2(num3, pos3);
	ImgSetEffectUpdate(IMG_EFFECT_FADE_MASK);

	Script.pMsgCur += 9;
	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgSetEffect(void)
{
	TRACEOUT("[��ʂ�h�炷]\n");
	ImgSetEffectUpdate(IMG_EFFECT_SHAKE);

	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgSetBg2(void)
{
	u8* c = Script.pMsgCur;

	u8 num1 = ScriptGetMapBg2(c[0], c[1]);
	u8 num2 = ScriptGetMapEffect(c[2], c[3]);
	u8 num3 = ScriptGetMapEffect(c[4], c[5]);

	TRACEOUT("[�w�i���[�h(2)?: MAX_S%02d.img, %d, %d]\n", num1, num2, num3);

	ImgSetEffectClear(num2);
	ImgSetBg2(num1);
	ImgSetEffectUpdate(num3);
	BgmPlayNext();

	Script.pMsgCur += 6;
	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgSetFlash(void)
{
	TRACEOUT("[�t���b�V��]\n");

	ImgSetEffectClear(IMG_EFFECT_FADE_WHITE);
	ImgSetEffectUpdate(IMG_EFFECT_FADE_WHITE);

	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgSetVisual(void)
{
	u8* c = Script.pMsgCur;

	u8 num1 = ScriptGetMapVisual2(c[0], c[1]);
	u8 num2 = ScriptGetMapEffect(c[2], c[3]);
	u8 num3 = ScriptGetMapEffect(c[4], c[5]);

	TRACEOUT("[�r�W���A��: VIS%02d.img, %d, %d]\n", num1, num2, num3);

	ImgSetEffectClear(num2);
	ImgSetBgV(num1);
	ImgSetEffectUpdate(num3);
	BgmPlayNext();

	Script.pMsgCur += 6;
	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgSetHscene(void)
{
	u8* c = Script.pMsgCur;

	u8 num1 = ScriptGetMapDig(c[0], c[1]);
	u8 num2 = ScriptGetMapEffect(c[2], c[3]);
	u8 num3 = ScriptGetMapEffect(c[4], c[5]);

	TRACEOUT("[H�V�[��(HVS02d.img, %d, %d)]\n", num1, num2, num3);

	ImgSetEffectClear(num2);
	ImgSetBgH(num1);
	ImgSetEffectUpdate(num3);
	BgmPlayNext();

	Script.pMsgCur += 6;
	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgBgm(void)
{
	u8 c1 = *Script.pMsgCur++;

	if(c1 == 'f')
	{
		TRACEOUT("[BGM �t�F�[�h�A�E�g]\n");
		BgmStopOut();
	}
	else if (c1 == 'n')
	{
		u8 c2    = *Script.pMsgCur++;
		u8 c3    = *Script.pMsgCur++;
		u8 num   = ScriptGetMapDig(c2, c3);
		u8 bgmNo = ScriptGetMapBgm(num);

		TRACEOUT("[BGM �Đ�(next): tr_%03d.8ad\n", bgmNo);
		BgmSetNext(bgmNo, TRUE);
	}
	else if (c1 == 'w')
	{
		TRACEOUT("[BGM FADE WAIT]\n");
		// EMPTY
	}
	else if (c1 >= '0' && c1 <= '2')
	{
		u8 c2    = *Script.pMsgCur++;
		u8 num   = ScriptGetMapDig(c1, c2);
		u8 bgmNo = ScriptGetMapBgm(num);

		TRACEOUT("[BGM �Đ�2: tr_%03d.8ad\n", bgmNo);
		BgmPlay(bgmNo, TRUE);
	}
	else if (c1 == 's')
	{
		TRACEOUT("[BGM ��~]\n");
		BgmStop();
	}
	else
	{
		TRACEOUT("[cmd:4d]\n", c1);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgPcm(void)
{
	// PCM�֘A�͖��Ή��ł�
	u8 c1 = *Script.pMsgCur++;

	if(c1 == 'l')
	{
		u8 c2  = *Script.pMsgCur++;
		u8 c3  = *Script.pMsgCur++;
		u8 num = ScriptGetMapDig(c2, c3);

		TRACEOUT("[PCM���[�h(%d)]\n", num);
	}
	else if (c1 >= '0' && c1 <= '9')
	{
		u8 c2 = *Script.pMsgCur++;
		u8 c3 = *Script.pMsgCur++;
		u8 c4 = *Script.pMsgCur++;

		TRACEOUT("[PCM�Đ��w��(%c%c, %c%c)]\n", c1, c2, c3, c4);
	}
	else if(c1 == 'f')
	{
		TRACEOUT("[PCM�t�F�[�h�A�E�g?]\n");
	}
	else if(c1 == 'w')
	{
		TRACEOUT("[PCM wait?]\n");
	}
	else if(c1 == 's')
	{
		TRACEOUT("[PCM��~]\n");
	}
	else
	{
		TRACEOUT("[cmd:50][cmd:%x]\n", c1);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgOffset(void)
{
	u8 chr = *Script.pMsgCur++;

	TRACEOUT("[�\\���I�t�Z�b�g�w��: %x]\n", chr);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMsgSpeed(void)
{
	u8 chr = *Script.pMsgCur++;

	TRACEOUT("[�\\�����x�w��?: %x]\n", chr);
}
//---------------------------------------------------------------------------
IWRAM_CODE void ScriptExecMsgText(void)
{
	u16 buf[512] ALIGN(4);
	u16 cnt = 0;

	do
	{
		if(*Script.pMsgCur == 'r')
		{
			Script.pMsgCur++;
			continue;
		}

		buf[cnt++] = ((*Script.pMsgCur & 0x7f) << 8) | (*(Script.pMsgCur + 1));
		Script.pMsgCur += 2;

	} while((*Script.pMsgCur & 0x80) || (*Script.pMsgCur == 'r'));

	_ASSERT(cnt < 512 && "ScriptExecMsgText buf overflow");


	TextSetBufWork(buf, cnt);
}
