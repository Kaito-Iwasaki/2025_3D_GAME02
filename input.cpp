//=====================================================================
// 
// 入力処理 [input.cpp]
// Author : Kaito Iwasaki
//
//=====================================================================

//*********************************************************************
// 
// ***** インクルードファイル *****
// 
//*********************************************************************
#include "input.h"
#include "util.h"

//*********************************************************************
// 
// ***** マクロ定義 *****
// 
//*********************************************************************


//*********************************************************************
// 
// ***** グローバル変数 *****
// 
//*********************************************************************
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;

BYTE g_aKeyState[NUM_KEY_MAX];
BYTE g_aKeyTriggerState[NUM_KEY_MAX];
BYTE g_aKeyReleaseState[NUM_KEY_MAX];
int g_aKeyRepeatState[NUM_KEY_MAX];

DIMOUSESTATE g_aMouseState;
BYTE g_aMouseTriggerState[MOUSE_MAX];
BYTE g_aMouseReleaseState[MOUSE_MAX];

XINPUT_STATE g_joyKeyState;
XINPUT_STATE g_joyKeyTriggerState;
XINPUT_STATE g_joyKeyReleaseState;
int g_aJoyKeyRepeatState[JOYKEY_MAX];
int g_aJoystickState[JOYSTICK_MAX];
XINPUT_VIBRATION g_vibration;
int g_nCountVibration = 0;

//=====================================================================
// 
// ***** キーボード *****
//
//=====================================================================
//=====================================================================
// 初期化処理
//=====================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	LPDIRECTINPUT8 pInput = GetInput();
	
	// 入力デバイスの生成
	if (FAILED(pInput->CreateDevice(
		GUID_SysKeyboard,
		&g_pDevKeyboard,
		NULL
	)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	)))
	{
		return E_FAIL;
	}

	// キーボードのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitKeyboard(void)
{
	// 入力デバイス（キーボード）破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	// キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyTriggerState[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			g_aKeyReleaseState[nCntKey] = g_aKeyState[nCntKey] & (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]);
			g_aKeyState[nCntKey] = aKeyState[nCntKey];

			if (GetKeyboardPress(nCntKey))
			{
				g_aKeyRepeatState[nCntKey] += 1;
			}
			else
			{
				g_aKeyRepeatState[nCntKey] = 0;
			}
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();	// キーボードのアクセス権利を再取得
	}
}

//=====================================================================
// キーボードのプレス情報を取得
//=====================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//=====================================================================
// キーボードのトリガー情報を取得
//=====================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyTriggerState[nKey] & 0x80) ? true : false;
}

//=====================================================================
// キーボードのリリース情報を取得
//=====================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyReleaseState[nKey] & 0x80) ? true : false;
}

//=====================================================================
// キーボードのリピート情報を取得
//=====================================================================
bool GetKeyboardRepeat(int nKey, int nInterval)
{
	return (g_aKeyRepeatState[nKey] == 1 || g_aKeyRepeatState[nKey] >= INPUT_REPEAT_START && g_aKeyRepeatState[nKey] % nInterval == 0) ? true : false;
}

//=====================================================================
// キーボードの状態を取得
//=====================================================================
BYTE* GetKeyState(void)
{
	return &g_aKeyState[0];
}

//=====================================================================
// 
// ***** マウス *****
//
//=====================================================================
//=====================================================================
// 初期化処理
//=====================================================================
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	LPDIRECTINPUT8 pInput = GetInput();

	// 入力デバイスの生成
	if (FAILED(pInput->CreateDevice(
		GUID_SysMouse,
		&g_pDevMouse,
		NULL
	)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevMouse->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	)))
	{
		return E_FAIL;
	}

	// マウスのアクセス権を獲得
	g_pDevMouse->Acquire();

	return S_OK;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitMouse(void)
{
	// 入力デバイス（マウス）破棄
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateMouse()
{
	DIMOUSESTATE aMouseState;	// キーボードの入力情報

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &aMouseState)))
	{
		for (int nCntKey = 0; nCntKey < MOUSE_MAX; nCntKey++)
		{
			g_aMouseTriggerState[nCntKey] = (g_aMouseState.rgbButtons[nCntKey] ^ aMouseState.rgbButtons[nCntKey]) & aMouseState.rgbButtons[nCntKey];
			g_aMouseReleaseState[nCntKey] = g_aMouseState.rgbButtons[nCntKey] & (g_aMouseState.rgbButtons[nCntKey] ^ aMouseState.rgbButtons[nCntKey]);
			g_aMouseState.rgbButtons[nCntKey] = aMouseState.rgbButtons[nCntKey];
		}
		g_aMouseState.lX = aMouseState.lX;
		g_aMouseState.lY = aMouseState.lY;
		g_aMouseState.lZ = aMouseState.lZ;
	}
	else
	{
		g_pDevMouse->Acquire();	// キーボードのアクセス権利を再取得
	}
}

//=====================================================================
// マウスのプレス情報を取得
//=====================================================================
bool GetMousePress(MOUSE key)
{
	return (g_aMouseState.rgbButtons[key] & 0x80) ? true : false;
}

//=====================================================================
// マウスのトリガー情報を取得
//=====================================================================
bool GetMouseTrigger(MOUSE key)
{
	return (g_aMouseTriggerState[key] & 0x80) ? true : false;
}

//=====================================================================
// マウスのリリース情報を取得
//=====================================================================
bool GetMouseRelease(MOUSE key)
{
	return (g_aMouseReleaseState[key] & 0x80) ? true : false;
}

//=====================================================================
// マウスの取得処理
//=====================================================================
DIMOUSESTATE GetMouse(void)
{
	return g_aMouseState;
}

//=====================================================================
// マウス位置の取得処理
//=====================================================================
D3DXVECTOR2 GetMousePos(void)
{
	HWND hWnd = GetMainWindow();
	RECT rectCurrent;
	POINT point;

	GetClientRect(hWnd, &rectCurrent);
	GetCursorPos(&point);

	ScreenToClient(hWnd, &point);

	point.x = Clamp((int)(SCREEN_WIDTH * ((float)point.x / (rectCurrent.right - rectCurrent.left))), 0, SCREEN_WIDTH - 1);
	point.y = Clamp((int)(SCREEN_HEIGHT * ((float)point.y / (rectCurrent.bottom - rectCurrent.top))), 0, SCREEN_HEIGHT - 1);

	return D3DXVECTOR2(point.x, point.y);
}

//=====================================================================
// 
// ***** ジョイパッド *****
//
//=====================================================================
//=====================================================================
// 初期化処理
//=====================================================================
HRESULT InitJoypad(void)
{
	// メモリのクリア
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));
	memset(&g_vibration, 0, sizeof(XINPUT_VIBRATION));
	g_nCountVibration = 0;

	// XINPUTのステートを有効にする
	XInputEnable(true);

	return S_OK;
}

//=====================================================================
// 終了処理
//=====================================================================
void UninitJoypad(void)
{
	// XInputのステートを無効
	XInputEnable(false);
}

//=====================================================================
// 更新処理
//=====================================================================
void UpdateJoypad(void)
{
	XINPUT_STATE joyKeyState;
	XINPUT_GAMEPAD gamepad;

	// ジョイパッドの状態
	if (XInputGetState(0, &joyKeyState) == ERROR_SUCCESS)
	{
		// ジョイキー
		gamepad = joyKeyState.Gamepad;

		g_joyKeyTriggerState.Gamepad.wButtons = (g_joyKeyState.Gamepad.wButtons ^ joyKeyState.Gamepad.wButtons)
			& joyKeyState.Gamepad.wButtons;
		g_joyKeyReleaseState.Gamepad.wButtons = g_joyKeyState.Gamepad.wButtons & (g_joyKeyState.Gamepad.wButtons
			^ joyKeyState.Gamepad.wButtons);
		g_joyKeyState = joyKeyState;

		for (int nCntKey = 0; nCntKey < JOYKEY_MAX; nCntKey++)
		{
			if (GetJoypadPress((JOYKEY)nCntKey))
			{
				g_aJoyKeyRepeatState[nCntKey] += 1;
			}
			else
			{
				g_aJoyKeyRepeatState[nCntKey] = 0;
			}
		}

		// スティック
		if (gamepad.sThumbLX < -INPUT_DEADZONE)
		{// L左
			g_aJoystickState[JOYSTICK_L_LEFT]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_L_LEFT] = 0;
		}

		if (gamepad.sThumbLX > INPUT_DEADZONE)
		{// L右
			g_aJoystickState[JOYSTICK_L_RIGHT]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_L_RIGHT] = 0;
		}

		if (gamepad.sThumbLY < -INPUT_DEADZONE)
		{// L下
			g_aJoystickState[JOYSTICK_L_DOWN]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_L_DOWN] = 0;
		}

		if (gamepad.sThumbLY > INPUT_DEADZONE)
		{// L上
			g_aJoystickState[JOYSTICK_L_UP]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_L_UP] = 0;
		}

		if (gamepad.sThumbLY < -INPUT_DEADZONE)
		{// R左
			g_aJoystickState[JOYSTICK_R_LEFT]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_R_LEFT] = 0;
		}

		if (gamepad.sThumbLY > INPUT_DEADZONE)
		{// R右
			g_aJoystickState[JOYSTICK_R_RIGHT]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_R_RIGHT] = 0;
		}

		if (gamepad.sThumbLY < -INPUT_DEADZONE)
		{// R下
			g_aJoystickState[JOYSTICK_R_DOWN]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_R_DOWN] = 0;
		}

		if (gamepad.sThumbLY > INPUT_DEADZONE)
		{// R上
			g_aJoystickState[JOYSTICK_R_UP]++;
		}
		else
		{
			g_aJoystickState[JOYSTICK_R_UP] = 0;
		}
	}

	// 振動情報の更新
	if (g_nCountVibration > -1) g_nCountVibration--;
	if (g_nCountVibration == 0)
	{
		g_vibration.wLeftMotorSpeed = 0;
		g_vibration.wRightMotorSpeed = 0;
		XInputSetState(0, &g_vibration);
	}
}

//=====================================================================
// ジョイパッドの取得
//=====================================================================
XINPUT_STATE* GetJoypad(void)
{
	return &g_joyKeyState;
}

//=====================================================================
// ジョイパッドのプレス情報を取得
//=====================================================================
bool GetJoypadPress(JOYKEY key)
{
	return (g_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=====================================================================
// ジョイパッドのトリガー情報を取得
//=====================================================================
bool GetJoypadTrigger(JOYKEY key)
{
	return (g_joyKeyTriggerState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=====================================================================
// ジョイパッドのリリース情報を取得
//=====================================================================
bool GetJoypadRelease(JOYKEY key)
{
	return (g_joyKeyReleaseState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//=====================================================================
// ジョイパッドのリピート情報を取得
//=====================================================================
bool GetJoypadRepeat(JOYKEY key, int nInterval)
{
	return (
		g_aJoyKeyRepeatState[key] == 1 ||
		g_aJoyKeyRepeatState[key] >= INPUT_REPEAT_START &&
		g_aJoyKeyRepeatState[key] % nInterval == 0
		) ? true : false;
}

//=====================================================================
// ジョイスティックのプレス情報を取得
//=====================================================================
bool GetJoystickPress(JOYSTICK stick)
{
	return g_aJoystickState[stick] > 0;
}

//=====================================================================
// ジョイスティックのプレス情報を取得
//=====================================================================
bool GetJoystickTrigger(JOYSTICK stick)
{
	return g_aJoystickState[stick] == 1;
}

//=====================================================================
// ジョイスティックのリピート情報を取得
//=====================================================================
bool GetJoystickRepeat(JOYSTICK stick, int nInterval)
{
	return (
		g_aJoystickState[stick] == 1 ||
		g_aJoystickState[stick] >= INPUT_REPEAT_START &&
		g_aJoystickState[stick] % nInterval == 0
		) ? true : false;
}

//=====================================================================
// 振動情報の設定処理
//=====================================================================
void SetVibration(WORD wLeftMotorSpeed, WORD wRightMotorSpeed, int nCountVibration)
{
	g_nCountVibration = nCountVibration;
	g_vibration.wLeftMotorSpeed = wLeftMotorSpeed; // use any value between 0-65535 here
	g_vibration.wRightMotorSpeed = wRightMotorSpeed; // use any value between 0-65535 here
	XInputSetState(0, &g_vibration);
}