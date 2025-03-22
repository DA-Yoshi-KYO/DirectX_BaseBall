#pragma once

#include "GameObject.h"
#include "Defines.h"

constexpr int ce_nMaxHomerunPolyLine = 40;

class CField : public CGameObject
{
public:
	CField();
	~CField();
	void Update() override;
	void Draw() override;
	void SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType = 0) override;
	void SetCamera(CCamera* camera);
	virtual void OnCollision(Collision::Result collision) override;
	virtual Collision::Info GetCollision() override;
private:
	std::unique_ptr<Model> m_pField;
	std::unique_ptr<CCamera> m_pCamera;			// カメラ情報(コンポジション)
	ModelParam m_tFieldParam;
	Collision::Info m_Ground;
	Collision::Info m_LeftFaul;
	Collision::Info m_RightFaul;
	Collision::Info m_InOutBorder;
	std::vector<Collision::Info> m_HomeRunZone;
	Collision::Info m_HomeRunFence[ce_nMaxHomerunPolyLine];
};
