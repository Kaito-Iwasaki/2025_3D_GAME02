//=====================================================================
//
// Polygon [filed.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "polygon.h"
#include "input.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;
POLYGON g_aPolygon[MAX_POLYGON];
int g_nCounterPolygon = 0;

//=====================================================================
// 初期化処理
//=====================================================================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体の初期化
	memset(&g_aPolygon[0], 0, sizeof g_aPolygon);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL
	);

	g_nCounterPolygon = 0;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitPolygon(void)
{
	if (g_pVtxBuffPolygon != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdatePolygon(void)
{
	POLYGON* pPolygon = &g_aPolygon[0];
	VERTEX_3D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++, pPolygon++)
	{
		pPolygon->fPower = fabsf(sinf((float)g_nCounterPolygon * 0.025f));

		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(-pPolygon->obj.size.x / 2, 0.0f, +pPolygon->obj.size.z / 2);
		pVtx[1].pos = D3DXVECTOR3(+pPolygon->obj.size.x / 2, 0.0f, +pPolygon->obj.size.z / 2);
		pVtx[2].pos = D3DXVECTOR3(-pPolygon->obj.size.x / 2, 0.0f, -pPolygon->obj.size.z / 2);
		pVtx[3].pos = D3DXVECTOR3(+pPolygon->obj.size.x / 2, 0.0f, -pPolygon->obj.size.z / 2);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		D3DXCOLOR col = D3DXCOLOR(pPolygon->obj.color.r, pPolygon->obj.color.g, pPolygon->obj.color.b, pPolygon->fPower);

		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		pVtx += 4;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffPolygon->Unlock();

	g_nCounterPolygon++;
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	POLYGON* pPolygon = &g_aPolygon[0];

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++, pPolygon++)
	{
		if (pPolygon->obj.bVisible == false || pPolygon->bUsed == false)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pPolygon->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(
			&mtxRot,
			pPolygon->obj.rot.y, pPolygon->obj.rot.x, pPolygon->obj.rot.z
		);
		D3DXMatrixMultiply(
			&pPolygon->mtxWorld,
			&pPolygon->mtxWorld,
			&mtxRot
		);

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pPolygon->obj.pos.x, pPolygon->obj.pos.y, pPolygon->obj.pos.z
		);
		D3DXMatrixMultiply(
			&pPolygon->mtxWorld,
			&pPolygon->mtxWorld,
			&mtxTrans
		);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pPolygon->mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// ライトを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPolygon * 4, 2);

		// ライトを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

void SetPolygon(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	POLYGON* pPolygon = &g_aPolygon[0];
	VERTEX_3D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++, pPolygon++)
	{
		if (pPolygon->bUsed == true)
		{
			pVtx += 4;
			continue;
		}

		ZeroMemory(pPolygon, sizeof(POLYGON));
		pPolygon->bUsed = true;
		pPolygon->obj.pos = pos;
		pPolygon->obj.size = size;
		pPolygon->obj.rot = rot;
		pPolygon->obj.color = col;
		pPolygon->obj.bVisible = true;

		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(-pPolygon->obj.size.x / 2, 0.0f, +pPolygon->obj.size.z / 2);
		pVtx[1].pos = D3DXVECTOR3(+pPolygon->obj.size.x / 2, 0.0f, +pPolygon->obj.size.z / 2);
		pVtx[2].pos = D3DXVECTOR3(-pPolygon->obj.size.x / 2, 0.0f, -pPolygon->obj.size.z / 2);
		pVtx[3].pos = D3DXVECTOR3(+pPolygon->obj.size.x / 2, 0.0f, -pPolygon->obj.size.z / 2);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].col = pPolygon->obj.color;
		pVtx[1].col = pPolygon->obj.color;
		pVtx[2].col = pPolygon->obj.color;
		pVtx[3].col = pPolygon->obj.color;

		break;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffPolygon->Unlock();
}