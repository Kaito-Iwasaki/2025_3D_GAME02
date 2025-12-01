//=====================================================================
// 
// 汎用関数 [util.h]
// Author : Kaito Iwasaki
//
//=====================================================================
#ifndef _UTIL_H_
#define _UTIL_H_

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
#define D3DXVECTOR3_ZERO	D3DXVECTOR3(0.0f, 0.0f, 0.0f)					// ベクトル初期値
#define D3DXCOLOR_ZERO		D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)
#define D3DXCOLOR_WHITE		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define D3DXCOLOR_BLACK		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)

#define BIT_CHECK(a,b)   (a >> b) & 1
#define BIT_SET(a,b)     a |= (1 << b)
#define BIT_UNSET(a,b)   a &= ~(1 << b)

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
int Clamp(int nNum, int nMin, int nMax);
void Clamp(int* pNum, int nMin, int nMax);
float Clampf(float fNum, float fMin, float fMax);
void Clampf(float* pNum, float fMin, float fMax);
float Lerpf(float a, float b, float t);
int RandRange(int nMin, int nMax);
float DotProduct(D3DXVECTOR2 vectorA, D3DXVECTOR2 vectorB);
float DotProduct(D3DXVECTOR3 vectorA, D3DXVECTOR3 vectorB);
D3DXVECTOR3 CrossProduct(D3DXVECTOR3 vectorA, D3DXVECTOR3 vectorB);
D3DXVECTOR3 Normalize(D3DXVECTOR3 vector);
D3DXVECTOR3 Direction(D3DXVECTOR3 from, D3DXVECTOR3 to);
D3DXVECTOR3 Direction(float fAngle);
D3DXVECTOR3 GetRandomVector(void);
float Angle(float fromX, float fromY, float toX, float toY);
float GetFixedRotation(float fRot);
float Magnitude(D3DXVECTOR3 vector);
float Magnitude(D3DXVECTOR3 from, D3DXVECTOR3 to);
D3DXVECTOR2 Vector3To2(D3DXVECTOR3 source);
D3DXVECTOR3 Vector2To3(D3DXVECTOR2 source, float fValueZ = 0.0f);
bool IsCrossingVector(D3DXVECTOR3 vec0, D3DXVECTOR3 vec1, D3DXVECTOR3 pos, D3DXVECTOR3 posOld, D3DXVECTOR3* vecHit);
bool LoadBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount);
bool SaveBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount);

#endif