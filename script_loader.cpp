//=====================================================================
//
// script_loader [script_loader.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "script_loader.h"

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
void _Read_SCRIPT(FILE* pFile, SCRIPTDATA** ppBuffer);


//=====================================================================
// スクリプト読み込み処理
//=====================================================================
void LoadScript(const char* pFileName, SCRIPTDATA* pBuffer)
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
				OutputDebugString("[script_loader.cpp] Script Start\n");
				_Read_SCRIPT(pFile, &pBuffer);
			}
			
		}

		OutputDebugString("[script_loader.cpp] Script End\n");
		
	}
	else
	{
		OutputDebugString("[script_loader.cpp] Script Failed\n");
	}
}

void _Read_SCRIPT(FILE* pFile, SCRIPTDATA** ppBuffer)
{
	char aStrLine[MAX_READABLE_CHAR] = {};

	int nTextureCount = 0;
	int nModelCount = 0;

	while (true)
	{
		// 一行読み込む
		if (fscanf(pFile, "%s", &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}

		if (strcmp(&aStrLine[0], "NUM_TEXTURE") == 0)
		{// テクスチャ数読み込み
			fscanf(pFile, " = %d", &(*ppBuffer)->nNumTexture);
		}
		else if (strcmp(&aStrLine[0], "TEXTURE_FILENAME") == 0)
		{// テクスチャファイル名読み込み
			if (nTextureCount < MAX_LOADABLE_TEXTURE)
			{// 最大読み込み数まで読み込む
				fscanf(pFile, " = %s", &(*ppBuffer)->aFilenameTexture[nTextureCount][0]);
				nTextureCount++;
			}
		}
		else if (strcmp(&aStrLine[0], "NUM_MODEL") == 0)
		{// モデル数読み込み
			fscanf(pFile, " = %d", &(*ppBuffer)->nNumModel);
		}
		else if (strcmp(&aStrLine[0], "MODEL_FILENAME") == 0)
		{// テクスチャファイル名読み込み
			if (nModelCount < MAX_LOADABLE_MODEL)
			{// 最大読み込み数まで読み込む
				fscanf(pFile, " = %s", &(*ppBuffer)->aFilenameModel[nModelCount][0]);
				nModelCount++;
			}
		}
		else if (strcmp(&aStrLine[0], "MODELSET") == 0)
		{// モデルセット情報読み込み
			MODELSETDATA* pData = &(*ppBuffer)->aInfoModelSet[(*ppBuffer)->nCountModelSet];

			while (true)
			{
				// 一行読み込む
				if (fscanf(pFile, "%s", &aStrLine[0]) == EOF)
				{// ファイルの最後まで読み込んだら終了する
					break;
				}

				if (strcmp(&aStrLine[0], "END_MODELSET") == 0)
				{
					break;
				}
				else if (strcmp(&aStrLine[0], "TYPE") == 0)
				{
					fscanf(pFile, " = %d", &pData->nType);
				}
				else if (strcmp(&aStrLine[0], "POS") == 0)
				{
					fscanf(pFile, " = %f %f %f", &pData->pos.x, &pData->pos.y, &pData->pos.z);
				}
				else if (strcmp(&aStrLine[0], "ROT") == 0)
				{
					fscanf(pFile, " = %f %f %f", &pData->rot.x, &pData->rot.y, &pData->rot.z);
				}
			}

			(*ppBuffer)->nCountModelSet++;
		}
	}
}