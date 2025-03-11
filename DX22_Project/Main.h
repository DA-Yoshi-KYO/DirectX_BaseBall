#ifndef __MAIN_H__
#define __MAIN_H__

#include <Windows.h>
#include <string>

HRESULT Init(HWND hWnd, UINT width, UINT height);
void Uninit();
void Update();
void Draw();

std::string GetStringForKey();
void SetRender2D();
void SetRender3D();
#endif // __MAIN_H__