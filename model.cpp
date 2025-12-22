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
#include "player.h"
#include "model_loader.h"
#include "DebugProc.h"
#include "camera.h"

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
MESHDATA g_aMeshData[MAX_LOADABLE_MODEL];

float g_aMeshHeight[MAX_LOADABLE_MODEL] =
{
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	200,
	0,
	0,
	0,
	0,
	0,

};

//=====================================================================
// 初期化処理
//=====================================================================
void InitModel(void)
{
	ZeroMemory(&g_aModel[0], sizeof(MODEL) * MAX_MODEL);
	ZeroMemory(&g_aMeshData[0], sizeof(MESHDATA) * MAX_LOADABLE_MODEL);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitModel(void)
{
	MESHDATA* pMeshData = &g_aMeshData[0];

	for (int nCntMesh = 0; nCntMesh < MAX_LOADABLE_MODEL; nCntMesh++, pMeshData++)
	{
		ReleaseMesh(pMeshData);
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
	CAMERA* pCamera = GetCamera();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DMATERIAL9 matDef;			// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;				// マテリアルデータへのポインタ

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUsed == false) continue;	// 使用中でないならスキップ

		D3DXVECTOR3 vecCamLook = pCamera->posR - pCamera->posV;
		vecCamLook.y = 0;
		D3DXVECTOR3 vecCamToModel = pModel->obj.pos - pCamera->posV;
		vecCamToModel.y = 0;
		float ToLookDotToModel = DotProduct(Normalize(vecCamLook), Normalize(vecCamToModel));

		//if (ToLookDotToModel < 0)
		//{
		//	continue;
		//}

		MESHDATA* pMeshData = &g_aMeshData[pModel->nType];

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
		pMat = (D3DXMATERIAL*)pMeshData->pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)pMeshData->dwNumMat; nCntMat++)
		{
			if (Magnitude(vecCamToModel) < 300)
			{
				pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(
					pMat[nCntMat].MatD3D.Diffuse.r,
					pMat[nCntMat].MatD3D.Diffuse.g,
					pMat[nCntMat].MatD3D.Diffuse.b,
					pModel->obj.color.a * 0.25f
				);
			}
			else
			{
				pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(
					pMat[nCntMat].MatD3D.Diffuse.r,
					pMat[nCntMat].MatD3D.Diffuse.g,
					pMat[nCntMat].MatD3D.Diffuse.b,
					pModel->obj.color.a
				);
			}

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, pMeshData->apTexture[nCntMat]);

			// モデル（パーツ）の描画
			pMeshData->pMesh->DrawSubset(nCntMat);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=====================================================================
// モデル設定処理
//=====================================================================
void SetModel(int nType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, COLLISIONTYPE collisionType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MODEL* pModel = &g_aModel[0];

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUsed == true) continue; // 使用中ならスキップ

		// 構造体の初期化
		ZeroMemory(pModel, sizeof(MODEL));
		pModel->bUsed = true;
		pModel->obj.pos = pos;
		pModel->obj.rot = rot;
		pModel->obj.color = D3DXCOLOR_WHITE;
		pModel->nType = nType;
		pModel->collisionType = collisionType;

		break;
	}
}

//=====================================================================
// モデル読み込み処理
//=====================================================================
void LoadModel(const char* pFilename, int nIdx)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESHDATA* pMeshData = &g_aMeshData[nIdx];
	int nNumVtx = 0;
	DWORD dwSizeFVF;
	BYTE* pVtxBuff;

	ReleaseMesh(pMeshData);

	// 頂点の最大・最小位置の初期化
	pMeshData->vtxMax = D3DXVECTOR3(-100000, -100000, -100000);
	pMeshData->vtxMin = D3DXVECTOR3(100000, 100000, 100000);

	// Xファイルの読み込み
	HRESULT hr = D3DXLoadMeshFromX(
		pFilename,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pMeshData->pBuffMat,
		NULL,
		&pMeshData->dwNumMat,
		&pMeshData->pMesh
	);

	if FAILED(hr)
	{
		char aStrErr[512] = {};
		sprintf(&aStrErr[0], "やばい（モデルが）\n%s", pFilename);
		MessageBox(GetMainWindow(), &aStrErr[0], "え？", MB_ICONERROR);
		DestroyWindow(GetMainWindow());
		return;
	}

	// 頂点数を取得
	nNumVtx = pMeshData->pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(pMeshData->pMesh->GetFVF());

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pMeshData->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標を代入

		if (vtx.x < pMeshData->vtxMin.x)
		{// 頂点のX軸最小位置
			pMeshData->vtxMin.x = vtx.x;
		}
		else if (vtx.x > pMeshData->vtxMax.x)
		{// 頂点のX軸最大位置
			pMeshData->vtxMax.x = vtx.x;
		}

		if (vtx.z < pMeshData->vtxMin.z)
		{// 頂点のZ軸最小位置
			pMeshData->vtxMin.z = vtx.z;
		}
		else if (vtx.z > pMeshData->vtxMax.z)
		{// 頂点のZ軸最大位置
			pMeshData->vtxMax.z = vtx.z;
		}

		if (vtx.y < pMeshData->vtxMin.y)
		{// 頂点のY軸最小位置
			pMeshData->vtxMin.y = vtx.y;
		}
		else if (vtx.y > pMeshData->vtxMax.y)
		{// 頂点のY軸最大位置
			pMeshData->vtxMax.y = vtx.y;
		}

		pVtxBuff += dwSizeFVF;
	}

	D3DXMATERIAL* pMat;

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pMeshData->pBuffMat->GetBufferPointer();

	// テクスチャの読み込み
	for (int i = 0; i < (int)pMeshData->dwNumMat; i++)
	{
		if (pMat[i].pTextureFilename != NULL)
		{// テクスチャファイルが存在する
			D3DXCreateTextureFromFile(
				pDevice,
				pMat[i].pTextureFilename,
				&pMeshData->apTexture[i]
			);
		}
	}
}

//=====================================================================
// モデル読み込み処理
//=====================================================================
void LoadModel(const char* pFilename, MESHDATA* pMeshData)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nNumVtx = 0;
	DWORD dwSizeFVF;
	BYTE* pVtxBuff;

	ReleaseMesh(pMeshData);

	// 頂点の最大・最小位置の初期化
	pMeshData->vtxMax = D3DXVECTOR3(-100000, -100000, -100000);
	pMeshData->vtxMin = D3DXVECTOR3(100000, 100000, 100000);

	// Xファイルの読み込み
	HRESULT hr = D3DXLoadMeshFromX(
		pFilename,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pMeshData->pBuffMat,
		NULL,
		&pMeshData->dwNumMat,
		&pMeshData->pMesh
	);

	if FAILED(hr)
	{
		char aStrErr[512] = {};
		sprintf(&aStrErr[0], "やばい（モデルが）\n%s", pFilename);
		MessageBox(GetMainWindow(), &aStrErr[0], "え？", MB_ICONERROR);
		DestroyWindow(GetMainWindow());
		return;
	}

	// 頂点数を取得
	nNumVtx = pMeshData->pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(pMeshData->pMesh->GetFVF());

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	pMeshData->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標を代入

		if (vtx.x < pMeshData->vtxMin.x)
		{// 頂点のX軸最小位置
			pMeshData->vtxMin.x = vtx.x;
		}
		else if (vtx.x > pMeshData->vtxMax.x)
		{// 頂点のX軸最大位置
			pMeshData->vtxMax.x = vtx.x;
		}

		if (vtx.z < pMeshData->vtxMin.z)
		{// 頂点のZ軸最小位置
			pMeshData->vtxMin.z = vtx.z;
		}
		else if (vtx.z > pMeshData->vtxMax.z)
		{// 頂点のZ軸最大位置
			pMeshData->vtxMax.z = vtx.z;
		}

		if (vtx.y < pMeshData->vtxMin.y)
		{// 頂点のY軸最小位置
			pMeshData->vtxMin.y = vtx.y;
		}
		else if (vtx.y > pMeshData->vtxMax.y)
		{// 頂点のY軸最大位置
			pMeshData->vtxMax.y = vtx.y;
		}

		pVtxBuff += dwSizeFVF;
	}

	D3DXMATERIAL* pMat;

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pMeshData->pBuffMat->GetBufferPointer();

	// テクスチャの読み込み
	for (int i = 0; i < (int)pMeshData->dwNumMat; i++)
	{
		if (pMat[i].pTextureFilename != NULL)
		{// テクスチャファイルが存在する
			D3DXCreateTextureFromFile(
				pDevice,
				pMat[i].pTextureFilename,
				&pMeshData->apTexture[i]
			);
		}
	}
}

void ReleaseMesh(MESHDATA* pMeshData)
{
	for (int i = 0; i < MAX_TEXTURE_PER_MODEL; i++)
	{
		if (pMeshData->apTexture[i] != NULL)
		{// テクスチャの破棄
			pMeshData->apTexture[i]->Release();
			pMeshData->apTexture[i] = NULL;
		}
	}

	if (pMeshData->pMesh != NULL)
	{// メッシュの破棄
		pMeshData->pMesh->Release();
		pMeshData->pMesh = NULL;
	}

	if (pMeshData->pBuffMat != NULL)
	{// マテリアルの破棄
		pMeshData->pBuffMat->Release();
		pMeshData->pBuffMat = NULL;
	}
}

BYTE CollisionModel(D3DXVECTOR3* pos, D3DXVECTOR3 posOld, D3DXVECTOR3 size)
{
	MODEL* pModel = &g_aModel[0];
	BYTE byHitAll = MODEL_HIT_NONE;

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUsed == false) continue;	// 使用中でないならスキップ
		if (pModel->collisionType == COLLISIONTYPE_DISABLED) continue;
		if (pModel->collisionType == COLLISIONTYPE_OBSTACLE && GetPlayer()->currentState == PLAYERSTATE_SLIDING) continue;

		BYTE byHit = MODEL_HIT_NONE;
		D3DXVECTOR3 vtxMin = g_aMeshData[pModel->nType].vtxMin;
		D3DXVECTOR3 vtxMax = g_aMeshData[pModel->nType].vtxMax;

		// モデルの衝突判定処理
		if (
			posOld.x > pModel->obj.pos.x + vtxMin.x
			&& posOld.x < pModel->obj.pos.x + vtxMax.x
			&& posOld.z < pModel->obj.pos.z + vtxMax.z
			&& posOld.z > pModel->obj.pos.z + vtxMin.z
			&& posOld.y < pModel->obj.pos.y + vtxMax.y
			&& posOld.y + size.y > pModel->obj.pos.y + vtxMin.y
			)
		{
			byHit |= MODEL_HIT_IN;
		}

		if (
			posOld.x <= pModel->obj.pos.x + vtxMin.x
			&& pos->x > pModel->obj.pos.x + vtxMin.x
			&& pos->z < pModel->obj.pos.z + vtxMax.z
			&& pos->z > pModel->obj.pos.z + vtxMin.z
			&& pos->y < pModel->obj.pos.y + vtxMax.y
			&& pos->y + size.y >= pModel->obj.pos.y + vtxMin.y
			)
		{// 左
			byHit |= MODEL_HIT_LEFT;
		}
		if (
			posOld.x >= pModel->obj.pos.x + vtxMax.x
			&& pos->x < pModel->obj.pos.x + vtxMax.x
			&& pos->z < pModel->obj.pos.z + vtxMax.z
			&& pos->z > pModel->obj.pos.z + vtxMin.z
			&& pos->y < pModel->obj.pos.y + vtxMax.y
			&& pos->y + size.y >= pModel->obj.pos.y + vtxMin.y
			)
		{// 右
			byHit |= MODEL_HIT_RIGHT;
		}

		if (
			posOld.z <= pModel->obj.pos.z + vtxMin.z
			&& pos->z > pModel->obj.pos.z + vtxMin.z
			&& pos->x > pModel->obj.pos.x + vtxMin.x
			&& pos->x < pModel->obj.pos.x + vtxMax.x
			&& pos->y < pModel->obj.pos.y + vtxMax.y
			&& pos->y + size.y >= pModel->obj.pos.y + vtxMin.y
			)
		{// 前
			byHit |= MODEL_HIT_FRONT;
		}
		if (
			posOld.z >= pModel->obj.pos.z + vtxMax.z
			&& pos->z < pModel->obj.pos.z + vtxMax.z
			&& pos->x > pModel->obj.pos.x + vtxMin.x
			&& pos->x < pModel->obj.pos.x + vtxMax.x
			&& pos->y < pModel->obj.pos.y + vtxMax.y
			&& pos->y + size.y >= pModel->obj.pos.y + vtxMin.y
			)
		{// 後ろ
			byHit |= MODEL_HIT_BACK;
		}

		if (byHit & MODEL_HIT_LEFT)
		{// 左から衝突
			pos->x = pModel->obj.pos.x + vtxMin.x;
			OutputDebugString("Hit Left\n");
		}
		if (byHit & MODEL_HIT_RIGHT)
		{// 右から衝突
			pos->x = pModel->obj.pos.x + vtxMax.x;
			OutputDebugString("Hit Right\n");
		}
		if (byHit & MODEL_HIT_FRONT)
		{// 前から衝突
			pos->z = pModel->obj.pos.z + vtxMin.z;
			OutputDebugString("Hit Front\n");
		}
		if (byHit & MODEL_HIT_BACK)
		{// 後ろから衝突
			pos->z = pModel->obj.pos.z + vtxMax.z;
			OutputDebugString("Hit Back\n");
		}

		if (
			posOld.y >= pModel->obj.pos.y + vtxMax.y
			&& pos->y <= pModel->obj.pos.y + vtxMax.y
			&& pos->x > pModel->obj.pos.x + vtxMin.x
			&& pos->x < pModel->obj.pos.x + vtxMax.x
			&& pos->z > pModel->obj.pos.z + vtxMin.z
			&& pos->z < pModel->obj.pos.z + vtxMax.z
			)
		{// 上
			byHit |= MODEL_HIT_TOP;
		}

		if (
			posOld.y + size.y <= pModel->obj.pos.y + vtxMin.y
			&& pos->y + size.y > pModel->obj.pos.y + vtxMin.y
			&& pos->x > pModel->obj.pos.x + vtxMin.x
			&& pos->x < pModel->obj.pos.x + vtxMax.x
			&& pos->z > pModel->obj.pos.z + vtxMin.z
			&& pos->z < pModel->obj.pos.z + vtxMax.z
			)
		{// 下
			byHit |= MODEL_HIT_BOTTOM;
		}

		if (byHit & MODEL_HIT_TOP)
		{// 上から衝突
			pos->y = pModel->obj.pos.y + vtxMax.y;
		}
		if (byHit & MODEL_HIT_BOTTOM)
		{// 下から衝突
			pos->y = pModel->obj.pos.y + vtxMin.y - size.y;
		}

		byHitAll |= byHit;
	}

	return byHitAll;
}

float GetModelGroundHeight(D3DXVECTOR3 pos)
{
	MODEL* pModel = &g_aModel[0];
	float fGround = 0.0f;

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		D3DXVECTOR3 vtxMin = g_aMeshData[pModel->nType].vtxMin;
		D3DXVECTOR3 vtxMax = g_aMeshData[pModel->nType].vtxMax;
		float fModelTop = pModel->obj.pos.y + g_aMeshHeight[pModel->nType];

		if (
			pos.x >= pModel->obj.pos.x + vtxMin.x
			&& pos.x <= pModel->obj.pos.x + vtxMax.x
			&& pos.z <= pModel->obj.pos.z + vtxMax.z
			&& pos.z >= pModel->obj.pos.z + vtxMin.z
			&& pos.y >= fModelTop
		)
		{
			if (fModelTop > fGround)
			{
				fGround = fModelTop;
			}
		}
	}

	return fGround;
}