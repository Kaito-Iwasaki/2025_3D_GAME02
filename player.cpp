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
#include "effect.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_TEXTURE			(8)

#define PLAYER_JUMPPOWER	(16.0f)
#define PLAYER_SPEED		(14.0f)

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
void _UpdatePlayerControl(void);
void _UpdatePlayerMotion(void);
void _OnPlayerStateChanged(void);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
PLAYER g_player;
D3DXVECTOR3 g_dir = D3DXVECTOR3(0, 0, 1);
int nMode = 0;

//D3DXVECTOR3 g_waypoints[] = {
//	D3DXVECTOR3(0)
//}

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
	g_player.obj.pos = D3DXVECTOR3(10400, 0, -2900);
	g_player.obj.size = D3DXVECTOR3(0, 110, 0);
	g_player.bJump = true;

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

	g_dir = D3DXVECTOR3(-1, 0, 0);
	nMode = 0;
	GetCamera()->rot.y = D3DXToRadian(180);

	GetCamera()->posOffset = D3DXVECTOR3(sinf(g_player.obj.rot.y) * 350.0f, 200, cosf(g_player.obj.rot.y) * 350.0f);
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
	
	_UpdatePlayerControl();

	float fAngle = atan2f(g_dir.x, g_dir.z);

	// プレイヤーのY軸の移動量を反映
	g_player.obj.pos.y += g_player.move.y;

	// プレイヤーの向きを向きの移動先に近づける
	float fRotDiff = GetFixedRotation(g_player.rotMove.y - g_player.obj.rot.y);
	g_player.obj.rot.y = GetFixedRotation(g_player.obj.rot.y + fRotDiff * 0.1f);

	// プレイヤーのY軸の移動量に重力を加算
	g_player.move.y -= GAME_GRAVITY;

	BYTE byHit = CollisionModel(&g_player.obj.pos, g_player.posOld, g_player.obj.size);

	if (byHit & (MODEL_HIT_TOP | MODEL_HIT_BOTTOM))
	{
		g_player.move.y = 0;
	}

	if (g_player.currentState != PLAYERSTATE_CLEAR)
	{
		g_player.bJump = (byHit & MODEL_HIT_TOP) ? false : true;

		if (g_player.bJump)
		{
			if (g_player.obj.pos.y < g_player.posOld.y)
			{
				SetPlayerState(PLAYERSTATE_FALL);
			}
			else
			{
				SetPlayerState(PLAYERSTATE_JUMP);
			}
		}
		else
		{
			if (INPUT_PRESS_GAME_DOWN)
			{
				SetPlayerState(PLAYERSTATE_SLIDING);
			}
			else
			{
				SetPlayerState(PLAYERSTATE_MOVE);
			}
		}
	}

	// 影の設定
	SetShadowPosition(g_player.nIdxShadow, D3DXVECTOR3(g_player.obj.pos.x, g_player.obj.pos.y + 0.001f, g_player.obj.pos.z));
	SetShadowSize(g_player.nIdxShadow, D3DXVECTOR3(40.0f, 0.0f, 40.0f));
	if (g_player.bJump)
	{
		SetShadowAlpha(g_player.nIdxShadow, 0.0f);
	}
	else
	{
		SetShadowAlpha(g_player.nIdxShadow, 0.3f);
	}


	float fAngleDir = atan2f(g_dir.x, g_dir.z);
	pCamera->posOffset = D3DXVECTOR3(sinf(fAngleDir) * 350.0f, 200, cosf(fAngleDir) * 350.0f);
	D3DXVECTOR3 vecMoved = g_player.obj.pos - g_player.posOld;
	float fMove = Magnitude(D3DXVECTOR3(vecMoved.x * sinf(fAngle), 0, vecMoved.z * cosf(fAngle)));

	if (fMove == 0.0f)
	{
		SetPlayerState(PLAYERSTATE_DIED);
	}

	if (g_player.currentState == PLAYERSTATE_FALL && g_player.nCounterState > 60)
	{
		SetPlayerState(PLAYERSTATE_DIED);
	}

	if (byHit & MODEL_HIT_IN)
	{
		SetPlayerState(PLAYERSTATE_DIED);
	}

	if (g_player.currentState != g_player.previousState)
	{
		_OnPlayerStateChanged();
	}
	g_player.nCounterState++;

	switch (nMode)
	{
	case 0:
		if (g_player.obj.pos.x < 0)
		{
			g_dir = D3DXVECTOR3(0, 0, 1);
			pCamera->rot.y = D3DXToRadian(-90);
			nMode++;
		}
		break;

	case 1:
		if (g_player.obj.pos.z > 8700)
		{
			g_dir = D3DXVECTOR3(-1, 0, 0);
			pCamera->rot.y = D3DXToRadian(180);
			nMode++;
		}
		break;

	case 2:
		if (g_player.obj.pos.x < -5000)
		{
			g_dir = D3DXVECTOR3(0, 0, -1);
			pCamera->rot.y = D3DXToRadian(-90);
			pCamera->fDistance += 200;
			nMode++;
		}
		break;

	case 3:
		if (g_player.obj.pos.z < 1700)
		{
			g_dir = D3DXVECTOR3(-1, 0, 0);
			pCamera->rot.y = D3DXToRadian(0);
			nMode++;
		}
		break;

	case 4:
		if (g_player.obj.pos.x < -14800)
		{
			g_dir = D3DXVECTOR3(0, 0, 1);
			pCamera->rot.y = D3DXToRadian(90);
			nMode++;
		}
		break;

	case 5:
		if (g_player.obj.pos.z > 9700)
		{
			g_dir = D3DXVECTOR3(1, 0, 0);
			pCamera->rot.y = D3DXToRadian(180);
			nMode++;
		}
		break;

	case 6:
		if (g_player.obj.pos.x > -7800)
		{
			g_dir = D3DXVECTOR3(0, 0, -1);
			pCamera->rot.y = D3DXToRadian(-90);
			nMode++;
		}
		break;

	case 7:
		if (g_player.obj.pos.z < 4100)
		{
			g_dir = D3DXVECTOR3(0, 0, 1);
			pCamera->rot.y = D3DXToRadian(-90);
			nMode++;
		}
		break;

	case 8:
		if (g_player.obj.pos.z > 9500)
		{
			g_dir = D3DXVECTOR3(-1, 0, 0);
			pCamera->rot.y = D3DXToRadian(180);
			nMode++;
		}
		break;

	case 9:
		if (g_player.obj.pos.x < -14800)
		{
			g_dir = D3DXVECTOR3(0, 0, -1);
			pCamera->rot.y = D3DXToRadian(90);
			nMode++;
		}
		break;

	case 10:
		if (g_player.obj.pos.z < 1700)
		{
			g_dir = D3DXVECTOR3(1, 0, 0);
			pCamera->rot.y = D3DXToRadian(0);
			nMode++;
		}
		break;

	case 11:
		if (g_player.obj.pos.x > -5000)
		{
			g_dir = D3DXVECTOR3(0, 0, 1);
			pCamera->rot.y = D3DXToRadian(-90);
			nMode++;
		}
		break;

	case 12:
		SetPlayerState(PLAYERSTATE_CLEAR);
		//SetFade(SCENE_RESULT);
		break;
	}

	PrintDebugProc("モード:%d\n", nMode);
	PrintDebugProc("スコア:%d\n", g_player.nScore);
	PrintDebugProc("位置:%d %d %d\n", (int)g_player.obj.pos.x, (int)g_player.obj.pos.y, (int)g_player.obj.pos.z);

	_UpdatePlayerMotion();
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

	if (bBlendMotion)
	{
		if (g_player.bBlendMotion == true)
		{
			g_player.nKey = g_player.nKeyBlend;
			g_player.nNumKey = g_player.nNumKeyBlend;
			g_player.motionType = g_player.motionTypeBlend;
			g_player.nCounterMotion = g_player.nCounterMotionBlend;
			g_player.bLoopMotion = g_player.bLoopMotionBlend;
		}

		g_player.bBlendMotion = true;
		g_player.motionTypeBlend = type;
		g_player.nNumKeyBlend = pMotionInfo[type].nNumKey;
		g_player.bLoopMotionBlend = pMotionInfo[type].bLoop;
		g_player.nKeyBlend = 0;
		g_player.nCounterMotionBlend = 0;
		g_player.nCounterBlend = 0;
		g_player.nFrameBlend = nFrameMotion;
		g_player.bFinishMotion = false;
	}
	else
	{
		g_player.motionType = type;
		g_player.nNumKey = pMotionInfo[type].nNumKey;
		g_player.bLoopMotion = pMotionInfo[type].bLoop;
		g_player.nKey = 0;
		g_player.nCounterMotion = 0;
		g_player.bFinishMotion = false;
	}
}

//=====================================================================
// プレイヤーのモーション取得処理
//=====================================================================
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

//=====================================================================
// プレイヤーの状態設定処理
//=====================================================================
void SetPlayerState(PLAYERSTATE state)
{
	g_player.currentState = state;
}

//=====================================================================
// プレイヤーの操作処理
//=====================================================================
void _UpdatePlayerControl(void)
{
	if (g_player.currentState == PLAYERSTATE_DIED)	return;
	if (g_player.currentState == PLAYERSTATE_CLEAR)	return;
	if (g_player.currentState == PLAYERSTATE_END)	return;

	D3DXVECTOR3 dir = D3DXVECTOR3_ZERO;			// 入力方向

	// 前回の位置を記録
	g_player.posOld = g_player.obj.pos;

	dir = g_dir;
	float fAngle = atan2f(g_dir.x, g_dir.z);

	// プレイヤー操作
	if (g_player.currentState != PLAYERSTATE_SLIDING)
	{
		if (INPUT_PRESS_GAME_LEFT)
		{// 左移動
			dir.x -= cosf(fAngle);
			dir.z -= -sinf(fAngle);
		}
		if (INPUT_PRESS_GAME_RIGHT)
		{// 右移動
			dir.x += cosf(fAngle);
			dir.z += -sinf(fAngle);
		}
	}

	if (INPUT_TRIGGER_GAME_UP && g_player.bJump == false)
	{// ジャンプ
		PlaySound(SOUND_LABEL_SE_JUMP);
		g_player.move.y = PLAYER_JUMPPOWER;
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
#ifdef _DEBUG
		if (GetKeyboardPress(DIK_LSHIFT))
		{
			g_player.obj.pos += dir / fMagnitude * PLAYER_SPEED;
		}
#endif
	}
}

//=====================================================================
// プレイヤーのモーション処理
//=====================================================================
void _UpdatePlayerMotion(void)
{
	MOTION* pMotion = &g_player.motion;
	MOTION_INFO* pMotionInfo = &pMotion->aMotionInfo[0];
	PART* pPart = &pMotion->aPart[0];

	int nKeyNext = (g_player.nKey + 1) % g_player.nNumKey;
	KEY_INFO* currentKeyInfo = &pMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey];
	KEY_INFO* nextKeyInfo = &pMotionInfo[g_player.motionType].aKeyInfo[nKeyNext];
	float fRateKey = (float)g_player.nCounterMotion / (float)currentKeyInfo->nFrame;

	for (int nCntPart = 0; nCntPart < g_player.motion.nNumPart; nCntPart++, pPart++)
	{
		KEY* currentKey = &currentKeyInfo->aKey[nCntPart];
		KEY* nextKey = &nextKeyInfo->aKey[nCntPart];
		D3DXVECTOR3 vecDiffKeyPos = nextKey->pos - currentKey->pos;
		D3DXVECTOR3 vecDiffKeyRot = GetFixedRotation(nextKey->rot - currentKey->rot);

		if (g_player.bBlendMotion)
		{
			int nBlendKeyNext = (g_player.nKeyBlend + 1) % g_player.nNumKeyBlend;
			KEY_INFO* currentBlendKeyInfo = &pMotionInfo[g_player.motionTypeBlend].aKeyInfo[g_player.nKeyBlend];
			KEY_INFO* nextBlendKeyInfo = &pMotionInfo[g_player.motionTypeBlend].aKeyInfo[nBlendKeyNext];
			float fRateBlendKey = (float)g_player.nCounterMotionBlend / (float)currentBlendKeyInfo->nFrame;
			float fRateBlend = (float)g_player.nCounterBlend / (float)g_player.nFrameBlend;

			KEY* currentBlendKey = &currentBlendKeyInfo->aKey[nCntPart];
			KEY* nextBlendKey = &nextBlendKeyInfo->aKey[nCntPart];

			// ブレンドキーの差分を求める
			D3DXVECTOR3 vecDiffBlendKeyPos = nextBlendKey->pos - currentBlendKey->pos;
			D3DXVECTOR3 vecDiffBlendKeyRot = GetFixedRotation(nextBlendKey->rot - currentBlendKey->rot);

			// ブレンドキーの位置を求める
			D3DXVECTOR3 vecBlendKeyPos = currentBlendKey->pos + vecDiffBlendKeyPos * fRateBlendKey;
			D3DXVECTOR3 vecBlendKeyRot = GetFixedRotation(currentBlendKey->rot + vecDiffBlendKeyRot * fRateBlendKey);

			// 今のキーからブレンドキーまでの差分を求める
			D3DXVECTOR3 vecDiffBlendPos = vecBlendKeyPos - currentKey->pos;
			D3DXVECTOR3 vecDiffBlendRot = GetFixedRotation(vecBlendKeyRot - currentKey->rot);

			// ブレンドの位置を求める
			pPart->obj.pos = currentKey->pos + pPart->offset.pos + vecDiffBlendPos * fRateBlend;
			pPart->obj.rot = GetFixedRotation(currentKey->rot + pPart->offset.rot + vecDiffBlendRot * fRateBlend);
		}
		else
		{
			pPart->obj.pos = currentKey->pos + pPart->offset.pos + vecDiffKeyPos * fRateKey;
			pPart->obj.rot = GetFixedRotation(currentKey->rot + pPart->offset.rot + vecDiffKeyRot * fRateKey);
		}
	}

	if (g_player.bFinishMotion == false)
	{// モーション情報の設定
		if (g_player.bBlendMotion)
		{
			KEY_INFO* currentBlendKeyInfo = &pMotionInfo[g_player.motionTypeBlend].aKeyInfo[g_player.nKeyBlend];

			g_player.nCounterMotionBlend++;
			if (g_player.nCounterMotionBlend >= currentBlendKeyInfo->nFrame)
			{
				int nBlendKeyNext = (g_player.nKeyBlend + 1) % g_player.nNumKeyBlend;

				if (nBlendKeyNext == g_player.nNumKeyBlend - 1 && g_player.bLoopMotionBlend == false)
				{
					g_player.bFinishMotion = true;
				}

				g_player.nKeyBlend = nBlendKeyNext;
				g_player.nCounterMotionBlend = 0;
			}

			if (g_player.nCounterBlend >= g_player.nFrameBlend)
			{
				g_player.bBlendMotion = false;
				g_player.nKey = g_player.nKeyBlend;
				g_player.nNumKey = g_player.nNumKeyBlend;
				g_player.motionType = g_player.motionTypeBlend;
				g_player.nCounterMotion = g_player.nCounterMotionBlend;
				g_player.bLoopMotion = g_player.bLoopMotionBlend;
			}
			g_player.nCounterBlend++;
		}
		else
		{
			g_player.nCounterMotion++;
			if (g_player.nCounterMotion >= currentKeyInfo->nFrame)
			{
				if (nKeyNext == g_player.nNumKey - 1 && g_player.bLoopMotion == false)
				{
					g_player.bFinishMotion = true;
				}

				g_player.nKey = nKeyNext;
				g_player.nCounterMotion = 0;
			}
		}
	}
}

//=====================================================================
// プレイヤーの状態変更時の処理
//=====================================================================
void _OnPlayerStateChanged(void)
{
	CAMERA* pCamera = GetCamera();

	switch (g_player.currentState)
	{
	case PLAYERSTATE_MOVE:
		if (g_player.previousState == PLAYERSTATE_FALL)
		{
			SetPlayerMotion(MOTIONTYPE_MOVE, false, 0);
		}
		else
		{
			SetPlayerMotion(MOTIONTYPE_MOVE, true, 10);
		}
		break;

	case PLAYERSTATE_JUMP:
		SetPlayerMotion(MOTIONTYPE_JUMP, true, 5);
		break;

	case PLAYERSTATE_FALL:
		break;

	case PLAYERSTATE_SLIDING:
		if (g_player.previousState == PLAYERSTATE_FALL)
		{
			SetPlayerMotion(MOTIONTYPE_SLIDING, false, 0);
		}
		else
		{
			SetPlayerMotion(MOTIONTYPE_SLIDING, true, 10);
		}
		break;

	case PLAYERSTATE_DIED:
		SetPlayerMotion(MOTIONTYPE_DIED, true, 10);
		SetVibration(20000, 20000, 40);
		SetFade(SCENE_RESULT);
		g_player.move.y = 0;
		pCamera->bEnabled = false;
		break;

	case PLAYERSTATE_CLEAR:
		SetPlayerMotion(MOTIONTYPE_CLEAR, false, 0);
		break;

	default: 
		break;
	}

	g_player.nCounterState = 0;
	g_player.previousState = g_player.currentState;
}