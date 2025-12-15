//=====================================================================
//
// MeshField [meshmeshfield.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "meshfield.h"
#include "input.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\sakai000.png"
#define INIT_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(200.0f, 0.0f, 200.0f)
#define INIT_COLOR			D3DXCOLOR_WHITE
#define NUM_BLOCK_X			(2)
#define NUM_BLOCK_Z			(2)

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
LPDIRECT3DTEXTURE9 g_pTexBuffMeshField = NULL;
MESHFIELD g_aMeshField[MAX_MESHFIELD];

//=====================================================================
// 初期化処理
//=====================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESHFIELD* pMeshField = &g_aMeshField[0];
	
	ZeroMemory(pMeshField, sizeof(MESHFIELD));

	if (TEXTURE_FILENAME)
	{// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffMeshField
		);
	}

	SetMeshField(0, D3DXVECTOR3_ZERO, INIT_SIZE, NUM_BLOCK_X, NUM_BLOCK_Z);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitMeshField(void)
{
	MESHFIELD* pMeshField = &g_aMeshField[0];

	if (g_pTexBuffMeshField != NULL)
	{// テクスチャの破棄
		g_pTexBuffMeshField->Release();
		g_pTexBuffMeshField = NULL;
	}

	for (int nCntMField = 0; nCntMField < MAX_MESHFIELD; nCntMField++, pMeshField++)
	{
		if (pMeshField->pVtxBuff != NULL)
		{// 頂点バッファの破棄
			pMeshField->pVtxBuff->Release();
			pMeshField->pVtxBuff = NULL;
		}

		if (pMeshField->pIdxBuff != NULL)
		{// インデックスバッファの破棄
			pMeshField->pIdxBuff->Release();
			pMeshField->pIdxBuff = NULL;
		}
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateMeshField(void)
{
	VERTEX_3D* pVtx;
	MESHFIELD* pMeshField = &g_aMeshField[0];

	for (int nCntMField = 0; nCntMField < MAX_MESHFIELD; nCntMField++, pMeshField++)
	{
		if (pMeshField->bUsed == false) continue; // 使用中でないならパス

		// 頂点バッファをロックして頂点情報へのポインタを取得
		pMeshField->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点バッファをアンロック
		pMeshField->pVtxBuff->Unlock();
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESHFIELD* pMeshField = &g_aMeshField[0];
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	for (int nCntMField = 0; nCntMField < MAX_MESHFIELD; nCntMField++, pMeshField++)
	{
		if (pMeshField->bUsed == false) continue; // 使用中でないならパス

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pMeshField->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(
			&mtxRot,
			pMeshField->obj.rot.y, pMeshField->obj.rot.x, pMeshField->obj.rot.z
		);
		D3DXMatrixMultiply(
			& pMeshField->mtxWorld,
			& pMeshField->mtxWorld,
			&mtxRot
		);

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pMeshField->obj.pos.x, pMeshField->obj.pos.y, pMeshField->obj.pos.z
		);
		D3DXMatrixMultiply(
			&pMeshField->mtxWorld,
			&pMeshField->mtxWorld,
			&mtxTrans
		);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pMeshField->mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, pMeshField->pVtxBuff, 0, sizeof(VERTEX_3D));

		// インデックスバッファをデータストリームに設定
		pDevice->SetIndices(pMeshField->pIdxBuff);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		if (pMeshField->obj.bVisible)
		{// 表示状態
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffMeshField);

			// ポリゴンの描画
			// （描画するポリゴン数＝インデックス数－２）
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				(pMeshField->nSegmentX + 1) * (pMeshField->nSegmentZ + 1),			// 用意した頂点数
				0,
				((4 + 2 * pMeshField->nSegmentX) * pMeshField->nSegmentZ) - 2 - 2	// 描画するポリゴン数
			);
		}
	}
}

//=====================================================================
// メッシュフィールドの設定処理
//=====================================================================
void SetMeshField(int nTexId, D3DXVECTOR3 pos, D3DXVECTOR3 size, int nSegmentX, int nSegmentZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESHFIELD* pMeshField = &g_aMeshField[0];

	for (int nCntMField = 0; nCntMField < MAX_MESHFIELD; nCntMField++, pMeshField++)
	{
		if (pMeshField->bUsed == true) continue; // 使用中ならパス

		memset(pMeshField, 0, sizeof(MESHFIELD));
		pMeshField->bUsed = true;
		pMeshField->obj.pos = pos;
		pMeshField->obj.size = D3DXVECTOR3(size.x * nSegmentX, 0.0f, size.z * nSegmentZ);
		pMeshField->obj.color = INIT_COLOR;
		pMeshField->obj.bVisible = true;
		pMeshField->nTexId = nTexId;
		pMeshField->nSegmentX = nSegmentX;
		pMeshField->nSegmentZ = nSegmentZ;

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(
			sizeof(VERTEX_3D) * (nSegmentX + 1) * (nSegmentZ + 1),
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&pMeshField->pVtxBuff,
			NULL
		);

		VERTEX_3D* pVtx;
		D3DXVECTOR3 vecOrigin = D3DXVECTOR3(-pMeshField->obj.size.x / 2, 0.0f, pMeshField->obj.size.z / 2);
		D3DXVECTOR3 vecOffset = D3DXVECTOR3(pMeshField->obj.size.x / nSegmentX, 0.0f, -pMeshField->obj.size.z / nSegmentZ);

		// 頂点バッファをロックして頂点情報へのポインタを取得
		pMeshField->pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点情報を設定
		for (int nCntVtxZ = 0; nCntVtxZ < nSegmentZ + 1; nCntVtxZ++)
		{
			for (int nCntVtxX = 0; nCntVtxX < nSegmentX + 1; nCntVtxX++)
			{
				pVtx->pos = D3DXVECTOR3(
					vecOrigin.x + vecOffset.x * nCntVtxX,
					pMeshField->obj.pos.y, 
					vecOrigin.z + vecOffset.z * nCntVtxZ
				);
				pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				pVtx->col = pMeshField->obj.color;
				pVtx->tex = D3DXVECTOR2(1.0f / nSegmentX * nCntVtxX, 1.0f / nSegmentZ * nCntVtxZ);
				pVtx++;
			}
		}

		// 頂点バッファをアンロック
		pMeshField->pVtxBuff->Unlock();

		WORD* pIdx;		// インデックス情報へのポインタ

		// インデックスバッファの生成
		// （インデックスバッファのサイズは、メッシュでのポリゴン描画に必要な分用意する）
		// （最低頂点数４＋２頂点×横の分割数×縦の分割数－最終行の重複している頂点数）
		pDevice->CreateIndexBuffer(
			sizeof(WORD) * (((4 + 2 * nSegmentX) * nSegmentZ) - 2),
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&pMeshField->pIdxBuff,
			NULL
		);

		// インデックスバッファをロックし、頂点番号データへのポインタを取得
		pMeshField->pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

		// 頂点番号データの設定
		for (int nCntIdxZ = 0; nCntIdxZ < nSegmentZ; nCntIdxZ++)
		{
			for (int nCntIdxX = 0; nCntIdxX < nSegmentX + 1; nCntIdxX++)
			{
				pIdx[0] = (nCntIdxZ + 1) * (nSegmentX + 1) + nCntIdxX;
				pIdx[1] = nCntIdxZ * (nSegmentX + 1) + nCntIdxX;
				pIdx += 2;

				// 最終行以外の最終列では折り返すためにインデックスをもう二つ設定する
				if (nCntIdxX == nSegmentX && nCntIdxZ != nSegmentZ - 1)
				{
					pIdx[0] = nCntIdxZ * (nSegmentX + 1) + nCntIdxX;
					pIdx[1] = (nCntIdxZ + 1) * (nSegmentX + 1) + nCntIdxX + 1;
					pIdx += 2;
				}
			}
		}

		// インデックスバッファをアンロック
		pMeshField->pIdxBuff->Unlock();

		break;
	}
}