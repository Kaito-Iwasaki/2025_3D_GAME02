//=====================================================================
//
// Result [Result.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Result.h"
#include "input.h"
#include "fade.h"
#include "model.h"
#include "camera.h"
#include "light.h"
#include "util.h"
#include "model_loader.h"
#include "sound.h"
#include "sphere.h"
#include "cylinder.h"
#include "field.h"
#include "decal.h"
#include "font.h"
#include "player.h"
#include "polygon.h"
#include "Ranking.h"

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
SCRIPTDATA g_dataResult;
FONT* g_pFontResult = NULL;
int g_nCounterStateResult;

//=====================================================================
// 初期化処理
//=====================================================================
void InitResult(void)
{
	PLAYER* pPlayer = GetPlayer();

	StopSound();

	InitCamera();
	InitModel();
	InitSphere();
	InitCylinder();
	InitField();
	InitPolygon();
	InitDecal();
	InitFont();

	ZeroMemory(&g_dataResult, sizeof(g_dataResult));
	LoadScript("data\\model.txt", &g_dataResult);

	// メッシュの読み込み
	for (int nCntMesh = 0; nCntMesh < g_dataResult.nNumModel; nCntMesh++)
	{
		LoadModel(&g_dataResult.aFilenameModel[nCntMesh][0], nCntMesh);
	}

	// メッシュの配置
	for (int nCntModel = 0; nCntModel < g_dataResult.nCountModelSet; nCntModel++)
	{
		SetModel(
			g_dataResult.aInfoModelSet[nCntModel].nType,
			g_dataResult.aInfoModelSet[nCntModel].pos,
			D3DXVECTOR3(
				D3DXToRadian(g_dataResult.aInfoModelSet[nCntModel].rot.x),
				D3DXToRadian(g_dataResult.aInfoModelSet[nCntModel].rot.y),
				D3DXToRadian(g_dataResult.aInfoModelSet[nCntModel].rot.z)
			),
			g_dataResult.aInfoModelSet[nCntModel].collisionType
		);
	}

	SetDecal(
		DECAL_LABEL_NULL,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXVECTOR3(0, 0, 0),
		D3DXCOLOR(0, 0, 0, 0.5f)
	);

	g_pFontResult = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f),
		120,
		"",
		DT_CENTER | DT_VCENTER
	);

	if (pPlayer->currentState == PLAYERSTATE_CLEAR)
	{
		sprintf(&g_pFontResult->aText[0], "GAME CLEAR\n\n YOUR SCORE\n%d", pPlayer->nScore);
	}
	else
	{
		sprintf(&g_pFontResult->aText[0], "GAME OVER\n\n YOUR SCORE\n%d", pPlayer->nScore);
	}

	SaveScore(pPlayer->nScore);

	g_nCounterStateResult = 0;
	PlaySound(SOUND_LABEL_BGM_RESULT);

	// カメラの設定
	CAMERA* pCamera = GetCamera();
	pCamera->nMode = 3;
	pCamera->posOffset = D3DXVECTOR3(0, 1000, 0);
	pCamera->fDistance = 3000;
	SetCameraPosR(D3DXVECTOR3(-5000, 3000, 2000));
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitResult(void)
{
	UninitModel();
	UninitSphere();
	UninitCylinder();
	UninitField();
	UninitPolygon();
	UninitDecal();
	UninitFont();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateResult(void)
{
	if (INPUT_TRIGGER_UI_CONTINUE || g_nCounterStateResult > 60 * 8)
	{
		SetFade(SCENE_RANKING);
	}

	UpdateModel();
	UpdateSphere();
	UpdateCylinder();
	UpdateField();
	UpdatePolygon();

	g_nCounterStateResult++;
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawResult(void)
{
	DrawSphere();
	DrawCylinder();
	DrawField();
	DrawPolygon();
	DrawModel();
	DrawDecal();
	DrawFont();
}