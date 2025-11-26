//=====================================================================
//
// Cylinder [cylinder.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "cylinder.h"
#include "input.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\sakai000b.png"
#define INIT_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(200.0f, 200.0f, 200.0f)
#define INIT_COLOR			D3DXCOLOR_WHITE
#define NUM_SEGMENT_X		(16)
#define NUM_SEGMENT_Y		(4)

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCylinder = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffCylinder = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffCylinder = NULL;
CYLINDER g_Cylinder;

//=====================================================================
// 初期化処理
//=====================================================================
void InitCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体の初期化
	memset(&g_Cylinder, 0, sizeof(CYLINDER));
	g_Cylinder.obj.pos = INIT_POS;
	g_Cylinder.obj.size = INIT_SIZE;
	g_Cylinder.obj.color = INIT_COLOR;
	g_Cylinder.obj.bVisible = true;

	if (TEXTURE_FILENAME)
	{// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffCylinder
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * (NUM_SEGMENT_X + 1) * (NUM_SEGMENT_Y + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCylinder,
		NULL
	);

	VERTEX_3D* pVtx;
	D3DXVECTOR3 vecOrigin = D3DXVECTOR3(0, g_Cylinder.obj.size.y, 0);
	
	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffCylinder->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	for (int nCntVtxY = 0; nCntVtxY < NUM_SEGMENT_Y + 1; nCntVtxY++)
	{
		for (int nCntVtxX = 0; nCntVtxX < NUM_SEGMENT_X + 1; nCntVtxX++)
		{
			float fAngle = -(D3DX_PI * 2) / NUM_SEGMENT_X * nCntVtxX;
			D3DXVECTOR3 vecOffset = D3DXVECTOR3(
				sinf(fAngle) * g_Cylinder.obj.size.z,
				vecOrigin.y - (g_Cylinder.obj.size.y / NUM_SEGMENT_Y) * nCntVtxY,
				cosf(fAngle) * g_Cylinder.obj.size.x
			);
			pVtx->pos = vecOffset;
			pVtx->nor = -D3DXVECTOR3(sinf(fAngle), 0.0f, cosf(fAngle));
			pVtx->col = D3DXCOLOR_WHITE;
			pVtx->tex = D3DXVECTOR2(nCntVtxX, nCntVtxY);
			pVtx++;
		}
	}

	// 頂点バッファをアンロック
	g_pVtxBuffCylinder->Unlock();

	WORD* pIdx;		// インデックス情報へのポインタ

	// インデックス数
	// （２頂点×横の頂点数×縦の分割数＋２頂点×横に折り返す回数）
	int nNumIdx = (NUM_SEGMENT_X + 1) * 2 * NUM_SEGMENT_Y + (NUM_SEGMENT_Y - 1) * 2;

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffCylinder,
		NULL
	);

	// インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffCylinder->Lock(0, 0, (void**)&pIdx, 0);

	// 頂点番号データの設定
	for (int nCntIdxY = 0; nCntIdxY < NUM_SEGMENT_Y; nCntIdxY++)
	{
		for (int nCntIdxX = 0; nCntIdxX < NUM_SEGMENT_X + 1; nCntIdxX++)
		{
			pIdx[0] = (nCntIdxY + 1) * (NUM_SEGMENT_X + 1) + nCntIdxX;
			pIdx[1] = nCntIdxY * (NUM_SEGMENT_X + 1) + nCntIdxX;
			pIdx += 2;

			if (nCntIdxX == NUM_SEGMENT_X && nCntIdxY != NUM_SEGMENT_Y - 1)
			{
				pIdx[0] = nCntIdxY * (NUM_SEGMENT_X + 1) + nCntIdxX;
				pIdx[1] = (nCntIdxY + 1) * (NUM_SEGMENT_X + 1) + nCntIdxX + 1;
				pIdx += 2;
			}
		}
	}

	// インデックスバッファをアンロック
	g_pIdxBuffCylinder->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitCylinder(void)
{
	if (g_pTexBuffCylinder != NULL)
	{// テクスチャの破棄
		g_pTexBuffCylinder->Release();
		g_pTexBuffCylinder = NULL;
	}

	if (g_pVtxBuffCylinder != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffCylinder->Release();
		g_pVtxBuffCylinder = NULL;
	}

	if (g_pIdxBuffCylinder != NULL)
	{// インデックスバッファの破棄
		g_pIdxBuffCylinder->Release();
		g_pIdxBuffCylinder = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateCylinder(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Cylinder.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(
		&mtxRot,
		g_Cylinder.obj.rot.y, g_Cylinder.obj.rot.x, g_Cylinder.obj.rot.z
	);
	D3DXMatrixMultiply(
		&g_Cylinder.mtxWorld,
		&g_Cylinder.mtxWorld,
		&mtxRot
	);

	// 位置を反映
	D3DXMatrixTranslation(
		&mtxTrans,
		g_Cylinder.obj.pos.x, g_Cylinder.obj.pos.y, g_Cylinder.obj.pos.z
	);
	D3DXMatrixMultiply(
		&g_Cylinder.mtxWorld,
		&g_Cylinder.mtxWorld,
		&mtxTrans
	);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Cylinder.mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCylinder, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffCylinder);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	if (g_Cylinder.obj.bVisible)
	{// 表示状態
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffCylinder);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			(NUM_SEGMENT_X + 1) * (NUM_SEGMENT_Y + 1),			// 用意した頂点数
			0,
			((4 + 2 * NUM_SEGMENT_X) * NUM_SEGMENT_Y) - 2 - 2	// 描画するポリゴン数
		);
	}
}