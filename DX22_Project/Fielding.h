#pragma once

#include "Defines.h"
#include "GameObject.h"

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

	bool m_bHold;
	int OperationSearch();
};