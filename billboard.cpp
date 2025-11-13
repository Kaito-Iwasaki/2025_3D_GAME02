//=====================================================================
//
// Billboard [billboard.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "billboard.h"
#include "input.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\sakai000.png"
#define INIT_POS			D3DXVECTOR3(200.0f, 20.0f, 0.0f)
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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffBillboard = NULL;
BILLBOARD g_Billboard;
D3DXMATRIX g_mtxWorldBillboard;	// ワールドマトリックス

//=====================================================================
// 初期化処理
//=====================================================================
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体の初期化
	memset(&g_Billboard, 0, sizeof(BILLBOARD));
	g_Billboard.obj.pos = INIT_POS;
	g_Billboard.obj.size = INIT_SIZE;
	g_Billboard.obj.color = INIT_COLOR;
	g_Billboard.obj.bVisible = true;

	if (TEXTURE_FILENAME)
	{// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffBillboard
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL
	);

	VERTEX_3D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(-g_Billboard.obj.size.x / 2.0f, g_Billboard.obj.size.y / 2.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Billboard.obj.size.x / 2.0f, g_Billboard.obj.size.y / 2.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-g_Billboard.obj.size.x / 2.0f, -g_Billboard.obj.size.y / 2.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Billboard.obj.size.x / 2.0f, -g_Billboard.obj.size.y / 2.0f, 0.0f);

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

	// 頂点バッファをアンロック
	g_pVtxBuffBillboard->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitBillboard(void)
{
	if (g_pTexBuffBillboard != NULL)
	{// テクスチャの破棄
		g_pTexBuffBillboard->Release();
		g_pTexBuffBillboard = NULL;
	}

	if (g_pVtxBuffBillboard != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateBillboard(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawBillboard(void)
{
	if (g_Billboard.obj.bVisible == false) return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTrans, mtxView;	// 計算用マトリックス

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBillboard);

	// ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);
	g_mtxWorldBillboard._41 = 0.0f;
	g_mtxWorldBillboard._42 = 0.0f;
	g_mtxWorldBillboard._43 = 0.0f;

	// 位置を反映
	D3DXMatrixTranslation(
		&mtxTrans,
		g_Billboard.obj.pos.x, g_Billboard.obj.pos.y, g_Billboard.obj.pos.z
	);
	D3DXMatrixMultiply(
		&g_mtxWorldBillboard,
		&g_mtxWorldBillboard,
		&mtxTrans
	);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);

	// Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexBuffBillboard);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}