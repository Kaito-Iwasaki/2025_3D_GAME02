//=====================================================================
//
// camera.cppのヘッダファイル [camera.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

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


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	CAMERAMODE_NORMAL = 0,
	CAMERAMODE_GAME,
	CAMERAMODE_CLEAR,
	CAMERAMODE_MAX
}CAMERAMODE;


//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	D3DXVECTOR3 posV;			// 視点
	D3DXVECTOR3 posVDest;		// 視点
	D3DXVECTOR3 posR;			// 注視点
	D3DXVECTOR3 posRDest;		// 注視点
	D3DXVECTOR3 posOffset;		
	D3DXVECTOR3 vecU;			// 上方向へのベクトル
	D3DXVECTOR3 rot;			// 向き
	D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX mtxView;			// ビューマトリックス
	float fDistance;			// 視点と注視点までの距離
	int nCounterState;
	int nMode;
	bool bEnabled;
}CAMERA;


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
CAMERA* GetCamera(void);
void SetCameraPosV(D3DXVECTOR3 pos);
void SetCameraPosR(D3DXVECTOR3 pos);

#endif