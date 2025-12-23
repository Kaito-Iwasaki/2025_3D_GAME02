//=====================================================================
//
// Ranking [Ranking.h]
// Author : 
// 
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "scene.h"
#include "ranking.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "font.h"
#include "player.h"
#include "util.h"
#include "camera.h"
#include "model_loader.h"
#include "polygon.h"
#include "sphere.h"
#include "cylinder.h"
#include "field.h"
#include "decal.h"

//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
typedef enum
{
	RANKINGSTATE_INTRO = 0,
	RANKINGSTATE_NORMAL,
	RANKINGSTATE_MAX
}RANKINGSTATE;

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define FILEPATH_RANKING		"data\\ranking.bin"
#define MAX_PLACE				(5)

#define INIT_RANKING			{ 15000, 10000, 7500, 5000, 3000 }

#define FADE_START				(600)

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
int compare(const void* arg1, const void* arg2);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
int g_aRanking[MAX_PLACE] = INIT_RANKING;

RANKINGSTATE g_stateRanking = RANKINGSTATE_INTRO;
int g_nCountStateRanking = 0;
int g_nHighlight = -1;
FONT* g_apFontNum[MAX_PLACE] = {};
SCRIPTDATA g_dataRanking;

//=====================================================================
// 初期化処理
//=====================================================================
void InitRanking(void)
{
	char aString[MAX_PATH] = {};

	InitCamera();
	InitModel();
	InitSphere();
	InitCylinder();
	InitField();
	InitPolygon();
	InitDecal();
	InitFont();

	g_stateRanking = RANKINGSTATE_INTRO;
	g_nCountStateRanking = 0;
	memset(g_apFontNum, 0, sizeof(g_apFontNum));

	LoadBin(FILEPATH_RANKING, &g_aRanking[0], sizeof(int), MAX_PLACE);

	qsort(&g_aRanking[0], MAX_PLACE, sizeof(int), compare);

	SetDecal(
		DECAL_LABEL_NULL,
		D3DXVECTOR3(SCREEN_CENTER, SCREEN_VCENTER, 0),
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0),
		D3DXVECTOR3(0, 0, 0),
		D3DXCOLOR(0, 0, 0, 0.5f)
	);

	SetFont(
		FONT_LABEL_DONGURI,
		D3DXVECTOR3(0, 50.0f, 0.0f),
		D3DXVECTOR3(SCREEN_WIDTH, 100.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		80,
		"RANKING",
		DT_CENTER
	);

	for (int nCount = 0; nCount < MAX_PLACE; nCount++)
	{
		sprintf(&aString[0], "%2d, %06d", nCount + 1, g_aRanking[nCount]);
		float posX;

		if (nCount % 2 == 0)
		{
			posX = 0 - SCREEN_WIDTH / 2;
		}
		else
		{
			posX = SCREEN_WIDTH - SCREEN_WIDTH / 2;
		}

		g_apFontNum[nCount] = SetFont(
			FONT_LABEL_DONGURI,
			D3DXVECTOR3(posX, 150.0f + (nCount * 100.0f), 0.0f),
			D3DXVECTOR3(SCREEN_WIDTH, 100.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			100,
			&aString[0],
			DT_CENTER
		);
	}

	ZeroMemory(&g_dataRanking, sizeof(SCRIPTDATA));
	LoadScript("data\\model.txt", &g_dataRanking);

	for (int nCntMesh = 0; nCntMesh < g_dataRanking.nNumModel; nCntMesh++)
	{
		LoadModel(&g_dataRanking.aFilenameModel[nCntMesh][0], nCntMesh);
	}

	for (int nCntModel = 0; nCntModel < g_dataRanking.nCountModelSet; nCntModel++)
	{
		SetModel(
			g_dataRanking.aInfoModelSet[nCntModel].nType,
			g_dataRanking.aInfoModelSet[nCntModel].pos,
			D3DXVECTOR3(
				D3DXToRadian(g_dataRanking.aInfoModelSet[nCntModel].rot.x),
				D3DXToRadian(g_dataRanking.aInfoModelSet[nCntModel].rot.y),
				D3DXToRadian(g_dataRanking.aInfoModelSet[nCntModel].rot.z)
			),
			g_dataRanking.aInfoModelSet[nCntModel].collisionType
		);
	}

	if (GetPreviousScene() != SCENE_RESULT)
	{
		StopSound();
		PlaySound(SOUND_LABEL_BGM_RESULT);
	}

	CAMERA* pCamera = GetCamera();
	pCamera->nMode = 3;
	pCamera->posOffset = D3DXVECTOR3(0, 1000, 0);
	pCamera->fDistance = 3000;
	SetCameraPosR(D3DXVECTOR3(-5000, 3000, 2000));
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitRanking(void)
{
	g_nHighlight = -1;
	memset(g_apFontNum, 0, sizeof(g_apFontNum));

	UninitModel();
	UninitSphere();
	UninitCylinder();
	UninitField();
	UninitPolygon();
	UninitDecal();
	UninitFont();
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateRanking(void)
{
	UpdateModel();
	UpdateSphere();
	UpdateCylinder();
	UpdateField();
	UpdatePolygon();

	for (int nCount = 0; nCount < MAX_PLACE; nCount++)
	{
		if (nCount % 2 == 0)
		{
			g_apFontNum[nCount]->obj.pos.x += 15.0f;
			Clampf(&g_apFontNum[nCount]->obj.pos.x, g_apFontNum[nCount]->obj.pos.x, 0.0f);
		}
		else
		{
			g_apFontNum[nCount]->obj.pos.x -= 15.0f;
			Clampf(&g_apFontNum[nCount]->obj.pos.x, 0.0f, g_apFontNum[nCount]->obj.pos.x);
		}
	}

	if (g_nHighlight != -1 && g_nCountStateRanking % 5 == 0)
	{
		if (g_nCountStateRanking % 10 == 0)
		{
			g_apFontNum[g_nHighlight]->obj.color = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{
			g_apFontNum[g_nHighlight]->obj.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	g_nCountStateRanking++;

	if (INPUT_TRIGGER_UI_CONTINUE || g_nCountStateRanking > FADE_START)
	{
		SetFade(SCENE_TITLE);
	}
}

//=====================================================================
// 描画処理
//=====================================================================
void DrawRanking(void)
{
	DrawSphere();
	DrawCylinder();
	DrawField();
	DrawPolygon();
	DrawModel();
	DrawDecal();
	DrawFont();
}

void SaveScore(int nScore)
{
	int* pRanking;
	char pFilename[MAX_PATH];

	pRanking = &g_aRanking[0];
	strcpy(pFilename, FILEPATH_RANKING);

	// ファイルからランキングを読み込み
	LoadBin(pFilename, pRanking, sizeof(int), MAX_PLACE);

	// 並び変える
	qsort(pRanking, MAX_PLACE, sizeof(int), compare);

	if (pRanking[MAX_PLACE - 1] < nScore)
	{// 一番低いスコアより現在のスコアの方が高かったら置き換える
		pRanking[MAX_PLACE - 1] = nScore;
	}
	else
	{// そうでなければ保存しない
		return;
	}

	// 並び変える
	qsort(pRanking, MAX_PLACE, sizeof(int), compare);

	// 点滅情報の設定
	for (int nCount = MAX_PLACE - 1; nCount >= 0; nCount--)
	{// ランキングを下から確認していく
		if (pRanking[nCount] == nScore)
		{
			// 同じだったら点滅情報を設定
			g_nHighlight = nCount;
			break;
		}
	}

	// ファイルにランキングを書き出し
	SaveBin(pFilename, pRanking, sizeof(int), MAX_PLACE);
}

//=====================================================================
// qsort関数の判定処理
//=====================================================================
int compare(const void* arg1, const void* arg2)
{
	int va = *(const int*)arg1;
	int vb = *(const int*)arg2;

	if (va > vb) return -1;
	if (va < vb) return 1;
	return 0;
}