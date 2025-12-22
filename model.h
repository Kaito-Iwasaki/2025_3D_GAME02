//=====================================================================
//
// model.cppのヘッダファイル [model.h]
// Author : 
// 
//=====================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "baseObject.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_TEXTURE_PER_MODEL		(32)
#define MAX_MODEL					(1024)

#define MODEL_HIT_NONE				(0x00)
#define MODEL_HIT_IN				(0x01)
#define MODEL_HIT_FRONT				(0x02)
#define MODEL_HIT_BACK				(0x04)
#define MODEL_HIT_LEFT				(0x08)
#define MODEL_HIT_RIGHT				(0x10)
#define MODEL_HIT_TOP				(0x20)
#define MODEL_HIT_BOTTOM			(0x40)

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	COLLISIONTYPE_NORMAL = 0,
	COLLISIONTYPE_DISABLED,
	COLLISIONTYPE_OBSTACLE,
	COLLISIONTYPE_MAX
}COLLISIONTYPE;

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	BASEOBJECT obj;
	D3DXMATRIX mtxWorld;
	int nType;
	bool bUsed;
	bool bCollision;
	COLLISIONTYPE collisionType;
}MODEL;

typedef struct
{
	LPDIRECT3DTEXTURE9 apTexture[MAX_TEXTURE_PER_MODEL];	// テクスチャへのポインタ
	LPD3DXMESH pMesh;										// メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat;									// マテリアルへのポインタ
	DWORD dwNumMat;											// マテリアル数
	D3DXVECTOR3 vtxMin, vtxMax;								// 端の頂点位置
}MESHDATA;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
void SetModel(int nType, D3DXVECTOR3 pos, D3DXVECTOR3 rot, COLLISIONTYPE collitionType);
void LoadModel(const char* pFilename, int nIdx);
void LoadModel(const char* pFilename, MESHDATA* pMeshData);
void ReleaseMesh(MESHDATA* pMeshData);
BYTE CollisionModel(D3DXVECTOR3* pos, D3DXVECTOR3 posOld, D3DXVECTOR3 size = D3DXVECTOR3_ZERO);
float GetModelGroundHeight(D3DXVECTOR3 pos);

#endif