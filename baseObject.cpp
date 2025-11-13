//=====================================================================
//
// 描画オブジェクト [baseObject.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "baseObject.h"

//=====================================================================
// 頂点位置の設定処理（自動）
//=====================================================================
void SetVertexPos(VERTEX_2D* pVtx, BASEOBJECT obj)
{
	float fLength = sqrtf(obj.size.x * obj.size.x + obj.size.y * obj.size.y) * 0.5f;
	float fAngle = atan2f(obj.size.x, obj.size.y);

	pVtx[0].pos.x = obj.pos.x + sinf(obj.rot.z + D3DX_PI + fAngle) * fLength;
	pVtx[0].pos.y = obj.pos.y + cosf(obj.rot.z + D3DX_PI + fAngle) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = obj.pos.x + sinf(obj.rot.z + D3DX_PI - fAngle) * fLength;
	pVtx[1].pos.y = obj.pos.y + cosf(obj.rot.z + D3DX_PI - fAngle) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = obj.pos.x + sinf(obj.rot.z - fAngle) * fLength;
	pVtx[2].pos.y = obj.pos.y + cosf(obj.rot.z - fAngle) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = obj.pos.x + sinf(obj.rot.z + fAngle) * fLength;
	pVtx[3].pos.y = obj.pos.y + cosf(obj.rot.z + fAngle) * fLength;
	pVtx[3].pos.z = 0.0f;
}

//=====================================================================
// 頂点位置の設定処理（手動）
//=====================================================================
void SetVertexPos(VERTEX_2D* pVtx, D3DXVECTOR3 pos, D3DXVECTOR3 size, float fRot)
{
	float fLength = sqrtf(size.x * size.x + size.y * size.y) * 0.5f;
	float fAngle = atan2f(size.x, size.y);

	pVtx[0].pos.x = pos.x + sinf(fRot + D3DX_PI + fAngle) * fLength;
	pVtx[0].pos.y = pos.y + cosf(fRot + D3DX_PI + fAngle) * fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pos.x + sinf(fRot + D3DX_PI - fAngle) * fLength;
	pVtx[1].pos.y = pos.y + cosf(fRot + D3DX_PI - fAngle) * fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pos.x + sinf(fRot - fAngle) * fLength;
	pVtx[2].pos.y = pos.y + cosf(fRot - fAngle) * fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pos.x + sinf(fRot + fAngle) * fLength;
	pVtx[3].pos.y = pos.y + cosf(fRot + fAngle) * fLength;
	pVtx[3].pos.z = 0.0f;
}

//=====================================================================
// 頂点位置の設定処理（手動）
//=====================================================================
void SetVertexPos(VERTEX_2D* pVtx, D3DXVECTOR3 leftTop, D3DXVECTOR3 rightTop, D3DXVECTOR3 leftBottom, D3DXVECTOR3 rightBottom)
{
	pVtx[0].pos.x = leftTop.x;
	pVtx[0].pos.y = leftTop.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = rightTop.x;
	pVtx[1].pos.y = rightTop.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = leftBottom.x;
	pVtx[2].pos.y = leftBottom.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = rightBottom.x;
	pVtx[3].pos.y = rightBottom.y;
	pVtx[3].pos.z = 0.0f;
}

//=====================================================================
// 頂点RHWの設定処理
//=====================================================================
void SetVertexRHW(VERTEX_2D* pVtx, float rhw)
{
	pVtx[0].rhw = rhw;
	pVtx[1].rhw = rhw;
	pVtx[2].rhw = rhw;
	pVtx[3].rhw = rhw;
}

//=====================================================================
// 頂点カラーの設定処理
//=====================================================================
void SetVertexColor(VERTEX_2D* pVtx, D3DXCOLOR col)
{
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
}

//=====================================================================
// テクスチャ位置の設定処理（自動）
//=====================================================================
void SetVertexTexturePos(VERTEX_2D* pVtx, bool bInversed)
{
	if (bInversed)
	{// 反転
		pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
	}
	else
	{// 通常
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

}

//=====================================================================
// テクスチャ位置の設定処理（手動）
//=====================================================================
void SetVertexTexturePos(VERTEX_2D* pVtx, D3DXVECTOR2 leftTop, D3DXVECTOR2 rightTop, D3DXVECTOR2 leftBottom, D3DXVECTOR2 rightBottom, bool bInversed)
{
	if (bInversed)
	{// 反転
		pVtx[0].tex = rightTop;
		pVtx[1].tex = leftTop;
		pVtx[2].tex = rightBottom;
		pVtx[3].tex = leftBottom;
	}
	else
	{// 通常
		pVtx[0].tex = leftTop;
		pVtx[1].tex = rightTop;
		pVtx[2].tex = leftBottom;
		pVtx[3].tex = rightBottom;
	}
}

//=====================================================================
// テクスチャ位置の設定処理（テクスチャアニメーション）
//=====================================================================
void SetVertexTexturePos(VERTEX_2D* pVtx, int nTextureX, int nTextureY, int nSizeX, int nSizeY, bool bInversed)
{
	if (bInversed)
	{// 反転
		pVtx[0].tex = D3DXVECTOR2((1.0f / nSizeX) * (nTextureX + 1) - 0.01f, (1.0f / nSizeY) * nTextureY + 0.01f);
		pVtx[1].tex = D3DXVECTOR2((1.0f / nSizeX) * nTextureX, (1.0f / nSizeY) * nTextureY + 0.01f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / nSizeX) * (nTextureX + 1) - 0.01f, (1.0f / nSizeY) * (nTextureY + 1));
		pVtx[3].tex = D3DXVECTOR2((1.0f / nSizeX) * nTextureX, (1.0f / nSizeY) * (nTextureY + 1));
	}
	else
	{// 通常
		pVtx[0].tex = D3DXVECTOR2((1.0f / nSizeX) * nTextureX + 0.01f, (1.0f / nSizeY) * nTextureY + 0.01f);
		pVtx[1].tex = D3DXVECTOR2((1.0f / nSizeX) * (nTextureX + 1), (1.0f / nSizeY) * nTextureY + 0.01f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / nSizeX) * nTextureX + 0.01f, (1.0f / nSizeY) * (nTextureY + 1));
		pVtx[3].tex = D3DXVECTOR2((1.0f / nSizeX) * (nTextureX + 1), (1.0f / nSizeY) * (nTextureY + 1));
	}
}

void SetVertexTexturePos(VERTEX_3D* pVtx, int nTextureX, int nTextureY, int nSizeX, int nSizeY, bool bInversed)
{
	if (bInversed)
	{// 反転
		pVtx[0].tex = D3DXVECTOR2((1.0f / nSizeX) * (nTextureX + 1) - 0.01f, (1.0f / nSizeY) * nTextureY + 0.01f);
		pVtx[1].tex = D3DXVECTOR2((1.0f / nSizeX) * nTextureX, (1.0f / nSizeY) * nTextureY + 0.01f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / nSizeX) * (nTextureX + 1) - 0.01f, (1.0f / nSizeY) * (nTextureY + 1));
		pVtx[3].tex = D3DXVECTOR2((1.0f / nSizeX) * nTextureX, (1.0f / nSizeY) * (nTextureY + 1));
	}
	else
	{// 通常
		pVtx[0].tex = D3DXVECTOR2((1.0f / nSizeX) * nTextureX + 0.01f, (1.0f / nSizeY) * nTextureY + 0.01f);
		pVtx[1].tex = D3DXVECTOR2((1.0f / nSizeX) * (nTextureX + 1), (1.0f / nSizeY) * nTextureY + 0.01f);
		pVtx[2].tex = D3DXVECTOR2((1.0f / nSizeX) * nTextureX + 0.01f, (1.0f / nSizeY) * (nTextureY + 1));
		pVtx[3].tex = D3DXVECTOR2((1.0f / nSizeX) * (nTextureX + 1), (1.0f / nSizeY) * (nTextureY + 1));
	}
}

bool IsObjectOutOfScreen(BASEOBJECT obj, DWORD flags)
{
	bool bOutofScreen = false;

	if (flags & OOS_TOP)
	{
		bOutofScreen = bOutofScreen || obj.pos.y < 0 - obj.size.y;
	}
	if (flags & OOS_BOTTOM)
	{
		bOutofScreen = bOutofScreen || obj.pos.y > SCREEN_HEIGHT + obj.size.y;
	}
	if (flags & OOS_LEFT)
	{
		bOutofScreen = bOutofScreen || obj.pos.x < 0 - obj.size.x;
	}
	if (flags & OOS_RIGHT)
	{
		bOutofScreen = bOutofScreen || obj.pos.x > SCREEN_WIDTH + obj.size.x;
	}

	return bOutofScreen;
}

bool IsObjectOutOfScreen(BASEOBJECT obj, RECT rect, DWORD flags)
{
	bool bOutofScreen = false;

	if (flags & OOS_TOP)
	{
		bOutofScreen = bOutofScreen || obj.pos.y < rect.top - obj.size.y;
	}
	if (flags & OOS_BOTTOM)
	{
		bOutofScreen = bOutofScreen || obj.pos.y > rect.bottom + obj.size.y;
	}
	if (flags & OOS_LEFT)
	{
		bOutofScreen = bOutofScreen || obj.pos.x < rect.left - obj.size.x;
	}
	if (flags & OOS_RIGHT)
	{
		bOutofScreen = bOutofScreen || obj.pos.x > rect.right + obj.size.x;
	}

	return bOutofScreen;
}
