//=====================================================================
//
// model_loader.cppのヘッダファイル [model_loader.h]
// Author : Kaito Iwasaki
// 
//=====================================================================
#ifndef _MODEL_LOADER_H_
#define _MODEL_LOADER_H_

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "main.h"
#include "model.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define MAX_LOADABLE_TEXTURE	(256)
#define MAX_LOADABLE_MODEL		(256)

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	int nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	COLLISIONTYPE collisionType;
}MODELSETDATA;

typedef struct
{
	int nNumTexture;
	char aFilenameTexture[MAX_LOADABLE_TEXTURE][MAX_PATH];

	int nNumModel;
	char aFilenameModel[MAX_LOADABLE_MODEL][MAX_PATH];

	int nCountModelSet;

	MODELSETDATA aInfoModelSet[MAX_MODEL];
}SCRIPTDATA;

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
void LoadScript(const char* pFileName, SCRIPTDATA* pBuffer);

#endif