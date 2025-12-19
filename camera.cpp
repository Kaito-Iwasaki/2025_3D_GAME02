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
#define CAMERA_SPEED	(10.0f)

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
	PLAYER* pPlayer = GetPlayer();

	g_camera.posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.fDistance = 1100.0f;
	g_camera.rot = D3DXVECTOR3(0.0f, D3DXToRadian(-90), 0.0f);
	g_camera.bEnabled = true;

	g_camera.nCounterState = 0;

	g_camera.nMode = 0;
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
	int nCamSpeed = GetKeyboardPress(DIK_LSHIFT) ? CAMERA_SPEED * 5 : CAMERA_SPEED;

	if (g_camera.bEnabled == false) return;

	switch (g_camera.nMode)
	{
	case 0:
		g_camera.posRDest.x = pPlayer->obj.pos.x + g_camera.posOffset.x;
		g_camera.posRDest.y = pPlayer->obj.pos.y + g_camera.posOffset.y;
		g_camera.posRDest.z = pPlayer->obj.pos.z + g_camera.posOffset.z;
		g_camera.posVDest.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posVDest.y = g_camera.posRDest.y + 100.0f;
		g_camera.posVDest.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;

		g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * 0.1f;
		g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y) * 0.1f;
		g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * 0.1f;
		g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * 0.1f;
		g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y) * 0.1f;
		g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * 0.1f;

		break;

	case 1:
		// それぞれのプログラムに処理を一任する
		break;

	case 2:

		if (GetKeyboardPress(DIK_R))
		{
			g_camera.posR.y += nCamSpeed;
		}
		if (GetKeyboardPress(DIK_F))
		{
			g_camera.posR.y -= nCamSpeed;
		}

		if (GetKeyboardPress(DIK_Z))
		{
			g_camera.rot.y -= 0.05f;
		}
		if (GetKeyboardPress(DIK_C))
		{
			g_camera.rot.y += 0.05f;
		}

		if (GetKeyboardPress(DIK_Q))
		{
			g_camera.posR.y -= nCamSpeed;
			g_camera.posV.y -= nCamSpeed;
		}
		if (GetKeyboardPress(DIK_E))
		{
			g_camera.posR.y += nCamSpeed;
			g_camera.posV.y += nCamSpeed;
		}

		if (GetKeyboardPress(DIK_W))
		{
			g_camera.posR.x += sinf(g_camera.rot.y) * nCamSpeed;
			g_camera.posR.z += cosf(g_camera.rot.y) * nCamSpeed;
		}
		if (GetKeyboardPress(DIK_S))
		{
			g_camera.posR.x -= sinf(g_camera.rot.y) * nCamSpeed;
			g_camera.posR.z -= cosf(g_camera.rot.y) * nCamSpeed;
		}
		if (GetKeyboardPress(DIK_A))
		{
			g_camera.posR.x -= cosf(g_camera.rot.y) * nCamSpeed;
			g_camera.posR.z += sinf(g_camera.rot.y) * nCamSpeed;
		}
		if (GetKeyboardPress(DIK_D))
		{
			g_camera.posR.x += cosf(g_camera.rot.y) * nCamSpeed;
			g_camera.posR.z -= sinf(g_camera.rot.y) * nCamSpeed;
		}

		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;

		break;

	case 3:
		g_camera.rot.y += 0.01f;

		g_camera.posVDest.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posVDest.y = g_camera.posRDest.y + g_camera.posOffset.y;
		g_camera.posVDest.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;

		g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * 0.1f;
		g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y) * 0.1f;
		g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * 0.1f;
		g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * 0.1f;
		g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y) * 0.1f;
		g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * 0.1f;
	}
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
		1000000.0f
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

void SetCameraPosV(D3DXVECTOR3 pos)
{
	g_camera.posV = pos;
	g_camera.posVDest = pos;
}

void SetCameraPosR(D3DXVECTOR3 pos)
{
	g_camera.posR = pos;
	g_camera.posRDest = pos;
}