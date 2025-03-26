#pragma once

#include "GameObject.h"
#include "Defines.h"

class CRunning : public CGameObject
{
public:
	CRunning();
	virtual ~CRunning()override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual void SetModel(ModelParam param, Model* model, bool isAnime = false)override;
private:
	enum class RunnerKind
	{
		FirstRunner,
		SecondRunner,
		ThirdRunner,
		BatterRunner,

		Max
	};
	std::unique_ptr<Model> m_pModel;
	struct RunnnerParam
	{
		ModelParam m_tModelParam;
		bool m_bAlive;
		bool m_bStayPrevBase;
		bool m_bRunning;
		RunnerKind m_eArriveKind;
		float m_fSpeed;
	}m_tRunnerParam[(int)RunnerKind::Max];
};
