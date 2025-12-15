//=====================================================================
//
// Effect [effect.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "effect.h"
#include "input.h"
#include "shadow.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\effect000.jpg"
#define INIT_POS			D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(15.0f, 15.0f, 0.0f)
#define INIT_COLOR			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffEffect = NULL;
EFFECT g_aEffect[MAX_EFFECT];

//=====================================================================
// 初期化処理
//=====================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT* pEffect = &g_aEffect[0];

	if (TEXTURE_FILENAME)
	{// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffEffect
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL
	);

	VERTEX_3D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pEffect = &g_aEffect[0];
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++, pVtx += 4)
	{
		// 構造体の初期化
		memset(pEffect, 0, sizeof(EFFECT));
		pEffect->obj.pos = INIT_POS;
		pEffect->obj.size = INIT_SIZE;
		pEffect->obj.color = INIT_COLOR;
		pEffect->bUsed = false;
		pEffect->obj.bVisible = false;

		pVtx[0].pos = D3DXVECTOR3(-pEffect->obj.size.x / 2.0f, pEffect->obj.size.y / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEffect->obj.size.x / 2.0f, pEffect->obj.size.y / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-pEffect->obj.size.x / 2.0f, -pEffect->obj.size.y / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEffect->obj.size.x / 2.0f, -pEffect->obj.size.y / 2.0f, 0.0f);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].col = INIT_COLOR;
		pVtx[1].col = INIT_COLOR;
		pVtx[2].col = INIT_COLOR;
		pVtx[3].col = INIT_COLOR;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitEffect(void)
{
	if (g_pTexBuffEffect != NULL)
	{// テクスチャの破棄
		g_pTexBuffEffect->Release();
		g_pTexBuffEffect = NULL;
	}

	if (g_pVtxBuffEffect != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateEffect(void)
{
	EFFECT* pEffect = &g_aEffect[0];
	VERTEX_3D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++, pVtx += 4)
	{
		if (pEffect->bUsed == false) continue;

		if (pEffect->nCounterState > 30)
		{
			pEffect->bUsed = false;
			continue;
		}

		pEffect->obj.size =  (pEffect->originalSize / (float)pEffect->nCounterState);

		pVtx[0].pos = D3DXVECTOR3(-pEffect->obj.size.x / 2.0f, pEffect->obj.size.y / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pEffect->obj.size.x / 2.0f, pEffect->obj.size.y / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-pEffect->obj.size.x / 2.0f, -pEffect->obj.size.y / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pEffect->obj.size.x / 2.0f, -pEffect->obj.size.y / 2.0f, 0.0f);

		pEffect->obj.pos += pEffect->move;

		pEffect->nCounterState++;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EFFECT* pEffect = &g_aEffect[0];
	D3DXMATRIX mtxTrans, mtxView;	// 計算用マトリックス

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUsed == false) continue;
		if (pEffect->obj.bVisible == false) continue;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pEffect->mtxWorld);

		// ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&pEffect->mtxWorld, NULL, &mtxView);
		pEffect->mtxWorld._41 = 0.0f;
		pEffect->mtxWorld._42 = 0.0f;
		pEffect->mtxWorld._43 = 0.0f;

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pEffect->obj.pos.x, pEffect->obj.pos.y, pEffect->obj.pos.z
		);
		D3DXMatrixMultiply(
			&pEffect->mtxWorld,
			&pEffect->mtxWorld,
			&mtxTrans
		);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pEffect->mtxWorld);

		// アルファテストを有効にする
		// →　ピクセルのアルファ値が０より大きければ書き込みをする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

		// 加算合成を適用
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// ライトを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffEffect);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);

		// 加算合成を解除
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// アルファテストを無効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

		// ライトを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move)
{
	EFFECT* pEffect = &g_aEffect[0];
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUsed == true) continue;

		ZeroMemory(pEffect, sizeof(EFFECT));
		pEffect->bUsed = true;
		pEffect->obj.pos = pos;
		pEffect->originalSize = size;
		pEffect->obj.size = size;
		pEffect->move = move;
		pEffect->obj.bVisible = true;
		pEffect->nCounterState = 0;

		break;
	}
}