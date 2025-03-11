#pragma once

#include "GameObject.h"

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
	void InitText();
	enum E_FIELD
	{
		NONE_FIELD = 0,
		FAIRWAY,
		BUNKER,

		MAX_FIELD
	};
	Model* m_pModel[MAX_FIELD];
	int FieldMap[MAP_X][MAP_Y];

	CCamera* m_pCamera;			// カメラ情報(コンポジション)
};