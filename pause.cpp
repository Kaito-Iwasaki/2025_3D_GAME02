//=====================================================================
// 
// ポーズ画面処理 [pause.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "pause.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "decal.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_POS_X			(SCREEN_WIDTH / 2)
#define TEXTURE_POS_Y			(SCREEN_HEIGHT / 2 - 100)
#define TEXTURE_SIZE			(1)
#define TEXTURE_SCALE_X			(500)
#define TEXTURE_SCALE_Y			(100)

#define COLOR_DESELECTED		D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f)

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSE_MENU_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;				// 頂点バッファへのポインタ
PAUSE g_menu;

const char* g_aPauseTexFileName[PAUSE_MENU_MAX] = {
	"data\\TEXTURE\\pause000.png",
	"data\\TEXTURE\\pause001.png",
	"data\\TEXTURE\\pause002.png"
};

D3DXCOLOR g_aColorSelected[PAUSE_MENU_MAX] = {
	D3DXCOLOR(0.3f, 0.9f, 1.0f, 1.0f),
	D3DXCOLOR(0.3f, 1.0f, 0.2f, 1.0f),
	D3DXCOLOR(1.0f, 0.4f, 0.9f, 1.0f)
};

//=====================================================================
// 初期化処理
//=====================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

	memset(&g_menu, 0, sizeof(PAUSE));
	g_menu.obj.pos = D3DXVECTOR3(TEXTURE_POS_X, TEXTURE_POS_Y, 0.0f);
	g_menu.nSelect = 0;

	for (int nCount = 0; nCount < PAUSE_MENU_MAX; nCount++)
	{// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			g_aPauseTexFileName[nCount],
			&g_apTexturePause[nCount]
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * PAUSE_MENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitPause(void)
{
	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		// テクスチャの破棄
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdatePause(void)
{
	if (GetFadeState() != FADESTATE_NONE) return;	// フェード中は処理しない

	if (INPUT_TRIGGER_UI_ACCEPT)
	{
		switch (g_menu.nSelect)
		{
		case PAUSE_MENU_CONTINUE:
			TogglePauseGame(false);
			break;

		case PAUSE_MENU_RETRY:
			if (GetCurrentScene() == SCENE_GAME)
			{
				SetFade(SCENE_GAME);
			}

			break;

		case PAUSE_MENU_QUIT:
			SetFade(SCENE_TITLE);
			break;
		}
	}

	if (GetKeyboardRepeat(DIK_W) || GetJoypadRepeat(JOYKEY_UP) || GetJoystickRepeat(JOYSTICK_L_UP))
	{
		g_menu.nSelect--;
	}
	if (GetKeyboardRepeat(DIK_S) || GetJoypadRepeat(JOYKEY_DOWN) || GetJoystickRepeat(JOYSTICK_L_DOWN))
	{
		g_menu.nSelect++;
	}

	g_menu.nSelect = (g_menu.nSelect + PAUSE_MENU_MAX) % PAUSE_MENU_MAX;
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D* pVtx;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < PAUSE_MENU_MAX; nCount++, pVtx += 4)
	{
		// 頂点情報を設定
		SetVertexPos(pVtx,
			D3DXVECTOR3(g_menu.obj.pos.x - TEXTURE_SCALE_X / 2, g_menu.obj.pos.y + (100 * nCount) - TEXTURE_SCALE_Y / 2, 0.0f),
			D3DXVECTOR3(g_menu.obj.pos.x + TEXTURE_SCALE_X / 2, g_menu.obj.pos.y + (100 * nCount) - TEXTURE_SCALE_Y / 2, 0.0f),
			D3DXVECTOR3(g_menu.obj.pos.x - TEXTURE_SCALE_X / 2, g_menu.obj.pos.y + (100 * nCount) + TEXTURE_SCALE_Y / 2, 0.0f),
			D3DXVECTOR3(g_menu.obj.pos.x + TEXTURE_SCALE_X / 2, g_menu.obj.pos.y + (100 * nCount) + TEXTURE_SCALE_Y / 2, 0.0f)
		);
		SetVertexRHW(pVtx, 1.0f);
		if (nCount == g_menu.nSelect)
		{
			SetVertexColor(pVtx, g_aColorSelected[g_menu.nSelect]);
		}
		else
		{
			SetVertexColor(pVtx, COLOR_DESELECTED);
		}
		SetVertexTexturePos(pVtx);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffPause->Unlock();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}
}

//=====================================================================
// ポーズメニューカーソル指定処理
//=====================================================================
void SetPauseMenuCursor(int nCursor)
{
	g_menu.nSelect = nCursor;
}