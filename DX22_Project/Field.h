#pragma once

#include "GameObject.h"
#include "Defines.h"
#include "Ball.h"

constexpr int ce_nMaxHomerunPolyLine = 40;
constexpr float ce_fGroundY = 0.0f;
constexpr float ce_fFenceHeight = ce_fGroundY + 15.0f;

class CField : public CGameObject
{
private:
	CField();
public:
	~CField();
	void Update() override;
	void Draw() override;
	void SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType = 0) override;
	virtual void OnCollision(Collision::Result collision) override;
	virtual Collision::Info GetCollision() override;
	static std::unique_ptr<CField>& GetInstance();
private:
	std::unique_ptr<Model> m_pField;
	ModelParam m_tFieldParam;
	Collision::Info m_Ground;
	Collision::Info m_LeftFaul;
	Collision::Info m_RightFaul;
	Collision::Info m_InOutBorder;
	std::vector<Collision::Info> m_HomeRunZone;
};
