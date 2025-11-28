//=====================================================================
//
// Wall [filed.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "wall.h"
#include "input.h"
#include "DebugProc.h"
#include "player.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define TEXTURE_FILENAME	"data\\TEXTURE\\wall000.jpg"
#define INIT_POS			D3DXVECTOR3(0.0f, 0.0f, 200.0f)
#define INIT_SIZE			D3DXVECTOR3(200.0f, 100.0f, 0.0f)
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
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;
LPDIRECT3DTEXTURE9 g_pTexBuffWall = NULL;
WALL g_Wall[MAX_WALL];

//=====================================================================
// 初期化処理
//=====================================================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	WALL* pWall = &g_Wall[0];

	// 構造体の初期化
	memset(&g_Wall[0], 0, sizeof(WALL) * MAX_WALL);
	for (int i = 0; i < MAX_WALL; i++, pWall++)
	{
		pWall->obj.size = INIT_SIZE;
		pWall->obj.rot.y = D3DX_PI * 0.5f * (i);
		pWall->obj.pos = D3DXVECTOR3(sinf(pWall->obj.rot.y), 0.0f, cosf(pWall->obj.rot.y)) * 200.0f;
		pWall->obj.color = INIT_COLOR;
		pWall->obj.bVisible = true;
	}

	if (TEXTURE_FILENAME)
	{// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			TEXTURE_FILENAME,
			&g_pTexBuffWall
		);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL
	);

	VERTEX_3D* pVtx;

	// 頂点バッファをロックして頂点情報へのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	pWall = &g_Wall[0];
	for (int i = 0; i < MAX_WALL; i++, pWall++, pVtx += 4)
	{
		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(
			-pWall->obj.size.x,
			pWall->obj.size.y,
			0.0f
		);
		pVtx[1].pos = D3DXVECTOR3(
			pWall->obj.size.x,
			pWall->obj.size.y,
			0.0f
		);
		pVtx[2].pos = D3DXVECTOR3(
			-pWall->obj.size.x,
			pWall->obj.pos.y,
			0.0f
		);
		pVtx[3].pos = D3DXVECTOR3(
			pWall->obj.size.x,
			pWall->obj.pos.y,
			0.0f
		);

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
	g_pVtxBuffWall->Unlock();
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitWall(void)
{
	if (g_pTexBuffWall != NULL)
	{// テクスチャの破棄
		g_pTexBuffWall->Release();
		g_pTexBuffWall = NULL;
	}

	if (g_pVtxBuffWall != NULL)
	{// 頂点バッファの破棄
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateWall(void)
{
	WALL* pWall = &g_Wall[0];
	PLAYER* pPlayer = GetPlayer();

	//for (int i = 0; i < MAX_WALL; i++, pWall++)
	{
		if (GetKeyboardPress(DIK_R))
		{
			pWall->obj.rot.y += 0.1f;
		}
		if (GetKeyboardPress(DIK_E))
		{
			pWall->obj.pos.x += 1.0f;
		}

		D3DXVECTOR3 vec0 = D3DXVECTOR3(
			pWall->obj.pos.x - pWall->obj.size.x * cosf(pWall->obj.rot.y),
			0,
			pWall->obj.pos.z - pWall->obj.size.x * -sinf(pWall->obj.rot.y)
		);
		D3DXVECTOR3 vec1 = D3DXVECTOR3(
			pWall->obj.pos.x + pWall->obj.size.x * cosf(pWall->obj.rot.y),
			0,
			pWall->obj.pos.z + pWall->obj.size.x * -sinf(pWall->obj.rot.y)
		);

		D3DXVECTOR3 vecLineWall = vec1 - vec0;				// 壁の境界線ベクトル
		D3DXVECTOR3 vecToPos = pPlayer->obj.pos - vec0;		// 壁の始まりからプレイヤー位置までのベクトル
		D3DXVECTOR3 vecToPosOld = pPlayer->posOld - vec0;	// 壁の始まりから前回のプレイヤー位置までのベクトル
		D3DXVECTOR3 vecMove = pPlayer->obj.pos - pPlayer->posOld;	// プレイヤーの移動ベクトル
		D3DXVECTOR3 vecNor = Normalize(D3DXVECTOR3(vecLineWall.y, 0.0f, -vecLineWall.x));	// 法線ベクトル

		PrintDebugProc("Pos : %f, %f, %f\n", pPlayer->obj.pos.x, pPlayer->obj.pos.y, pPlayer->obj.pos.z);

		PrintDebugProc("vecLineWall : %f, %f, %f\n", vecLineWall.x, vecLineWall.y, vecLineWall.z);
		PrintDebugProc("vecToPos : %f, %f, %f\n", vecToPos.x, vecToPos.y, vecToPos.z);
		PrintDebugProc("vecToPosOld : %f, %f, %f\n", vecToPosOld.x, vecToPosOld.y, vecToPosOld.z);
		PrintDebugProc("isEqual? : %d, %d, %d\n\n", vecToPos.x == vecToPosOld.x, vecToPos.y == vecToPosOld.y, vecToPos.z == vecToPosOld.z);

		D3DXVECTOR3 vecCross0 = CrossProduct(vecLineWall, vecToPos);
		PrintDebugProc("vecCross0 : %f, %f, %f\n", vecCross0.x, vecCross0.y, vecCross0.z);

		D3DXVECTOR3	a = CrossProduct(vecToPos, vecMove);
		D3DXVECTOR3	b = CrossProduct(vecLineWall, vecMove);
		float fRate = a.y / b.y;

		PrintDebugProc("rate : %f\n", fRate);

		D3DXVECTOR3 vecHit = vec0 + D3DXVECTOR3(vecLineWall.x * fRate, pPlayer->obj.pos.y, vecLineWall.z * fRate);
		D3DXVECTOR3 vecToHit = vecHit - pPlayer->obj.pos;
		D3DXVECTOR3 vecOver = pPlayer->obj.pos - vecHit;
		float fMagnitudeOver = Magnitude(vecOver);


		//float fDot = -vecMove.x * vecNor.x + -vecMove.z * vecNor.z;

		if (vecCross0.y > 0 && fRate >= 0.0f && fRate <= 1.0f)
		{
			pPlayer->obj.pos -= vecOver;
			PrintDebugProc("vecHit : %f, %f, %f\n", vecHit.x, vecHit.y, vecHit.z);
			PrintDebugProc("vecOver : %f, %f, %f\n", vecOver.x, vecOver.y, vecOver.z);
		}
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	WALL* pWall = &g_Wall[0];

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int i = 0; i < MAX_WALL; i++, pWall++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pWall->mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(
			&mtxRot,
			pWall->obj.rot.y, pWall->obj.rot.x, pWall->obj.rot.z
		);
		D3DXMatrixMultiply(
			&pWall->mtxWorld,
			&pWall->mtxWorld,
			&mtxRot
		);

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pWall->obj.pos.x, pWall->obj.pos.y, pWall->obj.pos.z
		);
		D3DXMatrixMultiply(
			&pWall->mtxWorld,
			&pWall->mtxWorld,
			&mtxTrans
		);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pWall->mtxWorld);

		if (pWall->obj.bVisible)
		{// 表示状態
			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexBuffWall);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
		}
	}
}