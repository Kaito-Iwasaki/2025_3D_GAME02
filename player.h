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


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	MOTIONTYPE_NETURAL = 0,
	MOTIONTYPE_MOVE,
	MOTIONTYPE_ACTION,
	MOTIONTYPE_JUMP,
	MOTIONTYPE_LANDING,
	MOTIONTYPE_SLIDING,
	MOTIONTYPE_DIED,
	MOTIONTYPE_MAX
}MOTIONTYPE;

typedef enum
{
	PLAYERSTATE_NONE = 0,
	PLAYERSTATE_NETURAL,
	PLAYERSTATE_MOVE,
	PLAYERSTATE_JUMP,
	PLAYERSTATE_FALL,
	PLAYERSTATE_SLIDING,
	PLAYERSTATE_DIED,
	PLAYERSTATE_MAX
}PLAYERSTATE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;								// 位置・回転情報
	D3DXMATRIX mtxWorld;						// ワールドマトリックス
	D3DXVECTOR3 posOld;							// 過去の位置
	D3DXVECTOR3 move;							// 移動先の位置
	D3DXVECTOR3 rotMove;						// 移動先の角度
	int nIdxShadow;								// シャドウのインデックス
	bool bJump;
	PLAYERSTATE currentState;
	PLAYERSTATE previousState;
	int nCounterState;

	MOTION motion;								// モーション
	MOTIONTYPE motionType;						// モーションの種類
	bool bLoopMotion;							// ループするかどうか
	int nNumKey;								// キー総数
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
void SetPlayerMotion(MOTIONTYPE type, bool bBlendMotion, int nFrameMotion);
MOTIONTYPE GetCurrentPlayerMotion(void);
void SetPlayerState(PLAYERSTATE state);

#endif