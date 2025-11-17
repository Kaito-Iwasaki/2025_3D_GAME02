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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffMeshField = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;
MESHFIELD g_MeshField;
D3DXMATRIX g_mtxWorldMeshField;	// ワールドマトリックス

//=====================================================================
// 初期化処理
//=====================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nMaxVertex = 9;

	// 構造体の初期化
	memset(&g_MeshField, 0, sizeof(MESHFIELD));
	g_MeshField.obj.pos = INIT_POS;
	g_MeshField.obj.size = INIT_SIZE;
	g_MeshField.obj.color = INIT_COLOR;
	g_MeshField.obj.bVisible = true;

	if (TEXTURE_FILENAME)
	{// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffMeshField
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * nMaxVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL
	);

	VERTEX_3D* pVtx;
	D3DXVECTOR3 vecOrigin = g_MeshField.obj.pos + D3DXVECTOR3(-g_MeshField.obj.size.x / 2, 0.0f, g_MeshField.obj.size.z / 2);
	D3DXVECTOR3 vecOffset = D3DXVECTOR3(g_MeshField.obj.size.x / NUM_BLOCK_X, 0.0f, -g_MeshField.obj.size.z / NUM_BLOCK_Z);

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(vecOrigin.x + vecOffset.x * 0, 0.0f, vecOrigin.z + vecOffset.z * 0);
	pVtx[1].pos = D3DXVECTOR3(vecOrigin.x + vecOffset.x * 1, 0.0f, vecOrigin.z + vecOffset.z * 0);
	pVtx[2].pos = D3DXVECTOR3(vecOrigin.x + vecOffset.x * 2, 0.0f, vecOrigin.z + vecOffset.z * 0);
	pVtx[3].pos = D3DXVECTOR3(vecOrigin.x + vecOffset.x * 0, 0.0f, vecOrigin.z + vecOffset.z * 1);
	pVtx[4].pos = D3DXVECTOR3(vecOrigin.x + vecOffset.x * 1, 0.0f, vecOrigin.z + vecOffset.z * 1);
	pVtx[5].pos = D3DXVECTOR3(vecOrigin.x + vecOffset.x * 2, 0.0f, vecOrigin.z + vecOffset.z * 1);
	pVtx[6].pos = D3DXVECTOR3(vecOrigin.x + vecOffset.x * 0, 0.0f, vecOrigin.z + vecOffset.z * 2);
	pVtx[7].pos = D3DXVECTOR3(vecOrigin.x + vecOffset.x * 1, 0.0f, vecOrigin.z + vecOffset.z * 2);
	pVtx[8].pos = D3DXVECTOR3(vecOrigin.x + vecOffset.x * 2, 0.0f, vecOrigin.z + vecOffset.z * 2);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR_WHITE;
	pVtx[1].col = D3DXCOLOR_WHITE;
	pVtx[2].col = D3DXCOLOR_WHITE;
	pVtx[3].col = D3DXCOLOR_WHITE;
	pVtx[4].col = D3DXCOLOR_WHITE;
	pVtx[5].col = D3DXCOLOR_WHITE;
	pVtx[6].col = D3DXCOLOR_WHITE;
	pVtx[7].col = D3DXCOLOR_WHITE;
	pVtx[8].col = D3DXCOLOR_WHITE;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロック
	g_pVtxBuffMeshField->Unlock();

	WORD* pIdx;		// インデックス情報へのポインタ

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * 14,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL
	);

	// インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	// 頂点番号データの設定

	//for (int nIdx = 0; nIdx < 14; nIdx++, pIdx++)
	//{
	//	if ((nIdx + 1) % ((4 + 2 * NUM_BLOCK_X) + 1) == 0)
	//	{
	//		*pIdx = 2;
	//	}
	//	else if ((nIdx + 1) % ((4 + 2 * NUM_BLOCK_X) + 2) == 0)
	//	{
	//		*pIdx = 6;
	//	}
	//	else if (nIdx % 2 == 0)
	//	{
	//		*pIdx = (nIdx / 3) + 3 + (nIdx % 3 - 1);
	//	}
	//	else if (nIdx % 2 == 1)
	//	{
	//		*pIdx = (nIdx % 8) / 2;
	//	}

	//}

	pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;

	// インデックスバッファをアンロック
	g_pIdxBuffMeshField->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitMeshField(void)
{
	if (g_pTexBuffMeshField != NULL)
	{// テクスチャの破棄
		g_pTexBuffMeshField->Release();
		g_pTexBuffMeshField = NULL;
	}

	if (g_pVtxBuffMeshField != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	if (g_pIdxBuffMeshField != NULL)
	{// インデックスバッファの破棄
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateMeshField(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(
		&mtxRot,
		g_MeshField.obj.rot.y, g_MeshField.obj.rot.x, g_MeshField.obj.rot.z
	);
	D3DXMatrixMultiply(
		&g_mtxWorldMeshField,
		&g_mtxWorldMeshField,
		&mtxRot
	);

	// 位置を反映
	D3DXMatrixTranslation(
		&mtxTrans,
		g_MeshField.obj.pos.x, g_MeshField.obj.pos.y, g_MeshField.obj.pos.z
	);
	D3DXMatrixMultiply(
		&g_mtxWorldMeshField,
		&g_mtxWorldMeshField,
		&mtxTrans
	);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	if (g_MeshField.obj.bVisible)
	{// 表示状態
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffMeshField);

		// ポリゴンの描画
		//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 12);
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			9,
			0,
			12
		);
	}
}