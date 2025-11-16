#ifndef __INPUT_H__
#define __INPUT_H__

#include <Windows.h>
#undef max
#undef min

HRESULT InitInput();
void UninitInput();
void UpdateInput();

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