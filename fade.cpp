//=====================================================================
// 
// フェード処理 [fade.cpp]
// Author:岩崎桧翔
//
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "fade.h"
#include "input.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_COLOR			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// フェード色
#define FADE_SCALE			(0.025f)	// フェード速度

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;
FADE g_fade;

//=====================================================================
// 初期化処理
//=====================================================================
void InitFade(SCENE sceneNext)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ
	VERTEX_2D* pVtx;			// 頂点情報へのポインタ

	// デバイスの取得
	pDevice = GetDevice();

	// フェード構造体の初期化
	memset(&g_fade, 0, sizeof(FADE));
	g_fade.obj.pos = D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0.0f);
	g_fade.obj.size = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	g_fade.obj.color = INIT_COLOR;
	g_fade.state = FADESTATE_IN;
	g_fade.sceneNext = sceneNext;
	g_fade.fFadeScale = FADE_SCALE;
	g_fade.bStopSound = false;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL
	);

	// シーンの設定
	SetScene(sceneNext);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitFade(void)
{
	// 頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateFade(void)
{
	if (g_fade.state != FADESTATE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN))
		{
			if (g_fade.obj.color.a > 0 && g_fade.obj.color.a < 1)
			{// Enterキーを押されたら早くする
				g_fade.fFadeScale = FADE_SCALE * 2;
			}
		}

		if (g_fade.state == FADESTATE_IN)
		{// フェードイン
			g_fade.obj.color.a -= g_fade.fFadeScale;

			if (g_fade.obj.color.a <= 0.0f)
			{// フェードイン終了
				g_fade.obj.color.a = 0.0f;
				g_fade.state = FADESTATE_NONE;
				g_fade.fFadeScale = FADE_SCALE;

			}
		}
		else if (g_fade.state == FADESTATE_OUT)
		{// フェードアウト
			g_fade.obj.color.a += g_fade.fFadeScale;;

			if (g_fade.obj.color.a >= 1.0f)
			{// フェードアウト&フェードインへ移行
				g_fade.obj.color.a = 1.0f;
				g_fade.state = FADESTATE_IN;

				SetScene(g_fade.sceneNext, g_fade.bStopSound);
			}
		}
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報の設定
	SetVertexPos(pVtx, g_fade.obj);
	SetVertexRHW(pVtx, 1.0f);
	SetVertexColor(pVtx, g_fade.obj.color);
	SetVertexTexturePos(pVtx);

	// 頂点バッファをアンロック
	g_pVtxBuffFade->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=====================================================================
// フェード切り替え処理
//=====================================================================
void SetFade(SCENE sceneNext, bool bStopSound)
{
	if (g_fade.state == FADESTATE_NONE)
	{
		g_fade.state = FADESTATE_OUT;
		g_fade.sceneNext = sceneNext;
		g_fade.fFadeScale = FADE_SCALE;
		g_fade.bStopSound = bStopSound;
	}
}

//=====================================================================
// 現在のフェード状態を取得
//=====================================================================
FADESTATE GetFadeState(void)
{
	return g_fade.state;
}