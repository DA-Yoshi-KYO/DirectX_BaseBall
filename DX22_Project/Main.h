#ifndef __MAIN_H__
#define __MAIN_H__

#include <Windows.h>
#include <string>
#include "Scene.h"

HRESULT Init(HWND hWnd, UINT width, UINT height);
void Uninit();
void Update();
void Draw();

void AppEnd();

HWND GetMyWindow();
int GetFPS();
CScene* GetScene();
void ChangeScene(CScene* inScene);
void FadeIn(std::function<void()> onFadeComplete);
void FadeOut(std::function<void()> onFadeComplete);

#endif // __MAIN_H__
