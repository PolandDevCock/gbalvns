#include "ad_arm.h"
#include "gbfs.h"

/* playad.iwram.c
   8AD decoder engine

Copyright 2003 Damian Yerrick

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*/
//---------------------------------------------------------------------------
const s8 AdIndiceTable[16] = {
	-1, -1, -1, -1, 2, 4, 7, 12,
	-1, -1, -1, -1, 2, 4, 7, 12,
};

const u16 AdStepTable[89] = {
	      7,    8,    9,   10,   11,   12,   13,   14,   16,   17,
	     19,   21,   23,   25,   28,   31,   34,   37,   41,   45,
	     50,   55,   60,   66,   73,   80,   88,   97,  107,  118,
	    130,  143,  157,  173,  190,  209,  230,  253,  279,  307,
	    337,  371,  408,  449,  494,  544,  598,  658,  724,  796,
	    876,  963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
	   2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
	   5894, 6484, 7132, 7845, 8630, 9493,10442,11487,12635,13899,
	  15289,16818,18500,20350,22385,24623,27086,29794,32767,
};


//---------------------------------------------------------------------------
ST_AD Ad;


//---------------------------------------------------------------------------
EWRAM_CODE void AdInit(void)
{
	_Memset(&Ad, 0x00, sizeof(ST_AD));


	REG_SOUNDCNT_X = SNDSTAT_ENABLE;
	REG_SOUNDCNT_L = 0;
	REG_SOUNDCNT_H = SNDA_VOL_100 | SNDA_TIMER0 | SNDA_R_ENABLE | SNDA_L_ENABLE;

	REG_TM0CNT_L   = 0x10000 - AD_SAMPLE_TIME;
	REG_TM0CNT_H   = TIMER_FREQ_PER_1 | TIMER_START;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AdSetData(u8* pData, u32 size, bool isLoop)
{
	Ad.act    = AD_ACT_STOP;
	Ad.isLoop = isLoop;
	Ad.pCur   = pData;
	Ad.pTop   = pData;
	Ad.pEnd   = pData + size;

	Ad.lastSample = 0;
	Ad.lastIdx    = 0;
	Ad.bufIdx     = 0;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AdSetData2(char* pStr, bool isLoop)
{
	u8* pData = GbfsGetSafePointer(pStr);
	u32 size  = GbfsGetFileSize();

	AdSetData(pData, size, isLoop);
}
//---------------------------------------------------------------------------
EWRAM_CODE void AdReset(void)
{
	Ad.pCur = Ad.pTop;

	Ad.lastSample = 0;
	Ad.lastIdx    = 0;
	Ad.bufIdx     = 0;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AdPlay(void)
{
	Ad.act = AD_ACT_READY;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AdStop(void)
{
	REG_DMA1CNT = 0;

	Ad.act = AD_ACT_STOP;
}
//---------------------------------------------------------------------------
EWRAM_CODE void AdStopOut(void)
{
	// TODO �t�F�[�h�A�E�g
	AdStop();
}
//---------------------------------------------------------------------------
IWRAM_CODE void AdIntrVcount(void)
{
	if(Ad.act != AD_ACT_READY && Ad.act != AD_ACT_PLAY)
	{
		return;
	}
	Ad.act = AD_ACT_PLAY;

	u8* pSrc = Ad.pCur;
	s8* pDst = Ad.buf[Ad.bufIdx];
	u32 len  = AD_BUF_SIZE;
	u32 by   = 0;

	Ad.pCur += AD_BUF_SIZE >> 1;

	s32 step;
	s32 diff;
	u32 code;

	while(len > 0)
	{
		if(Ad.lastIdx < 0)
		{
			Ad.lastIdx = 0;
		}
		if(Ad.lastIdx > 88)
		{
			Ad.lastIdx = 88;
		}
		step = AdStepTable[Ad.lastIdx];

		if(len & 1)
		{
			code = by >> 4;
		}
		else
		{
			by   = *pSrc++;
			code = by & 0x0f;
		}

		/* 0,1,2,3,4,5,6,9 */
		diff = step >> 3;
		if(code & 1)
		{
			diff += step >> 2;
		}
		if(code & 2)
		{
			diff += step >> 1;
		}
		if(code & 4)
		{
			diff += step;
		}
		if((code & 7) == 7)
		{
			diff += step >> 1;
		}
		if(code & 8)
		{
			diff = -diff;
		}

		Ad.lastIdx += AdIndiceTable[code & 0x07];

		Ad.lastSample += diff;
		if(Ad.lastSample < -32768)
		{
			Ad.lastSample = -32768;
		}
		if(Ad.lastSample > 32767)
		{
			Ad.lastSample = 32767;
		}
		*pDst++ = Ad.lastSample >> 8;

		len--;
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void AdIntrVblank(void)
{
	if(Ad.act == AD_ACT_PLAY)
	{
		AdIntrNextBuf();

		if(Ad.pCur >= Ad.pEnd)
		{
			if(Ad.isLoop == TRUE)
			{
				AdReset();
			}
			else
			{
				AdStop();
			}
		}
	}
}
//---------------------------------------------------------------------------
IWRAM_CODE void AdIntrNextBuf(void)
{
	REG_DMA1CNT = 0;

	REG_DMA1SAD = (u32)Ad.buf[Ad.bufIdx];
	REG_DMA1DAD = (u32)&REG_FIFO_A;
	REG_DMA1CNT = (u32)DMA_SPECIAL | DMA_ENABLE | DMA32 | DMA_REPEAT | DMA_SRC_INC | DMA_DST_FIXED;

	Ad.bufIdx ^= 0x01;
}
