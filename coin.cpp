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
		pCoin->obj.rot += D3DXVECTOR3(0.025f, 0.025f, 0.025f);

		// モデルの衝突判定処理
		if (
			pPlayer->obj.pos.x > pCoin->obj.pos.x + vtxMin.x
			&& pPlayer->obj.pos.x < pCoin->obj.pos.x + vtxMax.x
			&& pPlayer->obj.pos.z < pCoin->obj.pos.z + vtxMax.z
			&& pPlayer->obj.pos.z > pCoin->obj.pos.z + vtxMin.z
			&& pPlayer->obj.pos.y < pCoin->obj.pos.y + vtxMax.y
			&& pPlayer->obj.pos.y + pPlayer->obj.size.y > pCoin->obj.pos.y + vtxMin.y
			)
		{
			pCoin->obj.bVisible = false;
		}
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
			pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(
				pMat[nCntMat].MatD3D.Diffuse.r,
				pMat[nCntMat].MatD3D.Diffuse.g,
				pMat[nCntMat].MatD3D.Diffuse.b,
				pCoin->obj.color.a
			);

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
		pCoin->obj.rot = D3DXVECTOR3(fRandomAngle, fRandomAngle, fRandomAngle);
		pCoin->obj.bVisible = true;
		pCoin->obj.color = D3DXCOLOR_WHITE;
		pCoin->nCounterState = 0;

		break;
	}
}