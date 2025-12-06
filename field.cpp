//=====================================================================
//
// Field [filed.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "field.h"
#include "input.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME		"data\\TEXTURE\\field000.jpg"
#define INIT_POS				D3DXVECTOR3(0.0f, -1550.0f, 0.0f)
#define INIT_SIZE				D3DXVECTOR3(10000.0f, 0.0f, 10000.0f)
#define INIT_COLOR				D3DXCOLOR_WHITE
#define INIT_TEXTURE_SIZE_X		(50.0f)
#define INIT_TEXTURE_SIZE_Y		(50.0f)

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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffField = NULL;
FIELD g_Field;
D3DXMATRIX g_mtxWorldField;	// ワールドマトリックス

//=====================================================================
// 初期化処理
//=====================================================================
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 構造体の初期化
	memset(&g_Field, 0, sizeof(FIELD));
	g_Field.obj.pos = INIT_POS;
	g_Field.obj.size = INIT_SIZE;
	g_Field.obj.color = INIT_COLOR;
	g_Field.obj.bVisible = true;

	if (TEXTURE_FILENAME)
	{// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffField
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffField,
		NULL
	);

	VERTEX_3D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].pos = D3DXVECTOR3(g_Field.obj.pos.x - g_Field.obj.size.x, 0.0f, g_Field.obj.pos.z + g_Field.obj.size.z);
	pVtx[1].pos = D3DXVECTOR3(g_Field.obj.pos.x + g_Field.obj.size.x, 0.0f, g_Field.obj.pos.z + g_Field.obj.size.z);
	pVtx[2].pos = D3DXVECTOR3(g_Field.obj.pos.x - g_Field.obj.size.x, 0.0f, g_Field.obj.pos.z - g_Field.obj.size.z);
	pVtx[3].pos = D3DXVECTOR3(g_Field.obj.pos.x + g_Field.obj.size.x, 0.0f, g_Field.obj.pos.z - g_Field.obj.size.z);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR_WHITE;
	pVtx[1].col = D3DXCOLOR_WHITE;
	pVtx[2].col = D3DXCOLOR_WHITE;
	pVtx[3].col = D3DXCOLOR_WHITE;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((g_Field.obj.size.x / INIT_TEXTURE_SIZE_X), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, (g_Field.obj.size.z / INIT_TEXTURE_SIZE_Y));
	pVtx[3].tex = D3DXVECTOR2((g_Field.obj.size.x / INIT_TEXTURE_SIZE_X), (g_Field.obj.size.z / INIT_TEXTURE_SIZE_Y));

	// 頂点バッファをアンロック
	g_pVtxBuffField->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitField(void)
{
	if (g_pTexBuffField != NULL)
	{// テクスチャの破棄
		g_pTexBuffField->Release();
		g_pTexBuffField = NULL;
	}

	if (g_pVtxBuffField != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateField(void)
{

}

//=====================================================================
// 描画処理
//=====================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldField);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(
		&mtxRot,
		g_Field.obj.rot.y, g_Field.obj.rot.x, g_Field.obj.rot.z
	);
	D3DXMatrixMultiply(
		&g_mtxWorldField,
		&g_mtxWorldField,
		&mtxRot
	);

	// 位置を反映
	D3DXMatrixTranslation(
		&mtxTrans,
		g_Field.obj.pos.x, g_Field.obj.pos.y, g_Field.obj.pos.z
	);
	D3DXMatrixMultiply(
		&g_mtxWorldField,
		&g_mtxWorldField,
		&mtxTrans
	);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	if (g_Field.obj.bVisible)
	{// 表示状態
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexBuffField);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}