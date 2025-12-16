//=====================================================================
//
// DebugProc [DebugProc.cpp]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "DebugProc.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** 構造体 *****
// 
//*********************************************************************


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


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPD3DXFONT g_pFontDebug = NULL;
char g_aStrDebug[2048];

//=====================================================================
// 初期化処理
//=====================================================================
void InitDebugProc(void)
{
#ifdef _DEBUG
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXFONT_DESC fontDesc = {
		30,								// Height
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

	D3DXCreateFontIndirect(pDevice, &fontDesc, &g_pFontDebug);

	ZeroMemory(&g_aStrDebug[0], sizeof(g_aStrDebug));
#endif
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitDebugProc(void)
{
#ifdef _DEBUG
	if (g_pFontDebug != NULL)
	{
		g_pFontDebug->Release();
		g_pFontDebug = NULL;
	}
#endif
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateDebugProc(void)
{
#ifdef _DEBUG

#endif
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawDebugProc(void)
{
#ifdef _DEBUG
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	g_pFontDebug->DrawText(NULL, &g_aStrDebug[0], -1, &rect, DT_TOP | DT_LEFT, D3DXCOLOR_WHITE);

	ZeroMemory(&g_aStrDebug[0], sizeof(g_aStrDebug));
#endif
}

void PrintDebugProc(const char* fmt, ...)
{
#ifdef _DEBUG
	va_list ap;
	char aBuffer[2048] = {};
	char aValue[64] = {};

	va_start(ap, fmt);

	for (int i = 0; i < strlen(fmt); i++)
	{
		if (fmt[i] == '%')
		{
			switch (fmt[i + 1])
			{
			case 'd':
				sprintf(aValue, "%d", va_arg(ap, int));
				break;

			case 'f':
				sprintf(aValue, "%f", (float)va_arg(ap, double));
				break;

			case 'c':
				sprintf(aValue, "%c", va_arg(ap, char));
				break;

			case 's':
				sprintf(aValue, "%s", va_arg(ap, char*));
				break;
			}
			strcat(aBuffer, aValue);
			i++;
		}
		else
		{
			aBuffer[strlen(aBuffer)] = fmt[i];
		}
	}

	va_end(ap);

	strcat(&g_aStrDebug[0], &aBuffer[0]);
#endif
}