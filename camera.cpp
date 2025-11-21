//=====================================================================
//
// カメラ処理 [camera.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "camera.h"
#include "input.h"
#include "util.h"
#include "player.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define CAMERA_SPEED	(5.0f)

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
CAMERA g_camera;

//=====================================================================
// 初期化処理
//=====================================================================
void InitCamera(void)
{
	g_camera.posV = D3DXVECTOR3(0.0f, 150.0f, -100.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//g_camera.fDistance = 500.0f;
	//g_camera.rot = D3DXVECTOR3(0.0f, D3DXToRadian(-45), 0.0f);

	g_camera.fDistance = 300.0f;
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_camera.nCounterState = 0;

}

//=====================================================================
// 終了処理
//=====================================================================
void UninitCamera(void)
{

}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateCamera(void)
{
	PLAYER* pPlayer = GetPlayer();

	if (GetKeyboardPress(DIK_Z))
	{
		g_camera.rot.y += 0.05f;
	}
	if (GetKeyboardPress(DIK_C))
	{
		g_camera.rot.y -= 0.05f;
	}

	g_camera.posRDest.x = pPlayer->obj.pos.x;
	g_camera.posRDest.y = pPlayer->obj.pos.y + 10.0f;
	g_camera.posRDest.z = pPlayer->obj.pos.z + 50.0f;
	g_camera.posVDest.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
	g_camera.posVDest.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;

	g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * 0.1f;
	g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y) * 0.1f;
	g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * 0.1f;
	g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * 0.1f;
	g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * 0.1f;

	//D3DXVECTOR3 vecPlayerMoved = pPlayer->obj.pos - pPlayer->posOld;

	//if (sqrtf(vecPlayerMoved.x * vecPlayerMoved.x + vecPlayerMoved.y * vecPlayerMoved.y) < 0.1f)
	//{
	//	g_camera.nCounterState++;
	//}
	//else
	//{
	//	g_camera.nCounterState = 0;
	//}

	//if (g_camera.nCounterState > 90)
	//{
	//	g_camera.rot.y += GetFixedRotation(pPlayer->obj.rot.y - g_camera.rot.y + D3DX_PI) * 0.05f;
	//}
}

//=====================================================================
// カメラ設定処理
//=====================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(
		&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		10000.0f
	);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスの設定
	D3DXMatrixLookAtLH(
		&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU
	);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//=====================================================================
// カメラ取得処理
//=====================================================================
CAMERA* GetCamera(void)
{
	return &g_camera;
}
