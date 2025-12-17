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
SCRIPTDATA g_data;
GAMESTATE g_gameState;

//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(void)
{
	InitCamera();
	InitField();
	InitShadow();
	InitPlayer();
	InitModel();
	InitEffect();
	InitCylinder();
	InitSphere();
	InitCoin();

	InitDebugProc();

	g_bIsPaused = false;
	SetGameState(GAMESTATE_NORMAL);

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

	UninitDebugProc();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
	PLAYER* pPlayer = GetPlayer();

#if _DEBUG
	if (GetKeyboardTrigger(DIK_F7))
	{
		ReloadGameModel();
	}
#endif

	if (GetKeyboardTrigger(DIK_DELETE))
	{
		SetFade(SCENE_GAME);
	}
	if (GetKeyboardTrigger(DIK_F2))
	{
		SetFade(SCENE_TITLE);
	}

	if (INPUT_TRIGGER_GAME_PAUSE)
	{
		g_bIsPaused ^= 1;
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
	}

	UpdateDebugProc();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGame(void)
{
	DrawField();
	DrawPlayer();
	DrawModel();
	DrawSphere();
	DrawCylinder();
	DrawEffect();
	DrawCoin();
	DrawShadow();

	DrawDebugProc();
}

void ReloadGameModel(void)
{
	UninitModel();
	UninitCoin();

	InitModel();
	InitCoin();

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
}