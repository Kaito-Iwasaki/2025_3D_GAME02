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
#define TEXTURE_FILENAME	"data\\TEXTURE\\sakai000.png"
#define INIT_POS			D3DXVECTOR3(0.0f, 50.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(100.0f, 100.0f, 100.0f)
#define INIT_COLOR			D3DXCOLOR_WHITE
#define NUM_BLOCK_X			(3)
#define NUM_BLOCK_Y			(3)

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

	ZeroMemory(pSphere, sizeof(pSphere));

	SetSphere(0, D3DXVECTOR3_ZERO, INIT_SIZE, NUM_BLOCK_X, NUM_BLOCK_Y);
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

		if (pSphere->obj.bVisible)
		{// 表示状態
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffSphere);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLEFAN,
				0,
				0,
				((NUM_BLOCK_Y - 1) * (NUM_BLOCK_X + 1) + 2),			// 用意した頂点数
				0,
				3
			);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				10,			// 用意した頂点数
				5,
				6
			);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLEFAN,
				0,
				0,
				10,			// 用意した頂点数
				13,
				3
			);
		}
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

		Clamp(&nSegmentU, 3, nSegmentU);
		Clamp(&nSegmentV, 2, nSegmentV);

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
			sizeof(VERTEX_3D) * ((NUM_BLOCK_Y - 1) * (NUM_BLOCK_X + 1) + 2),
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pSphere->pVtxBuff,
			NULL
		);

		VERTEX_3D* pVtx;
		D3DXVECTOR3 vecOrigin = D3DXVECTOR3(0.0f, pSphere->obj.size.y / 2, 0.0f);

		// 頂点バッファをロックして頂点情報へのポインタを取得
		pSphere->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		int nCount = 0;

		pVtx[0].pos = D3DXVECTOR3(0.0f, 90.0f, 0.0f);

		pVtx[1].pos = D3DXVECTOR3(sinf(D3DXToRadian(0)) * 50.0f, 60.0f, cosf(D3DXToRadian(0)) * 50.0f);
		pVtx[2].pos = D3DXVECTOR3(sinf(D3DXToRadian(-120)) * 50.0f, 60.0f, cosf(D3DXToRadian(-120)) * 50.0f);
		pVtx[3].pos = D3DXVECTOR3(sinf(D3DXToRadian(-240)) * 50.0f, 60.0f, cosf(D3DXToRadian(-240)) * 50.0f);
		pVtx[4].pos = D3DXVECTOR3(sinf(D3DXToRadian(0)) * 50.0f, 60.0f, cosf(D3DXToRadian(0)) * 50.0f);

		pVtx[5].pos = D3DXVECTOR3(sinf(D3DXToRadian(0)) * 50.0f, 30.0f, cosf(D3DXToRadian(0)) * 50.0f);
		pVtx[6].pos = D3DXVECTOR3(sinf(D3DXToRadian(-120)) * 50.0f, 30.0f, cosf(D3DXToRadian(-120)) * 50.0f);
		pVtx[7].pos = D3DXVECTOR3(sinf(D3DXToRadian(-240)) * 50.0f, 30.0f, cosf(D3DXToRadian(-240)) * 50.0f);
		pVtx[8].pos = D3DXVECTOR3(sinf(D3DXToRadian(0)) * 50.0f, 30.0f, cosf(D3DXToRadian(0)) * 50.0f);

		pVtx[9].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//pVtx[0].nor = Normalize(pVtx[0].pos - pSphere->obj.pos);
		//pVtx[1].nor = Normalize(pVtx[1].pos - pSphere->obj.pos);
		//pVtx[2].nor = Normalize(pVtx[2].pos - pSphere->obj.pos);
		//pVtx[3].nor = Normalize(pVtx[3].pos - pSphere->obj.pos);
		//pVtx[4].nor = Normalize(pVtx[4].pos - pSphere->obj.pos);
		//pVtx[5].nor = Normalize(pVtx[5].pos - pSphere->obj.pos);
		//pVtx[6].nor = Normalize(pVtx[6].pos - pSphere->obj.pos);
		//pVtx[7].nor = Normalize(pVtx[7].pos - pSphere->obj.pos);
		//pVtx[8].nor = Normalize(pVtx[8].pos - pSphere->obj.pos);
		//pVtx[9].nor = Normalize(pVtx[9].pos - pSphere->obj.pos);

		//pVtx[0].col = pSphere->obj.color;
		//pVtx[1].col = pSphere->obj.color;
		//pVtx[2].col = pSphere->obj.color;
		//pVtx[3].col = pSphere->obj.color;
		//pVtx[4].col = pSphere->obj.color;
		//pVtx[5].col = pSphere->obj.color;
		//pVtx[6].col = pSphere->obj.color;
		//pVtx[7].col = pSphere->obj.color;
		//pVtx[8].col = pSphere->obj.color;
		//pVtx[9].col = pSphere->obj.color;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.3f);
		pVtx[2].tex = D3DXVECTOR2(0.3f, 0.3f);
		pVtx[3].tex = D3DXVECTOR2(0.6f, 0.3f);
		pVtx[4].tex = D3DXVECTOR2(1.0f, 0.3f);

		pVtx[5].tex = D3DXVECTOR2(0.0f, 0.6f);
		pVtx[6].tex = D3DXVECTOR2(0.3f, 0.6f);
		pVtx[7].tex = D3DXVECTOR2(0.6f, 0.6f);
		pVtx[8].tex = D3DXVECTOR2(1.0f, 0.6f);

		pVtx[9].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点情報を設定
		for (int nCntVtxV = 0; nCntVtxV < nSegmentV + 1; nCntVtxV++)
		{
			for (int nCntVtxU = 0; nCntVtxU < nSegmentU + 1; nCntVtxU++, pVtx++)
			{
				if (nCntVtxV == 0)
				{
					pVtx->pos = vecOrigin;
					pVtx->tex = D3DXVECTOR2(0.0f, 0.0f);
				}
				else if (nCntVtxV == nSegmentV)
				{
					pVtx->pos = vecOrigin - D3DXVECTOR3(0.0f, pSphere->obj.size.y, 0.0f);
					pVtx->tex = D3DXVECTOR2(1.0f, 1.0f);

				}
				else
				{
					float fAngle = (D3DX_PI * 2) / nSegmentU * nCntVtxU;
					pVtx->pos.x = -sinf(fAngle) * pSphere->obj.size.x / 2.0f;
					pVtx->pos.z = cosf(fAngle) * pSphere->obj.size.z / 2.0f;
					pVtx->tex = D3DXVECTOR2((float)nCntVtxU / (float)nSegmentU, (float)nCntVtxV / (float)nSegmentV);
				}

				pVtx->nor = Normalize(pVtx->pos - pSphere->obj.pos);
				pVtx->col = pSphere->obj.color;
				nCount++;

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
		// （上面と底面の頂点数＋２頂点×横の頂点数×縦の分割数＋２頂点×横に折り返す回数）
		pDevice->CreateIndexBuffer(
			sizeof(WORD) * 18,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&pSphere->pIdxBuff,
			NULL
		);

		// インデックスバッファをロックし、頂点番号データへのポインタを取得
		pSphere->pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		pIdx[0] = 0;
		pIdx[1] = 4;
		pIdx[2] = 3;
		pIdx[3] = 2;
		pIdx[4] = 1;
		pIdx[5] = 5;
		pIdx[6] = 1;
		pIdx[7] = 6;
		pIdx[8] = 2;
		pIdx[9] = 7;
		pIdx[10] = 3;
		pIdx[11] = 8;
		pIdx[12] = 4;
		pIdx[13] = 9;
		pIdx[14] = 5;
		pIdx[15] = 6;
		pIdx[16] = 7;
		pIdx[17] = 8;

		// 頂点番号データの設定
		//for (int nCntIdxV = 0; nCntIdxV < nSegmentV - 1; nCntIdxV++)
		//{



			//if (nCntIdxV == 0)
			//{
			//	*pIdx = 0;
			//	pIdx++;

			//	for (int nCntIdxU = 0; nCntIdxU < nSegmentU; nCntIdxU++)
			//	{
			//		*pIdx = nCntIdxU + 1;
			//		pIdx++;
			//	}
			//}
			//else if (nCntIdxV == nSegmentV - 2)
			//{
			//	for (int nCntIdxU = 0; nCntIdxU < nSegmentU; nCntIdxU++)
			//	{
			//		*pIdx = 1 + nCntIdxU + (pSphere->nSegmentX * (nCntIdxV - 1));
			//		pIdx++;
			//	}

			//	*pIdx = 1 + pSphere->nSegmentX * nCntIdxV;
			//}
			//else
			//{

			//}
		//}

		// インデックスバッファをアンロック
		pSphere->pIdxBuff->Unlock();

		break;
	}
}