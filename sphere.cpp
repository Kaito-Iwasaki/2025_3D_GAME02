//=====================================================================
//
// Sphere [sphere.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "sphere.h"
#include "input.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\skybox00.jpg"
#define INIT_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(50000.0f, 50000.0f, 50000.0f)
#define INIT_COLOR			D3DXCOLOR_WHITE
#define NUM_BLOCK_X			(8)
#define NUM_BLOCK_Y			(8)

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


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSphere = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffSphere = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSphere = NULL;
SPHERE g_aSphere[MAX_SPHERE];

//=====================================================================
// 初期化処理
//=====================================================================
void InitSphere(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SPHERE* pSphere = &g_aSphere[0];

	ZeroMemory(pSphere, sizeof(SPHERE));

	SetSphere(0, INIT_POS, INIT_SIZE, NUM_BLOCK_X, NUM_BLOCK_Y);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitSphere(void)
{
	SPHERE* pSphere = &g_aSphere[0];

	if (g_pTexBuffSphere != NULL)
	{// テクスチャの破棄
		g_pTexBuffSphere->Release();
		g_pTexBuffSphere = NULL;
	}

	for (int nCntSphere = 0; nCntSphere < MAX_SPHERE; nCntSphere++, pSphere++)
	{
		if (pSphere->pVtxBuff != NULL)
		{// 頂点バッファの破棄
			pSphere->pVtxBuff->Release();
			pSphere->pVtxBuff = NULL;
		}

		if (pSphere->pIdxBuff != NULL)
		{// インデックスバッファの破棄
			pSphere->pIdxBuff->Release();
			pSphere->pIdxBuff = NULL;
		}
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateSphere(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawSphere(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SPHERE* pSphere = &g_aSphere[0];
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	for (int nCntSphere = 0; nCntSphere < MAX_SPHERE; nCntSphere++, pSphere++)
	{
		if (pSphere->bUsed == false) continue; // 使用中でないならパス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pSphere->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(
			&mtxRot,
			pSphere->obj.rot.y, pSphere->obj.rot.x, pSphere->obj.rot.z
		);
		D3DXMatrixMultiply(
			&pSphere->mtxWorld,
			&pSphere->mtxWorld,
			&mtxRot
		);

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pSphere->obj.pos.x, pSphere->obj.pos.y, pSphere->obj.pos.z
		);
		D3DXMatrixMultiply(
			&pSphere->mtxWorld,
			&pSphere->mtxWorld,
			&mtxTrans
		);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pSphere->mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pSphere->pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(pSphere->pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		if (pSphere->obj.bVisible)
		{// 表示状態
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffSphere);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLEFAN,
				0,
				0,
				pSphere->nMaxVtx,			// 用意した頂点数
				0,
				pSphere->nSegmentX
			);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				pSphere->nMaxVtx,			// 用意した頂点数
				pSphere->nSegmentX + 1,
				pSphere->nMaxIdx - (pSphere->nSegmentX + 1) * 2 - 3
			);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLEFAN,
				0,
				0,
				pSphere->nMaxVtx,			// 用意した頂点数
				(pSphere->nMaxIdx - 1) - (pSphere->nSegmentX + 1),
				pSphere->nSegmentX
			);
		}

		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

void SetSphere(int nTexId, D3DXVECTOR3 pos, D3DXVECTOR3 size, int nSegmentU, int nSegmentV)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SPHERE* pSphere = &g_aSphere[0];

	for (int nCntSphere = 0; nCntSphere < MAX_SPHERE; nCntSphere++, pSphere++)
	{
		if (pSphere->bUsed == true) continue; // 使用中ならパス

		memset(pSphere, 0, sizeof(SPHERE));
		pSphere->bUsed = true;
		pSphere->obj.pos = pos;
		pSphere->obj.size = INIT_SIZE;
		pSphere->obj.color = INIT_COLOR;
		pSphere->obj.bVisible = true;
		pSphere->nTexId = nTexId;
		pSphere->nSegmentX = nSegmentU;
		pSphere->nSegmentZ = nSegmentV;
		pSphere->nMaxVtx = ((nSegmentV - 1) * (nSegmentU + 1) + 2);
		pSphere->nMaxIdx = (1 + nSegmentU + 1) * 2 + 2 * (nSegmentU + 1) * (nSegmentV - 2) + (nSegmentV - 3) * 2;

		Clamp(&nSegmentU, 3, nSegmentU);
		Clamp(&nSegmentV, 3, nSegmentV);

		if (TEXTURE_FILENAME)
		{// テクスチャの読み込み
			D3DXCreateTextureFromFile(
				pDevice,
				TEXTURE_FILENAME,
				&g_pTexBuffSphere
			);
		}

		// 頂点バッファの生成
		// スフィアの頂点数＝（縦の分割数－１）×（横の分割数＋１）＋上下の２頂点
		pDevice->CreateVertexBuffer(
			sizeof(VERTEX_3D) * pSphere->nMaxVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pSphere->pVtxBuff,
			NULL
		);

		VERTEX_3D* pVtx;

		// 頂点バッファをロックして頂点情報へのポインタを取得
		pSphere->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtxV = 0; nCntVtxV < nSegmentV + 1; nCntVtxV++)
		{
			float fAngleV = D3DX_PI / nSegmentV * nCntVtxV;

			for (int nCntVtxU = 0; nCntVtxU < nSegmentU + 1; nCntVtxU++)
			{
				float fAngleU = (-D3DX_PI * 2.0f) / nSegmentU * nCntVtxU;

				if (nCntVtxV == 0)
				{
					pVtx->pos = D3DXVECTOR3(0.0f, pSphere->obj.size.y / 2, 0.0f);;
					pVtx->tex = D3DXVECTOR2(0.0f, 0.0f);
				}
				else if (nCntVtxV == nSegmentV)
				{
					pVtx->pos = D3DXVECTOR3(0.0f, -pSphere->obj.size.y / 2, 0.0f);
					pVtx->tex = D3DXVECTOR2(1.0f, 1.0f);
				}
				else
				{
					pVtx->pos = D3DXVECTOR3(
						sinf(fAngleU) * pSphere->obj.size.x / 2 * sinf(fAngleV),
						cosf(fAngleV) * pSphere->obj.size.y / 2,
						cosf(fAngleU) * pSphere->obj.size.z / 2 * sinf(fAngleV)
					);
					pVtx->tex = D3DXVECTOR2((float)nCntVtxU / (float)nSegmentU, (float)nCntVtxV / (float)nSegmentV);
					//pVtx->tex = D3DXVECTOR2((float)nCntVtxU, (float)nCntVtxV);
				}

				pVtx->nor = Normalize(pVtx->pos);
				pVtx->col = pSphere->obj.color;

				pVtx++;

				if (nCntVtxV == 0 || nCntVtxV == nSegmentV)
				{
					break;
				}

			}
		}

		// 頂点バッファをアンロック
		pSphere->pVtxBuff->Unlock();

		WORD* pIdx;		// インデックス情報へのポインタ

		// インデックスバッファの生成
		// （インデックスバッファのサイズは、メッシュでのポリゴン描画に必要な分用意する）
		// 
		pDevice->CreateIndexBuffer(
			sizeof(WORD) * pSphere->nMaxIdx,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&pSphere->pIdxBuff,
			NULL
		);

		// インデックスバッファをロックし、頂点番号データへのポインタを取得
		pSphere->pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);


		for (int nCntIdxV = 0; nCntIdxV < nSegmentV; nCntIdxV++)
		{
			for (int nCntIdxU = 0; nCntIdxU < nSegmentU + 1; nCntIdxU++)
			{
				if (nCntIdxV == 0)
				{
					*pIdx = 0;
					pIdx++;

					for (int nCount = 0; nCount < nSegmentU + 1; nCount++)
					{
						*pIdx = nCount + 1;
						pIdx++;
					}

					break;
				}
				else if (nCntIdxV == nSegmentV - 1)
				{
					*pIdx = pSphere->nMaxVtx - 1;
					pIdx++;

					for (int nCount = 0; nCount < nSegmentU + 1; nCount++)
					{
						*pIdx = (pSphere->nMaxVtx - 1) - (nCount + 1);
						pIdx++;
					}

					break;
				}

				pIdx[0] = (nCntIdxU + 1) + nCntIdxV * (nSegmentU + 1);
				pIdx[1] = (nCntIdxU + 1) + (nCntIdxV - 1) * (nSegmentU + 1);

				pIdx += 2;

				if (nCntIdxV != (nSegmentV - 2) && nCntIdxU == nSegmentU)
				{
					pIdx[0] = (nCntIdxU + 1) + (nCntIdxV - 1) * (nSegmentU + 1);
					pIdx[1] = ((nCntIdxU + 1) + nCntIdxV * (nSegmentU + 1)) + 1;
					pIdx += 2;
				}

			}
		}

		// インデックスバッファをアンロック
		pSphere->pIdxBuff->Unlock();

		break;
	}
}