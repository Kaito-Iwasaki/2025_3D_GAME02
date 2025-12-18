//=====================================================================
// 
// pauseBg [PauseBg.cpp]
// Author:
//
//=====================================================================
#include "pauseBg.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPauseBg = NULL;
BASEOBJECT g_pauseBg;

//=====================================================================
// 
// ***** 初期化処理 *****
// 
//=====================================================================
void InitPauseBg(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	memset(&g_pauseBg, 0, sizeof(BASEOBJECT));
	g_pauseBg.pos = D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0);
	g_pauseBg.size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	g_pauseBg.color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
	g_pauseBg.bVisible = true;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPauseBg,
		NULL
	);
}

//=====================================================================
// 
// ***** 終了処理 *****
// 
//=====================================================================
void UninitPauseBg(void)
{
	if (g_pVtxBuffPauseBg != NULL)
	{
		g_pVtxBuffPauseBg->Release();
		g_pVtxBuffPauseBg = NULL;
	}
}

//=====================================================================
// 
// ***** 更新処理 *****
// 
//=====================================================================
void UpdatePauseBg(void)
{

}

//=====================================================================
// 
// ***** 描画処理 *****
// 
//=====================================================================
void DrawPauseBg(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffPauseBg->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	SetVertexPos(pVtx, g_pauseBg);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_pauseBg.color);
	SetVertexTexturePos(pVtx);

	// 頂点バッファをアンロック
	g_pVtxBuffPauseBg->Unlock();

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPauseBg, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

BASEOBJECT* GetPauseBg(void)
{
	return &g_pauseBg;
}