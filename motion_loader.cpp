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
void _Read_SCRIPT(FILE* pFile, MOTION* pBuffer);

void _Read_CHARACTERSET(FILE* pFile, MOTION* pBuffer);
void _Read_PARTSSET(FILE* pFile, PART* pBuffer);

void _Read_MOTIONSET(FILE* pFile, MOTION_INFO* pBuffer);
void _Read_KEYSET(FILE* pFile, KEY_INFO* pBuffer);
void _Read_KEY(FILE* pFile, KEY* pBuffer);


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************


//=====================================================================
// スクリプト読み込み処理
//=====================================================================
void LoadMotionScript(const char* pFileName, MOTION* pBuffer)
{
	FILE* pFile = fopen(pFileName, "r");
	
	if (pFile != NULL)
	{
		char aStrLine[MAX_READABLE_CHAR] = {};
		char* pStr;

		while (true)
		{
			// 一行読み込む
			if (ReadWord(pFile, &aStrLine[0]) == EOF)
			{// ファイルの最後まで読み込んだら終了する
				break;
			}

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

void _Read_SCRIPT(FILE* pFile, MOTION* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};
	int nCountMotion = 0;
	int nCountPart = 0;

	while (true)
	{
		// 一行読み込む
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}
		
		if (strcmp(&aStrLine[0], "END_SCRIPT") == 0)
		{// スクリプトの終了が宣言されたら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "NUM_MODEL") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->nNumPart);
		}

		if (strcmp(&aStrLine[0], "MODEL_FILENAME") == 0)
		{
			if (nCountPart < pBuffer->nNumPart)
			{
				fscanf(pFile, " = %s", &pBuffer->aPartFilename[nCountPart][0]);
				nCountPart++;
			}
		}

		if (strcmp(&aStrLine[0], "CHARACTERSET") == 0)
		{// モーション読み込み
			_Read_CHARACTERSET(pFile, pBuffer);
		}
		if (strcmp(&aStrLine[0], "MOTIONSET") == 0)
		{// モーション読み込み
			_Read_MOTIONSET(pFile, &pBuffer->aMotionInfo[nCountMotion]);
			nCountMotion++;
		}
	}
}

void _Read_CHARACTERSET(FILE* pFile, MOTION* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};
	int nCountPart = 0;

	while (true)
	{
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "END_CHARACTERSET") == 0)
		{
			break;
		}

		if (strcmp(&aStrLine[0], "NUM_PARTS") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->nNumPart);
		}
		if (strcmp(&aStrLine[0], "PARTSSET") == 0)
		{
			_Read_PARTSSET(pFile, &pBuffer->aPart[nCountPart]);
			nCountPart++;
		}

	}
}

void _Read_PARTSSET(FILE* pFile, PART* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	while (true)
	{
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "END_PARTSSET") == 0)
		{
			break;
		}

		if (strcmp(&aStrLine[0], "INDEX") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->nIdxPart);
		}
		if (strcmp(&aStrLine[0], "PARENT") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->nIdxModelParent);
		}
		if (strcmp(&aStrLine[0], "POS") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->offset.pos.x, &pBuffer->offset.pos.y, &pBuffer->offset.pos.z);
		}
		if (strcmp(&aStrLine[0], "ROT") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->offset.rot.x, &pBuffer->offset.rot.y, &pBuffer->offset.rot.z);
		}
	}
}

void _Read_MOTIONSET(FILE* pFile, MOTION_INFO* pBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};
	int nCountKeyInfo = 0;
	
	while (true)
	{
		// 一行読み込む
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_MOTIONSET") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "LOOP") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->bLoop);
		}
		else if (strcmp(&aStrLine[0], "NUM_KEY") == 0)
		{
			fscanf(pFile, " = %d", &pBuffer->nNumKey);
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
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
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
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "END_KEY") == 0)
		{
			break;
		}
		else if (strcmp(&aStrLine[0], "POS") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->pos.x, &pBuffer->pos.y, &pBuffer->pos.z);
		}
		else if (strcmp(&aStrLine[0], "ROT") == 0)
		{
			fscanf(pFile, " = %f %f %f", &pBuffer->rot.x, &pBuffer->rot.y, &pBuffer->rot.z);
		}
	}
}