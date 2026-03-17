//=====================================================================
//
// ゲーム画面 [game.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "game.h"
#include "field.h"
#include "player.h"
#include "shadow.h"
#include "wall.h"
#include "model_loader.h"
#include "effect.h"
#include "input.h"
#include "model.h"
#include "meshfield.h"
#include "cylinder.h"
#include "sphere.h"
#include "debugProc.h"
#include "fade.h"
#include "camera.h"
#include "sound.h"
#include "coin.h"
#include "pauseBg.h"
#include "pause.h"
#include "font.h"
#include "polygon.h"
#include "decal.h"

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
bool g_bIsPaused = false;		// ポーズ中か
bool g_bPauseHide = false;		// ポーズ画面非表示（デバッグ用）
SCRIPTDATA g_data;				// マップ情報
GAMESTATE g_gameState;			// ゲーム状態
int g_nCounterStateGame;		// 
FONT* g_pFontScore = NULL;		// スコアテキスト
FONT* g_pFontClear = NULL;		// クリアテキスト
DECAL* g_pDecalLife[PLYAER_MAX_LIFE] = {};	// ライフ画像

//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(void)
{
	PLAYER* pPlayer = GetPlayer();

	// 鳴っている音を停止
	StopSound();

	InitCamera();
	InitField();
	InitShadow();
	InitPlayer();
	InitModel();
	InitEffect();
	InitCylinder();
	InitSphere();
	InitCoin();
	InitPause();
	InitPauseBg();
	InitFont();
	InitPolygon();
	InitDecal();
	InitDebugProc();

	// ポーズ状態の初期化
	g_bIsPaused = false;
	g_bPauseHide = false;

	// ゲーム状態の初期化
	SetGameState(GAMESTATE_NORMAL);

	// スコアテキストの設定
	g_pFontScore = SetFont(FONT_LABEL_DONGURI,
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f),
		60,
		"0",
		DT_LEFT | DT_TOP
	);

	// ライフ表示の設定
	SetFont(FONT_LABEL_DONGURI,
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f),
		60,
		"\n LIFE : ",
		DT_LEFT | DT_TOP
	);
	for (int i = 0; i < PLYAER_MAX_LIFE; i++)
	{// ライフの分テクスチャを表示
		g_pDecalLife[i] = SetDecal(
			DECAL_LABEL_LIFE,
			D3DXVECTOR3(190 + 50 * i, 92, 0),
			D3DXVECTOR3(50, 50, 0),
			D3DXVECTOR3_ZERO,
			D3DXCOLOR_WHITE
		);
	}

	// クリアテキストの設定
	g_pFontClear = SetFont(FONT_LABEL_DONGURI,
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f),
		100,
		"",
		DT_CENTER | DT_VCENTER
	);


	// マップ情報の読み込み
	ZeroMemory(&g_data, sizeof(SCRIPTDATA));
	LoadScript("data\\model.txt", &g_data);

	for (int nCntMesh = 0; nCntMesh < g_data.nNumModel; nCntMesh++)
	{// モデルの読み込み
		LoadModel(&g_data.aFilenameModel[nCntMesh][0], nCntMesh);
	}

	for (int nCntModel = 0; nCntModel < g_data.nCountModelSet; nCntModel++)
	{// モデルの配置
		SetModel(
			g_data.aInfoModelSet[nCntModel].nType,
			g_data.aInfoModelSet[nCntModel].pos,
			D3DXVECTOR3(
				D3DXToRadian(g_data.aInfoModelSet[nCntModel].rot.x),
				D3DXToRadian(g_data.aInfoModelSet[nCntModel].rot.y),
				D3DXToRadian(g_data.aInfoModelSet[nCntModel].rot.z)
			),
			g_data.aInfoModelSet[nCntModel].collisionType
		);
	}

	// カメラ設定
	CAMERA* pCamera = GetCamera();
	SetCameraPosV(pPlayer->obj.pos);
	SetCameraPosR(pPlayer->obj.pos);
	pCamera->rot.y = pPlayer->obj.rot.y;

	// BGMの再生
	PlaySound(SOUND_LABEL_BGM_GAME);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGame(void)
{
	UninitField();
	UninitPlayer();
	UninitModel();
	UninitEffect();
	UninitShadow();
	UninitCylinder();
	UninitSphere();
	UninitCoin();
	UninitPause();
	UninitPauseBg();
	UninitFont();
	UninitPolygon();
	UninitDecal();
	UninitDebugProc();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
	PLAYER* pPlayer = GetPlayer();
	CAMERA* pCamera = GetCamera();

#if _DEBUG
	if (GetKeyboardTrigger(DIK_F7) && g_bIsPaused)
	{
		g_bPauseHide ^= 1;
	}
	if (GetKeyboardTrigger(DIK_DELETE))
	{
		SetFade(SCENE_GAME);
	}
	if (GetKeyboardTrigger(DIK_F2))
	{
		SetFade(SCENE_TITLE);
	}

	if (GetKeyboardTrigger(DIK_F1))
	{
		pCamera->nMode = pCamera->nMode == 0 ? 2 : 0;
	}
#endif

	if (INPUT_TRIGGER_GAME_PAUSE && GetFadeState() == FADESTATE_NONE && g_gameState != GAMESTATE_CLEAR)
	{
		g_bIsPaused ^= 1;
		TogglePauseGame(g_bIsPaused);
	}

	PrintDebugProc("ポーズ : %d\n", g_bIsPaused);

	if (g_bIsPaused == false)
	{
		UpdateField();
		UpdatePlayer();
		UpdateShadow();
		UpdateModel();
		UpdateEffect();
		UpdateCylinder();
		UpdateSphere();
		UpdateCoin();
		UpdatePolygon();

		sprintf(&g_pFontScore->aText[0], " SCORE : %d", pPlayer->nScore);
		g_pFontScore->obj.pos.y = (0.0f - g_pFontScore->obj.pos.y) * 0.1f;

		for (int i = 0; i < PLYAER_MAX_LIFE; i++)
		{
			if (i < pPlayer->nLife)
			{
				g_pDecalLife[i]->obj.bVisible = true;
			}
			else
			{
				g_pDecalLife[i]->obj.bVisible = false;
			}
		}

		g_nCounterStateGame++;
	}
	else if (g_bIsPaused && g_bPauseHide == false)
	{
		UpdatePause();
		UpdatePauseBg();
	}

	UpdateDebugProc();

	switch (g_gameState)
	{
	case GAMESTATE_CLEAR:
		sprintf(&g_pFontClear->aText[0], "GAME CLEAR\n\nLIFE BONUS : %d", pPlayer->nLife * 500);

		if (INPUT_TRIGGER_UI_CONTINUE || g_nCounterStateGame > 60 * 7)
		{
			SetGameState(GAMESTATE_END);
		}
		break;

	case GAMESTATE_END:
		SetFade(SCENE_RESULT);
		break;

	default:
		break;
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGame(void)
{
	DrawField();
	DrawPlayer();
	DrawSphere();
	DrawCylinder();
	DrawCoin();
	DrawPolygon();
	DrawModel();
	DrawEffect();
	DrawShadow();
	DrawDecal();
	DrawFont();

	if (g_bIsPaused && g_bPauseHide == false)
	{// ポーズ画面描画
		DrawPauseBg();
		DrawPause();
	}

	// デバッグ表示
	DrawDebugProc();
}

//=====================================================================
// マップ情報再読み込み処理
//=====================================================================
void ReloadGameModel(void)
{
	// 再読み込みするオブジェクトの解放
	UninitModel();
	UninitCoin();
	UninitShadow();
	UninitPolygon;

	// 再読み込みするオブジェクトの初期化
	InitModel();
	InitCoin();
	InitShadow();
	InitPolygon();

	// マップ情報のバッファをクリア
	ZeroMemory(&g_data, sizeof(SCRIPTDATA));

	// スクリプトから読み込んでマップ情報を設定
	LoadScript("data\\model.txt", &g_data);

	for (int nCntMesh = 0; nCntMesh < g_data.nNumModel; nCntMesh++)
	{// モデル読み込み
		LoadModel(g_data.aFilenameModel[nCntMesh], nCntMesh);
	}

	for (int nCntModel = 0; nCntModel < g_data.nCountModelSet; nCntModel++)
	{// モデル配置
		SetModel(
			g_data.aInfoModelSet[nCntModel].nType,
			g_data.aInfoModelSet[nCntModel].pos,
			D3DXVECTOR3(
				D3DXToRadian(g_data.aInfoModelSet[nCntModel].rot.x),
				D3DXToRadian(g_data.aInfoModelSet[nCntModel].rot.y),
				D3DXToRadian(g_data.aInfoModelSet[nCntModel].rot.z)
			),
			g_data.aInfoModelSet[nCntModel].collisionType
		);
	}
}

//=====================================================================
// ゲーム状態設定処理
//=====================================================================
void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterStateGame = 0;
}

//=====================================================================
// ポーズ画面切り替え処理
//=====================================================================
void TogglePauseGame(bool bPause)
{
	SetPauseMenuCursor(0);
	g_bIsPaused = bPause;
}

//=====================================================================
// スコア加算処理
//=====================================================================
void AddScore(int nScore)
{
	// スコア加算
	GetPlayer()->nScore += nScore;

	// スコアのテキスト表示を揺らす
	// （ここで下げた後UpdateGameで戻していく）
	g_pFontScore->obj.pos.y = -50.0f;

	// コイン効果音
	PlaySound(SOUND_LABEL_SE_COIN);
}