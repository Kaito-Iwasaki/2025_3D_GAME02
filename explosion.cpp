//=====================================================================
//
// Explosion [explosion.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "explosion.h"
#include "input.h"
#include "shadow.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\explosion000.png"
#define INIT_POS			D3DXVECTOR3(0.0f, 20.0f, 0.0f)
#define INIT_SIZE			D3DXVECTOR3(15.0f, 15.0f, 0.0f)
#define INIT_COLOR			D3DXCOLOR_WHITE

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffExplosion = NULL;
EXPLOSION g_aExplosion[MAX_EXPLOSION];

//=====================================================================
// 初期化処理
//=====================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EXPLOSION* pExplosion = &g_aExplosion[0];

	if (TEXTURE_FILENAME)
	{// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffExplosion
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL
	);

	VERTEX_3D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pExplosion = &g_aExplosion[0];
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++, pVtx += 4)
	{
		// 構造体の初期化
		memset(pExplosion, 0, sizeof(EXPLOSION));
		pExplosion->obj.pos = INIT_POS;
		pExplosion->obj.size = INIT_SIZE;
		pExplosion->obj.color = INIT_COLOR;
		pExplosion->bUsed = false;
		pExplosion->obj.bVisible = false;

		pVtx[0].pos = D3DXVECTOR3(-pExplosion->obj.size.x / 2.0f, pExplosion->obj.size.y / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pExplosion->obj.size.x / 2.0f, pExplosion->obj.size.y / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-pExplosion->obj.size.x / 2.0f, -pExplosion->obj.size.y / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pExplosion->obj.size.x / 2.0f, -pExplosion->obj.size.y / 2.0f, 0.0f);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].col = D3DXCOLOR_WHITE;
		pVtx[1].col = D3DXCOLOR_WHITE;
		pVtx[2].col = D3DXCOLOR_WHITE;
		pVtx[3].col = D3DXCOLOR_WHITE;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitExplosion(void)
{
	if (g_pTexBuffExplosion != NULL)
	{// テクスチャの破棄
		g_pTexBuffExplosion->Release();
		g_pTexBuffExplosion = NULL;
	}

	if (g_pVtxBuffExplosion != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateExplosion(void)
{
	EXPLOSION* pExplosion = &g_aExplosion[0];
	VERTEX_3D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++, pVtx += 4)
	{
		if (pExplosion->bUsed == false) continue;

		if (pExplosion->nCounterState % 5 == 0)
		{
			pExplosion->nPatternAnim++;
		}

		if (pExplosion->nPatternAnim > 4)
		{
			pExplosion->bUsed = false;
			continue;
		}

		SetVertexTexturePos(pVtx, pExplosion->nPatternAnim, 0, 5, 1, false);

		pExplosion->nCounterState++;
	}

	// 頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	EXPLOSION* pExplosion = &g_aExplosion[0];
	D3DXMATRIX mtxTrans, mtxView;	// 計算用マトリックス

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	pExplosion = &g_aExplosion[0];
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->obj.bVisible == false) continue;
		if (pExplosion->bUsed == false) continue;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pExplosion->mtxWorld);

		// ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&pExplosion->mtxWorld, NULL, &mtxView);
		pExplosion->mtxWorld._41 = 0.0f;
		pExplosion->mtxWorld._42 = 0.0f;
		pExplosion->mtxWorld._43 = 0.0f;

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pExplosion->obj.pos.x, pExplosion->obj.pos.y, pExplosion->obj.pos.z
		);
		D3DXMatrixMultiply(
			&pExplosion->mtxWorld,
			&pExplosion->mtxWorld,
			&mtxTrans
		);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pExplosion->mtxWorld);

		// Zテストを無効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// アルファテストを有効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

		// 加算合成を適用
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffExplosion);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);

		// Zテストを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// アルファテストを無効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

		// 加算合成を解除
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

void SetExplosion(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	EXPLOSION* pExplosion = &g_aExplosion[0];
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUsed == true) continue;

		ZeroMemory(pExplosion, sizeof(EXPLOSION));
		pExplosion->bUsed = true;
		pExplosion->obj.pos = pos;
		pExplosion->move = move;
		pExplosion->obj.bVisible = true;
		pExplosion->nCounterState = 0;

		break;
	}
}