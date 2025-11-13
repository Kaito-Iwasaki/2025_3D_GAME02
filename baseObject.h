//=====================================================================
//
// baseObject.cppのヘッダファイル [baseObject.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _BASEOBJECT_H_
#define _BASEOBJECT_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define OOS_TOP			(0x0001)	// 画面外判定（上）
#define OOS_BOTTOM		(0x0002)	// 画面外判定（下）
#define OOS_LEFT		(0x0004)	// 画面外判定（左）
#define OOS_RIGHT		(0x0008)	// 画面外判定（右）
#define OOS_ALL			OOS_TOP | OOS_BOTTOM | OOS_LEFT | OOS_RIGHT	// 画面外判定（上）

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
//*********************************************************************
// 描画オブジェクト
//*********************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 size;		// サイズ
	D3DXCOLOR	color;		// 色
	bool bVisible;			// 表示状態
	bool bInversed;			// テクスチャ反転
}BASEOBJECT;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void SetVertexPos(VERTEX_2D* pVtx, BASEOBJECT obj);
void SetVertexPos(VERTEX_2D* pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 size, float fRot = 0.0f);
void SetVertexPos(VERTEX_2D* pVtx, D3DXVECTOR3 leftTop, D3DXVECTOR3 rightTop, D3DXVECTOR3 leftBottom, D3DXVECTOR3 rightBottom);
void SetVertexRHW(VERTEX_2D* pVtx, float rhw);
void SetVertexColor(VERTEX_2D* pVtx, D3DXCOLOR col);
void SetVertexTexturePos(VERTEX_2D* pVtx, bool bInversed = false);
void SetVertexTexturePos(VERTEX_2D* pVtx, D3DXVECTOR2 leftTop, D3DXVECTOR2 rightTop, D3DXVECTOR2 leftBottom, D3DXVECTOR2 rightBottom, bool bInversed = false);
void SetVertexTexturePos(VERTEX_2D* pVtx, int nTextureX, int nTextureY, int nSizeX, int nSizeY, bool bInversed = false);
void SetVertexTexturePos(VERTEX_3D* pVtx, int nTextureX, int nTextureY, int nSizeX, int nSizeY, bool bInversed = false);
bool IsObjectOutOfScreen(BASEOBJECT obj, DWORD flags = OOS_ALL);
bool IsObjectOutOfScreen(BASEOBJECT obj, RECT rect, DWORD flags = OOS_ALL);

#endif
