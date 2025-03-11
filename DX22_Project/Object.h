#pragma once
#include "GameObject.h"
#include "Collision.h"

class CObject : public CGameObject
{
public:
	CObject();
	~CObject();
	void Update() override;
	void Draw() override;
	void SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType = 0) override;
	void SetCamera(CCamera* camera);
	void OnCollision(Collision::Result collision) override;
	Collision::Info GetCollision(int i, int j);
	Collision::Box GetBox(int i, int j);
private:
	void InitText();
	enum E_MODEL
	{
		NONE_MODEL = 0,
		TREE,
		TREE2,
		BALL,
		CUP,
		BEAR,
		DETAIL,

		MAX_MODEL
	};
	Model* m_pModel[MAX_MODEL];
	Collision::Box m_Box[MAP_Y][MAP_X];
	Collision::Info m_CollisionArray[MAP_Y][MAP_X];

	int ObjectMap[MAP_Y][MAP_X];

	CCamera* m_pCamera;			// カメラ情報(コンポジション)
};

