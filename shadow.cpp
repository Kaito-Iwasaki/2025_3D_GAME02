//=====================================================================
//
// Shadow [filed.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "shadow.h"
#include "input.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\shadow000.jpg"
#define INIT_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(10.0f, 0.0f, 10.0f)
#define INIT_COLOR			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f)

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffShadow = NULL;
SHADOW g_aShadow[MAX_SHADOW];

//=====================================================================
// 初期化処理
//=====================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SHADOW* pShadow = &g_aShadow[0];

	// 構造体の初期化
	//memset(pShadow, 0, sizeof(pShadow));
	for (int i = 0; i < MAX_SHADOW; i++, pShadow++)
	{
		pShadow->obj.pos = INIT_POS;
		pShadow->obj.size = INIT_SIZE;
		pShadow->obj.color = INIT_COLOR;
		pShadow->obj.bVisible = true;
	}


	if (TEXTURE_FILENAME)
	{// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffShadow
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL
	);

	VERTEX_3D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	pShadow = &g_aShadow[0];
	for (int i = 0; i < MAX_SHADOW; i++, pShadow++, pVtx += 4)
	{
		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(pShadow->obj.pos.x - pShadow->obj.size.x, 0.0f, pShadow->obj.pos.z + pShadow->obj.size.z);
		pVtx[1].pos = D3DXVECTOR3(pShadow->obj.pos.x + pShadow->obj.size.x, 0.0f, pShadow->obj.pos.z + pShadow->obj.size.z);
		pVtx[2].pos = D3DXVECTOR3(pShadow->obj.pos.x - pShadow->obj.size.x, 0.0f, pShadow->obj.pos.z - pShadow->obj.size.z);
		pVtx[3].pos = D3DXVECTOR3(pShadow->obj.pos.x + pShadow->obj.size.x, 0.0f, pShadow->obj.pos.z - pShadow->obj.size.z);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].col = INIT_COLOR;
		pVtx[1].col = INIT_COLOR;
		pVtx[2].col = INIT_COLOR;
		pVtx[3].col = INIT_COLOR;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitShadow(void)
{
	if (g_pTexBuffShadow != NULL)
	{// テクスチャの破棄
		g_pTexBuffShadow->Release();
		g_pTexBuffShadow = NULL;
	}

	if (g_pVtxBuffShadow != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateShadow(void)
{
	VERTEX_3D* pVtx;
	SHADOW* pShadow = &g_aShadow[0];

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < MAX_SHADOW; i++, pShadow++, pVtx += 4)
	{
		if (pShadow->bUsed == false) continue;

		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(-pShadow->obj.size.x, 0.0f, +pShadow->obj.size.z);
		pVtx[1].pos = D3DXVECTOR3(+pShadow->obj.size.x, 0.0f, +pShadow->obj.size.z);
		pVtx[2].pos = D3DXVECTOR3(-pShadow->obj.size.x, 0.0f, -pShadow->obj.size.z);
		pVtx[3].pos = D3DXVECTOR3(+pShadow->obj.size.x, 0.0f, -pShadow->obj.size.z);

		pVtx[0].col = pShadow->obj.color;
		pVtx[1].col = pShadow->obj.color;
		pVtx[2].col = pShadow->obj.color;
		pVtx[3].col = pShadow->obj.color;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	SHADOW* pShadow = &g_aShadow[0];

	// 減算合成を適用
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int i = 0; i < MAX_SHADOW; i++, pShadow++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pShadow->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(
			&mtxRot,
			pShadow->obj.rot.y, pShadow->obj.rot.x, pShadow->obj.rot.z
		);
		D3DXMatrixMultiply(
			&pShadow->mtxWorld,
			&pShadow->mtxWorld,
			&mtxRot
		);

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pShadow->obj.pos.x, pShadow->obj.pos.y, pShadow->obj.pos.z
		);
		D3DXMatrixMultiply(
			&pShadow->mtxWorld,
			&pShadow->mtxWorld,
			&mtxTrans
		);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorld);

		if (pShadow->bUsed && pShadow->obj.bVisible)
		{// 表示状態
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffShadow);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		}
	}

	// 通常の合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

int SetShadow(void)
{
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUsed == false)
		{
			memset(&g_aShadow[nCntShadow], 0, sizeof(SHADOW));
			g_aShadow[nCntShadow].bUsed = true;
			g_aShadow[nCntShadow].obj.size = INIT_SIZE;
			g_aShadow[nCntShadow].obj.color = INIT_COLOR;
			g_aShadow[nCntShadow].obj.bVisible = true;
			return nCntShadow;
		}
	}

	return -1;
}

void SetShadowPosition(int nIdxShadow, D3DXVECTOR3 pos)
{
	if (nIdxShadow == -1) return;
	if (g_aShadow[nIdxShadow].bUsed == false) return;

	g_aShadow[nIdxShadow].obj.pos = pos;
}

void SetShadowSize(int nIdxShadow, D3DXVECTOR3 size)
{
	if (nIdxShadow == -1) return;
	if (g_aShadow[nIdxShadow].bUsed == false) return;

	g_aShadow[nIdxShadow].obj.size = D3DXVECTOR3(
		size.x,
		0.0f,
		size.z
	);
}

void SetShadowAlpha(int nIdxShadow, float alpha)
{
	if (nIdxShadow == -1) return;
	if (g_aShadow[nIdxShadow].bUsed == false) return;

	g_aShadow[nIdxShadow].obj.color.a = alpha;
}

void RemoveShadow(int nIdxShadow)
{
	if (nIdxShadow == -1) return;
	if (g_aShadow[nIdxShadow].bUsed == false) return;

	g_aShadow[nIdxShadow].bUsed = false;
}
