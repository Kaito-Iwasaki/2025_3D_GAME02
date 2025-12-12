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

	LoadScript("data\\model.txt", &g_dataTitle);

	for (int nCntMesh = 0; nCntMesh < g_dataTitle.nNumModel; nCntMesh++)
	{
		LoadModel(g_dataTitle.aFilenameModel[nCntMesh], nCntMesh);
	}

	for (int nCntModel = 0; nCntModel < g_dataTitle.nCountModelSet; nCntModel++)
	{
		SetModel(
			g_dataTitle.aInfoModelSet[nCntModel].nType,
			g_dataTitle.aInfoModelSet[nCntModel].pos,
			D3DXVECTOR3(
				D3DXToRadian(g_dataTitle.aInfoModelSet[nCntModel].rot.x),
				D3DXToRadian(g_dataTitle.aInfoModelSet[nCntModel].rot.y),
				D3DXToRadian(g_dataTitle.aInfoModelSet[nCntModel].rot.z)
			)
		);
	}

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
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateTitle(void)
{
	if (GetKeyboardTrigger(DIK_RETURN))
	{
		SetFade(SCENE_GAME);
	}

	UpdateModel();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawTitle(void)
{
	DrawModel();
}