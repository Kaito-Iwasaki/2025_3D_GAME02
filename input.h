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
bool GetJoystickRepeat(JOYSTICK stick, int nInterval = INPUT_REPEAT_INTERVAL);
void SetVibration(WORD wLeftMotorSpeed, WORD wRightMotorSpeed, int nCountVibration = -1);

#endif