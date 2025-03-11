#include "Main.h"
#include <memory>
#include "DirectX.h"
#include "Defines.h"
#include "Geometory.h"
#include "Sprite.h"
#include "Input.h"
#include "ShaderList.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "FadeBlack.h"
#include <dinput.h>

//--- グローバル変数
CScene* g_pScene; // シーン 
CFade* g_pFade; // フェード 

IDirectInput8* g_pDirectInput = nullptr;
IDirectInputDevice8* g_pKeyboardDevice = nullptr;

HRESULT Init(HWND hWnd, UINT width, UINT height)
{
	HRESULT hr;
	// DirectX初期化
	hr = InitDirectX(hWnd, width, height, false);
	// 初期化の例
	hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDirectInput, nullptr);
	hr = g_pDirectInput->CreateDevice(GUID_SysKeyboard, &g_pKeyboardDevice, nullptr);
	hr = g_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	hr = g_pKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	g_pKeyboardDevice->Acquire();
	if (FAILED(hr)) { return hr; }

	// 他機能初期化
	Geometory::Init();
	Sprite::Init();
	InitInput();
	ShaderList::Init();

	// フェード作成 
	g_pFade = new CFadeBlack();
	g_pFade->SetFade(1.0f, true);

	// シーン作成 
	g_pScene = new CSceneTitle();
	g_pScene->SetFade(g_pFade); // シーンに使用するフェードを設定 

	return hr;
}

void Uninit()
{
	delete g_pScene;
	delete g_pFade;
	ShaderList::Uninit();
	UninitInput();
	Sprite::Uninit();
	Geometory::Uninit();
	UninitDirectX();
}

void Update()
{
	UpdateInput();
	g_pScene->RootUpdate();
	srand(timeGetTime());

	// シーン切り替え判定 
	if (g_pScene->ChangeScene()) {
		// 次のシーンの情報を取得 
		int scene = g_pScene->NextScene();

		// 現在のシーンを削除 
		delete g_pScene;

		// シーンの切り替え 
		switch (scene) {
		case 0: g_pScene = new CSceneTitle(); break; // TITLE 
		case 1: g_pScene = new CSceneGame(); break; // GAME 
		}

		// 次シーンに向けて初期設定 
		g_pFade->Start(true);   // フェード開始 
		g_pScene->SetFade(g_pFade); // フェードクラスをシーンに設定 
	}
}

void Draw()
{
	BeginDrawDirectX();

	// 軸線の表示
#ifdef _DEBUG
	// グリッド
	DirectX::XMFLOAT4 lineColor(0.5f, 0.5f, 0.5f, 1.0f);
	float size = DEBUG_GRID_NUM * DEBUG_GRID_MARGIN;
	for (int i = 1; i <= DEBUG_GRID_NUM; ++i)
	{
		float grid = i * DEBUG_GRID_MARGIN;
		DirectX::XMFLOAT3 pos[2] = {
			DirectX::XMFLOAT3(grid, 0.0f, size),
			DirectX::XMFLOAT3(grid, 0.0f,-size),
		};
		Geometory::AddLine(pos[0], pos[1], lineColor);
		pos[0].x = pos[1].x = -grid;
		Geometory::AddLine(pos[0], pos[1], lineColor);
		pos[0].x = size;
		pos[1].x = -size;
		pos[0].z = pos[1].z = grid;
		Geometory::AddLine(pos[0], pos[1], lineColor);
		pos[0].z = pos[1].z = -grid;
		Geometory::AddLine(pos[0], pos[1], lineColor);
	}
	// 軸
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(size,0,0), DirectX::XMFLOAT4(1,0,0,1));
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,size,0), DirectX::XMFLOAT4(0,1,0,1));
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,0,size), DirectX::XMFLOAT4(0,0,1,1));
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(-size,0,0),  DirectX::XMFLOAT4(0,0,0,1));
	Geometory::AddLine(DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,0,-size),  DirectX::XMFLOAT4(0,0,0,1));

	Geometory::DrawLines();

	// カメラの値
	static bool camPosSwitch = false;
	if (IsKeyTrigger(VK_RETURN)) {
		camPosSwitch ^= true;
	}

	DirectX::XMVECTOR camPos;
	if (camPosSwitch) {
		camPos = DirectX::XMVectorSet(2.5f, 30.5f, -40.0f, 0.0f);
	}
	else {
		camPos = DirectX::XMVectorSet(2.5f, 3.5f, -4.0f, 0.0f);
	}

	// ジオメトリ用カメラ初期化
	DirectX::XMFLOAT4X4 mat[2];
	DirectX::XMStoreFloat4x4(&mat[0], DirectX::XMMatrixTranspose(
		DirectX::XMMatrixLookAtLH(
			camPos,
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		)));
	DirectX::XMStoreFloat4x4(&mat[1], DirectX::XMMatrixTranspose(
		DirectX::XMMatrixPerspectiveFovLH(
			DirectX::XMConvertToRadians(60.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f)
	));
	Geometory::SetView(mat[0]);
	Geometory::SetProjection(mat[1]);
#endif

	g_pScene->RootDraw();
	EndDrawDirectX();
}

std::string GetStringForKey()
{
	char value;
	std::string ReturnString;
	ReturnString.clear();
	// 数字チェック
	for (int i = 48; i < 58; i++)
	{
		value = i;
		if (IsKeyTrigger(value))
		{
			if (IsKeyPress(VK_SHIFT))
			{
				value = i - 16;
				ReturnString += value;
				if (i == 48) return "Error";
				return ReturnString;
			}
			else
			{
				ReturnString += value;
				return ReturnString;
			}
		}
	}

	// 文字チェック
	for (int i = 65; i < 90; i++)
	{
		value = i;
		if (IsKeyTrigger(value))
		{
			if (IsKeyPress(VK_SHIFT))
			{
				value = i;
				ReturnString += value;
				return ReturnString;
			}
			else
			{
				value = i + 32;
				ReturnString += value;
				return ReturnString;
			}
		}
	}

	if (IsKeyTrigger(VK_BACK)) return "Back";

	return "Error";
}

void SetRender2D()
{
	RenderTarget* pRTV = GetDefaultRTV();	// デフォルトのRenderTargetViewを取得
	SetRenderTargets(1, &pRTV, nullptr);		// 第三引数がNULLの場合、2D表示となる
}

void SetRender3D()
{
	RenderTarget* pRTV = GetDefaultRTV();	// デフォルトのRenderTargetViewを取得 
	DepthStencil* pDSV = GetDefaultDSV();	// デフォルトのDepthStencilViewを取得
	SetRenderTargets(1, &pRTV, pDSV);		// 第三引数がNULLの場合、2D表示となる
}

// EOF