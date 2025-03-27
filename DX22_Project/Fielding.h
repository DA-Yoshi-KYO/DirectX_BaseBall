#pragma once

#include "Defines.h"
#include "GameObject.h"
#include "Collision.h"
#include "Field.h"
#include "Running.h"

enum class ChatchPattern
{
	Grounder,
	Fry,
	NotChatch
};

class CFielding : public CGameObject
{
public:
	CFielding();
	virtual ~CFielding()override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual void SetModel(ModelParam param, Model* model, bool isAnime = false) override;
private:
	enum class FieldMember
	{
		Pitcher,
		Chatcher,
		First,
		Second,
		Third,
		Short,
		Left,
		Center,
		Right,

		Max
	};
	std::unique_ptr<Model> m_pFieldMember[(int)FieldMember::Max];
	ModelParam m_tParam[(int)FieldMember::Max];
	Collision::Info m_Collision[(int)FieldMember::Max];
	int m_nOperationNo;
	int m_nBaseNearNo[(int)BaseKind::Max];
	void BaseCover();
	void Throwing(BaseKind kind);
	bool m_bHold;
	bool m_bBaseCovered[(int)BaseKind::Max];
	int OperationSearch();
	int BaseSearch(BaseKind kind);
	DirectX::XMFLOAT3 m_fThrowDirection;
	ChatchPattern m_eChatch;
	void OutProcess(CRunning::RunnerParam* RunnerParam,BaseKind kind);
public:
	Collision::Info GetCollision(FieldMember Member);
	DirectX::XMFLOAT3 GetThrowDirection();
	ChatchPattern GetChatchPattern();
};