//=====================================================================
//
// Player [player.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "player.h"
#include "baseObject.h"
#include "input.h"
#include "util.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "sound.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_TEXTURE		(8)

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
LPD3DXMESH g_pMeshPlayer = NULL;			// メッシュ（頂点情報）へのポインタ
LPDIRECT3DTEXTURE9 g_apTexturePlayer[MAX_TEXTURE] = {};
LPD3DXBUFFER g_pBuffMatPlayer = NULL;	// マテリアルへのポインタ
DWORD g_dwNumMatPlayer = 0;				// マテリアル数
PLAYER g_player;

//=====================================================================
// 初期化処理
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	memset(&g_player, 0, sizeof(PLAYER));
	g_player.obj.bVisible = true;

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data\\MODEL\\Perfect_Animal.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatPlayer,
		NULL,
		&g_dwNumMatPlayer,
		&g_pMeshPlayer
	);

	D3DXMATERIAL* pMat;

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatPlayer->GetBufferPointer();

	// テクスチャの読み込み
	for (int i = 0; i < (int)g_dwNumMatPlayer; i++)
	{
		if (pMat[i].pTextureFilename != NULL)
		{// テクスチャファイルが存在する
			D3DXCreateTextureFromFile(
				pDevice,
				pMat[i].pTextureFilename,
				&g_apTexturePlayer[i]
			);
		}
	}

	g_player.nIdxShadow = SetShadow();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitPlayer(void)
{
	for (int i = 0; i < (int)g_dwNumMatPlayer; i++)
	{
		if (g_apTexturePlayer[i] != NULL)
		{
			g_apTexturePlayer[i]->Release();
			g_apTexturePlayer[i] = NULL;
		}
	}

	if (g_pMeshPlayer != NULL)
	{// メッシュの破棄
		g_pMeshPlayer->Release();
		g_pMeshPlayer = NULL;
	}

	if (g_pBuffMatPlayer != NULL)
	{// マテリアルの破棄
		g_pBuffMatPlayer->Release();
		g_pBuffMatPlayer = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdatePlayer(void)
{
	CAMERA* pCamera = GetCamera();

	D3DXVECTOR3 dir = D3DXVECTOR3_ZERO;
	D3DXVECTOR3 dirPad = D3DXVECTOR3_ZERO;
	XINPUT_STATE* joypad = GetJoypad();

	g_player.posOld = g_player.obj.pos;

	// カメラ移動
	if (GetKeyboardPress(DIK_A))
	{// 左
		dir += D3DXVECTOR3(
			-cosf(pCamera->rot.y),
			0.0f,
			sinf(pCamera->rot.y)
		);
	}
	if (GetKeyboardPress(DIK_D))
	{// 右
		dir -= D3DXVECTOR3(
			-cosf(pCamera->rot.y),
			0.0f,
			sinf(pCamera->rot.y)
		);
	}
	if (GetKeyboardPress(DIK_W))
	{// 前
		dir -= D3DXVECTOR3(
			sinf(pCamera->rot.y + D3DX_PI),
			0.0f,
			cosf(pCamera->rot.y + D3DX_PI)
		);
	}
	if (GetKeyboardPress(DIK_S))
	{// 後方
		dir -= D3DXVECTOR3(
			sinf(pCamera->rot.y),
			0.0f,
			cosf(pCamera->rot.y)
		);
	}

	if (GetKeyboardTrigger(DIK_SPACE))
	{
		SetBullet(
			g_player.obj.pos + D3DXVECTOR3(0.0f, 10.0f, 0.0f),
			D3DXVECTOR3(sinf(g_player.obj.rot.y), 0.0f, cosf(g_player.obj.rot.y)) * 10.0f
		);
		PlaySound(SOUND_LABEL_SE_SHOOT);
	}

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		g_player.move.y = 10.0f;
	}

	dirPad.x = joypad->Gamepad.sThumbLX;
	dirPad.z = joypad->Gamepad.sThumbLY;

	float joypadMagnitude = sqrtf(dirPad.x * dirPad.x + dirPad.z * dirPad.z);

	if (joypadMagnitude != 0 && joypadMagnitude > INPUT_DEADZONE)
	{
		dirPad.x = (float)joypad->Gamepad.sThumbLX / (INPUT_MAX_MAGNITUDE - INPUT_DEADZONE);
		dirPad.z = (float)joypad->Gamepad.sThumbLY / (INPUT_MAX_MAGNITUDE - INPUT_DEADZONE);
		dir = dirPad;
	}

	float fMagnitude = sqrtf(dir.x * dir.x + dir.z * dir.z);

	if (fMagnitude != 0)
	{
		g_player.rotMove.y = atan2f(dir.x, dir.z) + D3DX_PI;
		g_player.obj.pos += dir / fMagnitude * 1.5f;
	}
	g_player.obj.pos.y += g_player.move.y;

	float fRotDiff = GetFixedRotation(g_player.rotMove.y - g_player.obj.rot.y);

	g_player.obj.rot.y = GetFixedRotation(g_player.obj.rot.y + fRotDiff * 0.1f);

	g_player.move.y -= 0.6f;
	Clampf(&g_player.obj.pos.y, 0.0f, g_player.obj.pos.y);

	SetShadowPosition(g_player.nIdxShadow, D3DXVECTOR3(g_player.obj.pos.x, 0.01f, g_player.obj.pos.z));
	SetShadowSize(g_player.nIdxShadow, D3DXVECTOR3(10.0f, 0.01f, 10.0f) + D3DXVECTOR3(g_player.obj.pos.y * 0.03f, 0.0f, g_player.obj.pos.y * 0.03f));
	SetShadowAlpha(g_player.nIdxShadow, Clampf(0.5f - g_player.obj.pos.y * 0.001f, 0.0f, 0.5f));
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;				// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(
		&mtxRot,
		g_player.obj.rot.y, g_player.obj.rot.x, g_player.obj.rot.z
	);
	D3DXMatrixMultiply(
		&g_player.mtxWorld,
		&g_player.mtxWorld,
		&mtxRot
	);

	// 位置を反映
	D3DXMatrixTranslation(
		&mtxTrans,
		g_player.obj.pos.x, g_player.obj.pos.y, g_player.obj.pos.z
	);
	D3DXMatrixMultiply(
		&g_player.mtxWorld,
		&g_player.mtxWorld,
		&mtxTrans
	);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatPlayer; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_apTexturePlayer[nCntMat]);

		// モデル（パーツ）の描画
		g_pMeshPlayer->DrawSubset(nCntMat);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

PLAYER* GetPlayer(void)
{
	return &g_player;
}
