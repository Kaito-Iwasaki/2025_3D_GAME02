//=====================================================================
//
// model_loader [model_loader.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "model_loader.h"
#include "script.h"
#include <cassert>
#include "polygon.h"

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
			// 一行読み込む
			if (ReadWord(pFile, &aStrLine[0]) == EOF)
			{// ファイルの最後まで読み込んだら終了する
				break;
			}

			if (strcmp(&aStrLine[0], "SCRIPT") == 0)
			{
				OutputDebugString("[model_loader.cpp] Script Start\n");
				_Read_SCRIPT(pFile, &pBuffer);
			}
			
		}

		OutputDebugString("[model_loader.cpp] Script End\n");
		
	}
	else
	{
		OutputDebugString("[model_loader.cpp] Script Failed\n");
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
		if (ReadWord(pFile, &aStrLine[0]) == EOF)
		{// ファイルの最後まで読み込んだら終了する
			break;
		}
		else if (strcmp(&aStrLine[0], "END_SCRIPT") == 0)
		{// スクリプトの終了が宣言されたら終了する
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
				if (ReadWord(pFile, &aStrLine[0]) == EOF)
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
					assert(pData->nType < (*ppBuffer)->nNumModel);
				}
				else if (strcmp(&aStrLine[0], "POS") == 0)
				{
					fscanf(pFile, " = %f %f %f", &pData->pos.x, &pData->pos.y, &pData->pos.z);
				}
				else if (strcmp(&aStrLine[0], "ROT") == 0)
				{
					fscanf(pFile, " = %f %f %f", &pData->rot.x, &pData->rot.y, &pData->rot.z);
				}
				else if (strcmp(&aStrLine[0], "COLLISION") == 0)
				{
					fscanf(pFile, " = %d", &pData->collisionType);
				}
			}

			(*ppBuffer)->nCountModelSet++;
		}
		else if (strcmp(&aStrLine[0], "COINSET") == 0)
		{
			COINSETDATA* pData = &(*ppBuffer)->aInfoCoinSet[(*ppBuffer)->nCountCoinSet];

			while (true)
			{
				// 一行読み込む
				if (ReadWord(pFile, &aStrLine[0]) == EOF)
				{// ファイルの最後まで読み込んだら終了する
					break;
				}

				if (strcmp(&aStrLine[0], "END_COINSET") == 0)
				{
					break;
				}
				else if (strcmp(&aStrLine[0], "POS") == 0)
				{
					fscanf(pFile, " = %f %f %f", &pData->pos.x, &pData->pos.y, &pData->pos.z);
				}

				(*ppBuffer)->nCountCoinSet++;
			}

			SetCoin(pData->pos + D3DXVECTOR3(0, 50, 0));
		}
		else if (strcmp(&aStrLine[0], "POLYGONSET") == 0)
		{
			POLYGONSETDATA data;
			ZeroMemory(&data, sizeof(POLYGONSETDATA));
			data.col = D3DXCOLOR_WHITE;

			while (true)
			{
				// 一行読み込む
				if (ReadWord(pFile, &aStrLine[0]) == EOF)
				{// ファイルの最後まで読み込んだら終了する
					break;
				}

				if (strcmp(&aStrLine[0], "END_POLYGONSET") == 0)
				{
					break;
				}
				else if (strcmp(&aStrLine[0], "POS") == 0)
				{
					fscanf(pFile, " = %f %f %f", &data.pos.x, &data.pos.y, &data.pos.z);
				}
				else if (strcmp(&aStrLine[0], "SIZE") == 0)
				{
					fscanf(pFile, " = %f %f %f", &data.size.x, &data.size.y, &data.size.z);
				}
				else if (strcmp(&aStrLine[0], "ROT") == 0)
				{
					float fRotX, fRotY, fRotZ;

					fscanf(pFile, " = %f %f %f", &fRotX, &fRotY, &fRotZ);

					data.rot.x = D3DXToRadian(fRotX);
					data.rot.y = D3DXToRadian(fRotY);
					data.rot.z = D3DXToRadian(fRotZ);
				}
				else if (strcmp(&aStrLine[0], "COLOR") == 0)
				{
					fscanf(pFile, " = %f %f %f", &data.col.r, &data.col.g, &data.col.b);
				}
			}

			SetPolygon(data.pos, data.size, data.rot, data.col);
		}
	}
}