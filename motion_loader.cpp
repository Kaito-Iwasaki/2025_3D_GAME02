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
void _Read_SCRIPT(FILE* pFile, MOTION_INFO** ppBuffer);
void _Read_MOTIONSET(FILE* pFile, MOTION_INFO** ppBuffer);
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
			if (fscanf(pFile, "%s", &aStrLine[0]) == EOF)
			{
				break;
			}

			if (strcmp(&aStrLine[0], "SCRIPT") == 0)
			{
				OutputDebugString("[motion_loader.cpp] Script Start\n");
				_Read_SCRIPT(pFile, &pBuffer);
			}

		}

		OutputDebugString("[motion_loader.cpp] Script End\n");

	}
	else
	{
		OutputDebugString("[motion_loader.cpp] Script Failed\n");
	}
}

void _Read_SCRIPT(FILE* pFile, MOTION_INFO** ppBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};
	MOTION_INFO* pData = *ppBuffer;

	while (true)
	{
		// 一行読み込む
		if (fscanf(pFile, "%s", &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}
		else if (strcmp(&aStrLine[0], "END_SCRIPT") == 0)
		{// スクリプトの終了が宣言されたら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "MOTIONSET") == 0)
		{// モーション読み込み
			_Read_MOTIONSET(pFile, &pData);
			pData++;
		}
	}
}

void _Read_MOTIONSET(FILE* pFile, MOTION_INFO** ppBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};
	MOTION_INFO* pData = *ppBuffer;
	KEY_INFO* pKeyInfo = &pData->aKeyInfo[0];
	
	while (true)
	{
		// 一行読み込む
		if (fscanf(pFile, "%s", &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_MOTIONSET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "LOOP") == 0)
		{
			fscanf(pFile, " = %d", &pData->bLoop);
		}
		else if (strcmp(&aStrLine[0], "NUM_KEY") == 0)
		{
			fscanf(pFile, " = %d", &pData->nNumKey);
		}
		else if (strcmp(&aStrLine[0], "KEYSET") == 0)
		{
			_Read_KEYSET(pFile, pKeyInfo);
			pKeyInfo++;
		}
	}
}

void _Read_KEYSET(FILE* pFile, KEY_INFO* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};
	KEY* pKey = &pBuffer->aKey[0];
	
	while (true)
	{
		// 一行読み込む
		if (fscanf(pFile, "%s", &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_KEYSET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "FRAME") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->nFrame);
		}
		else if (strcmp(&aStrLine[0], "KEY") == 0)
		{
			_Read_KEY(pFile, pKey);
			pKey++;
		}
	} 
}

void _Read_KEY(FILE* pFile, KEY* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	while (true)
	{
		// 一行読み込む
		if (fscanf(pFile, "%s", &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_KEY") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "POS") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->fPosX, &pBuffer->fPosY, &pBuffer->fPosZ);
		}
		else if (strcmp(&aStrLine[0], "ROT") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->fRotX, &pBuffer->fRotY, &pBuffer->fRotZ);
		}
	}
}