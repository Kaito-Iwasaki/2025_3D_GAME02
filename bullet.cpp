//=====================================================================
//
// Bullet [bullet.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "bullet.h"
#include "input.h"
#include "shadow.h"
#include "explosion.h"
#include "effect.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\sakai000.png"
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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffBullet = NULL;
BULLET g_aBullet[MAX_BULLET];

//=====================================================================
// 初期化処理
//=====================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET* pBullet = &g_aBullet[0];

	if (TEXTURE_FILENAME)
	{// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffBullet
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL
	);

	VERTEX_3D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pBullet = &g_aBullet[0];
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++, pVtx += 4)
	{
		// 構造体の初期化
		memset(pBullet, 0, sizeof(BULLET));
		pBullet->obj.pos = INIT_POS;
		pBullet->obj.size = INIT_SIZE;
		pBullet->obj.color = INIT_COLOR;
		pBullet->bUsed = false;
		pBullet->obj.bVisible = false;

		pVtx[0].pos = D3DXVECTOR3(-pBullet->obj.size.x / 2.0f, pBullet->obj.size.y / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pBullet->obj.size.x / 2.0f, pBullet->obj.size.y / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-pBullet->obj.size.x / 2.0f, -pBullet->obj.size.y / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pBullet->obj.size.x / 2.0f, -pBullet->obj.size.y / 2.0f, 0.0f);

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
	g_pVtxBuffBullet->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitBullet(void)
{
	if (g_pTexBuffBullet != NULL)
	{// テクスチャの破棄
		g_pTexBuffBullet->Release();
		g_pTexBuffBullet = NULL;
	}

	if (g_pVtxBuffBullet != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateBullet(void)
{
	BULLET* pBullet = &g_aBullet[0];
	VERTEX_3D* pVtx;

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUsed == false) continue;

		if (pBullet->nCounterState > 60 || abs((long)pBullet->obj.pos.x) > 200.0f || abs((long)pBullet->obj.pos.z) > 200.0f)
		{
			pBullet->bUsed = false;
			RemoveShadow(pBullet->nIdxShadow);
			SetExplosion(D3DXVECTOR3(pBullet->obj.pos.x, 0.1f, pBullet->obj.pos.z), D3DXVECTOR3(pBullet->obj.pos.x, 0.1f, pBullet->obj.pos.z));
			continue;
		}

		pBullet->obj.pos += pBullet->move;
		SetShadowPosition(pBullet->nIdxShadow, D3DXVECTOR3(pBullet->obj.pos.x, 0.1f, pBullet->obj.pos.z));
		SetEffect(D3DXVECTOR3(pBullet->obj.pos.x, 0.1f, pBullet->obj.pos.z), D3DXVECTOR3_ZERO);

		pBullet->nCounterState++;
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	BULLET* pBullet = &g_aBullet[0];
	D3DXMATRIX mtxTrans, mtxView;	// 計算用マトリックス

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	pBullet = &g_aBullet[0];
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->obj.bVisible == false) continue;
		if (pBullet->bUsed == false) continue;

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pBullet->mtxWorld);

		// ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// ポリゴンをカメラに対して正面に向ける
		D3DXMatrixInverse(&pBullet->mtxWorld, NULL, &mtxView);
		pBullet->mtxWorld._41 = 0.0f;
		pBullet->mtxWorld._42 = 0.0f;
		pBullet->mtxWorld._43 = 0.0f;

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pBullet->obj.pos.x, pBullet->obj.pos.y, pBullet->obj.pos.z
		);
		D3DXMatrixMultiply(
			&pBullet->mtxWorld,
			&pBullet->mtxWorld,
			&mtxTrans
		);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pBullet->mtxWorld);

		// Zテストを無効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffBullet);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);

		// Zテストを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	BULLET* pBullet = &g_aBullet[0];
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUsed == true) continue;

		ZeroMemory(pBullet, sizeof(BULLET));
		pBullet->bUsed = true;
		pBullet->obj.pos = pos;
		pBullet->move = move;
		pBullet->obj.bVisible = true;
		pBullet->nCounterState = 0;
		pBullet->nIdxShadow = SetShadow();
		SetShadowPosition(pBullet->nIdxShadow, D3DXVECTOR3(pBullet->obj.pos.x, 0.1f, pBullet->obj.pos.z));

		break;
	}
}