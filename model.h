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

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_TEXTURE_PER_MODEL		(32)
#define MAX_MODEL					(1024)
#define MAX_PART					(32)

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
}MODEL;

typedef struct
{
	LPDIRECT3DTEXTURE9 apTexture[MAX_TEXTURE_PER_MODEL];	// テクスチャへのポインタ
	LPD3DXMESH pMesh;										// メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat;									// マテリアルへのポインタ
	DWORD dwNumMat;											// マテリアル数
	int nIdxModelParent;									// 親モデルのインデックス
	int nIdxPart;
	BASEOBJECT obj;
	BASEOBJECT offset;
	D3DXMATRIX mtxWorld;
}PART;

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
// ***** 列挙型 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
void SetModel(int nType, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void LoadModel(const char* pFilename, int nIdx);

#endif