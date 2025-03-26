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

	enum class RunnerKind
	{
		FirstRunner,
		SecondRunner,
		ThirdRunner,
		BatterRunner,

		Max,

		HomeIn = 99
	};
private:
	std::unique_ptr<Model> m_pModel;

	// ランナーの統合情報
	struct RunnerParam
	{
		ModelParam m_tModelParam;	// モデルの統合情報
		bool m_bAlive;				// 自分が走者として生きているか
		bool m_bStayPrevBase;		// インプレーが始まる前の自分の塁に着いているか
		bool m_bRunning;			// 走塁中かどうか
		RunnerKind m_eArriveKind;	// インプレー中に到達した塁
		float m_fSpeed;				// 走力
	}static m_tRunnerParam[(int)RunnerKind::Max];

	void RunnerMove(RunnerKind kind);
	void RunnerCheck();
	void BaseStateCheck();
public:
	static void SetOut(RunnerKind kind,bool isForcePlay);
	static void HomeRun();
};
