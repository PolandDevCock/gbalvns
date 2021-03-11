#include "script2.h"
#include "script.h"
#include "anime.h"
#include "img.h"
#include "bgm.h"
#include "siori.h"
#include "history.h"
#include "text.h"


// script2.c
// �u���v�C�x���g�p�[�T�{��


//---------------------------------------------------------------------------
const ST_SCRIPT_EVENT_TABLE ScriptEventTable[SCRIPT_MAX_EVENT_CNT] = {
	{ 0x00, (void*)ScriptExecEventEnd        },
	{ 0x01, (void*)ScriptExecEventEffect     },
	{ 0x03, (void*)ScriptExecEventSkip2      },
	{ 0x04, (void*)ScriptExecEventJump       },
	{ 0x05, (void*)ScriptExecEventSelect     },
	{ 0x06, (void*)ScriptExecEventSkip1      },
	{ 0x07, (void*)ScriptExecEventSetPrev    },
	{ 0x0a, (void*)ScriptExecEventSetBg      },
	{ 0x14, (void*)ScriptExecEventSetBgCls   },
	{ 0x16, (void*)ScriptExecEventSetBgH     },
	{ 0x22, (void*)ScriptExecEventSetChr     },
	{ 0x24, (void*)ScriptExecEventSetChr2    },
	{ 0x28, (void*)ScriptExecEventSetMark2   },
	{ 0x38, (void*)ScriptExecEventSetEffect2 },
	{ 0x3d, (void*)ScriptExecEventIf         },
	{ 0x3e, (void*)ScriptExecEventIf2        },
	{ 0x47, (void*)ScriptExecEventSetFlag    },
	{ 0x48, (void*)ScriptExecEventAddFlag    },
	{ 0x54, (void*)ScriptExecEventMsg        },
	{ 0x5a, (void*)ScriptExecEventSkip1      },
	{ 0x5c, (void*)ScriptExecEventSkip2      },
	{ 0x60, (void*)ScriptExecEventSkip1      },
	{ 0x61, (void*)ScriptExecEventSkip3      },
	{ 0x62, (void*)ScriptExecEventSkip2      },
	{ 0x63, (void*)ScriptExecEventSkip1      },
	{ 0x64, (void*)ScriptExecEventSkip1      },
	{ 0x65, (void*)ScriptExecEventSkip1      },
	{ 0x66, (void*)ScriptExecEventSkip1      },
	{ 0x6e, (void*)ScriptExecEventSetBgm     },
	{ 0x6f, (void*)ScriptExecEventSkip1      },
	{ 0x73, (void*)ScriptExecEventSkip1      },
	{ 0x7c, (void*)ScriptExecEventEnding     },
	{ 0x7d, (void*)ScriptExecEventEnding2    },
	{ 0x7e, (void*)ScriptExecEventEnding3    },
};

//---------------------------------------------------------------------------
extern ST_SCRIPT Script;


//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEvent(void)
{
	u8  op;
	u16 i;

	Script.pEventOld = Script.pEventCur;
	Script.isLoop    = TRUE;

	do
	{
		op = *Script.pEventCur++;

		for(i=0; i<SCRIPT_MAX_EVENT_CNT; i++)
		{
			if(op == ScriptEventTable[i].op)
			{
				TRACEOUT("--------------------------------\n");
				TRACEOUT("[�C�x���g: 0x%02x]\n", (u32)op);

				ScriptEventTable[i].pFunc();
				break;
			}
		}
		TRACEOUT("%x\n", op);
		_ASSERT(i < SCRIPT_MAX_EVENT_CNT);


	} while(Script.isLoop == TRUE);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventEnd(void)
{
	TRACEOUT("[�u���b�N�I��]\n");

	BgmStop();
	HistoryInit();
	TextInit();

	Script.isNext = FALSE;
	Script.isLoop = FALSE;
	ScriptSetAct(SCRIPT_ACT_END);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventEffect(void)
{
	TRACEOUT("[�������]\n");

	u8 sel   = *Script.pEventCur++;
	u8 msgNo = *Script.pEventCur++;

	switch(sel)
	{
	case 0x01:
		TRACEOUT("[���ɂ�聨��]-[���b�Z�[�W: %d]\n", msgNo);
		ImgSetEffectClear(IMG_EFFECT_FADE_GUNYARI);
		ScriptSetMsg(msgNo);
		ImgSetEffectUpdate(IMG_EFFECT_FADE_BLACK);
		break;

	case 0x02:
		TRACEOUT("[�Á����ɂ��]-[���b�Z�[�W: %d]\n", msgNo);
		ImgSetEffectClear(IMG_EFFECT_FADE_BLACK);
		BgmPlayNext();
		ScriptSetMsg(msgNo);
		ImgSetEffectUpdate(IMG_EFFECT_FADE_GUNYARI);
		break;

	case 0x03:
		TRACEOUT("[�܂̎�: %02x]\n", msgNo);
		AnimeSetType(ANIME_TYPE_NAMIDA, FALSE);
		ScriptSetAct(SCRIPT_ACT_ANIME);
		break;

	case 0x04:
		TRACEOUT("[���ɂ��2(�َ���)]-[���b�Z�[�W:%d]\n", msgNo);
		ImgSetEffectClear(IMG_EFFECT_FADE_GUNYARI2);
		ScriptSetMsg(msgNo);
		break;

	default:
		TRACEOUT("�ُ��0x01�R�}���h�ł�(%02x,%02x)\n", msgNo);
		break;
	}

	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventSkip1(void)
{
	TRACEOUT("[��1]\n");
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventJump(void)
{
	u8 scnNo   = *Script.pEventCur++;
	u8 eventNo = *Script.pEventCur++;

	TRACEOUT("[�W�����v: SCN%03d.dat Block %d]\n", scnNo, eventNo);

	ScriptSetScn(scnNo);
	ScriptSetEvent(eventNo);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventSelect(void)
{
	TRACEOUT("[�I����]\n");

	ScriptSetAct(SCRIPT_ACT_SELECT);
	Script.isNext = FALSE;
	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventSetPrev(void)
{
	TRACEOUT("[�O�̑I�����ɖ߂�}�[�N�ʒu]\n");

	SioriSavePrev();
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventSetBg(void)
{
	u8 bgNo = ScriptGetMapBg(*Script.pEventCur++);

	TRACEOUT("[�w�i�̂݃��[�h: MAX_S%02d.img]\n", bgNo);
	ImgSetBg2(bgNo);

	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventSetBgCls(void)
{
	u8 no = *Script.pEventCur++;

	TRACEOUT("[��ʃN���A? %02d]\n", no);

	ImgSetEffectClear(no);
	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventSetBgH(void)
{
	u8 hNo = *Script.pEventCur++;

	TRACEOUT("[H�V�[�����[�h: MAX_H%02d.img]\n", hNo);

	ImgSetBgH(hNo);
	ImgSetEffectUpdate(IMG_EFFECT_NORMAL);
	BgmPlayNext();

	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventSetChr(void)
{
	u8 chrNo = *Script.pEventCur++;
	u8 pos   = ScriptGetMapChrPos(*Script.pEventCur++);

	TRACEOUT("[�L�����N�^���[�h: MAX_C%02X.img %02x]\n", chrNo, pos);

	ImgSetChr2(chrNo, pos);
	ImgSetEffectUpdate(IMG_EFFECT_FADE_MASK);
	BgmPlayNext();

	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventSetChr2(void)
{
	u8 chrNo = *Script.pEventCur++;

	// 1Byte�� �j�����܂�
	Script.pEventCur++;

	TRACEOUT("[�L�����N�^���[�h2?(center?): MAX_C%02X.img]\n", chrNo);

	ImgSetChr2(chrNo, IMG_CHR_TYPE_MID);
	ImgSetEffectUpdate(IMG_EFFECT_FADE_MASK);
	BgmPlayNext();

	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventSetMark2(void)
{
	TRACEOUT("[�I�����̑O�ɑ��݂���f�[�^]\n");
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventSetEffect2(void)
{
	u8 effectNo = ScriptGetMapEffect(0, *Script.pEventCur++);

	TRACEOUT("[�\\������: %02x]\n", effectNo);
	ImgSetEffectUpdate(effectNo);
	BgmPlayNext();

	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventIf(void)
{
	u8 no     = ScriptGetMapFlag(*Script.pEventCur++);
	u8 src    = ScriptGetFlag(no);
	u8 dst    = *Script.pEventCur++;
	u8 offset = *Script.pEventCur++;

	TRACEOUT("[if�� no:%02x val:%02x == 0x%02x pc += %02x]\n", no, src, dst, offset);

	if(src == dst)
	{
		Script.pEventCur += offset;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventIf2(void)
{
	u8 no     = ScriptGetMapFlag(*Script.pEventCur++);
	u8 src    = ScriptGetFlag(no);
	u8 dst    = *Script.pEventCur++;
	u8 offset = *Script.pEventCur++;

	TRACEOUT("[if��(�ے�) no:%02x val:%02x == 0x%02x pc += %02x]\n", no, src, dst, offset);

	if(src != dst)
	{
		Script.pEventCur += offset;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventSetFlag(void)
{
	u8 flagNo = ScriptGetMapFlag(*Script.pEventCur++);
	u8 val    = *Script.pEventCur++;

	TRACEOUT("[�t���O�̒l�ݒ�: %02x = 0x%02x]\n", flagNo, val);

	ScriptSetFlag(flagNo, val);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventAddFlag(void)
{
	u8 flagNo = ScriptGetMapFlag(*Script.pEventCur++);
	u8 add    = *Script.pEventCur++;

	TRACEOUT("[�t���O���Z: %02x += 0x%02x\n", flagNo, add);

	ScriptAddFlag(flagNo, add);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventMsg(void)
{
	u8 msgNo = *Script.pEventCur++;

	TRACEOUT("[���b�Z�[�W: %d]\n", msgNo);

	ScriptSetMsg(msgNo);
	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventSkip2(void)
{
	Script.pEventCur++;

	TRACEOUT("[��2]\n");
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventSkip3(void)
{
	Script.pEventCur += 2;

	TRACEOUT("[��3]\n");
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventSetBgm(void)
{
	u8 bgmNo = ScriptGetMapBgm(*Script.pEventCur++);

	TRACEOUT("[BGM�Đ�: %02x]\n", bgmNo);

	BgmPlay(bgmNo, TRUE);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventEnding(void)
{
	TRACEOUT("[�G���f�B���O�֌W ��]\n");
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventEnding2(void)
{
	u8 bgmNo = ScriptGetMapBgm(*Script.pEventCur++);

	TRACEOUT("[�G���f�B���OBGM�ݒ� & �N��:%02x %d]\n", bgmNo);

	SioriSavePrev();

	TextSetClearNoBuf();
	BgmPlay(bgmNo, TRUE);
	AnimeSetType(ANIME_TYPE_ENDING, TRUE);

	ScriptSetAct(SCRIPT_ACT_ANIME);
	Script.isLoop = FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecEventEnding3(void)
{
	// 1Byte�� �j�����܂�
	Script.pEventCur++;

	TRACEOUT("[�G���f�B���O�ԍ��ݒ�]\n");
	/*
	 0 12 ���Ǝ�
	 1 12 ���� BAD
	 2 12 �j��
	 3 12 �g�[�X�^�[
	 4 11 ���D HAPPY
	 5 12 ���D BAD
	 6 11 ���� HAPPY
	 7 12 ���� BAD
	 8 10 �ڗ��q True
	 9 11 �ڗ��q HAPPY
	 a 01 ��c����
	 b 14 �َ���
	 c 12 �َ��� BAD
	*/

	// �ڗ��q True �����Ă��邩�`�F�b�N�����܂�
	if(ScriptGetFlag(0x08) == 1)
	{
		ScriptSetFlag(0, 3);
	}
	else
	{
		if(ScriptGetFlag(0) == 0)
		{
			ScriptSetFlag(0, 2);
		}
		else
		{
			ScriptSetFlag(0, 1);
		}
	}
}
