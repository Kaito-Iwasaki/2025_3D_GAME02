//=====================================================================
//
// Model [model.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "model.h"
#include "script_loader.h"

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
MODEL g_aModel[MAX_MODEL];

//=====================================================================
// 初期化処理
//=====================================================================
void InitModel(void)
{
	MODEL* pModel = &g_aModel[0];
	MODELSETDATA* pData;

	ZeroMemory(&g_aModel[0], sizeof(MODEL) * MAX_MODEL);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitModel(void)
{
	MODEL* pModel = &g_aModel[0];

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		for (int i = 0; i < (int)pModel->dwNumMat; i++)
		{
			if (pModel->apTexture[i] != NULL)
			{
				pModel->apTexture[i]->Release();
				pModel->apTexture[i] = NULL;
			}
		}

		if (pModel->pMesh != NULL)
		{// メッシュの破棄
			pModel->pMesh->Release();
			pModel->pMesh = NULL;
		}

		if (pModel->pBuffMat != NULL)
		{// マテリアルの破棄
			pModel->pBuffMat->Release();
			pModel->pBuffMat = NULL;
		}
	}

}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateModel(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MODEL* pModel = &g_aModel[0];
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;				// マテリアルデータへのポインタ

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUsed == false) continue;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pModel->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(
			&mtxRot,
			pModel->obj.rot.y, pModel->obj.rot.x, pModel->obj.rot.z
		);
		D3DXMatrixMultiply(
			&pModel->mtxWorld,
			&pModel->mtxWorld,
			&mtxRot
		);

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pModel->obj.pos.x, pModel->obj.pos.y, pModel->obj.pos.z
		);
		D3DXMatrixMultiply(
			&pModel->mtxWorld,
			&pModel->mtxWorld,
			&mtxTrans
		);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pModel->mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)pModel->dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, pModel->apTexture[nCntMat]);

			// モデル（パーツ）の描画
			pModel->pMesh->DrawSubset(nCntMat);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}

}

void SetModel(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MODEL* pModel = &g_aModel[0];

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUsed == true) continue; // すでにセットされていたらスキップ

		ZeroMemory(pModel, sizeof(MODEL));
		pModel->bUsed = true;
		pModel->obj.pos = pos;
		pModel->obj.rot = rot;

		// Xファイルの読み込み
		HRESULT hr = D3DXLoadMeshFromX(
			pFilename,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&pModel->pBuffMat,
			NULL,
			&pModel->dwNumMat,
			&pModel->pMesh
		);
		if FAILED(hr)
		{
			MessageBox(GetMainWindow(), "ヤバい（モデルが）", "え？", MB_ICONERROR);
			DestroyWindow(GetMainWindow());
			break;
		}

		D3DXMATERIAL* pMat;

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

		// テクスチャの読み込み
		for (int i = 0; i < (int)pModel->dwNumMat; i++)
		{
			if (pMat[i].pTextureFilename != NULL)
			{// テクスチャファイルが存在する
				D3DXCreateTextureFromFile(
					pDevice,
					pMat[i].pTextureFilename,
					&pModel->apTexture[i]
				);
			}
		}

		break;
	}
}