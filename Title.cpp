//=====================================================================
//
// Title [Title.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "Title.h"
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
SCRIPTDATA g_dataTitle;
FONT* g_pFontTitle = NULL;
int g_nCounterStateTitle;

//=====================================================================
// 初期化処理
//=====================================================================
void InitTitle(void)
{
	StopSound();

	InitCamera();
	InitModel();
	InitSphere();
	InitCylinder();
	InitField();
	InitPolygon();
	InitDecal();
	InitFont();

	ZeroMemory(&g_dataTitle, sizeof(g_dataTitle));
	LoadScript("data\\model.txt", &g_dataTitle);

	// メッシュの読み込み
	for (int nCntMesh = 0; nCntMesh < g_dataTitle.nNumModel; nCntMesh++)
	{
		LoadModel(&g_dataTitle.aFilenameModel[nCntMesh][0], nCntMesh);
	}

	// メッシュの配置
	for (int nCntModel = 0; nCntModel < g_dataTitle.nCountModelSet; nCntModel++)
	{
		SetModel(
			g_dataTitle.aInfoModelSet[nCntModel].nType,
			g_dataTitle.aInfoModelSet[nCntModel].pos,
			D3DXVECTOR3(
				D3DXToRadian(g_dataTitle.aInfoModelSet[nCntModel].rot.x),
				D3DXToRadian(g_dataTitle.aInfoModelSet[nCntModel].rot.y),
				D3DXToRadian(g_dataTitle.aInfoModelSet[nCntModel].rot.z)
			),
			g_dataTitle.aInfoModelSet[nCntModel].collisionType
		);
	}

	SetDecal(
		DECAL_LABEL_LOGO,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0),
		D3DXVECTOR3(900, 400, 0),
		D3DXVECTOR3(0, 0, 0),
		D3DXCOLOR_WHITE
	);

	g_pFontTitle = SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3(0,SCREEN_HEIGHT - 170,0),
		D3DXVECTOR3(SCREEN_WIDTH, 100, 0),
		D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f),
		80,
		"PRESS ENTER OR START BUTTON",
		DT_CENTER | DT_VCENTER
	);

	g_nCounterStateTitle = 0;
	PlaySound(SOUND_LABEL_BGM_TITLE);

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
void UninitTitle(void)
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
void UpdateTitle(void)
{
	if (INPUT_TRIGGER_UI_CONTINUE)
	{
		SetFade(SCENE_GAME);
	}

	UpdateModel();
	UpdateSphere();
	UpdateCylinder();
	UpdateField();
	UpdatePolygon();

	if (g_nCounterStateTitle % 30 == 0)
	{
		g_pFontTitle->obj.bVisible ^= 1;
	}

	if (g_nCounterStateTitle > 60 * 20)
	{
		SetFade(SCENE_RANKING);
	}

	g_nCounterStateTitle++;
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawTitle(void)
{
	DrawSphere();
	DrawCylinder();
	DrawField();
	DrawPolygon();
	DrawModel();
	DrawDecal();
	DrawFont();
}