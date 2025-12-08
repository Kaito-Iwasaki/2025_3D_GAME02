//=====================================================================
//
// Test [Test.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Test.h"
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
bool g_bIsPausedTest = false;
SCRIPTDATA g_dataTest;

//=====================================================================
// 初期化処理
//=====================================================================
void InitTest(void)
{
	InitCamera();
	InitField();
	InitShadow();
	InitPlayer();
	InitModel();
	InitCylinder();
	InitSphere();
	InitDebugProc();

	g_bIsPausedTest = false;

	LoadScript("data\\model.txt", &g_dataTest);

	for (int nCntMesh = 0; nCntMesh < g_dataTest.nNumModel; nCntMesh++)
	{
		LoadModel(g_dataTest.aFilenameModel[nCntMesh], nCntMesh);
	}

	for (int nCntModel = 0; nCntModel < g_dataTest.nCountModelSet; nCntModel++)
	{
		SetModel(
			g_dataTest.aInfoModelSet[nCntModel].nType,
			g_dataTest.aInfoModelSet[nCntModel].pos,
			D3DXVECTOR3(
				D3DXToRadian(g_dataTest.aInfoModelSet[nCntModel].rot.x),
				D3DXToRadian(g_dataTest.aInfoModelSet[nCntModel].rot.y),
				D3DXToRadian(g_dataTest.aInfoModelSet[nCntModel].rot.z)
			)
		);
	}
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitTest(void)
{
	UninitField();
	UninitPlayer();
	UninitShadow();
	UninitModel();
	UninitCylinder();
	UninitSphere();
	UninitDebugProc();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateTest(void)
{
	PLAYER* pPlayer = GetPlayer();

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
		g_bIsPausedTest ^= 1;
	}

	PrintDebugProc("ポーズ : %d\n", g_bIsPausedTest);

	if (g_bIsPausedTest == false)
	{
		UpdateField();
		UpdatePlayer();
		UpdateShadow();
		UpdateModel();
		UpdateCylinder();
		UpdateSphere();
	}

	SetCameraPosV(pPlayer->obj.pos + D3DXVECTOR3(0, 50.0f, -50.0f));

	UpdateDebugProc();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawTest(void)
{
	DrawField();
	DrawPlayer();
	DrawModel();
	DrawSphere();
	DrawCylinder();
	DrawShadow();
	DrawDebugProc();
}