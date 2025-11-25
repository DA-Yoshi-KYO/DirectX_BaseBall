#ifndef __INPUT_H__
#define __INPUT_H__

#include <Windows.h>
#undef max
#undef min

#define MOUSEBUTTON_L	0
#define MOUSEBUTTON_R	1
#define MOUSEBUTTON_M	2
#define MOUSEBUTTON_X1	3
#define MOUSEBUTTON_X2	4


HRESULT InitInput();
void UninitInput();
void UpdateInput();
POINT* GetMousePosition(bool CenterIsZero = true);
void SetMousePosition(POINT inPos, bool CenterIsZero = true);
bool IsMouseButtonPress(DWORD dwBtnID);
bool IsMouseButtonTrigger(DWORD dwBtnID);
bool IsMouseButtonRelease(DWORD dwBtnID);

enum class Input
{
    Right,
    Left,
    Up,
    Down,
    A,
    B,
    X,
    Y,
    R1,
    R2,
    L1,
    L2
};

bool IsKeyPress(BYTE key);
bool IsKeyPress(int PlayerNo, Input key);
bool IsKeyTrigger(BYTE key);
bool IsKeyTrigger(int PlayerNo, Input key);
bool IsKeyRelease(BYTE key);
bool IsKeyRepeat(BYTE key);

#endif // __INPUT_H__