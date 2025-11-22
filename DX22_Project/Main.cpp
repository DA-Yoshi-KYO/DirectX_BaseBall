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
#include "Camera.h"
#include "PostProcessChain.h"

// Scenes
#include "SceneTitle.h"
#include "Transition.h"

CScene* g_pScene;
CScene* g_pNextScene;
CTransition* g_pTransition;
bool g_bSceneChanging = false;
bool g_bDebugMode = false;

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
    CCamera::GetInstance();
    CPostProcessChain::GetInstance();

    // Scenes
    g_pScene = new CSceneTitle();
    g_pScene->Init();
    g_pTransition = new CTransition();
    g_pTransition->Init();
    FadeIn(nullptr);

	return hr;
}

void Uninit()
{
    // Scenes
    g_pScene->Uninit();
    SAFE_DELETE(g_pScene);
    g_pTransition->Uninit();
    SAFE_DELETE(g_pTransition);

    // SingletonInstances
    CPostProcessChain::GetInstance()->Release();
    CCamera::GetInstance()->Release();
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
    if (CDebugSystem::GetInstance()->IsUpdate())
    {
        if (g_bSceneChanging)
        {
            CDebugSystem::GetInstance()->ReleaseGameObject();

            g_pScene->Uninit();
            delete g_pScene;
            g_pScene = g_pNextScene;
            g_pScene->Init();
            g_bSceneChanging = false;
        }

        g_pScene->Update();
        g_pTransition->Update();
    }

    if (IsKeyPress(VK_SHIFT))
    {
        if (IsKeyTrigger(VK_RETURN))
        {
            g_bDebugMode ^= true;
        }
    }

    if(g_bDebugMode) CDebugSystem::GetInstance()->Update();
}

void Draw()
{
	BeginDrawDirectX();

    g_pScene->Draw();
    g_pTransition->Draw();
    if (g_bDebugMode) CDebugSystem::GetInstance()->Draw();

	EndDrawDirectX();
}

CScene* GetScene()
{
    return g_pScene;
}

void ChangeScene(CScene* inScene)
{
    g_pNextScene = inScene;
    g_bSceneChanging = true;
}

void FadeIn(std::function<void()> onFadeComplete)
{
    g_pTransition->FadeIn(50, onFadeComplete);
}

void FadeOut(std::function<void()> onFadeComplete)
{
    g_pTransition->FadeOut(50, onFadeComplete);
}
