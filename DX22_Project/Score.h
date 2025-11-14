#pragma once

#include "GameObject.h"

class CScore : public CGameObject
{
public:
	CScore();
	~CScore();
	void Init() override;
	void Update() override;
	void Draw() override;
	void AddScore();
	void SetScoreOnePos(DirectX::XMFLOAT3 inPos) { m_f3ScoreOnePos = inPos; }
private:
	int m_nScore;
	DirectX::XMFLOAT2 m_f2ScoreUVPos[2];
	DirectX::XMFLOAT3 m_f3ScoreOnePos;

};
