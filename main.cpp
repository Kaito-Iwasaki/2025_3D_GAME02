//=====================================================================
// 
// 3Dゲーム [main.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include <crtdbg.h>
#include "main.h"
#include "input.h"
#include "sound.h"
#include "scene.h"
#include "Game.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************
#define CLASS_NAME		"WindowsClass"				// ウィンドウクラスの名前
#define WINDOW_NAME		"DirectX9"					// ウィンドウの名前
#define DEFAULT_COLOR	D3DXCOLOR(0, 0, 0, 255)		// 画面クリア時の背景色

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindowed);
void Uninit(void);
void Update(void);
void Draw(void);

//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECT3D9 g_pD3D = NULL;					// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;		// Direct3Dデバイスへのポインタ
LPDIRECTINPUT8 g_pInput = NULL;
HWND g_hWnd;

//=====================================================================
// 
// ***** メイン関数 *****
// 
//=====================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
	// メモリリーク検知用のフラグ
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif

	// ウィンドウクラスの設定
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),						// WNDCLASSのメモリサイズ
		CS_CLASSDC,								// ウィンドウのスタイル
		WindowProc,								// ウィンドウプロシージャ
		0,										// 0（使わない）
		0,										// 0（使わない）
		hInstance,								// インスタンスハンドル
		LoadIcon(hInstance, IDI_APPLICATION),	// タスクバーのアイコン
		LoadCursor(NULL, IDC_ARROW),			// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),				// クライアント領域の背景色
		NULL,									// メニューバー
		CLASS_NAME,								// ウィンドウクラスの名前
		LoadIcon(hInstance, IDI_APPLICATION)	// ファイルのアイコン
	};

	MSG msg;											// メッセージ格納用
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };	// 画面サイズ
	DWORD dwCurrentTime;								// 現在の時刻
	DWORD dwExecLastTime;								// 最終処理時の時刻
	DWORD dwFrameCount;									// フレームカウント
	DWORD dwFPSLastTime;								// 

	// ウィンドウの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウを生成
	g_hWnd = CreateWindowEx(
		0,							// 拡張ウィンドウスタイル
		CLASS_NAME,					// ウィンドウクラスの名前
		WINDOW_NAME,				// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,		// ウィンドウスタイル
		CW_USEDEFAULT,				// ウィンドウの左上X座標
		CW_USEDEFAULT,				// ウィンドウの左上Y座標
		(rect.right - rect.left),	// ウィンドウの幅
		(rect.bottom - rect.top),	// ウィンドウの高さ
		NULL,						// 親ウィンドウのハンドル
		NULL,						// メニューハンドルまたは子ウィンドウID
		hInstance,					// インスタンスハンドル
		NULL						// ウィンドウ作成データ
	);

	// 初期化処理
	if (FAILED(Init(hInstance, g_hWnd, WINDOWED)))
	{// 初期化処理が失敗したら終了
		return -1;
	}

	// 分解能を設定(タイマーの精度を1msにする)
	timeBeginPeriod(1);

	dwCurrentTime = timeGetTime();
	dwExecLastTime = dwCurrentTime;

	// ウィンドウの表示
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windowsの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受信したらプログラムを終了する
				break;
			}
			else
			{
				// メッセージの設定
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectXの処理
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60分の1秒経過
				// 処理時刻を記録
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();
			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=====================================================================
// ***** ウィンドウプロシージャ *****
//=====================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:	// ウィンドウ破棄のメッセージ
		PostQuitMessage(0);	// WM_QUITメッセージを送る
		break;

	case WM_KEYDOWN:	// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				// ウィンドウを破棄する（WM_DESTROYメッセージを送る）
				DestroyWindow(hWnd);
			}
			else
			{
				return 0;
			}

			break;
		}
		break;

	case WM_CLOSE:	// 閉じるボタン押下のメッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{
			// ウィンドウを破棄する（WM_DESTROYメッセージを送る）
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;	// 0を返さないと終了してしまう
		}
		break;

	case WM_SETFOCUS:
#ifdef _DEBUG
		if (GetCurrentScene() == SCENE_GAME)
		{
			ReloadGameModel();
		}
		break;
#endif
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 既定の処理を返す
}

//=====================================================================
// 初期化処理
//=====================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindowed)
{
	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;
	HRESULT hr;

	// 乱数の種を設定
	srand((unsigned int)timeGetTime());

	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	hr = g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindowed;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// Direct3Dデバイスの作成
	hr = g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice
	);
	if (FAILED(hr))
	{// 描画処理をハードウェア、頂点処理をCPUに
		hr = g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice
		);
		if (FAILED(hr))
		{// 描画処理、頂点処理ともにCPUに
			hr = g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice
			);
			if (FAILED(hr))
			{// すべて不可能なら終了
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャの拡縮補完の設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャの拡縮補完の設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャの繰り返しの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャの繰り返しの設定

	// テクスチャステージステート
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// DirectInputオブジェクトの作成
	hr = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&g_pInput,
		NULL
	);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// キーボードの初期化処理
	hr = InitKeyboard(hInstance, hWnd);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// マウスの初期化処理
	hr = InitMouse(hInstance, hWnd);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// ジョイパッドの初期化処理
	hr = InitJoypad();
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// サウンドの初期化処理
	InitSound(hWnd);

	// シーンの初期化処理
	InitScene();

	return S_OK;
}

//=====================================================================
// 終了処理
//=====================================================================
void Uninit(void)
{
	// シーンの終了処理
	UninitScene();

	// サウンドの終了処理
	UninitSound();

	// キーボードの終了処理
	UninitKeyboard();

	// キーボードの終了処理
	UninitMouse();

	// ジョイパッドの終了処理
	UninitJoypad();

	// DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}

	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void Update(void)
{
	// キーボードの更新処理
	UpdateKeyboard();

	// キーボードの更新処理
	UpdateMouse();

	// ジョイパッドの更新処理
	UpdateJoypad();

	// シーンの更新処理
	UpdateScene();
}

//=====================================================================
// 描画処理
//=====================================================================
void Draw(void)
{
	// 画面クリア（バックバッファとＺバッファのクリア）
	g_pD3DDevice->Clear(
		0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		DEFAULT_COLOR, 1.0f, 0
	);

	// 描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// オブジェクトの描画
		// シーンを描画
		DrawScene();

		// 描画終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=====================================================================
// Direct3Dデバイスの取得処理
//=====================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=====================================================================
// DirectInputオブジェクトの取得処理
//=====================================================================
LPDIRECTINPUT8 GetInput(void)
{
	return g_pInput;
}

//=====================================================================
// ウィンドウの取得処理
//=====================================================================
HWND GetMainWindow(void)
{
	return g_hWnd;
}