/*
	XInputを使用したControllerの入出力
	2024/10/13 制作開始 動作未確認　　秋野
	2024/10/15 バイブレーションの追加　動作未確認　秋野
	2024/12/07 トリガー機能の追加
*/

/* Include */
#include"Controller.h"
#include <string>


/* Global変数 */
int g_nControllerIndex = 0;
int g_LeftVibrationFream[PAD_MAX];
int g_RightVibrationFream[PAD_MAX];
XINPUT_STATE OldButtons[PAD_MAX];

/* 構造体 */
XINPUT_STATE def[PAD_MAX];
XINPUT_STATE state[PAD_MAX];
XINPUT_VIBRATION vibration[PAD_MAX];

/* ゲームパッドの更新処理 */
void Controller_Update()
{
	for (int i = 0; i < PAD_MAX; i++)
	{
		OldButtons[i] = state[i];
		if (XInputGetState(i, &state[i]) != ERROR_SUCCESS)
		{
			//std::string strErrMsg = "No";
			//strErrMsg += std::to_string(i + 1);
			//strErrMsg += "Controller DisConnect";
			//MessageBox(NULL, strErrMsg.c_str(),"ControllerError", MB_OK);
		}
		XInputSetState(i, &vibration[i]);

		/* 一定Frame経過時のバイブの無効化 */
		if (g_LeftVibrationFream[i] > 0)
		{
			g_LeftVibrationFream[i]--;
		}
		else
		{
			vibration[i].wLeftMotorSpeed = 0;
		}

		if (g_RightVibrationFream[i] > 0)
		{
			g_RightVibrationFream[i]--;
		}
		else
		{
			vibration[i].wRightMotorSpeed = 0;
		}

	}
}

/* 左アナログスティックの座標獲得 */
/* -32768 ～ 32767 */
DirectX::XMFLOAT2 CGetLStick(int index)
{
	DirectX::XMFLOAT2 Pos;
	g_nControllerIndex = index;

	Pos.x = state[g_nControllerIndex].Gamepad.sThumbLX;
	Pos.y = state[g_nControllerIndex].Gamepad.sThumbLY;

	if (fabsf(Pos.x) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) Pos.x = 0.0f;
	if (fabsf(Pos.y) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) Pos.y = 0.0f;
	
	if (Pos.x < 0.0f) Pos.x += XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	else if(Pos.x > 0.0f) Pos.x -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	if (Pos.y < 0.0f) Pos.y += XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	else if(Pos.y > 0.0f) Pos.y -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

	const float maxPow = 32767.0f - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	Pos.x = Pos.x / maxPow;
	Pos.y = Pos.y / maxPow;

	if (Pos.x >= 1.0f) Pos.x = 1.0f;
	if (Pos.x <= -1.0f) Pos.x = -1.0f;
	if (Pos.y >= 1.0f) Pos.y = 1.0f;
	if (Pos.y <= -1.0f) Pos.y = -1.0f;

	//Pos.y *= -1;

	return Pos;
}

/* 右アナログスティックの座標獲得 */
/* -32768 ～ 32767 */
DirectX::XMFLOAT2 CGetRStick()
{
	DirectX::XMFLOAT2 Pos;

	Pos.x = state[g_nControllerIndex].Gamepad.sThumbRX;
	Pos.y = state[g_nControllerIndex].Gamepad.sThumbRY;

	return Pos;
}

/* トリガーの値を返します */
/* x 左トリガー */
/* y 右トリガー */
/* 0 ～　255 */
DirectX::XMFLOAT2 CGetTrigger()
{
	DirectX::XMFLOAT2 Pos;

	Pos.x = state[g_nControllerIndex].Gamepad.bLeftTrigger;
	Pos.y = state[g_nControllerIndex].Gamepad.bRightTrigger;

	return Pos;
}

/* Buttonの入出力 */
/*
#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            0x0010
#define XINPUT_GAMEPAD_BACK             0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000
*/
bool CGetButtons(WORD InButton)
{
	return (state[g_nControllerIndex].Gamepad.wButtons & InButton);
}

bool CGetButtonsTriger(WORD InButton)
{
	if (state[g_nControllerIndex].Gamepad.wButtons & InButton)
	{
		if (OldButtons[g_nControllerIndex].Gamepad.wButtons & InButton)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return false;
	}

}

void CSetIndex(int index)
{
	if (index >= PAD_MAX)
	{
		MessageBox(NULL, "ControllerIndexOutofRange", "Error", MB_OK);
	}
	g_nControllerIndex = index;
}

bool CGetActive(int index)
{
	return XInputGetState(index, &state[index]) == ERROR_SUCCESS;
}


/* バイブレーションの設定を行います */
/* Freame 実行するFrame数 Strength バイブの強度*/
/* 強度 0　～　65535 */
void CSetLeftVibration(int Frame, int Strength)
{
	g_LeftVibrationFream[g_nControllerIndex] = Frame;
	vibration[g_nControllerIndex].wLeftMotorSpeed = Strength;
}

/* バイブレーションの設定を行います */
/* Freame 実行するFrame数 Strength バイブの強度*/
/* 強度 0　～　65535 */
void CSetRigthVibration(int Frame, int Strength)
{
	g_RightVibrationFream[g_nControllerIndex] = Frame;
	vibration[g_nControllerIndex].wRightMotorSpeed = Strength;
}

/* バイブレーションを止めます */
void CSetStopVibration()
{
	g_LeftVibrationFream[g_nControllerIndex] = g_RightVibrationFream[g_nControllerIndex] = 0;
	vibration[g_nControllerIndex].wLeftMotorSpeed = vibration[g_nControllerIndex].wRightMotorSpeed = 0;
}