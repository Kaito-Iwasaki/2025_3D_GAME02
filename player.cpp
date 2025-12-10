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
#include "Game.h"
#include "fade.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_TEXTURE			(8)

#define PLAYER_JUMPPOWER	(14.0f)
#define PLAYER_SPEED		(5.0f)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void _OnPlayerStateChanged(void);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
PLAYER g_player;
D3DXVECTOR3 g_dir = D3DXVECTOR3(0, 0, 1);
int nMode = 0;

//=====================================================================
// 初期化処理
//=====================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATERIAL* pMat;
	MOTION* pMotion = &g_player.motion;

	// 構造体の初期化
	memset(&g_player, 0, sizeof(PLAYER));
	g_player.obj.bVisible = true;
	g_player.nIdxShadow = SetShadow();
	g_player.obj.pos = D3DXVECTOR3(0.0f, 350.0f, 0.0f);

	// モーションスクリプトから各情報を読み込む
	LoadMotionScript("data\\motion_character00.txt", &g_player.motion);

	// パーツ（モデル）の読み込み
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

	// 最初のモーションを設定
	SetPlayerMotion(MOTIONTYPE_MOVE, false, 30);

	SetPlayerState(PLAYERSTATE_MOVE);

	g_dir = D3DXVECTOR3(0, 0, 1);
	nMode = 0;
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
			{// テクスチャの破棄
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
	XINPUT_STATE* joypad = GetJoypad();			// ジョイパッドへのポインタ

	D3DXVECTOR3 dir = D3DXVECTOR3_ZERO;			// キーボードの入力方向
	D3DXVECTOR3 dirPad = D3DXVECTOR3_ZERO;		// パッドの入力方向

	// 前回の位置を記録
	g_player.posOld = g_player.obj.pos;

	dir = g_dir;

	float fAngle = atan2f(g_dir.x, g_dir.z);

	PrintDebugProc("%f", fAngle);

	// プレイヤー操作
	if (GetKeyboardPress(DIK_A))
	{// 左移動
		dir.x -= cosf(fAngle);
		dir.z -= -sinf(fAngle);
	}
	if (GetKeyboardPress(DIK_D))
	{// 右移動
		dir.x += cosf(fAngle);
		dir.z += -sinf(fAngle);
	}

	if (GetKeyboardTrigger(DIK_W) && g_player.bJump == false)
	{// ジャンプ
		g_player.move.y = PLAYER_JUMPPOWER;
	}

	// パッドの入力方向を取得
	dirPad.x = joypad->Gamepad.sThumbLX;
	dirPad.z = joypad->Gamepad.sThumbLY;

	// パッドの入力方向ベクトルの大きさを計算
	float joypadMagnitude = sqrtf(dirPad.x * dirPad.x + dirPad.z * dirPad.z);

	// パッドの入力方向ベクトルの正規化
	if (joypadMagnitude != 0 && joypadMagnitude > INPUT_DEADZONE)
	{// 大きさが０でなくデッドゾーンを超えていたら処理
		dirPad.x = (float)joypad->Gamepad.sThumbLX / (INPUT_MAX_MAGNITUDE - INPUT_DEADZONE);
		dirPad.z = (float)joypad->Gamepad.sThumbLY / (INPUT_MAX_MAGNITUDE - INPUT_DEADZONE);
		dir = dirPad;
	}

	// キーボード入力方向ベクトルの大きさを計算
	float fMagnitude = sqrtf(dir.x * dir.x + dir.z * dir.z);

	// キーボードの入力方向ベクトルの正規化
	if (fMagnitude != 0)
	{// 大きさが０でないなら
		// プレイヤーの向きの移動先を移動方向の向きに設定
		g_player.rotMove.y = atan2f(dir.x, dir.z) + D3DX_PI;

		// プレイヤーの位置に移動量を反映
		g_player.obj.pos += dir / fMagnitude * PLAYER_SPEED;
	}

	// プレイヤーのY軸の移動量を反映
	g_player.obj.pos.y += g_player.move.y;

	// プレイヤーの向きを向きの移動先に近づける
	float fRotDiff = GetFixedRotation(g_player.rotMove.y - g_player.obj.rot.y);
	g_player.obj.rot.y = GetFixedRotation(g_player.obj.rot.y + fRotDiff * 0.1f);

	// プレイヤーのY軸の移動量に重力を加算
	g_player.move.y -= GAME_GRAVITY;

	// 影の設定
	SetShadowPosition(g_player.nIdxShadow, D3DXVECTOR3(g_player.obj.pos.x, 0.01f, g_player.obj.pos.z));
	SetShadowSize(g_player.nIdxShadow, D3DXVECTOR3(40.0f, 0.0f, 40.0f) + D3DXVECTOR3(g_player.obj.pos.y * 0.08f, 0.0f, g_player.obj.pos.y * 0.08f));
	SetShadowAlpha(g_player.nIdxShadow, Clampf(0.5f - g_player.obj.pos.y * 0.001f, 0.0f, 0.5f));

	if (g_player.obj.pos.y < -500)
	{
		SetFade(GetCurrentScene());
	}

	BYTE byHit = CollisionModel(&g_player.obj.pos, g_player.posOld);

	if (byHit & (MODEL_HIT_TOP | MODEL_HIT_BOTTOM))
	{
		g_player.move.y = 0;
	}

	g_player.bJump = (byHit & MODEL_HIT_TOP) ? false : true;

	if (g_player.bJump)
	{
		SetPlayerState(PLAYERSTATE_JUMP);
	}
	else
	{
		if (GetKeyboardPress(DIK_S))
		{
			SetPlayerState(PLAYERSTATE_SLIDING);

		}
		else
		{
			SetPlayerState(PLAYERSTATE_MOVE);

		}
	}

	if (g_player.currentState != g_player.previousState)
	{
		_OnPlayerStateChanged();
		g_player.previousState = g_player.currentState;
	}


	int nKeyNext = (g_player.nKey + 1) % g_player.nNumKey;
	KEY_INFO* currentKeyInfo = &pMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey];
	KEY_INFO* nextKeyInfo = &pMotionInfo[g_player.motionType].aKeyInfo[nKeyNext];
	PART* pPart = &pMotion->aPart[0];
	float fRateKey = (float)g_player.nCounterMotion / (float)currentKeyInfo->nFrame;

	for (int nCntPart = 0; nCntPart < g_player.motion.nNumPart; nCntPart++, pPart++)
	{
		KEY* currentKey = &currentKeyInfo->aKey[nCntPart];
		KEY* nextKey = &nextKeyInfo->aKey[nCntPart];

		D3DXVECTOR3 vecDiffPos = nextKey->pos - currentKey->pos;
		D3DXVECTOR3 vecDiffRot = GetFixedRotation(nextKey->rot - currentKey->rot);

		pPart->obj.pos = currentKey->pos + pPart->offset.pos + vecDiffPos * fRateKey;
		pPart->obj.rot = GetFixedRotation(currentKey->rot + pPart->offset.rot + vecDiffRot * fRateKey);
	}

	g_player.nCounterMotion++;
	if (g_player.nCounterMotion >= currentKeyInfo->nFrame)
	{
		g_player.nKey = nKeyNext;
		g_player.nCounterMotion = 0;
	}

	switch (nMode)
	{
	case 0:
		if (g_player.obj.pos.z > 8700)
		{
			g_dir = D3DXVECTOR3(-1, 0, 0);
			pCamera->rot.y = D3DXToRadian(180);
			pCamera->posOffset = D3DXVECTOR3(-350, 100, 0);
			nMode++;
		}
		break;

	case 1:
		if (g_player.obj.pos.x < -5000)
		{
			g_dir = D3DXVECTOR3(0, 0, -1);
			pCamera->rot.y = D3DXToRadian(-90);
			pCamera->posOffset = D3DXVECTOR3(0, 100, -350);
			pCamera->fDistance += 200;
			nMode++;
		}
		break;

	case 2:
		if (g_player.obj.pos.z < 1700)
		{
			g_dir = D3DXVECTOR3(-1, 0, 0);
			pCamera->rot.y = D3DXToRadian(0);
			pCamera->posOffset = D3DXVECTOR3(-350, 100, 0);
			nMode++;
		}
		break;
	}


	PrintDebugProc("\n[現在のモーション]\n");
	PrintDebugProc("キーカウント : %d\n", g_player.nCounterMotion);
	PrintDebugProc("現在のキー : %d\n", g_player.nKey);
	PrintDebugProc("合計キー数 : %d\n", g_player.nNumKey);
	PrintDebugProc("Rate : %f\n", fRateKey);
	//PrintDebugProc("\n[ブレンドのモーション]\n");
	//PrintDebugProc("キーカウント : %d\n", g_player.nCounterMotionBlend);
	//PrintDebugProc("現在のキー : %d\n", g_player.nKeyBlend);
	//PrintDebugProc("合計キー数 : %d\n", g_player.nNumKeyBlend);
	//PrintDebugProc("ブレンドカウント : %d", g_player.nCounterBlend);
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

//=====================================================================
// プレイヤーの取得処理
//=====================================================================
PLAYER* GetPlayer(void)
{
	return &g_player;
}

//=====================================================================
// モーションの設定処理
//=====================================================================
void SetPlayerMotion(MOTIONTYPE type, bool bBlendMotion, int nFrameMotion)
{
	MOTION* pMotion = &g_player.motion;
	MOTION_INFO* pMotionInfo = &pMotion->aMotionInfo[0];

	g_player.motionType = type;
	g_player.nNumKey = pMotionInfo[type].nNumKey;
	g_player.bLoopMotion = pMotionInfo[type].bLoop;
	g_player.nKey = 0;
	g_player.nCounterMotion = 0;
	g_player.bFinishMotion = false;
}

MOTIONTYPE GetCurrentPlayerMotion(void)
{
	if (g_player.bBlendMotion)
	{
		return g_player.motionTypeBlend;
	}
	else
	{
		return g_player.motionType;
	}
}

void SetPlayerState(PLAYERSTATE state)
{
	g_player.currentState = state;
}

void _OnPlayerStateChanged(void)
{
	switch (g_player.currentState)
	{
	case PLAYERSTATE_MOVE:
		SetPlayerMotion(MOTIONTYPE_MOVE, false, 0);
		break;

	case PLAYERSTATE_JUMP:
		SetPlayerMotion(MOTIONTYPE_JUMP, false, 0);
		break;

	case PLAYERSTATE_SLIDING:
		SetPlayerMotion(MOTIONTYPE_SLIDING, false, 0);
		break;
	}
}