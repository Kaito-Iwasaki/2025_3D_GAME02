//=====================================================================
// 
// input.cppのヘッダファイル [input.h]
// Author : Kaito Iwasaki
//
//=====================================================================
#ifndef _INPUT_H_
#define _INPUT_H_		// 二重インクルード防止のマクロ

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
#define NUM_KEY_MAX				(256)	// 最大キー数
#define INPUT_MAX_MAGNITUDE		(32767)		// スティックの最大移動量
#define INPUT_DEADZONE			(10000)		// スティックのデッドゾーン（0～32767）
#define INPUT_REPEAT_START		(20)	// リピート開始までのカウント
#define INPUT_REPEAT_INTERVAL	(8)		// リピート毎のカウント

#define INPUT_REPEAT_UI_UP			(GetKeyboardRepeat(DIK_W) || GetKeyboardRepeat(DIK_UP) || GetJoypadRepeat(JOYKEY_UP) || GetJoystickRepeat(JOYSTICK_L_UP))
#define INPUT_REPEAT_UI_DOWN		(GetKeyboardRepeat(DIK_S) || GetKeyboardRepeat(DIK_DOWN) || GetJoypadRepeat(JOYKEY_DOWN) || GetJoystickRepeat(JOYSTICK_L_DOWN))
#define INPUT_REPEAT_UI_LEFT		(GetKeyboardRepeat(DIK_A) || GetKeyboardRepeat(DIK_LEFT) || GetJoypadRepeat(JOYKEY_LEFT) || GetJoystickRepeat(JOYSTICK_L_LEFT))
#define INPUT_REPEAT_UI_RIGHT		(GetKeyboardRepeat(DIK_D) || GetKeyboardRepeat(DIK_RIGHT) || GetJoypadRepeat(JOYKEY_RIGHT) || GetJoystickRepeat(JOYSTICK_L_RIGHT))
#define INPUT_TRIGGER_UI_CONTINUE		(GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_START) || GetJoypadTrigger(JOYKEY_A))
#define INPUT_TRIGGER_UI_ACCEPT		(GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(JOYKEY_A))
#define INPUT_TRIGGER_GAME_UP		(GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UP) || GetJoypadTrigger(JOYKEY_UP) || GetJoystickTrigger(JOYSTICK_L_UP))
#define INPUT_TRIGGER_GAME_DOWN		(GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWN) || GetJoypadTrigger(JOYKEY_DOWN) || GetJoystickTrigger(JOYSTICK_L_DOWN))
#define INPUT_TRIGGER_GAME_LEFT		(GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_LEFT) || GetJoypadTrigger(JOYKEY_LEFT) || GetJoystickTrigger(JOYSTICK_L_LEFT))
#define INPUT_TRIGGER_GAME_RIGHT		(GetKeyboardTrigger(DIK_D) || GetKeyboardTrigger(DIK_RIGHT) || GetJoypadTrigger(JOYKEY_RIGHT) || GetJoystickTrigger(JOYSTICK_L_RIGHT))
#define INPUT_TRIGGER_GAME_PAUSE		(GetKeyboardTrigger(DIK_P) || GetJoypadTrigger(JOYKEY_START))
#define INPUT_PRESS_GAME_UP		(GetKeyboardPress(DIK_W) || GetKeyboardPress(DIK_UP) || GetJoypadPress(JOYKEY_UP) || GetJoystickPress(JOYSTICK_L_UP))
#define INPUT_PRESS_GAME_DOWN		(GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_DOWN) || GetJoypadPress(JOYKEY_DOWN) || GetJoystickPress(JOYSTICK_L_DOWN))
#define INPUT_PRESS_GAME_LEFT		(GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_LEFT) || GetJoypadPress(JOYKEY_LEFT) || GetJoystickPress(JOYSTICK_L_LEFT))
#define INPUT_PRESS_GAME_RIGHT		(GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_RIGHT) || GetJoypadPress(JOYKEY_RIGHT) || GetJoystickPress(JOYSTICK_L_RIGHT))


//*********************************************************************
// 
// ***** 列挙型 *****
// 
//*********************************************************************
//*********************************************************************
// マウスボタン
//*********************************************************************
typedef enum
{
	MOUSE_LEFT = 0,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_MAX
}MOUSE;

//*********************************************************************
// ジョイパッドボタン
//*********************************************************************
typedef enum
{
	JOYKEY_UP = 0,		// 十字キー上
	JOYKEY_DOWN,		// 十字キー下
	JOYKEY_LEFT,		// 十字キー左
	JOYKEY_RIGHT,		// 十字キー右
	JOYKEY_START,		// スタート
	JOYKEY_BACK,		// バック（セレクト）
	JOYKEY_LTHUMB,		// 左スティック押し込み
	JOYKEY_RTHUMB,		// 右スティック押し込み
	JOYKEY_LSHOULDER,	// LB
	JOYKEY_RSHOULDER,	// RB
	JOYKEY_A = 12,		// A
	JOYKEY_B,			// B
	JOYKEY_X,			// X
	JOYKEY_Y,			// Y
	JOYKEY_MAX
}JOYKEY;

//*********************************************************************
// ジョイスティック
//*********************************************************************
typedef enum
{
	JOYSTICK_L_UP = 0,	// 左ジョイスティック上
	JOYSTICK_L_DOWN,	// 左ジョイスティック下
	JOYSTICK_L_LEFT,	// 左ジョイスティック左
	JOYSTICK_L_RIGHT,	// 左ジョイスティック右
	JOYSTICK_R_UP,		// 右ジョイスティック上
	JOYSTICK_R_DOWN,	// 右ジョイスティック下
	JOYSTICK_R_LEFT,	// 右ジョイスティック左
	JOYSTICK_R_RIGHT,	// 右ジョイスティック右
	JOYSTICK_MAX
}JOYSTICK;

//*********************************************************************
// 
// ***** プロトタイプ宣言 *****
// 
//*********************************************************************
//*********************************************************************
// キーボード
//*********************************************************************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey, int nInterval = INPUT_REPEAT_INTERVAL);
BYTE* GetKeyState(void);
//*********************************************************************
// マウス
//*********************************************************************
HRESULT InitMouse(HINSTANCE hInstance, HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool GetMousePress(MOUSE key);
bool GetMouseTrigger(MOUSE key);
bool GetMouseRelease(MOUSE key);
DIMOUSESTATE GetMouse(void);
D3DXVECTOR2 GetMousePos(void);
//*********************************************************************
// ジョイパッド
//*********************************************************************
HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
XINPUT_STATE* GetJoypad(void);
bool GetJoypadPress(JOYKEY key);
bool GetJoypadTrigger(JOYKEY key);
bool GetJoypadRelease(JOYKEY key);
bool GetJoypadRepeat(JOYKEY key, int nInterval = INPUT_REPEAT_INTERVAL);
bool GetJoystickPress(JOYSTICK stick);
bool GetJoystickTrigger(JOYSTICK stick);
bool GetJoystickRepeat(JOYSTICK stick, int nInterval = INPUT_REPEAT_INTERVAL);
void SetVibration(WORD wLeftMotorSpeed, WORD wRightMotorSpeed, int nCountVibration = -1);

#endif