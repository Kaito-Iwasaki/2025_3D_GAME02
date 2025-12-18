//=====================================================================
//
// 画像オブジェクト [font.cpp]
// Author : Kaito Iwasaki
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "input.h"
#include "sound.h"
#include "util.h"

#include "font.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define INIT_POS_X				(SCREEN_WIDTH / 2)
#define INIT_POS_Y				(SCREEN_HEIGHT/ 2)
#define INIT_SIZE_X				(8.0f)
#define INIT_SIZE_Y				(32.0f)
#define INIT_COLOR				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)

//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************
typedef struct
{
	char aFontFileName[MAX_PATH];
	char aFontName[LF_FACESIZE];
}FONT_INFO;

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
FONT g_aFont[MAX_FONT] = {};

FONT_INFO g_aFontInfo[FONT_LABEL_MAX] = {
	{ "", "Terminal"},
	{ "data\\FONT\\x10y12pxDonguriDuel.ttf", "x10y12pxDonguriDuel" },
};

//=====================================================================
// 初期化処理
//=====================================================================
void InitFont(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイス
	VERTEX_2D* pVtx;							// 頂点情報
	FONT* pFont = &g_aFont[0];

	// 構造体の初期化
	memset(pFont, 0, sizeof(FONT) * MAX_FONT);

	// フォントを追加
	for (int nCount = 0; nCount < FONT_LABEL_MAX; nCount++)
	{
		AddFontResourceEx(&g_aFontInfo[nCount].aFontFileName[0], FR_PRIVATE, 0);
	}
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitFont(void)
{
	for (int nCount = 0; nCount < MAX_FONT; nCount++)
	{
		if (g_aFont[nCount].font != NULL)
		{
			g_aFont[nCount].font->Release();
			g_aFont[nCount].font = NULL;
		}
	}

	for (int nCount = 0; nCount < FONT_LABEL_MAX; nCount++)
	{
		RemoveFontResourceEx(&g_aFontInfo[nCount].aFontFileName[0], FR_PRIVATE, 0);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawFont(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	FONT* pFont = &g_aFont[0];

	// デバイスの取得
	pDevice = GetDevice();

	pFont = &g_aFont[0];
	for (int nCount = 0; nCount < MAX_FONT; nCount++, pFont++)
	{
		if (pFont != NULL && pFont->obj.bVisible && pFont->bUsed)
		{// テキスト描画
			RECT rect;
			float fMagnitude;

			rect = {
				(long)pFont->obj.pos.x,
				(long)pFont->obj.pos.y,
				(long)(pFont->obj.pos.x + pFont->obj.size.x),
				(long)(pFont->obj.pos.y + pFont->obj.size.y),
			};

			pFont->font->DrawText(NULL, pFont->aText, -1, &rect, pFont->format, pFont->obj.color);
		}
	}
}

//=====================================================================
// オブジェクト取得処理
//=====================================================================
FONT* GetFont(void)
{
	return &g_aFont[0];
}

//=====================================================================
// 画像の設定処理
//=====================================================================
FONT* SetFont(
	FONT_LABEL type,		// 使用するフォント
	D3DXVECTOR3 pos,		// 位置
	D3DXVECTOR3 size,		// サイズ
	D3DXCOLOR col,			// 色
	int nScale,				// テキストのサイズ4
	const char* aText,		// 表示するテキスト
	UINT format				// テキストフォーマット
)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FONT* pFont = &g_aFont[0];
	for (int nCount = 0; nCount < MAX_FONT; nCount++, pFont++)
	{
		if (pFont->font == NULL)
		{
			memset(pFont, 0, sizeof(FONT));
			pFont->bUsed = true;
			pFont->obj.pos = pos;
			pFont->obj.size = size;
			pFont->obj.color = col;
			pFont->nID = nCount;
			pFont->type = type;
			pFont->format = format;
			pFont->obj.bVisible = true;
			strcpy(&pFont->aText[0], aText);

			D3DXFONT_DESC fontDesc = {
				nScale,							// Height
				0,								// Width (0 = 自動)
				FW_NORMAL,						// Weight
				1,								// MipLevels
				FALSE,							// Italic
				DEFAULT_CHARSET,				// CharSet
				OUT_DEFAULT_PRECIS,				// OutputPrecision
				CLIP_DEFAULT_PRECIS,			// Quality
				DEFAULT_PITCH | FF_DONTCARE,	// PitchAndFamily
				""
			};

			strcpy(&fontDesc.FaceName[0], &g_aFontInfo[type].aFontName[0]);

			D3DXCreateFontIndirect(pDevice, &fontDesc, &pFont->font);

			return pFont;
		}
	}

	return NULL;
}


//=====================================================================
// 画像の削除処理（指定）
//=====================================================================
void DeleteFont(int nID)
{
	memset(&g_aFont[nID], 0, sizeof(FONT));
}

//=====================================================================
// 画像の削除処理（一括）
//=====================================================================
void DeleteFont(void)
{
	memset(&g_aFont[0], 0, sizeof(FONT) * MAX_FONT);
}
