//=====================================================================
//
// Motion_Loader [Motion_Loader.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "motion_loader.h"
#include "script.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_READABLE_CHAR	(256)

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void _Read_SCRIPT(FILE* pFile, MOTION_INFO* pBuffer);
void _Read_MOTIONSET(FILE* pFile, MOTION_INFO* pBuffer);
void _Read_KEYSET(FILE* pFile, KEY_INFO* pBuffer);
void _Read_KEY(FILE* pFile, KEY* pBuffer);


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************



/*	TODO
sscanf()を使った読み込み処理
	→「#」（コメント）の検知

*/


//=====================================================================
// スクリプト読み込み処理
//=====================================================================
void LoadMotionScript(const char* pFileName, MOTION_INFO* pBuffer)
{
	FILE* pFile = fopen(pFileName, "r");
	
	if (pFile != NULL)
	{
		char aStrLine[MAX_READABLE_CHAR] = {};

		while (true)
		{
			// 一行読み込む
			if (ReadLine(pFile, &aStrLine[0]))
			{// ファイルの最後まで読み込んだら終了する
				break;
			}
			sscanf(&aStrLine[0], "%s", &aStrLine[0]);

			if (strcmp(&aStrLine[0], "SCRIPT") == 0)
			{
				OutputDebugString("[motion_loader.cpp] Script Start\n");
				_Read_SCRIPT(pFile, pBuffer);
			}
		}

		OutputDebugString("[motion_loader.cpp] Script End\n");

	}
	else
	{
		OutputDebugString("[motion_loader.cpp] Script Failed\n");
	}
}

void _Read_SCRIPT(FILE* pFile, MOTION_INFO* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	while (true)
	{
		// 一行読み込む
		if (ReadLine(pFile, &aStrLine[0]))
		{// ファイルの最後まで読み込んだら終了する
			break;
		}
		sscanf(&aStrLine[0], "%s", &aStrLine[0]);
		
		if (strcmp(&aStrLine[0], "END_SCRIPT") == 0)
		{// スクリプトの終了が宣言されたら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "MOTIONSET") == 0)
		{// モーション読み込み
			_Read_MOTIONSET(pFile, pBuffer);
			pBuffer++;
		}
	}
}

void _Read_MOTIONSET(FILE* pFile, MOTION_INFO* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};
	char aStrWord[MAX_READABLE_CHAR] = {};
	int nCountKeyInfo = 0;
	
	while (true)
	{
		// 一行読み込む
		if (ReadLine(pFile, &aStrLine[0]))
		{// ファイルの最後まで読み込んだら終了する
			break;
		}
		sscanf(&aStrLine[0], "%s", &aStrLine[0]);

		if (strcmp(&aStrLine[0], "END_MOTIONSET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "LOOP") == 0)
		{
			int nFrame;
			sscanf(&aStrLine[0], "LOOP = %d", &nFrame);
			pBuffer->bLoop = (bool)nFrame;
		}
		else if (strcmp(&aStrLine[0], "NUM_KEY") == 0)
		{
			sscanf(&aStrLine[0], "NUM_KEY = %d", &pBuffer->nNumKey);
		}
		else if (strcmp(&aStrLine[0], "KEYSET") == 0)
		{
			_Read_KEYSET(pFile, &pBuffer->aKeyInfo[nCountKeyInfo]);
			nCountKeyInfo++;
		}
	}
}

void _Read_KEYSET(FILE* pFile, KEY_INFO* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};
	int nCountKey = 0;
	
	while (true)
	{
		// 一行読み込む
		if (ReadLine(pFile, &aStrLine[0]))
		{// ファイルの最後まで読み込んだら終了する
			break;
		}
		sscanf(&aStrLine[0], "%s", &aStrLine[0]);

		if (strcmp(&aStrLine[0], "END_KEYSET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "FRAME") == 0)
		{
			sscanf(&aStrLine[0], "FRAME = %d", &pBuffer->nFrame);
		}
		else if (strcmp(&aStrLine[0], "KEY") == 0)
		{
			_Read_KEY(pFile, &pBuffer->aKey[nCountKey]);
			nCountKey++;
		}
	} 
}

void _Read_KEY(FILE* pFile, KEY* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	while (true)
	{
		// 一行読み込む
		if (ReadLine(pFile, &aStrLine[0]))
		{// ファイルの最後まで読み込んだら終了する
			break;
		}
		sscanf(&aStrLine[0], "%s", &aStrLine[0]);

		if (strcmp(&aStrLine[0], "END_KEY") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "POS") == 0)
		{
			sscanf(&aStrLine[0], "POS = %f %f %f", &pBuffer->fPosX, &pBuffer->fPosY, &pBuffer->fPosZ);
		}
		else if (strcmp(&aStrLine[0], "ROT") == 0)
		{
			float fRotX, fRotY, fRotZ;
			sscanf(&aStrLine[0], "POS = %f %f %f", &fRotX, &fRotY, &fRotZ);

			pBuffer->fRotX = D3DXToRadian(fRotX);
			pBuffer->fRotY = D3DXToRadian(fRotY);
			pBuffer->fRotZ = D3DXToRadian(fRotZ);
		}
	}
}