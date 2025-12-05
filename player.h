//=====================================================================
//
// player.cppのヘッダファイル [player.h]
// Author : 
// 
//=====================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "baseObject.h"
#include "model.h"
#include "motion_loader.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define NUM_PART			(6)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	MOTIONTYPE_NETURAL = 0,
	MOTIONTYPE_MOVE,
	MOTIONTYPE_MAX
}MOTIONTYPE;


//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rotMove;
	int nIdxShadow;
	char* aFilenamePart[NUM_PART][MAX_PATH];
	PART aPart[NUM_PART];
	int nNumPart;

	MOTION_INFO aMotionInfo[MAX_MOTION_INFO];	// モーション情報
	int nNumMotion;								// モーションの総数
	MOTIONTYPE motionType;						// モーションの種類
	bool bLoopMotion;							// ループするかどうか
	int nNumKey;								// キー総数S
	int nKey;									// 現在のキーNo.
	int nCounterMotion;							// モーションのカウンター

	bool bFinishMotion;							// 現在のモーションが終了しているかどうか
	bool bBlendMotion;							// ブレンドモーションがあるかどうか
	MOTIONTYPE motionTypeBlend;					// ブレンドモーションの種類
	bool bLoopMotionBlend;						// ブレンドモーションがループするか
	int nNumKeyBlend;							// ブレンドモーションのキー数
	int nKeyBlend;								// ブレンドモーションのキーNo.
	int nCounterMotionBlend;					// ブレンドモーションのフレームカウンター

	int nFrameBlend;							// ブレンドのフレーム数
	int nCounterBlend;							// ブレンドのカウンター
}PLAYER;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER* GetPlayer(void);
void SetMotion(MOTIONTYPE type, bool bBlendMotion, int nFrameMotion);
MOTIONTYPE GetCurrentPlayerMotion(void);

#endif