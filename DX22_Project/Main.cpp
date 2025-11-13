// App
#include "Main.h"
#include "DirectX.h"

// Works
#include "Geometory.h"
#include "Sprite.h"
#include "Input.h"

// PreLoadAssets
#include "RendererComponent.h"
#include "ModelRenderer.h"
#include "SpriteRendererBase.h"

// SingletonInstances
#include "DebugSystem.h"

// Scenes
#include "SceneTitle.h"

CScene* g_pScene = nullptr;
bool g_bIsDebugMode = false;

HRESULT Init(HWND hWnd, UINT width, UINT height)
{
	HRESULT hr;

	// DirectX
	hr = InitDirectX(hWnd, width, height, false);
	if (FAILED(hr)) { return hr; }

    // Works
    hr = InitInput();
    if (FAILED(hr)) { return hr; }
	Geometory::Init();
	Sprite::Init();

    // SingletonInstances
    CDebugSystem::GetInstance();

    // Scenes
    g_pScene = new CSceneTitle();
    g_pScene->Init();

	return hr;
}

void Uninit()
{
    // Scenes
    g_pScene->Uninit();
    SAFE_DELETE(g_pScene);

    // SingletonInstances
    CDebugSystem::GetInstance()->Release();

    // PreLoadAssets
    CRendererComponent::UnloadShader();
    CModelRenderer::UnloadAll();
    CSpriteRendererBase::UnloadAll();

    // Works
	Sprite::Uninit();
	Geometory::Uninit();
    UninitInput();

    // DirectX
	UninitDirectX();
}

void Update()
{
    UpdateInput();
    g_pScene->Update();

    if (IsKeyPress(VK_SHIFT))
    {
        if (IsKeyTrigger(VK_RETURN))
        {
            g_bIsDebugMode ^= true;
        }
    }

    if(g_bIsDebugMode) CDebugSystem::GetInstance()->Update();
}

void Draw()
{
	BeginDrawDirectX();

    g_pScene->Draw();
    if (g_bIsDebugMode) CDebugSystem::GetInstance()->Draw();

	EndDrawDirectX();
}

CScene* GetScene()
{
    return g_pScene;
}
