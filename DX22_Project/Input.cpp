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
		return IsKeyPress('D') || CGetButtons(XINPUT_GAMEPAD_DPAD_LEFT);
		break;
	case InputPlayer1::Left:
		return IsKeyPress('A') || CGetButtons(XINPUT_GAMEPAD_DPAD_RIGHT);
		break;
	case InputPlayer1::Up:
		return IsKeyPress('W') || CGetButtons(XINPUT_GAMEPAD_DPAD_UP);
		break;
	case InputPlayer1::Down:
		return IsKeyPress('S') || CGetButtons(XINPUT_GAMEPAD_DPAD_DOWN);
		break;
	case InputPlayer1::A:
		return IsKeyPress(VK_SPACE) || CGetButtons(XINPUT_GAMEPAD_A);
		break;
	case InputPlayer1::B:
		return IsKeyPress(VK_ESCAPE) || CGetButtons(XINPUT_GAMEPAD_B);
		break;
	case InputPlayer1::X:
		return IsKeyPress('Q') || CGetButtons(XINPUT_GAMEPAD_X);
		break;
	case InputPlayer1::Y:
		return IsKeyPress('E') || CGetButtons(XINPUT_GAMEPAD_Y);
		break;
	case InputPlayer1::R1:
		return IsKeyPress('T') || CGetButtons(XINPUT_GAMEPAD_RIGHT_SHOULDER);
		break;
	case InputPlayer1::R2:
		return IsKeyPress('G');
		break;
	case InputPlayer1::L1:
		return IsKeyPress('F') || CGetButtons(XINPUT_GAMEPAD_LEFT_SHOULDER);
		break;
	case InputPlayer1::L2:
		return IsKeyPress('R');
		break;
	default:
		break;
	}
}
bool IsKeyPress(InputPlayer2 key)
{
	CSetIndex(PAD_USER2_NUMBER);

	switch (key)
	{
	case InputPlayer2::Right:
		return IsKeyPress('D') || CGetButtons(XINPUT_GAMEPAD_DPAD_LEFT);
		break;
	case InputPlayer2::Left:
		return IsKeyPress('A') || CGetButtons(XINPUT_GAMEPAD_DPAD_RIGHT);
		break;
	case InputPlayer2::Up:
		return IsKeyPress('W') || CGetButtons(XINPUT_GAMEPAD_DPAD_UP);
		break;
	case InputPlayer2::Down:
		return IsKeyPress('S') || CGetButtons(XINPUT_GAMEPAD_DPAD_DOWN);
		break;
	case InputPlayer2::A:
		return IsKeyPress(VK_SPACE) || CGetButtons(XINPUT_GAMEPAD_A);
		break;
	case InputPlayer2::B:
		return IsKeyPress(VK_ESCAPE) || CGetButtons(XINPUT_GAMEPAD_B);
		break;
	case InputPlayer2::X:
		return IsKeyPress('Q') || CGetButtons(XINPUT_GAMEPAD_X);
		break;
	case InputPlayer2::Y:
		return IsKeyPress('E') || CGetButtons(XINPUT_GAMEPAD_Y);
		break;
	case InputPlayer2::R1:
		return IsKeyPress('T') || CGetButtons(XINPUT_GAMEPAD_RIGHT_SHOULDER);
		break;
	case InputPlayer2::R2:
		return IsKeyPress('G');
		break;
	case InputPlayer2::L1:
		return IsKeyPress('F') || CGetButtons(XINPUT_GAMEPAD_LEFT_SHOULDER);
		break;
	case InputPlayer2::L2:
		return IsKeyPress('R');
		break;
	default:
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
		return IsKeyPress('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT);
		break;
	case InputPlayer1::Left:
		return IsKeyPress('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT);
		break;
	case InputPlayer1::Up:
		return IsKeyPress('W') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_UP);
		break;
	case InputPlayer1::Down:
		return IsKeyPress('S') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_DOWN);
		break;
	case InputPlayer1::A:
		return IsKeyPress(VK_SPACE) || CGetButtonsTriger(XINPUT_GAMEPAD_A);
		break;
	case InputPlayer1::B:
		return IsKeyPress(VK_ESCAPE) || CGetButtonsTriger(XINPUT_GAMEPAD_B);
		break;
	case InputPlayer1::X:
		return IsKeyPress('Q') || CGetButtonsTriger(XINPUT_GAMEPAD_X);
		break;
	case InputPlayer1::Y:
		return IsKeyPress('E') || CGetButtonsTriger(XINPUT_GAMEPAD_Y);
		break;
	case InputPlayer1::R1:
		return IsKeyPress('T') || CGetButtonsTriger(XINPUT_GAMEPAD_RIGHT_SHOULDER);
		break;
	case InputPlayer1::R2:
		return IsKeyPress('G');
		break;
	case InputPlayer1::L1:
		return IsKeyPress('F') || CGetButtonsTriger(XINPUT_GAMEPAD_LEFT_SHOULDER);
		break;
	case InputPlayer1::L2:
		return IsKeyPress('R');
		break;
	default:
		break;
	}
}
bool IsKeyTrigger(InputPlayer2 key)
{
	CSetIndex(PAD_USER2_NUMBER);

	switch (key)
	{
	case InputPlayer2::Right:
		return IsKeyPress('D') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_LEFT);
		break;
	case InputPlayer2::Left:
		return IsKeyPress('A') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_RIGHT);
		break;
	case InputPlayer2::Up:
		return IsKeyPress('W') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_UP);
		break;
	case InputPlayer2::Down:
		return IsKeyPress('S') || CGetButtonsTriger(XINPUT_GAMEPAD_DPAD_DOWN);
		break;
	case InputPlayer2::A:
		return IsKeyPress(VK_SPACE) || CGetButtonsTriger(XINPUT_GAMEPAD_A);
		break;
	case InputPlayer2::B:
		return IsKeyPress(VK_ESCAPE) || CGetButtonsTriger(XINPUT_GAMEPAD_B);
		break;
	case InputPlayer2::X:
		return IsKeyPress('Q') || CGetButtonsTriger(XINPUT_GAMEPAD_X);
		break;
	case InputPlayer2::Y:
		return IsKeyPress('E') || CGetButtonsTriger(XINPUT_GAMEPAD_Y);
		break;
	case InputPlayer2::R1:
		return IsKeyPress('T') || CGetButtonsTriger(XINPUT_GAMEPAD_RIGHT_SHOULDER);
		break;
	case InputPlayer2::R2:
		return IsKeyPress('G');
		break;
	case InputPlayer2::L1:
		return IsKeyPress('F') || CGetButtonsTriger(XINPUT_GAMEPAD_LEFT_SHOULDER);
		break;
	case InputPlayer2::L2:
		return IsKeyPress('R');
		break;
	default:
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