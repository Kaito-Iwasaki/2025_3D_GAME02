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

//=====================================================================
// 初期化処理
//=====================================================================
void InitTitle(void)
{
	StopSound();

	InitModel();
	InitSphere();
	InitCylinder();
	InitField();
	InitDecal();

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

	// カメラの設定
	CAMERA* pCamera = GetCamera();
	pCamera->nMode = 1;
	pCamera->rot = D3DXVECTOR3_ZERO;
	pCamera->posOffset = D3DXVECTOR3_ZERO;
	SetCameraPosV(D3DXVECTOR3(0, 5000, 0));
	SetCameraPosR(D3DXVECTOR3(-5000, 0, 5000));
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
	UninitDecal();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateTitle(void)
{
	if (INPUT_TRIGGER_UI_ACCEPT)
	{
		SetFade(SCENE_GAME);
	}

	UpdateModel();
	UpdateSphere();
	UpdateCylinder();
	UpdateField();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawTitle(void)
{
	DrawModel();
	DrawSphere();
	DrawCylinder();
	DrawField();
	DrawDecal();
}