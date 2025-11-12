#include "SoundList.h"

CSoundList::CSoundList()
	: m_pVecSound{}
{
}

CSoundList::~CSoundList()
{
}

void CSoundList::Load(CScene::SceneKind kind)
{
	LoadBGM(kind);
	LoadSE(kind);
}

void CSoundList::Destroy()
{
	for (auto itr = m_pVecSound.begin(); itr != m_pVecSound.end();)
	{
		(*itr) = nullptr;
		itr = m_pVecSound.erase(itr);
	}
}

void CSoundList::LoadBGM(CScene::SceneKind kind)
{
	switch (kind)
	{
	case CScene::SceneKind::Title:
		m_pVecSound.push_back(LoadSound(""));
		break;
	case CScene::SceneKind::TeamSelect:
		m_pVecSound.push_back(LoadSound(""));
		break;
	case CScene::SceneKind::MemberSelect:
		m_pVecSound.push_back(LoadSound(""));
		break;
	case CScene::SceneKind::Game:
		m_pVecSound.push_back(LoadSound(""));
		break;
	case CScene::SceneKind::Result:
		m_pVecSound.push_back(LoadSound(""));
		break;
	case CScene::SceneKind::Max:
		break;
	default:
		break;
	}
}

void CSoundList::LoadSE(CScene::SceneKind kind)
{
	switch (kind)
	{
	case CScene::SceneKind::Title:
		m_pVecSound.push_back(LoadSound(""));
		m_pVecSound.push_back(LoadSound(""));
		m_pVecSound.push_back(LoadSound(""));
		m_pVecSound.push_back(LoadSound(""));
		break;
	case CScene::SceneKind::TeamSelect:
		break;
	case CScene::SceneKind::MemberSelect:
		break;
	case CScene::SceneKind::Game:
		break;
	case CScene::SceneKind::Result:
		break;
	case CScene::SceneKind::Max:
		break;
	default:
		break;
	}
}
