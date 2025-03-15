#include <windows.h>
#include "Defines.h"
#include "Main.h"
#include <stdio.h>
#include <crtdbg.h>
#include "imgui_impl_win32.h"

// timeGetTime周りの使用
#pragma comment(lib, "winmm.lib")

//--- プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// エントリポイント
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//--- 変数宣言
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG message;

	// ウィンドクラス情報の設定
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.hInstance = hInstance;
	wcex.lpszClassName = "Class Name";
	wcex.lpfnWndProc = WndProc;
	wcex.style = CS_CLASSDC | CS_DBLCLKS;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

	// ウィンドウクラス情報の登録
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, "Failed to RegisterClassEx", "Error", MB_OK);
		return 0;
	}

	// ウィンドウの作成
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	DWORD style = WS_CAPTION | WS_SYSMENU;
	DWORD exStyle = WS_EX_OVERLAPPEDWINDOW;
	AdjustWindowRectEx(&rect, style, false, exStyle);
	hWnd = CreateWindowEx(
		exStyle, wcex.lpszClassName,
		NULL, style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		HWND_DESKTOP,
		NULL, hInstance, NULL
	);

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// 初期化処理
	if (FAILED(Init(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT)))
	{
		Uninit();
		UnregisterClass(wcex.lpszClassName, hInstance);
		return 0;
	}

	//--- FPS制御
	timeBeginPeriod(1);
	DWORD countStartTime = timeGetTime();
	DWORD preExecTime = countStartTime;
	DWORD time = timeGetTime();	//現在の処理時間
	DWORD oldTime = time;		//以前に実行した時間
	DWORD fpsCount = 0;			//FPS値計測カウンタ
	DWORD FPS = 0;				//直近のFPS
	DWORD fpsTime = time;		//fpsの計測し始め

	//--- ウィンドウの管理
	while (1)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&message, NULL, 0, 0))
			{
				break;
			}
			else
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
		else
		{
			time = timeGetTime();//ミリ秒 / FPS
			if (time - oldTime >= 1000 / 60)
			{
				Update();
				Draw();
				oldTime = time;
#ifdef _DEBUG	//デバッグ時のみ実行
				//処理回数をカウント
				fpsCount++;
				//前回の実行から一秒以上経過したら
				if (time - fpsTime >= 1000)
				{
					//整数型から文字列へ変換
					char mes[256];
					//sprintf→文字列に対してprintfで書き込む
					sprintf(mes, "FPS:%d", fpsCount);
					//FPSの表示
					SetWindowText(hWnd, mes);

					//次の計測の準備
					fpsCount = 0;
					fpsTime = time;
				}
#endif
			}
		}
	}


	// 終了時
	timeEndPeriod(1);
	Uninit();
	UnregisterClass(wcex.lpszClassName, hInstance);

	return 0;
}

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;  // ImGui が入力を処理した場合は OS 側の処理をスキップ

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}