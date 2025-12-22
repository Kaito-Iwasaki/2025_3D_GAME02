//=====================================================================
//
// コインオブジェクト [coin.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "coin.h"
#include "model.h"
#include "player.h"
#include "effect.h"
#include "sound.h"
#include "shadow.h"
#include "input.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MESH_FILENAME	"data\\MODEL\\coin00.x"

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
MESHDATA g_meshdataCoin;
COIN g_aCoin[MAX_COIN];

//=====================================================================
// 初期化処理
//=====================================================================
void InitCoin(void)
{
	ZeroMemory(&g_aCoin[0], sizeof(COIN) * MAX_COIN);

	LoadModel(MESH_FILENAME, &g_meshdataCoin);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitCoin(void)
{
	ReleaseMesh(&g_meshdataCoin);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateCoin(void)
{
	COIN* pCoin = &g_aCoin[0];
	PLAYER* pPlayer = GetPlayer();

	D3DXVECTOR3 vtxMin = g_meshdataCoin.vtxMin;
	D3DXVECTOR3 vtxMax = g_meshdataCoin.vtxMax;

	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++, pCoin++)
	{
		if (pCoin->bUsed == false) continue;

		pCoin->obj.rot += D3DXVECTOR3(0.025f, 0.025f, 0.025f);
		pCoin->obj.pos.y = pCoin->origin.y + sinf((float)pCoin->nCounterState * 0.1f) * 7;

		// モデルの衝突判定処理
		if (Magnitude(pPlayer->obj.pos + D3DXVECTOR3(0, 30, 0) - pCoin->obj.pos) < 120 && pCoin->obj.bVisible)
		{
			pCoin->obj.bVisible = false;
			pCoin->nCounterState = 0;
			pPlayer->nScore += 100;
			PlaySound(SOUND_LABEL_SE_COIN);
			SetVibration(20000, 20000, 10);
			for (int i = 0; i < 10; i++)
			{
				SetEffect(pCoin->obj.pos, D3DXVECTOR3(50, 50, 0), D3DXCOLOR(1, 1, 0, 1), GetRandomVector() * 10);
			}
		}

		float fGround = GetModelGroundHeight(pCoin->obj.pos);
		SetShadowPosition(pCoin->nIdxShadow, D3DXVECTOR3(pCoin->obj.pos.x, fGround + 0.001f, pCoin->obj.pos.z));
		SetShadowSize(pCoin->nIdxShadow, D3DXVECTOR3(40.0f, 0.0f, 40.0f) * (pCoin->obj.pos.y - fGround + 1000) * 0.001f);
		SetShadowAlpha(pCoin->nIdxShadow, 0.2f - (pCoin->obj.pos.y - fGround) * 0.00025f);

		if (pCoin->obj.bVisible == false)
		{
			SetShadowAlpha(pCoin->nIdxShadow, 0.0f);
		}

		if (pCoin->nCounterState > 60 * 10)
		{
			pCoin->obj.bVisible = true;
			pCoin->nCounterState = RandRange(0, 99);
		}

		pCoin->nCounterState++;
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawCoin(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	COIN* pCoin = &g_aCoin[0];
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;				// マテリアルデータへのポインタ

	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++, pCoin++)
	{
		if (pCoin->bUsed == false) continue;
		if (pCoin->obj.bVisible == false) continue;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pCoin->mtxWorld);

		// 向きを設定
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pCoin->obj.rot.y, pCoin->obj.rot.x, pCoin->obj.rot.z);
		D3DXMatrixMultiply(&pCoin->mtxWorld, &pCoin->mtxWorld, &mtxRot);

		// 位置を設定
		D3DXMatrixTranslation(&mtxTrans, pCoin->obj.pos.x, pCoin->obj.pos.y, pCoin->obj.pos.z);
		D3DXMatrixMultiply(&pCoin->mtxWorld, &pCoin->mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pCoin->mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_meshdataCoin.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_meshdataCoin.dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_meshdataCoin.apTexture[nCntMat]);

			// モデル（パーツ）の描画
			g_meshdataCoin.pMesh->DrawSubset(nCntMat);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

void SetCoin(D3DXVECTOR3 pos)
{
	COIN* pCoin = &g_aCoin[0];

	for (int nCntCoin = 0; nCntCoin < MAX_COIN; nCntCoin++, pCoin++)
	{
		if (pCoin->bUsed) continue;

		float fRandomAngle = RandRange(0, 627) * 0.01f;

		ZeroMemory(pCoin, sizeof(COIN));
		pCoin->bUsed = true;
		pCoin->obj.pos = pos;
		pCoin->origin = pos;
		pCoin->obj.rot = D3DXVECTOR3(fRandomAngle, fRandomAngle, fRandomAngle);
		pCoin->obj.bVisible = true;
		pCoin->obj.color = D3DXCOLOR_WHITE;
		pCoin->nCounterState = RandRange(0, 99);
		pCoin->nIdxShadow = -1;

		pCoin->nIdxShadow = SetShadow();

		break;
	}
}