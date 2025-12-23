//=====================================================================
//
// Line [Line.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "line.h"
#include "util.h"
#include "camera.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


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


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLine = NULL;
LINE g_aLine[MAX_LINE];

//=====================================================================
// 初期化処理
//=====================================================================
void InitLine(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ZeroMemory(&g_aLine[0], sizeof g_aLine);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 2 * MAX_LINE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLine,
		NULL
	);
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitLine(void)
{
	if (g_pVtxBuffLine != NULL)
	{
		g_pVtxBuffLine->Release();
		g_pVtxBuffLine = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateLine(void)
{
	LINE* pLine = &g_aLine[0];
	CAMERA* pCamera = GetCamera();
	VERTEX_3D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffLine->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCountLine = 0; nCountLine < MAX_LINE; nCountLine++, pLine++)
	{
		if (pLine->bUsed == false || pLine->obj.bVisible == false)
		{
			continue;
		}

		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, pLine->obj.size.z);

		pVtx[0].col = pLine->obj.color;
		pVtx[1].col = pLine->obj.color;
		
		pVtx += 2;
	}	
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawLine(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTrans, mtxRot;	// 計算用マトリックス
	LINE* pLine = &g_aLine[0];

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLine, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCountLine = 0; nCountLine < MAX_LINE; nCountLine++, pLine++)
	{
		if (pLine->bUsed == false || pLine->obj.bVisible == false)
		{
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pLine->mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(
			&mtxRot,
			pLine->obj.rot.y, pLine->obj.rot.x, pLine->obj.rot.z
		);
		D3DXMatrixMultiply(
			&pLine->mtxWorld,
			&pLine->mtxWorld,
			&mtxRot
		);

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pLine->obj.pos.x, pLine->obj.pos.y, pLine->obj.pos.z
		);
		D3DXMatrixMultiply(
			&pLine->mtxWorld,
			&pLine->mtxWorld,
			&mtxTrans
		);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pLine->mtxWorld);

		// ライトを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_LINESTRIP, nCountLine * 2, 1);

		// ライトを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

}

void SetLine(D3DXVECTOR3 pos, float fRot, float fLength, D3DXCOLOR col)
{
	LINE* pLine = &g_aLine[0];

	for (int nCountLine = 0; nCountLine < MAX_LINE; nCountLine++, pLine++)
	{
		if (pLine->bUsed == true)
		{
			continue;
		}

		ZeroMemory(pLine, sizeof(LINE));
		pLine->bUsed = true;
		pLine->obj.pos = pos;
		pLine->obj.rot.y = fRot;
		pLine->obj.size.z = fLength;
		pLine->obj.color = col;
		pLine->obj.bVisible = true;

		break;
	}
}