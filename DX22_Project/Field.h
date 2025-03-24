#pragma once

#include "GameObject.h"
#include "Defines.h"
#include "Ball.h"

constexpr int ce_nMaxHomerunPolyLine = 40;
constexpr float ce_fGroundY = 0.0f;
constexpr float ce_fFenceHeight = ce_fGroundY + 15.0f;
constexpr DirectX::XMFLOAT3 ce_fPitcherPos = { WORLD_AJUST, WORLD_AJUST-8.0f, WORLD_AJUST + 60.0f };

enum class BaseKind
{
	Home,
	First,
	Second,
	Third,

	Max
};

class CField : public CGameObject
{
private:
	CField();
public:
	~CField();
	void Update() override;
	void Draw() override;
	void SetModel(ModelParam param, Model* model, bool isAnime = false) override;
	void OnCollision(Collision::Result collision);
	static std::unique_ptr<CField>& GetInstance();
	DirectX::XMFLOAT3 GetPos();
	DirectX::XMFLOAT3 GetBasePos(BaseKind No);
	DirectX::XMFLOAT3 GetSize();
	DirectX::XMFLOAT3 GetRotation();
private:
	std::unique_ptr<Model> m_pField;
	ModelParam m_tFieldParam;
	std::unique_ptr<Model> m_pBase;
	std::unique_ptr<Model> m_pHomeBase;
	ModelParam m_tBaseParam[(int)BaseKind::Max];
	Collision::Info m_Ground;
	Collision::Info m_LeftFaul;
	Collision::Info m_RightFaul;
	Collision::Info m_InOutBorder;
	std::vector<Collision::Info> m_HomeRunZone;
private:
	void InitModel();
	void InitCollision();
};
