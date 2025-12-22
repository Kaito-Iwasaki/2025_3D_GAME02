//=====================================================================
// 
// 汎用関数 [util.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "util.h"

//=====================================================================
// 整数を範囲内に収める処理
//=====================================================================
int Clamp(int nNum, int nMin, int nMax)
{
	if (nNum < nMin)
	{
		return nMin;
	}
	else if (nNum > nMax)
	{
		return nMax;
	}

	return nNum;
}

//=====================================================================
// 整数を範囲内に収める処理（ポインタから直接編集）
//=====================================================================
bool Clamp(int* pNum, int nMin, int nMax)
{
	if (*pNum < nMin)
	{
		*pNum = nMin;
		return true;
	}
	else if (*pNum > nMax)
	{
		*pNum = nMax;
		return true;
	}

	return false;
}

//=====================================================================
// 小数を範囲内に収める処理
//=====================================================================
float Clampf(float fNum, float fMin, float fMax)
{
	if (fNum < fMin)
	{
		return fMin;
	}
	else if (fNum > fMax)
	{
		return fMax;
	}

	return fNum;
}

//=====================================================================
// 小数を範囲内に収める処理（ポインタから直接編集）
//=====================================================================
bool Clampf(float* pNum, float fMin, float fMax)
{
	if (*pNum < fMin)
	{
		*pNum = fMin;
		return true;
	}
	else if (*pNum > fMax)
	{
		*pNum = fMax;
		return true;
	}

	return false;
}

//=====================================================================
// 線形補間
//=====================================================================
float Lerpf(float a, float b, float t)
{
	return a * (1.0f - t) + b * t;
}

//=====================================================================
// ベクトルの内積
//=====================================================================
float DotProduct(D3DXVECTOR2 vectorA, D3DXVECTOR2 vectorB)
{
	return vectorA.x * vectorB.x + vectorA.y * vectorB.y;
}

float DotProduct(D3DXVECTOR3 vectorA, D3DXVECTOR3 vectorB)
{
	return vectorA.x * vectorB.x + vectorA.y * vectorB.y + vectorA.z * vectorB.z;
}

//=====================================================================
// ベクトルの外積
//=====================================================================
D3DXVECTOR3 CrossProduct(D3DXVECTOR3 vectorA, D3DXVECTOR3 vectorB)
{
	return D3DXVECTOR3(
		vectorA.y * vectorB.z - vectorA.z * vectorB.y,
		vectorA.x * vectorB.z - vectorA.z * vectorB.x,
		vectorA.y * vectorB.x - vectorA.x * vectorB.y
	);
}

//=====================================================================
// ベクトルの正規化処理
//=====================================================================
D3DXVECTOR3 Normalize(D3DXVECTOR3 vector)
{
	float fMagnitude = Magnitude(vector);

	if (fMagnitude != 0)
	{
		return D3DXVECTOR3(vector.x / fMagnitude, vector.y / fMagnitude, vector.z / fMagnitude);
	}
	else
	{
		return D3DXVECTOR3_ZERO;
	}
}

//=====================================================================
// 方向ベクトル（二つのベクトルから）
//=====================================================================
D3DXVECTOR3 Direction(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	return Normalize(to - from);
}

//=====================================================================
// 方向ベクトル（角度から）
//=====================================================================
D3DXVECTOR3 Direction(float fAngle)
{
	return D3DXVECTOR3(sin(fAngle), cos(fAngle), 0.0f);
}

//=====================================================================
// ランダムな方向のベクトルを生成
//=====================================================================
D3DXVECTOR3 GetRandomVector(void)
{
	float fTheta = RandRange(0, 627) * 0.01f;
	float fPhi = RandRange(0, 627) * 0.01f;;

	return D3DXVECTOR3(
		sin(fPhi) * sin(fTheta),
		cos(fTheta),
		cosf(fPhi) * sin(fTheta)
	);
}

//=====================================================================
// ベクトルの大きさ（距離）を求める処理
//=====================================================================
float Magnitude(D3DXVECTOR3 vector)
{
	return sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

//=====================================================================
// 二つのベクトルの大きさ（距離）を求める処理
//=====================================================================
float Magnitude(D3DXVECTOR3 from, D3DXVECTOR3 to)
{
	D3DXVECTOR3 fDistance = to - from;

	return sqrtf(fDistance.x * fDistance.x + fDistance.y * fDistance.y);
}

//=====================================================================
// 二つの線の角度を求める処理
//=====================================================================
float Angle(float fromX, float fromY, float toX, float toY)
{
	return atan2f(toX - fromX, toY - fromY);
}

//=====================================================================
// 角度の範囲を修正する処理
//=====================================================================
float GetFixedRotation(float fRot)
{
	// 角度の範囲を-3.14~3.14に収める
	if (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2;
	}
	else if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2;
	}

	return fRot;
}

D3DXVECTOR3 GetFixedRotation(D3DXVECTOR3 rot)
{
	// 角度の範囲を-3.14~3.14に収める
	if (rot.x > D3DX_PI)
	{
		rot.x -= D3DX_PI * 2;
	}
	else if (rot.x < -D3DX_PI)
	{
		rot.x += D3DX_PI * 2;
	}

	if (rot.y > D3DX_PI)
	{
		rot.y -= D3DX_PI * 2;
	}
	else if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2;
	}

	if (rot.z > D3DX_PI)
	{
		rot.z -= D3DX_PI * 2;
	}
	else if (rot.z < -D3DX_PI)
	{
		rot.z += D3DX_PI * 2;
	}

	return rot;
}

//=====================================================================
// 3次元ベクトルから2次元ベクトルへの変換処理
//=====================================================================
D3DXVECTOR2 Vector3To2(D3DXVECTOR3 source)
{
	return D3DXVECTOR2(source.x, source.y);
}

//=====================================================================
// 2次元ベクトルから3次元ベクトルへの変換処理
//=====================================================================
D3DXVECTOR3 Vector2To3(D3DXVECTOR2 source, float fValueZ)
{
	return D3DXVECTOR3(source.x, source.y, fValueZ);
}

//=====================================================================
// ベクトルとベクトルの交差判定
//=====================================================================
bool IsCrossingVector(
	D3DXVECTOR3 vec0,
	D3DXVECTOR3 vec1,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 posOld,
	D3DXVECTOR3* vecHit
)
{
	D3DXVECTOR3 vecLine = vec1 - vec0;
	D3DXVECTOR3 vecMove = pos - posOld;
	D3DXVECTOR3 vecToPos = pos - vec0;
	D3DXVECTOR3 vecToOld = posOld - vec0;


	return false;
}

//=====================================================================
// 指定した範囲の乱数（整数）を取得する処理
//=====================================================================
int RandRange(int nMin, int nMax)
{
	return (rand() % (nMax - nMin)) + nMin;
}

//=====================================================================
// バイナリファイル読み込み処理
//=====================================================================
bool LoadBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount)
{
	FILE* pFile;

	pFile = fopen(pFilePath, "rb");

	if (pFile != NULL)
	{
		fread(pBuffer, ElementSize, ElementCount, pFile);

		fclose(pFile);

		return true;
	}
	else
	{
		return false;
	}
}

//=====================================================================
// バイナリファイル書き出し処理
//=====================================================================
bool SaveBin(const char* pFilePath, void* pBuffer, size_t ElementSize, size_t ElementCount)
{
	FILE* pFile;

	pFile = fopen(pFilePath, "wb");

	if (pFile != NULL)
	{
		fwrite(pBuffer, ElementSize, ElementCount, pFile);

		fclose(pFile);

		return true;
	}
	else
	{
		return false;
	}
}
