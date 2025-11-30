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
#include "script_loader.h"
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

//=====================================================================
// 初期化処理
//=====================================================================
void InitGame(void)
{
	InitCamera();
	InitField();
	InitMeshField();
	InitWall();
	InitShadow();
	InitPlayer();
	InitBillboard();
	InitBullet();
	InitExplosion();
	InitEffect();
	InitModel();
	//InitCylinder();
	InitSphere();
	InitDebugProc();

	g_bIsPaused = false;

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
			D3DXVECTOR3(D3DXToRadian(g_data.aInfoModelSet[nCntModel].rot.x), D3DXToRadian(g_data.aInfoModelSet[nCntModel].rot.y), D3DXToRadian(g_data.aInfoModelSet[nCntModel].rot.z))
		);
	}
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGame(void)
{
	UninitField();
	UninitMeshField();
	UninitWall();
	UninitPlayer();
	UninitShadow();
	UninitBillboard();
	UninitBullet();
	UninitExplosion();
	UninitEffect();
	UninitModel();
	//UninitCylinder();
	UninitSphere();
	UninitDebugProc();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_DELETE))
	{
		SetFade(SCENE_GAME);
	}
	if (GetKeyboardTrigger(DIK_F2))
	{
		SetFade(SCENE_TITLE);
	}

	if (GetKeyboardTrigger(DIK_P))
	{
		g_bIsPaused ^= 1;
	}

	PrintDebugProc("ポーズ : %d\n", g_bIsPaused);

	if (g_bIsPaused == false)
	{
		UpdateField();
		UpdateMeshField();
		UpdatePlayer();
		UpdateWall();
		UpdateShadow();
		UpdateBillboard();
		UpdateBullet();
		UpdateExplosion();
		UpdateEffect();
		UpdateModel();
		//UpdateCylinder();
		UpdateSphere();
	}

	UpdateDebugProc();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGame(void)
{
	DrawField();
	DrawMeshField();
	DrawWall();
	DrawPlayer();
	DrawShadow();
	DrawBillboard();
	DrawBullet();
	DrawExplosion();
	DrawEffect();
	DrawModel();
	//DrawCylinder();
	DrawSphere();
	DrawDebugProc();
}