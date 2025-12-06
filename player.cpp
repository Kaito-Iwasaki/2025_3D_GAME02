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

//=====================================================================
// 初期化処理
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL* pMat;
	MOTION* pMotion = &g_player.motion;

	memset(&g_player, 0, sizeof(PLAYER));
	g_player.obj.bVisible = true;
	g_player.nIdxShadow = SetShadow();
	g_player.obj.pos = D3DXVECTOR3(0.0f, 350.0f, 0.0f);

	LoadMotionScript("data\\motion.txt", &g_player.motion);

	for (int nCntPart = 0; nCntPart < pMotion->nNumPart; nCntPart++)
	{
		int nIdxPart = pMotion->aPart[nCntPart].nIdxPart;

		D3DXLoadMeshFromX(
			&pMotion->aPartFilename[nIdxPart][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&pMotion->aPart[nCntPart].pBuffMat,
			NULL,
			&pMotion->aPart[nCntPart].dwNumMat,
			&pMotion->aPart[nCntPart].pMesh
		);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pMotion->aPart[nCntPart].pBuffMat->GetBufferPointer();

		// テクスチャの読み込み
		for (int i = 0; i < (int)pMotion->aPart[nCntPart].dwNumMat; i++)
		{
			if (pMat[i].pTextureFilename != NULL)
			{// テクスチャファイルが存在する
				D3DXCreateTextureFromFile(
					pDevice,
					pMat[i].pTextureFilename,
					&pMotion->aPart[nCntPart].apTexture[i]
				);
			}
		}
	}

	// 各パーツの階層構造設定
	//pMotion->aPart[0].nIdxModelParent = -1;
	//pMotion->aPart[0].offset.pos = D3DXVECTOR3(0, 20, 0);
	//pMotion->aPart[0].offset.rot = D3DXVECTOR3(0, 0, 0);

	//pMotion->aPart[1].nIdxModelParent = 0;
	//pMotion->aPart[1].offset.pos = D3DXVECTOR3(0, 18, 0);
	//pMotion->aPart[1].offset.rot = D3DXVECTOR3(0, 0, 0);

	//pMotion->aPart[2].nIdxModelParent = 0;
	//pMotion->aPart[2].offset.pos = D3DXVECTOR3(15, 15, 0);
	//pMotion->aPart[2].offset.rot = D3DXVECTOR3(0, 0, 0);

	//pMotion->aPart[3].nIdxModelParent = 2;
	//pMotion->aPart[3].offset.pos = D3DXVECTOR3(6, -25, 0);
	//pMotion->aPart[3].offset.rot = D3DXVECTOR3(0, 0, 0);

	//pMotion->aPart[4].nIdxModelParent = 0;
	//pMotion->aPart[4].offset.pos = D3DXVECTOR3(-15, 15, 0);
	//pMotion->aPart[4].offset.rot = D3DXVECTOR3(0, 0, 0);

	//pMotion->aPart[5].nIdxModelParent = 4;
	//pMotion->aPart[5].offset.pos = D3DXVECTOR3(-6, -25, 0);
	//pMotion->aPart[5].offset.rot = D3DXVECTOR3(0, 0, 0);

	SetMotion(MOTIONTYPE_MOVE, false, 30);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitPlayer(void)
{
	MOTION* pMotion = &g_player.motion;

	for (int nCntPart = 0; nCntPart < pMotion->nNumPart; nCntPart++)
	{
		for (int i = 0; i < (int)pMotion->aPart[nCntPart].dwNumMat; i++)
		{
			if (pMotion->aPart[nCntPart].apTexture[i] != NULL)
			{
				pMotion->aPart[nCntPart].apTexture[i]->Release();
				pMotion->aPart[nCntPart].apTexture[i] = NULL;
			}
		}

		if (pMotion->aPart[nCntPart].pMesh != NULL)
		{// メッシュの破棄
			pMotion->aPart[nCntPart].pMesh->Release();
			pMotion->aPart[nCntPart].pMesh = NULL;
		}

		if (pMotion->aPart[nCntPart].pBuffMat != NULL)
		{// マテリアルの破棄
			pMotion->aPart[nCntPart].pBuffMat->Release();
			pMotion->aPart[nCntPart].pBuffMat = NULL;
		}
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdatePlayer(void)
{
	CAMERA* pCamera = GetCamera();
	MOTION* pMotion = &g_player.motion;
	MOTION_INFO* pMotionInfo = &pMotion->aMotionInfo[0];

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
		g_player.move.y = 14.0f;
	}

	if (GetKeyboardTrigger(DIK_R))
	{
		SetMotion(MOTIONTYPE_NETURAL, true, 10);
	}
	else if (GetKeyboardTrigger(DIK_T))
	{
		SetMotion(MOTIONTYPE_MOVE, true, 10);
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
	SetShadowSize(g_player.nIdxShadow, D3DXVECTOR3(40.0f, 0.01f, 40.0f) + D3DXVECTOR3(g_player.obj.pos.y * 0.08f, 0.0f, g_player.obj.pos.y * 0.08f));
	SetShadowAlpha(g_player.nIdxShadow, Clampf(0.5f - g_player.obj.pos.y * 0.001f, 0.0f, 0.5f));

	if (fMagnitude != 0 && GetCurrentPlayerMotion() != MOTIONTYPE_NETURAL)
	{
		SetMotion(MOTIONTYPE_NETURAL, true, 10);
	}
	else if (fMagnitude == 0 && GetCurrentPlayerMotion() != MOTIONTYPE_MOVE)
	{
		SetMotion(MOTIONTYPE_MOVE, true, 10);
	}

	int nKeyNext = (g_player.nKey + 1) % g_player.nNumKey;
	KEY_INFO* currentKeyInfo = &pMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey];
	KEY_INFO* nextKeyInfo = &pMotionInfo[g_player.motionType].aKeyInfo[nKeyNext];

	// 全モデル（パーツの更新）
	for (int nCntPart = 0; nCntPart < pMotion->nNumPart; nCntPart++)
	{
		KEY* pCurrentKey = &currentKeyInfo->aKey[nCntPart];
		KEY* pNextKey = &nextKeyInfo->aKey[nCntPart];
		PART* pPart = &pMotion->aPart[nCntPart];
		float fRate = (float)g_player.nCounterMotion / (float)currentKeyInfo->nFrame;

		float fDiffPosX = pNextKey->fPosX - pCurrentKey->fPosX;
		float fDiffPosY = pNextKey->fPosY - pCurrentKey->fPosY;
		float fDiffPosZ = pNextKey->fPosZ - pCurrentKey->fPosZ;
		float fDiffRotX = GetFixedRotation(pNextKey->fRotX - pCurrentKey->fRotX);
		float fDiffRotY = GetFixedRotation(pNextKey->fRotY - pCurrentKey->fRotY);
		float fDiffRotZ = GetFixedRotation(pNextKey->fRotZ - pCurrentKey->fRotZ);

		float fRotX = GetFixedRotation(pCurrentKey->fRotX + pPart->offset.rot.x + fDiffRotX * fRate);
		float fRotY = GetFixedRotation(pCurrentKey->fRotY + pPart->offset.rot.y + fDiffRotY * fRate);
		float fRotZ = GetFixedRotation(pCurrentKey->fRotZ + pPart->offset.rot.z + fDiffRotZ * fRate);

		if (g_player.bBlendMotion)
		{
			int nKeyNextBlend = (g_player.nKeyBlend + 1) % g_player.nNumKeyBlend;
			KEY_INFO* currentKeyInfoBlend = &pMotionInfo[g_player.motionTypeBlend].aKeyInfo[g_player.nKeyBlend];
			KEY_INFO* nextKeyInfoBlend = &pMotionInfo[g_player.motionTypeBlend].aKeyInfo[nKeyNextBlend];
			float fRateKeyBlend = (float)g_player.nCounterMotionBlend / (float)currentKeyInfoBlend->nFrame;
			float fRateBlend = (float)g_player.nCounterBlend / (float)g_player.nFrameBlend;

			float fRotDiffXBlend = GetFixedRotation(nextKeyInfoBlend->aKey[nCntPart].fRotX - currentKeyInfoBlend->aKey[nCntPart].fRotX);
			float fRotXBlend = GetFixedRotation(currentKeyInfoBlend->aKey[nCntPart].fRotX + pPart->offset.rot.x + fRotDiffXBlend * fRateKeyBlend);
			float fDiffBlendX = GetFixedRotation(fRotXBlend - fRotX);
			
			float fRotDiffYBlend = GetFixedRotation(nextKeyInfoBlend->aKey[nCntPart].fRotY - currentKeyInfoBlend->aKey[nCntPart].fRotY);
			float fRotYBlend = GetFixedRotation(currentKeyInfoBlend->aKey[nCntPart].fRotY + pPart->offset.rot.y + fRotDiffYBlend * fRateKeyBlend);
			float fDiffBlendY = GetFixedRotation(fRotYBlend - fRotY);

			float fRotDiffZBlend = GetFixedRotation(nextKeyInfoBlend->aKey[nCntPart].fRotZ - currentKeyInfoBlend->aKey[nCntPart].fRotZ);
			float fRotZBlend = GetFixedRotation(currentKeyInfoBlend->aKey[nCntPart].fRotZ + pPart->offset.rot.z + fRotDiffZBlend * fRateKeyBlend);
			float fDiffBlendZ = GetFixedRotation(fRotZBlend - fRotZ);

			pPart->obj.rot.x = GetFixedRotation(fRotX + pPart->offset.rot.x + fDiffBlendX * fRateBlend);
			pPart->obj.rot.y = GetFixedRotation(fRotY + pPart->offset.rot.y + fDiffBlendY * fRateBlend);
			pPart->obj.rot.z = GetFixedRotation(fRotZ + pPart->offset.rot.z + fDiffBlendZ * fRateBlend);
		}
		else
		{
			pPart->obj.pos.x = pCurrentKey->fPosX + pMotion->aPart[nCntPart].offset.pos.x + fDiffPosX * fRate;
			pPart->obj.pos.y = pCurrentKey->fPosY + pMotion->aPart[nCntPart].offset.pos.y + fDiffPosY * fRate;
			pPart->obj.pos.z = pCurrentKey->fPosZ + pMotion->aPart[nCntPart].offset.pos.z + fDiffPosZ * fRate;
			pPart->obj.rot.x = fRotX;
			pPart->obj.rot.y = fRotY;
			pPart->obj.rot.z = fRotZ;
		}
	}

	if (g_player.bBlendMotion)
	{// ブレンドモーションが再生中なら
		// ブレンドモーションのカウンタを進める
		g_player.nCounterMotionBlend++;

		// プレイヤーのブレンドモーションのフレームが
		if (g_player.nCounterMotionBlend >= pMotionInfo[g_player.motionTypeBlend].aKeyInfo[g_player.nKeyBlend].nFrame)
		{
			g_player.nKeyBlend = (g_player.nKeyBlend + 1) % g_player.nNumKeyBlend;
			g_player.nCounterMotionBlend = 0;
		}

		// ブレンド状態のカウンタを進めて、
		// カウンタが一定の値に達したらブレンドの再生を終了してモーションを切り替える
		g_player.nCounterBlend++;
		if (g_player.nCounterBlend >= g_player.nFrameBlend)
		{
			g_player.bBlendMotion = false;
			g_player.motionType = g_player.motionTypeBlend;
			g_player.nNumKey = pMotionInfo[g_player.motionType].nNumKey;
			g_player.bLoopMotion = pMotionInfo[g_player.motionType].bLoop;
			g_player.nKey = g_player.nKeyBlend;
			g_player.nCounterMotion = g_player.nCounterMotionBlend;
		}
	}
	else
	{
		// モーションのカウンタを進める
		g_player.nCounterMotion++;

		// モーションのカウンタが再生フレーム数まで達したら次のキーに移行する
		if (g_player.nCounterMotion >= currentKeyInfo->nFrame)
		{
			g_player.nKey = nKeyNext;
			g_player.nCounterMotion = 0;
		}
	}


	PrintDebugProc("\n[現在のモーション]\n");
	PrintDebugProc("キーカウント : %d\n", g_player.nCounterMotion);
	PrintDebugProc("現在のキー : %d\n", g_player.nKey);
	PrintDebugProc("合計キー数 : %d\n", g_player.nNumKey);
	PrintDebugProc("\n[ブレンドのモーション]\n");
	PrintDebugProc("キーカウント : %d\n", g_player.nCounterMotionBlend);
	PrintDebugProc("現在のキー : %d\n", g_player.nKeyBlend);
	PrintDebugProc("合計キー数 : %d\n", g_player.nNumKeyBlend);
	PrintDebugProc("ブレンドカウント : %d", g_player.nCounterBlend);
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
	MOTION* pMotion = &g_player.motion;

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

	for (int nCntPart = 0; nCntPart < pMotion->nNumPart; nCntPart++)
	{
		D3DXMATRIX mtxParent;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pMotion->aPart[nCntPart].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(
			&mtxRot,
			pMotion->aPart[nCntPart].obj.rot.y, pMotion->aPart[nCntPart].obj.rot.x, pMotion->aPart[nCntPart].obj.rot.z
		);
		D3DXMatrixMultiply(
			&pMotion->aPart[nCntPart].mtxWorld,
			&pMotion->aPart[nCntPart].mtxWorld,
			&mtxRot
		);

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pMotion->aPart[nCntPart].obj.pos.x, pMotion->aPart[nCntPart].obj.pos.y, pMotion->aPart[nCntPart].obj.pos.z
		);
		D3DXMatrixMultiply(
			&pMotion->aPart[nCntPart].mtxWorld,
			&pMotion->aPart[nCntPart].mtxWorld,
			&mtxTrans
		);

		if (pMotion->aPart[nCntPart].nIdxModelParent != -1)
		{// 親モデルがあるなら
			mtxParent = pMotion->aPart[pMotion->aPart[nCntPart].nIdxModelParent].mtxWorld;
		}
		else
		{// 親モデルがないなら
			mtxParent = g_player.mtxWorld;
		}

		// パーツと親のマトリックス掛け合わせる
		D3DXMatrixMultiply(
			&pMotion->aPart[nCntPart].mtxWorld,
			&pMotion->aPart[nCntPart].mtxWorld,
			&mtxParent
		);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pMotion->aPart[nCntPart].mtxWorld);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pMotion->aPart[nCntPart].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)pMotion->aPart[nCntPart].dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, pMotion->aPart[nCntPart].apTexture[nCntMat]);

			// モデル（パーツ）の描画
			pMotion->aPart[nCntPart].pMesh->DrawSubset(nCntMat);
		}
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

PLAYER* GetPlayer(void)
{
	return &g_player;
}

void SetMotion(MOTIONTYPE type, bool bBlendMotion, int nFrameMotion)
{
	MOTION* pMotion = &g_player.motion;
	MOTION_INFO* pMotionInfo = &pMotion->aMotionInfo[0];

	if (bBlendMotion)
	{
		g_player.bBlendMotion = true;
		g_player.motionTypeBlend = type;
		g_player.nNumKeyBlend = pMotionInfo[type].nNumKey;
		g_player.bLoopMotionBlend = pMotionInfo[type].bLoop;
		g_player.nKeyBlend = 0;
		g_player.nCounterMotionBlend = 0;
		g_player.nFrameBlend = nFrameMotion;
		g_player.nCounterBlend = 0;
	}
	else
	{
		g_player.motionType = type;
		g_player.nNumKey = pMotionInfo[type].nNumKey;
		g_player.bLoopMotion = pMotionInfo[type].bLoop;
		g_player.nKey = 0;
		g_player.nCounterMotion = 0;
	}
}

MOTIONTYPE GetCurrentPlayerMotion(void)
{
	if (g_player.bBlendMotion)
	{
		return g_player.motionTypeBlend;
	}

	return g_player.motionType;
}
