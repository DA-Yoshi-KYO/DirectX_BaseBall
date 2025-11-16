#include "Input.h"
#include "Controller.h"

//--- グローバル変数
BYTE g_keyTable[256];
BYTE g_oldTable[256];

HRESULT InitInput()
{
	// 一番最初の入力
	GetKeyboardState(g_keyTable);
	return S_OK;
}

void UninitInput()
{

}

void UpdateInput()
{
	// 古い入力を更新
	memcpy_s(g_oldTable, sizeof(g_oldTable), g_keyTable, sizeof(g_keyTable));
	// 現在の入力を取得
	GetKeyboardState(g_keyTable);
}

bool IsKeyPress(BYTE key)
{
	return g_keyTable[key] & 0x80;
}


bool IsKeyTrigger(BYTE key)
{
	return (g_keyTable[key] ^ g_oldTable[key]) & g_keyTable[key] & 0x80;
}

bool IsKeyTrigger(int PlayerNo, Input key)
{
	bool bActive = CGetActive(PlayerNo);
	bool out = false;
	if (bActive)
	{
		switch (PlayerNo)
		{
		case 1:
			CSetIndex(PAD_USER1_NUMBER);
			break;
		case 2:
			CSetIndex(PAD_USER2_NUMBER);
			break;
		default:
			return out;
			break;
		}
	}

	switch (key)
	{
	case Right:
		if (bActive) out = CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT);
		else out = PlayerNo == 1 ? IsKeyTrigger('D') : IsKeyTrigger('L');
		break;
	case Left:
		if (bActive) out = CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT);
		else out = PlayerNo == 1 ? IsKeyTrigger('A') : IsKeyTrigger('J');
		break;
	case Up:
		if (bActive) out = CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_UP);
		else out = PlayerNo == 1 ? IsKeyTrigger('W') : IsKeyTrigger('I');
		break;
	case Down:
		if (bActive) out = CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_DOWN);
		else out = PlayerNo == 1 ? IsKeyTrigger('S') : IsKeyTrigger('K');
		break;
	case A:
		if (bActive) out = CGetButtonsTriger(XINPUT_GAMEPAD_A);
		else out = PlayerNo == 1 ? IsKeyTrigger(VK_SPACE) : IsKeyTrigger(VK_RETURN);
		break;
	case B:
		if (bActive) out = CGetButtonsTriger(XINPUT_GAMEPAD_B);
		else out = PlayerNo == 1 ? IsKeyTrigger(VK_ESCAPE) : IsKeyTrigger(VK_BACK);
		break;
	case X:
		if (bActive) out = CGetButtonsTriger(XINPUT_GAMEPAD_X);
		else out = PlayerNo == 1 ? IsKeyTrigger('Q') : IsKeyTrigger('U');
		break;
	case Y:
		if (bActive) out = CGetButtonsTriger(XINPUT_GAMEPAD_Y);
		else out = PlayerNo == 1 ? IsKeyTrigger('E') : IsKeyTrigger('O');
		break;
	case R1:
		if (bActive) out = CGetButtonsTriger(XINPUT_GAMEPAD_RIGHT_SHOULDER);
		else out = PlayerNo == 1 ? IsKeyTrigger('1') : IsKeyTrigger('7');
		break;
	case R2:
		if (bActive) out = CGetButtonsTriger(XINPUT_GAMEPAD_RIGHT_SHOULDER);
		else out = PlayerNo == 1 ? IsKeyTrigger('2') : IsKeyTrigger('8');
		break;
	case L1:
		if (bActive) out = CGetButtonsTriger(XINPUT_GAMEPAD_LEFT_SHOULDER);
		else out = PlayerNo == 1 ? IsKeyTrigger('3') : IsKeyTrigger('9');
		break;
	case L2:
		if (bActive) out = CGetButtonsTriger(XINPUT_GAMEPAD_LEFT_SHOULDER);
		else out = PlayerNo == 1 ? IsKeyTrigger('4') : IsKeyTrigger('0');
		break;
	default:
		break;
	}

	return out;
}

bool IsKeyPress(int PlayerNo, Input key)
{
	bool bActive = CGetActive(PlayerNo);
	bool out = false;
	if (bActive)
	{
		switch (PlayerNo)
		{
		case 1:
			CSetIndex(PAD_USER1_NUMBER);
			break;
		case 2:
			CSetIndex(PAD_USER2_NUMBER);
			break;
		default:
			return out;
			break;
		}
	}
	
	switch (key)
	{
	case Right:
		if (bActive) out = CGetButtons(XINPUT_GAMEPAD_DPAD_RIGHT);
		else out = PlayerNo == 1 ? IsKeyPress('D') : IsKeyPress('L');
		break;
	case Left:
		if (bActive) out = CGetButtons(XINPUT_GAMEPAD_DPAD_LEFT);
		else out = PlayerNo == 1 ? IsKeyPress('A') : IsKeyPress('J');
		break;
	case Up:
		if (bActive) out = CGetButtons(XINPUT_GAMEPAD_DPAD_UP);
		else out = PlayerNo == 1 ? IsKeyPress('W') : IsKeyPress('I');
		break;
	case Down:
		if (bActive) out = CGetButtons(XINPUT_GAMEPAD_DPAD_DOWN);
		else out = PlayerNo == 1 ? IsKeyPress('S') : IsKeyPress('K');
		break;
	case A:
		if (bActive) out = CGetButtons(XINPUT_GAMEPAD_A);
		else out = PlayerNo == 1 ? IsKeyPress(VK_SPACE) : IsKeyPress(VK_RETURN);
		break;
	case B:
		if (bActive) out = CGetButtons(XINPUT_GAMEPAD_B);
		else out = PlayerNo == 1 ? IsKeyPress(VK_ESCAPE) : IsKeyPress(VK_BACK);
		break;
	case X:
		if (bActive) out = CGetButtons(XINPUT_GAMEPAD_X);
		else out = PlayerNo == 1 ? IsKeyPress('Q') : IsKeyPress('U');
		break;
	case Y:
		if (bActive) out = CGetButtons(XINPUT_GAMEPAD_Y);
		else out = PlayerNo == 1 ? IsKeyPress('E') : IsKeyPress('O');
		break;
	case R1:
		if (bActive) out = CGetButtons(XINPUT_GAMEPAD_RIGHT_SHOULDER);
		else out = PlayerNo == 1 ? IsKeyPress('1') : IsKeyPress('7');
		break;
	case R2:
		if (bActive) out = CGetButtons(XINPUT_GAMEPAD_RIGHT_SHOULDER);
		else out = PlayerNo == 1 ? IsKeyPress('2') : IsKeyPress('8');
		break;
	case L1:
		if (bActive) out = CGetButtons(XINPUT_GAMEPAD_LEFT_SHOULDER);
		else out = PlayerNo == 1 ? IsKeyPress('3') : IsKeyPress('9');
		break;
	case L2:
		if (bActive) out = CGetButtons(XINPUT_GAMEPAD_LEFT_SHOULDER);
		else out = PlayerNo == 1 ? IsKeyPress('4') : IsKeyPress('0');
		break;
	default:
		break;
	}

	return out;
}

bool IsKeyRelease(BYTE key)
{
	return (g_keyTable[key] ^ g_oldTable[key]) & g_oldTable[key] & 0x80;
}
bool IsKeyRepeat(BYTE key)
{
	return false;
}