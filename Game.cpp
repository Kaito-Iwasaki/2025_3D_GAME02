//=====================================================================
//
// Game [Game.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Game.h"
#include "field.h"
#include "player.h"
#include "shadow.h"
#include "wall.h"
#include "model_loader.h"
#include "billboard.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "input.h"
#include "model.h"
#include "meshfield.h"
#include "cylinder.h"
#include "sphere.h"
#include "DebugProc.h"
#include "fade.h"
#include "camera.h"
#include "sound.h"
#include "coin.h"
#include "pauseBg.h"
#include "pause.h"
#include "font.h"
#include "line.h"
#include "polygon.h"

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
bool g_bIsPaused = false;
bool g_bPauseHide = false;
SCRIPTDATA g_data;
GAMESTATE g_gameState;
FONT* g_pFontScore = NULL;
FONT* g_pFontClear = NULL;
int g_nCounterStateGame;

//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(void)
{
	PLAYER* pPlayer = GetPlayer();

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

	InitDebugProc();

	g_bIsPaused = false;
	g_bPauseHide = false;
	SetGameState(GAMESTATE_NORMAL);
	g_pFontScore = SetFont(FONT_LABEL_DONGURI,
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f),
		50,
		"",
		DT_LEFT | DT_TOP
	);
	g_pFontClear = SetFont(FONT_LABEL_DONGURI,
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f),
		100,
		"",
		DT_CENTER | DT_VCENTER
	);

	ZeroMemory(&g_data, sizeof(SCRIPTDATA));
	LoadScript("data\\model.txt", &g_data);

	for (int nCntMesh = 0; nCntMesh < g_data.nNumModel; nCntMesh++)
	{
		LoadModel(&g_data.aFilenameModel[nCntMesh][0], nCntMesh);
	}

	for (int nCntModel = 0; nCntModel < g_data.nCountModelSet; nCntModel++)
	{
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

	SetCameraPosV(pPlayer->obj.pos);
	SetCameraPosR(pPlayer->obj.pos);
	GetCamera()->rot.y = pPlayer->obj.rot.y;

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

	if (INPUT_TRIGGER_GAME_PAUSE)
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

		sprintf(&g_pFontScore->aText[0], "SCORE : %d", pPlayer->nScore);
		g_pFontScore->obj.pos.y = (0.0f - g_pFontScore->obj.pos.y) * 0.1f;

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
		sprintf(&g_pFontClear->aText[0], "GAME CLEAR");

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
	DrawFont();

	if (g_bIsPaused && g_bPauseHide == false)
	{
		DrawPauseBg();
		DrawPause();
	}

	DrawDebugProc();
}

void ReloadGameModel(void)
{
	UninitModel();
	UninitCoin();
	UninitShadow();
	UninitPolygon;

	InitModel();
	InitCoin();
	InitShadow();
	InitPolygon();

	ZeroMemory(&g_data, sizeof(SCRIPTDATA));

	LoadScript("data\\model.txt", &g_data);

	for (int nCntMesh = 0; nCntMesh < g_data.nNumModel; nCntMesh++)
	{
		LoadModel(g_data.aFilenameModel[nCntMesh], nCntMesh);
	}

	for (int nCntModel = 0; nCntModel < g_data.nCountModelSet; nCntModel++)
	{
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

void SetGameState(GAMESTATE state)
{
	g_gameState = state;
	g_nCounterStateGame = 0;
}

void TogglePauseGame(bool bPause)
{
	SetPauseMenuCursor(0);
	g_bIsPaused = bPause;
}

void AddScore(int nScore)
{
	GetPlayer()->nScore += nScore;
	g_pFontScore->obj.pos.y = -50.0f;
	PlaySound(SOUND_LABEL_SE_COIN);
}