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
#include "DebugProc.h"
#include "wall.h"

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
PLAYER g_player;

const char* aFilenamePart[NUM_PART] = {
	"data\\MODEL\\body.x",
	"data\\MODEL\\head.x",
	"data\\MODEL\\arm_left.x",
	"data\\MODEL\\hand_left.x",
	"data\\MODEL\\arm_right.x",
	"data\\MODEL\\hand_right.x",
};

//static KEY_INFO g_aKeyNetural[] = {
//	{
//		40,
//		{
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//			{50.0f, 18.0f, 0.0f, 0.0f, 0.0f, -0.5f},
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//		}
//	},
//	{
//		40,
//		{
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//			{-50.0f, 18.0f, 0.0f, 0.0f, 0.0f, 0.5f},
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//		}
//	},
//		{
//		40,
//		{
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//			{0.0f, 68.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//			{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f},
//		}
//	},
//};

//=====================================================================
// 初期化処理
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL* pMat;
	MOTION_INFO* pMotionInfo = &g_player.aMotionInfo[0];

	memset(&g_player, 0, sizeof(PLAYER));
	g_player.obj.bVisible = true;
	g_player.nIdxShadow = SetShadow();
	g_player.nNumPart = NUM_PART;
	g_player.obj.pos = D3DXVECTOR3(0.0f, 350.0f, 0.0f);

	for (int nCntPart = 0; nCntPart < NUM_PART; nCntPart++)
	{
		D3DXLoadMeshFromX(
			aFilenamePart[nCntPart],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aPart[nCntPart].pBuffMat,
			NULL,
			&g_player.aPart[nCntPart].dwNumMat,
			&g_player.aPart[nCntPart].pMesh
		);


		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aPart[nCntPart].pBuffMat->GetBufferPointer();

		// テクスチャの読み込み
		for (int i = 0; i < (int)g_player.aPart[nCntPart].dwNumMat; i++)
		{
			if (pMat[i].pTextureFilename != NULL)
			{// テクスチャファイルが存在する
				D3DXCreateTextureFromFile(
					pDevice,
					pMat[i].pTextureFilename,
					&g_player.aPart[nCntPart].apTexture[i]
				);
			}
		}
	}

	// 各パーツの階層構造設定
	g_player.aPart[0].nIdxModelParent = -1;
	g_player.aPart[0].offset.pos = D3DXVECTOR3(0, 20, 0);
	g_player.aPart[0].offset.rot = D3DXVECTOR3(0, 0, 0);

	g_player.aPart[1].nIdxModelParent = 0;
	g_player.aPart[1].offset.pos = D3DXVECTOR3(0, 18, 0);
	g_player.aPart[1].offset.rot = D3DXVECTOR3(0, 0, 0);

	g_player.aPart[2].nIdxModelParent = 0;
	g_player.aPart[2].offset.pos = D3DXVECTOR3(15, 15, 0);
	g_player.aPart[2].offset.rot = D3DXVECTOR3(0, 0, 0);

	g_player.aPart[3].nIdxModelParent = 2;
	g_player.aPart[3].offset.pos = D3DXVECTOR3(6, -25, 0);
	g_player.aPart[3].offset.rot = D3DXVECTOR3(0, 0, 0);

	g_player.aPart[4].nIdxModelParent = 0;
	g_player.aPart[4].offset.pos = D3DXVECTOR3(-15, 15, 0);
	g_player.aPart[4].offset.rot = D3DXVECTOR3(0, 0, 0);

	g_player.aPart[5].nIdxModelParent = 4;
	g_player.aPart[5].offset.pos = D3DXVECTOR3(-6, -25, 0);
	g_player.aPart[5].offset.rot = D3DXVECTOR3(0, 0, 0);

	LoadMotionScript("data\\motion.txt", pMotionInfo);
	SetMotion(MOTIONTYPE_NETURAL);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitPlayer(void)
{
	for (int nCntPart = 0; nCntPart < NUM_PART; nCntPart++)
	{
		for (int i = 0; i < (int)g_player.aPart[nCntPart].dwNumMat; i++)
		{
			if (g_player.aPart[nCntPart].apTexture[i] != NULL)
			{
				g_player.aPart[nCntPart].apTexture[i]->Release();
				g_player.aPart[nCntPart].apTexture[i] = NULL;
			}
		}

		if (g_player.aPart[nCntPart].pMesh != NULL)
		{// メッシュの破棄
			g_player.aPart[nCntPart].pMesh->Release();
			g_player.aPart[nCntPart].pMesh = NULL;
		}

		if (g_player.aPart[nCntPart].pBuffMat != NULL)
		{// マテリアルの破棄
			g_player.aPart[nCntPart].pBuffMat->Release();
			g_player.aPart[nCntPart].pBuffMat = NULL;
		}
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

	// プレイヤー操作
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

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		g_player.move.y = 12.0f;
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
		g_player.obj.pos += dir / fMagnitude * 5.0f;
	}
	g_player.obj.pos.y += g_player.move.y;

	float fRotDiff = GetFixedRotation(g_player.rotMove.y - g_player.obj.rot.y);

	g_player.obj.rot.y = GetFixedRotation(g_player.obj.rot.y + fRotDiff * 0.1f);

	g_player.move.y -= 0.6f;

	SetShadowPosition(g_player.nIdxShadow, D3DXVECTOR3(g_player.obj.pos.x, 0.01f, g_player.obj.pos.z));
	SetShadowSize(g_player.nIdxShadow, D3DXVECTOR3(10.0f, 0.01f, 10.0f) + D3DXVECTOR3(g_player.obj.pos.y * 0.03f, 0.0f, g_player.obj.pos.y * 0.03f));
	SetShadowAlpha(g_player.nIdxShadow, Clampf(0.5f - g_player.obj.pos.y * 0.001f, 0.0f, 0.5f));

	int nKeyNext = (g_player.nKey + 1) % g_player.nNumKey;
	KEY_INFO* currentKeyInfo = &g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey];
	KEY_INFO* nextKeyInfo = &g_player.aMotionInfo[g_player.motionType].aKeyInfo[nKeyNext];

	// 全モデル（パーツの更新）
	for (int nCntPart = 0; nCntPart < NUM_PART; nCntPart++)
	{
		KEY* pCurrentKey = &currentKeyInfo->aKey[nCntPart];
		KEY* pNextKey = &nextKeyInfo->aKey[nCntPart];

		float fRate = (float)g_player.nCounterMotion / (float)currentKeyInfo->nFrame;
		float fFixedRate = fRate;

		float fDiffPosX = pNextKey->fPosX - pCurrentKey->fPosX;
		float fDiffPosY = pNextKey->fPosY - pCurrentKey->fPosY;
		float fDiffPosZ = pNextKey->fPosZ - pCurrentKey->fPosZ;
		float fDiffRotX = GetFixedRotation(pNextKey->fRotX - pCurrentKey->fRotX);
		float fDiffRotY = GetFixedRotation(pNextKey->fRotY - pCurrentKey->fRotY);
		float fDiffRotZ = GetFixedRotation(pNextKey->fRotZ - pCurrentKey->fRotZ);

		g_player.aPart[nCntPart].obj.pos.x = pCurrentKey->fPosX + g_player.aPart[nCntPart].offset.pos.x + fDiffPosX * fFixedRate;
		g_player.aPart[nCntPart].obj.pos.y = pCurrentKey->fPosY + g_player.aPart[nCntPart].offset.pos.y + fDiffPosY * fFixedRate;
		g_player.aPart[nCntPart].obj.pos.z = pCurrentKey->fPosZ + g_player.aPart[nCntPart].offset.pos.z + fDiffPosZ * fFixedRate;
		g_player.aPart[nCntPart].obj.rot.x = GetFixedRotation(currentKeyInfo->aKey[nCntPart].fRotX + g_player.aPart[nCntPart].offset.rot.x + fDiffRotX * fFixedRate);
		g_player.aPart[nCntPart].obj.rot.y = GetFixedRotation(currentKeyInfo->aKey[nCntPart].fRotY + g_player.aPart[nCntPart].offset.rot.y + fDiffRotY * fFixedRate);
		g_player.aPart[nCntPart].obj.rot.z = GetFixedRotation(currentKeyInfo->aKey[nCntPart].fRotZ + g_player.aPart[nCntPart].offset.rot.z + fDiffRotZ * fFixedRate);
	}
	g_player.nCounterMotion++;
	if (g_player.nCounterMotion >= currentKeyInfo->nFrame)
	{
		g_player.nKey = nKeyNext;
		g_player.nCounterMotion = 0;
	}
	PrintDebugProc("\nキーカウント : %d\n", g_player.nCounterMotion);
	PrintDebugProc("現在のキー : %d\n", g_player.nKey);
	PrintDebugProc("合計キー数 : %d\n", g_player.nNumKey);
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

	for (int nCntPart = 0; nCntPart < NUM_PART; nCntPart++)
	{
		D3DXMATRIX mtxParent;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player.aPart[nCntPart].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(
			&mtxRot,
			g_player.aPart[nCntPart].obj.rot.y, g_player.aPart[nCntPart].obj.rot.x, g_player.aPart[nCntPart].obj.rot.z
		);
		D3DXMatrixMultiply(
			&g_player.aPart[nCntPart].mtxWorld,
			&g_player.aPart[nCntPart].mtxWorld,
			&mtxRot
		);

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			g_player.aPart[nCntPart].obj.pos.x, g_player.aPart[nCntPart].obj.pos.y, g_player.aPart[nCntPart].obj.pos.z
		);
		D3DXMatrixMultiply(
			&g_player.aPart[nCntPart].mtxWorld,
			&g_player.aPart[nCntPart].mtxWorld,
			&mtxTrans
		);

		if (g_player.aPart[nCntPart].nIdxModelParent != -1)
		{// 親モデルがあるなら
			mtxParent = g_player.aPart[g_player.aPart[nCntPart].nIdxModelParent].mtxWorld;
		}
		else
		{// 親モデルがないなら
			mtxParent = g_player.mtxWorld;
		}

		// パーツと親のマトリックス掛け合わせる
		D3DXMatrixMultiply(
			&g_player.aPart[nCntPart].mtxWorld,
			&g_player.aPart[nCntPart].mtxWorld,
			&mtxParent
		);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.aPart[nCntPart].mtxWorld);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aPart[nCntPart].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aPart[nCntPart].dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_player.aPart[nCntPart].apTexture[nCntMat]);

			// モデル（パーツ）の描画
			g_player.aPart[nCntPart].pMesh->DrawSubset(nCntMat);
		}
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

PLAYER* GetPlayer(void)
{
	return &g_player;
}

void SetMotion(MOTIONTYPE type)
{
	g_player.motionType = type;
	g_player.nNumKey = g_player.aMotionInfo[type].nNumKey;
	g_player.nKey = 0;
	g_player.nCounterMotion = 0;
	g_player.bLoopMotion = true;
}
