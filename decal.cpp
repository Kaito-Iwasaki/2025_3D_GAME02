//=====================================================================
//
// 画像オブジェクト [decal.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "util.h"
#include "decal.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE_X				(8.0f)
#define INIT_SIZE_Y				(32.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDecal = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffDecal[DECAL_LABEL_MAX] = {};
DECAL g_aDecal[MAX_DECAL] = {};

const char* g_aDecalFileName[DECAL_LABEL_MAX] = {
	NULL,
	"data\\TEXTURE\\logo.png",
	"data\\TEXTURE\\result.png",
	"data\\TEXTURE\\life00.png",
};

//=====================================================================
// 初期化処理
//=====================================================================
void InitDecal(bool bPreload)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス
	DECAL* pDecal = &g_aDecal[0];

	// 構造体の初期化
	memset(pDecal, 0, sizeof(DECAL) * MAX_DECAL);

	if (bPreload == true)
	{// テクスチャのプリロード
		for (int nCount = 0; nCount < DECAL_LABEL_MAX; nCount++)
		{
			D3DXCreateTextureFromFile(
				pDevice,
				g_aDecalFileName[nCount],
				&g_pTexBuffDecal[nCount]
			);
		}
	}


	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_DECAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDecal,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitDecal(void)
{
	memset(&g_aDecal[0], 0, sizeof(DECAL) * MAX_DECAL);

	if (g_pVtxBuffDecal != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffDecal->Release();
		g_pVtxBuffDecal = NULL;
	}

	for (int nCount = 0; nCount < DECAL_LABEL_MAX; nCount++)
	{
		if (g_pTexBuffDecal[nCount] != NULL)
		{// テクスチャバッファの破棄
			g_pTexBuffDecal[nCount]->Release();
			g_pTexBuffDecal[nCount] = NULL;
		}
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawDecal(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;
	DECAL* pDecal = &g_aDecal[0];

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffDecal->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_DECAL; nCount++, pDecal++, pVtx += 4)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx, pDecal->obj);
		SetVertexRHW(pVtx, 1.0f);
		SetVertexColor(pVtx, pDecal->obj.color);
		SetVertexTexturePos(pVtx);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffDecal->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffDecal, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDecal = &g_aDecal[0];
	for (int nCount = 0; nCount < MAX_DECAL; nCount++, pDecal++)
	{
		if (pDecal->obj.bVisible == true && pDecal->bUsed == true)
		{// ポリゴン描画
			// テクスチャの設定
			if (pDecal->label != DECAL_LABEL_NULL)
			{
				pDevice->SetTexture(0, g_pTexBuffDecal[pDecal->label]);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//=====================================================================
// オブジェクト取得処理
//=====================================================================
DECAL* GetDecal(void)
{
	return &g_aDecal[0];
}

//=====================================================================
// 画像の設定処理
//=====================================================================
DECAL* SetDecal(DECAL_LABEL label, D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 rot, D3DXCOLOR col)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	DECAL* pDecal = &g_aDecal[0];
	for (int nCount = 0; nCount < MAX_DECAL; nCount++, pDecal++)
	{
		if (pDecal->bUsed == false)
		{
			if (label != DECAL_LABEL_NULL && g_pTexBuffDecal[label] == NULL)
			{
				D3DXCreateTextureFromFile(
					pDevice,
					g_aDecalFileName[label],
					&g_pTexBuffDecal[label]
				);
			}

			memset(pDecal, 0, sizeof(DECAL));
			pDecal->bUsed = true;
			pDecal->obj.pos = pos;
			pDecal->obj.size = size;
			pDecal->obj.color = col;
			pDecal->label = label;
			pDecal->nID = nCount;
			pDecal->obj.bVisible = true;

			return pDecal;
		}
	}

	return NULL;
}

//=====================================================================
// 画像の削除処理（指定）
//=====================================================================
void DeleteDecal(int nID)
{
	memset(&g_aDecal[nID], 0, sizeof(DECAL));
}

//=====================================================================
// 画像の削除処理（一括）
//=====================================================================
void DeleteDecal(void)
{
	memset(&g_aDecal[0], 0, sizeof(DECAL) * MAX_DECAL);
}
