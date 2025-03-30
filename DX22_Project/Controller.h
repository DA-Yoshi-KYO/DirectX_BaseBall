#pragma once
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")
#include <dinput.h>
#include <DirectXMath.h>

/* define */
#define PAD_USER1_NUMBER (0)
#define PAD_USER2_NUMBER (1)
#define PAD_MAX (2)

#define DEADZONE (13000.0f)
void Controller_Update();
DirectX::XMFLOAT2 CGetLStick();
DirectX::XMFLOAT2 CGetRStick();
DirectX::XMFLOAT2 CGetTrigger();
bool CGetButtons(WORD InButton);
void CSetLeftVibration(int Frame, int Strength);
void CSetRigthVibration(int Frame, int Strength);
void CSetStopVibration();
bool CGetButtonsTriger(WORD InButton);
void CSetIndex(int index);