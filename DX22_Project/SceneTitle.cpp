#include "SceneTitle.h"
#include "Input.h"
#include "Main.h"
#include "Sprite.h"
#include "Defines.h"

CSceneTitle::CSceneTitle()
	: m_pLogo(nullptr), m_pTran{}
	, m_BlackPos{}, m_BlackSize{}
	, m_StarPos{}, m_StarSize{}
	, m_Angle(0.0f)
{
	m_pLogo = new Texture();
	for (int i = 0; i < 2; i++)
	{
		m_pTran[i] = new Texture();
	}
	if (FAILED(m_pLogo->Create("Assets/Texture/Title.jpeg"))) MessageBox(NULL, "Load failed SceneTitle.", "Error", MB_OK);
	//if (FAILED(m_pTran[0]->Create("Assets/Texture/004.png"))) MessageBox(NULL, "Load failed SceneTitle.", "Error", MB_OK);
	//if (FAILED(m_pTran[1]->Create("Assets/Texture/TransitionStarA.png"))) MessageBox(NULL, "Load failed SceneTitle.", "Error", MB_OK);
	font = new DirectWriteSample();
	font->Init();

	m_StarPos = { 0.0f,0.0f };
	m_StarSize = { SCREEN_WIDTH, SCREEN_HEIGHT };

	for (int i = 0; i < 4; i++)
	{
		m_BlackSize[i] = { SCREEN_WIDTH * 2.0f, SCREEN_HEIGHT * 2.0f};
		switch (i)
		{
		case 0:
			m_BlackPos[i] = { 0.0f,-(SCREEN_HEIGHT + SCREEN_HEIGHT / 2.0f)};

			break;
		case 1:
			m_BlackPos[i] = {0.0f,(SCREEN_HEIGHT + SCREEN_HEIGHT / 2.0f) };
			break;
		case 2:
			m_BlackPos[i] = { -(SCREEN_WIDTH + SCREEN_WIDTH / 2.0f),0.0f};
			break;
		case 3:
			m_BlackPos[i] = { (SCREEN_WIDTH + SCREEN_WIDTH / 2.0f),0.0f};
			break;
		default:
			break;
		}
	}
}

CSceneTitle::~CSceneTitle()
{
	if (m_pLogo) {
		delete m_pLogo;
		m_pLogo = nullptr;
	}

	for (int i = 0; i < 2; i++)
	{
		SAFE_DELETE(m_pTran[i]);
	}

	font->Uninit();
}

void CSceneTitle::Update()
{
	if (IsKeyPress('Q'))
	{
		m_Angle += DirectX::XMConvertToRadians(5.0f);
		m_StarSize.x -= SCREEN_WIDTH / 10.0f;
		m_StarSize.y -= SCREEN_HEIGHT / 10.0f;
		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:
				m_BlackPos[i].y += SCREEN_HEIGHT / 2.0f / 10.0f;
				break;
			case 1:
				m_BlackPos[i].y -= SCREEN_HEIGHT / 2.0f / 10.0f;
				break;
			case 2:
				m_BlackPos[i].x += SCREEN_WIDTH / 2.0f / 10.0f;
				break;
			case 3:
				m_BlackPos[i].x -= SCREEN_WIDTH / 2.0f / 10.0f;
				break;
			default:
				break;
			}
		}
	}
	if (IsKeyPress('E'))
	{
		m_Angle -= DirectX::XMConvertToRadians(5.0f);
		m_StarSize.x += SCREEN_WIDTH / 10.0f;
		m_StarSize.y += SCREEN_HEIGHT / 10.0f;
		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:
				m_BlackPos[i].y -= SCREEN_HEIGHT / 2.0f / 10.0f;
				break;
			case 1:
				m_BlackPos[i].y += SCREEN_HEIGHT / 2.0f / 10.0f;
				break;
			case 2:
				m_BlackPos[i].x -= SCREEN_WIDTH / 2.0f / 10.0f;
				break;
			case 3:
				m_BlackPos[i].x += SCREEN_WIDTH / 2.0f / 10.0f;
				break;
			default:
				break;
			}
		}
	}
	

	if (IsKeyTrigger(VK_RETURN))
	{
		SetNext(1); // 切り替え先のシーンを設定（１はゲーム [
	}
	font->Update();
}

void CSceneTitle::Draw()
{
	SetRender2D();

	DirectX::XMFLOAT4X4 world, view, proj;
	DirectX::XMMATRIX mView = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 0.0f, -0.3f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	DirectX::XMMATRIX mProj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.1f, 100.0f);
	mView = DirectX::XMMatrixTranspose(mView);
	mProj = DirectX::XMMatrixTranspose(mProj);
	DirectX::XMStoreFloat4x4(&view, mView);
	DirectX::XMStoreFloat4x4(&proj, mProj);

	DirectX::XMFLOAT2 size = { SCREEN_WIDTH, SCREEN_HEIGHT };
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f);// フレームは倍率を変更せずに表示 
	DirectX::XMMATRIX mWorld = S * T;
	mWorld = DirectX::XMMatrixTranspose(mWorld);
	DirectX::XMStoreFloat4x4(&world, mWorld);

	Sprite::SetWorld(world);
	Sprite::SetView(view);
	Sprite::SetProjection(proj);
	Sprite::SetTexture(m_pLogo);
	Sprite::SetSize(size);
	Sprite::SetOffset({ 0.0f, 0.0f });
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	Sprite::Draw();

	//DrawStar();
	//DrawBlack();

	//font->Draw();
}

void CSceneTitle::DrawStar()
{
	DirectX::XMFLOAT4X4 world, view, proj;
	DirectX::XMMATRIX mView = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 0.0f, -0.3f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	DirectX::XMMATRIX mProj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.1f, 100.0f);
	mView = DirectX::XMMatrixTranspose(mView);
	mProj = DirectX::XMMatrixTranspose(mProj);
	DirectX::XMStoreFloat4x4(&view, mView);
	DirectX::XMStoreFloat4x4(&proj, mProj);

	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f);// フレームは倍率を変更せずに表示 
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(m_Angle);
	DirectX::XMMATRIX mWorld = S * Rz * T;
	mWorld = DirectX::XMMatrixTranspose(mWorld);
	DirectX::XMStoreFloat4x4(&world, mWorld);

	Sprite::SetWorld(world);
	Sprite::SetView(view);
	Sprite::SetProjection(proj);
	Sprite::SetTexture(m_pTran[1]);
	Sprite::SetSize(m_StarSize);
	Sprite::SetOffset(m_StarPos);
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	Sprite::Draw();
}

void CSceneTitle::DrawBlack()
{
	DirectX::XMFLOAT4X4 world, view, proj;
	DirectX::XMMATRIX mView = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(0.0f, 0.0f, -0.3f, 0.0f),
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	DirectX::XMMATRIX mProj = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.1f, 100.0f);
	mView = DirectX::XMMatrixTranspose(mView);
	mProj = DirectX::XMMatrixTranspose(mProj);
	DirectX::XMStoreFloat4x4(&view, mView);
	DirectX::XMStoreFloat4x4(&proj, mProj);

	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, -1.0f, 1.0f);// フレームは倍率を変更せずに表示 
	DirectX::XMMATRIX Rz = DirectX::XMMatrixRotationZ(m_Angle);
	DirectX::XMMATRIX mWorld = S * Rz * T;
	mWorld = DirectX::XMMatrixTranspose(mWorld);
	DirectX::XMStoreFloat4x4(&world, mWorld);

	Sprite::SetWorld(world);
	Sprite::SetView(view);
	Sprite::SetProjection(proj);
	Sprite::SetTexture(m_pTran[0]);
	Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	for (int i = 0; i < 4; i++)
	{
		Sprite::SetSize(m_BlackSize[i]);
		Sprite::SetOffset(m_BlackPos[i]);
		Sprite::Draw();
	}
}
