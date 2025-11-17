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
	//InitField();
	InitMeshField();
	InitWall();
	InitShadow();
	InitPlayer();
	InitBillboard();
	InitBullet();
	InitExplosion();
	InitEffect();
	InitModel();

	g_bIsPaused = false;

	LoadScript("data\\model.txt", &g_data);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitGame(void)
{
	//UninitField();
	UninitMeshField();
	UninitWall();
	UninitPlayer();
	UninitShadow();
	UninitBillboard();
	UninitBullet();
	UninitExplosion();
	UninitEffect();
	UninitModel();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P))
	{
		g_bIsPaused ^= 1;
	}

	if (g_bIsPaused == false)
	{
		//UpdateField();
		UpdateMeshField();
		UpdateWall();
		UpdatePlayer();
		UpdateShadow();
		UpdateBillboard();
		UpdateBullet();
		UpdateExplosion();
		UpdateEffect();
		UpdateModel();
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawGame(void)
{
	//DrawField();
	DrawMeshField();
	//DrawWall();
	DrawPlayer();
	DrawShadow();
	DrawBillboard();
	DrawBullet();
	DrawExplosion();
	DrawEffect();
	DrawModel();
}