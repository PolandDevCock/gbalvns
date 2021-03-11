#include "script.h"
#include "script2.h"
#include "script3.h"
#include "history.h"
#include "libmy/gbfs.h"
#include "libmy/key.h"
#include "img.h"
#include "anime.h"
#include "menu.h"
#include "siori.h"


//---------------------------------------------------------------------------
ST_SCRIPT Script;


//---------------------------------------------------------------------------
EWRAM_CODE void ScriptInit(void)
{
	_Memset(&Script, 0x00, sizeof(ST_SCRIPT));
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptInitFlag(void)
{
	_Memset(&Script.flag, 0x00, sizeof(Script.flag));

	// �Z�[�u�f�[�^������ꍇ�A�ꕔ�̃t���O�iScript.flag�j���㏑�����܂�
	SioriLoadFlag();
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExec(void)
{
	if(ImgIsEnd() == FALSE)
	{
		return;
	}

	if(TextIsEnd() == FALSE)
	{
		return;
	}


	switch(Script.act)
	{
	case SCRIPT_ACT_EVENT:
		ScriptExecEvent();
		break;

	case SCRIPT_ACT_MSG:
		ScriptExecMsg();
		break;

	case SCRIPT_ACT_ANIME:
		ScriptExecAnime();
		break;

	case SCRIPT_ACT_WAIT_PAGE:
		ScriptExecWaitPage();
		break;

	case SCRIPT_ACT_WAIT_KEY:
		ScriptExecWaitKey();
		break;

	case SCRIPT_ACT_HISTORY:
		ScriptExecHistory();
		break;

	case SCRIPT_ACT_SELECT:
		ScriptExecSelect();
		break;

	case SCRIPT_ACT_MENU:
		ScriptExecMenu();
		break;

	default:
		SystemError("[Err] ScriptExec Script.act = %d\n", Script.act);
		break;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecAnime(void)
{
	AnimeExec();

	if(AnimeIsEnd() == TRUE)
	{
		ScriptSetAct(SCRIPT_ACT_EVENT);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecWaitPage(void)
{
	if(ScriptCalcKey() == TRUE)
	{
		TextSetNewPage();

		ScriptSetAct(SCRIPT_ACT_MSG);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecWaitKey(void)
{
	if(ScriptCalcKey() == TRUE)
	{
		ScriptSetAct(SCRIPT_ACT_MSG);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecHistory(void)
{
	switch(Script.actStep)
	{
	case 0:
		HistoryInitAct();
		Script.actStep++;
		break;

	case 1:
		HistoryExec();

		if(HistoryIsEnd() == TRUE)
		{
			TextSetRestore();
			ScriptPopAct();
		}
		break;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecMenu(void)
{
	switch(Script.actStep)
	{
	case 0:
		MenuSetSystem();
		Script.actStep++;
		break;

	case 1:
		MenuExec();

		if(MenuIsEnd() == TRUE)
		{
			ScriptPopAct();
		}
		break;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecSelect(void)
{
	u16 trg = KeyGetTrg();
	s16 i;

	switch(Script.actStep)
	{
	case 0:
		// �₢�ƁA���ڐ����擾���܂�
		Script.selMsg = *Script.pEventCur++;
		Script.selCnt = *Script.pEventCur++;
		Script.selNum = -1;

		for(i=0; i<Script.selCnt; i++)
		{
			Script.selItem[i] = *Script.pEventCur++;
			Script.selJump[i] = *Script.pEventCur++;
		}

		TRACEOUT("[�I�����̐�] %d\n", Script.selCnt);
		Script.actStep++;
		break;

	case 1:
		// �₢��\�����܂�
		ScriptExecSelectSub(Script.selMsg, FALSE, FALSE);
		ImgSetEffectUpdate(IMG_EFFECT_TEXT);

		Script.actStep++;
		break;

	case 2:
		// ���͑҂�
		if(ScriptCalcKey() == TRUE)
		{
			TextSetNewPage();
			Script.actStep++;
		}
		break;

	case 3:
		// ���ڂ�\�������܂�
		TextSetClearXY();

		for(i=0; i<Script.selCnt; i++)
		{
			if(i == Script.selNum)
			{
				TextSetImgWhite();
			}
			else
			{
				TextSetImgGray();
			}

			ScriptExecSelectSub(Script.selItem[i], TRUE, FALSE);
			TextSetLf();
		}
		TextSetImgWhite();
		Script.actStep++;
		break;

	case 4:
		// �I��
		if((trg & KEY_A) && Script.selNum != -1)
		{
			Script.actStep++;
		}
		else if((trg & KEY_LEFT) && (HistoryIsEmpty() == FALSE))
		{
			Script.actStep = 3;
			ScriptPushAct(SCRIPT_ACT_HISTORY);
		}
		else if(trg & KEY_B)
		{
			Script.actStep = 3;
			ScriptPushAct(SCRIPT_ACT_MENU);
		}
		else if((trg & KEY_UP) && Script.selNum > 0)
		{
			Script.actStep = 3;
			Script.selNum--;
		}
		else if((trg & KEY_DOWN) && (Script.selNum + 1) < Script.selCnt)
		{
			Script.actStep = 3;
			Script.selNum++;
		}
		break;

	case 5:
		// �I���I��
		TextSetClearXY();
		ScriptExecSelectSub(Script.selItem[Script.selNum], TRUE, TRUE);
		TextSetClear();

		Script.isNext = FALSE;
		Script.pEventCur += Script.selJump[Script.selNum];

		ScriptSetAct(SCRIPT_ACT_EVENT);
		break;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptExecSelectSub(u8 msgNo, bool isSel, bool isBuf)
{
	u8* pMsgCur = Script.pMsg + *((u16*)Script.pMsg + (msgNo + 1));

	u16 buf[512] ALIGN(4);
	u16 cnt = 0;

	for(;;)
	{
		// TODO �X�N���v�g�̏����𖳎����Ă��܂��B�{���͂悭�Ȃ�

		if(*pMsgCur & 0x80)
		{
			break;
		}

		pMsgCur++;
	}

	do
	{
		if(*pMsgCur == 'r')
		{
			pMsgCur++;
			continue;
		}

		buf[cnt++] = ((*pMsgCur & 0x7f) << 8) | (*(pMsgCur + 1));
		pMsgCur += 2;

	} while((*pMsgCur & 0x80) || (*pMsgCur == 'r'));

	_ASSERT(cnt < 512 && "ScriptExecSelectSub buf overflow");


	if(isSel == TRUE)
	{
		TextSetBufWorkNoWait(buf, cnt, isBuf);
	}
	else
	{
		TextSetBufWork(buf, cnt);
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptSetAct(u16 act)
{
	Script.act = act;
	Script.actStep = 0;

	Script.actTmp = 0;
	Script.actStepTmp = 0;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptPushAct(u16 act)
{
	_ASSERT(Script.actTmp == 0);
	_ASSERT(Script.actStepTmp == 0);

	Script.actTmp = Script.act;
	Script.actStepTmp = Script.actStep;

	Script.act = act;
	Script.actStep = 0;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptPopAct(void)
{
	_ASSERT(Script.actTmp != 0);

	Script.act = Script.actTmp;
	Script.actStep = Script.actStepTmp;

	Script.actTmp = 0;
	Script.actStepTmp = 0;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptSetNext(bool is)
{
	Script.isNext = is;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptSetScn(u16 scnNo)
{
	char buf[64];
	_Sprintf(buf, SCRIPT_SCN_STR, scnNo);

	ST_SCRIPT_HEADER* p = (ST_SCRIPT_HEADER*)GbfsGetSafePointer(buf);

	Script.scnNo     = scnNo;
	Script.pEvent    = (u8*)p + p->eventOffset;
	Script.pMsg      = (u8*)p + p->msgOffset;
	Script.eventSize = p->eventSize;
	Script.msgSize   = p->msgSize;
	Script.pEventCur = NULL;
	Script.pMsgCur   = NULL;

	TRACEOUT("[�X�N���v�g���[�h: no.%d %x %x]\n", scnNo, Script.pEvent, Script.pMsg);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptSetEvent(u16 eventNo)
{
	_ASSERT(Script.pEvent != NULL);

	Script.pEventCur = Script.pEvent + *((u16*)Script.pEvent + (eventNo + 1));
	ScriptSetAct(SCRIPT_ACT_EVENT);

	TRACEOUT("[�C�x���g���[�h: no.%d %x]\n", eventNo, Script.pEventCur);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptSetMsg(u16 msgNo)
{
	_ASSERT(Script.pMsg != NULL);

	Script.pMsgCur = Script.pMsg + *((u16*)Script.pMsg + (msgNo + 1));
	ScriptSetAct(SCRIPT_ACT_MSG);

	TRACEOUT("[���b�Z�[�W���[�h: no.%d %x]\n", msgNo, Script.pMsg);
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptSetFlag(u8 no, u8 val)
{
	_ASSERT(no < SCRIPT_MAX_SYS_FLAG_CNT);
	Script.flag[no] = val;
}
//---------------------------------------------------------------------------
EWRAM_CODE void ScriptAddFlag(u8 no, u8 val)
{
	_ASSERT(no < SCRIPT_MAX_SYS_FLAG_CNT);
	Script.flag[no] += val;
}
//---------------------------------------------------------------------------
EWRAM_CODE u8 ScriptGetFlag(u8 no)
{
	_ASSERT(no < SCRIPT_MAX_SYS_FLAG_CNT);
	return Script.flag[no];
}
//---------------------------------------------------------------------------
// �V�i���I�t���O�̕ϊ�
EWRAM_CODE u8 ScriptGetMapFlag(u8 no)
{
	switch(no)
	{
	case 0:			// �G���f�B���O��ԋL�^(�񏉊����Ώ�)
		return 0;

	case 1:			// �G�t���O
		return 1;

	case 0x40:		// �ڗ��q�Ɩ�
	case 0x41:		// �������l�e
	case 0x42:		// �������ŕς��������
	case 0x43:		// �̈�قŁc
	case 0x44:		// �ڗ��q����u�͂��Ȃ������v
	case 0x45:		// �ڗ��q TRUE  (�񏉊����Ώ�)
	case 0x46:		// �ڗ��q HAPPY (�񏉊����Ώ�)
	case 0x47:		// ���D���[�gON
	case 0x48:		// ���D H�V�[��
	case 0x49:		// �����炭���g�p
	case 0x4a:		// ����ړ��ꏊ����p
	case 0x4b:		// �I���S�[������܂�
		return (no - 0x40) + 2;
	}

	SystemError("[Err] ScriptGetMapFlag Bad Flag No[%02x]", no);
	return 0;
}
//---------------------------------------------------------------------------
// �w�i�摜 �p�����[�^�ϊ�
EWRAM_CODE u8 ScriptGetMapBg(u8 no)
{
	switch(no)
	{
	case 4:
		return 2; // ����(�[��)
	case 5:
		return 2; // ����(�[��)
	case 6:
		return 3; // �x�ݎ���(�[��)
	case 32:
		return 31; // ����(�[��)
	case 33:
		return 31; // ����(��)
	case 35:
		return 34; // ����(��)�[��
	case 36:
		return 34; // ����(��)��
	case 38:
		return 11; // �̈�� (�[��)
	case 41:
		return 15; // ���� (�[��)
	case 42:
		return 15; // ���� (��)
	case 43:
		return 10; // �낤��(�[��)
	case 44:
		return 10; // �낤��(�[��)
	case 45:
		return 30; // �E����(�[��)
	case 46:
		return 30; // �E����(�[��)
	case 47:
		return 22; // �K�i (�[��)
	case 48:
		return 22; // �K�i�i�[��)
	case 49:
		return 12; // ���k��L��(�[��)
	case 50:
		return 12; // ���k��L��(�[��)
	case 51:
		return 24; // �̈�ق̒�(�[��)
	case 52:
		return 24; // �̈�ق̒�(��)
	case 53:
		return 12; // �����L��(�^����)
	case 54:
		return 18; // �S�̂Ƃт��(�[��)
	case 55:
		return 19; // �S�̂Ƃт�J(�[��)
	case 56:
		return 16;
	}

	return no;
}
//---------------------------------------------------------------------------
EWRAM_CODE u8 ScriptGetMapBg2(u8 c1, u8 c2)
{
	u8 no = ScriptGetMapDig(c1, c2);

	return ScriptGetMapBg(no);
}
//---------------------------------------------------------------------------
// �r�W���A���p�p�����[�^�ϊ�
EWRAM_CODE u8 ScriptGetMapVisual(u8 no)
{
	switch(no)
	{
	case 21:
		return 2;
	case 28:
		return 4;
	}

	return no;
}
//---------------------------------------------------------------------------
EWRAM_CODE u8 ScriptGetMapVisual2(u8 c1, u8 c2)
{
	u8 no = ScriptGetMapDig(c1, c2);

	return ScriptGetMapVisual(no);
}
//---------------------------------------------------------------------------
// �G�t�F�N�g�p�����[�^�̕ϊ�
EWRAM_CODE u8 ScriptGetMapEffect(u8 c1, u8 c2)
{
	u8 no  = ScriptGetMapDig(c1, c2);
	u8 ret = no;

	if(no == 99)
	{
		ret = IMG_EFFECT_NORMAL;
	}

	return ret;
}
//---------------------------------------------------------------------------
// BGM�p�����[�^�̕ϊ�
EWRAM_CODE u8 ScriptGetMapBgm(u8 no)
{
	if(no == 14)
	{
		return 2;
	}
	else if(no < 16)
	{
		return no + 2;
	}

	return no + 1;
}
//---------------------------------------------------------------------------
EWRAM_CODE u8 ScriptGetMapChrPos(u8 pos)
{
	switch (pos)
	{
	default:
	case 'l':
	case 'L':
		return IMG_CHR_TYPE_LEFT;

	case 'r':
	case 'R':
		return IMG_CHR_TYPE_RIGHT;

	case 'c':
	case 'C':
	case '0':
		return IMG_CHR_TYPE_MID;

	case 'a':
	case 'A':
		return IMG_CHR_TYPE_ALL;
	}
}
//---------------------------------------------------------------------------
EWRAM_CODE u8 ScriptGetMapDig(u8 c1, u8 c2)
{
	u8 ret = (c1 - '0') * 10 + (c2 - '0');

	return ret;
}
//---------------------------------------------------------------------------
EWRAM_CODE u8 ScriptGetMapTolower(u8 c)
{
	u8 ret;

	if(c >= 'a' && c <= 'z')
	{
		// �啶���ɕϊ�
		ret = c - ('a' - 'A');
	}
	else
	{
		ret = c;
	}

	return ret;
}
//---------------------------------------------------------------------------
EWRAM_CODE u8 ScriptGetMapHexToDig(u8 c)
{
	u8 ret;
	u8 c2 = ScriptGetMapTolower(c);

	if(c2 >= 'A' && c2 <= 'F')
	{
		ret = (c2 - 'A') + 10;
	}
	else
	{
		ret = c2 - '0';
	}

	return ret;
}
//---------------------------------------------------------------------------
EWRAM_CODE u8 ScriptGetMapHexToDig2(u8 c1, u8 c2)
{
	u8 ret = (ScriptGetMapHexToDig(c1) * 16) + ScriptGetMapHexToDig(c2);

	return ret;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool ScriptCalcKey()
{
	TextSetDispCur();


	u16 cnt = KeyGetCnt();
	u16 trg = KeyGetTrg();

	//�u���̑I�����܂Ői�ށv�t���O�̃`�F�b�N�����܂�
	if(Script.isNext == TRUE)
	{
		if(cnt & KEY_B)
		{
			Script.isNext = FALSE;
		}
		else
		{
			TextSetUnDispCur();
			return TRUE;
		}
	}

	if(cnt & KEY_R || trg & KEY_A || trg & KEY_DOWN)
	{
		TextSetUnDispCur();

		return TRUE;
	}
	else if((trg & KEY_LEFT) && (HistoryIsEmpty() == FALSE))
	{
		TextSetUnDispCur();
		ScriptPushAct(SCRIPT_ACT_HISTORY);

		return FALSE;
	}
	else if(trg & KEY_B)
	{
		TextSetUnDispCur();
		ScriptPushAct(SCRIPT_ACT_MENU);

		return FALSE;
	}

	return FALSE;
}
//---------------------------------------------------------------------------
EWRAM_CODE bool ScriptIsEnd(void)
{
	return (Script.act == SCRIPT_ACT_END) ? TRUE : FALSE;
}
