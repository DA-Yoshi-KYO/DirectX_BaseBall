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

bool IsKeyPress(InputPlayer1 key)
{
	CSetIndex(PAD_USER1_NUMBER);

	switch (key)
	{
	case InputPlayer1::Right:
		return CGetButtons(XINPUT_GAMEPAD_DPAD_LEFT);
		break;
	case InputPlayer1::Left:
		return CGetButtons(XINPUT_GAMEPAD_DPAD_RIGHT);
		break;
	case InputPlayer1::Up:
		return CGetButtons(XINPUT_GAMEPAD_DPAD_UP);
		break;
	case InputPlayer1::Down:
		return CGetButtons(XINPUT_GAMEPAD_DPAD_DOWN);
		break;
	case InputPlayer1::A:
		return CGetButtons(XINPUT_GAMEPAD_A);
		break;
	case InputPlayer1::B:
		return CGetButtons(XINPUT_GAMEPAD_B);
		break;
	case InputPlayer1::X:
		return CGetButtons(XINPUT_GAMEPAD_X);
		break;
	case InputPlayer1::Y:
		return CGetButtons(XINPUT_GAMEPAD_Y);
		break;
	case InputPlayer1::R1:
		return CGetButtons(XINPUT_GAMEPAD_RIGHT_SHOULDER);
		break;
	case InputPlayer1::R2:
		break;
	case InputPlayer1::L1:
		return CGetButtons(XINPUT_GAMEPAD_LEFT_SHOULDER);
		break;
	case InputPlayer1::L2:
		break;
	default:
		return false;
		break;
	}
}

bool IsKeyPress(InputPlayer2 key)
{
	CSetIndex(PAD_USER2_NUMBER);

	switch (key)
	{
	case InputPlayer2::Right:
		return IsKeyPress('D');
		break;
	case InputPlayer2::Left:
		return IsKeyPress('A');
		break;
	case InputPlayer2::Up:
		return IsKeyPress('W');
		break;
	case InputPlayer2::Down:
		return IsKeyPress('S');
		break;
	case InputPlayer2::A:
		return IsKeyPress(VK_SPACE);
		break;
	case InputPlayer2::B:
		return IsKeyPress(VK_ESCAPE);
		break;
	case InputPlayer2::X:
		return IsKeyPress('Q');
		break;
	case InputPlayer2::Y:
		return IsKeyPress('E');
		break;
	case InputPlayer2::R1:
		return IsKeyPress('T');
		break;
	case InputPlayer2::R2:
		return IsKeyPress('G');
		break;
	case InputPlayer2::L1:
		return IsKeyPress('F');
		break;
	case InputPlayer2::L2:
		return IsKeyPress('R');
		break;
	default:
		return false;
		break;
	}
}

bool IsKeyTrigger(BYTE key)
{
	return (g_keyTable[key] ^ g_oldTable[key]) & g_keyTable[key] & 0x80;
}

bool IsKeyTrigger(InputPlayer1 key)
{
	CSetIndex(PAD_USER1_NUMBER);

	switch (key)
	{
	case InputPlayer1::Right:
		return CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT);
		break;
	case InputPlayer1::Left:
		return CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT);
		break;
	case InputPlayer1::Up:
		return CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_UP);
		break;
	case InputPlayer1::Down:
		return CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_DOWN);
		break;
	case InputPlayer1::A:
		return CGetButtonsTriger(XINPUT_GAMEPAD_A);
		break;
	case InputPlayer1::B:
		return CGetButtonsTriger(XINPUT_GAMEPAD_B);
		break;
	case InputPlayer1::X:
		return CGetButtonsTriger(XINPUT_GAMEPAD_X);
		break;
	case InputPlayer1::Y:
		return CGetButtonsTriger(XINPUT_GAMEPAD_Y);
		break;
	case InputPlayer1::R1:
		return CGetButtonsTriger(XINPUT_GAMEPAD_RIGHT_SHOULDER);
		break;
	case InputPlayer1::R2:
		break;
	case InputPlayer1::L1:
		return CGetButtonsTriger(XINPUT_GAMEPAD_LEFT_SHOULDER);
		break;
	case InputPlayer1::L2:
		break;
	default:
		return false;
		break;
	}
}

bool IsKeyTrigger(InputPlayer2 key)
{
	CSetIndex(PAD_USER2_NUMBER);

	switch (key)
	{
	case InputPlayer2::Right:
		return IsKeyTrigger('D');
		break;
	case InputPlayer2::Left:
		return IsKeyTrigger('A');
		break;
	case InputPlayer2::Up:
		return IsKeyTrigger('W');
		break;
	case InputPlayer2::Down:
		return IsKeyTrigger('S');
		break;
	case InputPlayer2::A:
		return IsKeyTrigger(VK_SPACE);
		break;
	case InputPlayer2::B:
		return IsKeyTrigger(VK_ESCAPE);
		break;
	case InputPlayer2::X:
		return IsKeyTrigger('Q');
		break;
	case InputPlayer2::Y:
		return IsKeyTrigger('E');
		break;
	case InputPlayer2::R1:
		return IsKeyTrigger('T');
		break;
	case InputPlayer2::R2:
		return IsKeyTrigger('G');
		break;
	case InputPlayer2::L1:
		return IsKeyTrigger('F');
		break;
	case InputPlayer2::L2:
		return IsKeyTrigger('R');
		break;
	default:
		return false;
		break;
	}
}
bool IsKeyRelease(BYTE key)
{
	return (g_keyTable[key] ^ g_oldTable[key]) & g_oldTable[key] & 0x80;
}
bool IsKeyRepeat(BYTE key)
{
	return false;
}