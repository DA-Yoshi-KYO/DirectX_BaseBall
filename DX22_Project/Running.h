// ==============================
//    インクルード部
// ==============================
#pragma once
#include "GameObject.h"
#include "Defines.h"
#include "Field.h"

class CRunning : public CGameObject
{
public:
	CRunning();
	virtual ~CRunning()override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual void SetModel(ModelParam param, Model* model, bool isAnime = false)override;

	// インプレー前のランナーの種類
	enum class RunnerKind
	{
		FirstRunner,	// 一塁ランナー
		SecondRunner,	// 二塁ランナー
		ThirdRunner,	// 三塁ランナー
		BatterRunner,	// バッターランナー

		Max,
	};

	// ランナーの進行方向
	enum class Direction
	{
		Forward,	// 進塁
		Backward,	// 後退
		Stop,		// ベース上で停止
		BaseBetween	// ベース間で停止 
	};

	// ランナーの統合情報
	struct RunnerParam
	{
		ModelParam m_tModelParam;	// モデルの統合情報
		bool m_bAlive;				// 自分が走者として生きているか
		bool m_bStayPrevBase;		// インプレーが始まる前の自分の塁に着いているか
		bool m_bRunning;			// 走塁中かどうか
		BaseKind m_eArriveKind;		// インプレー中に到達した塁
		BaseKind m_ePrevArriveKind;	// インプレー中に到達した塁
		Direction m_eDirection;		// 進行方向
		float m_fSpeed;				// 走力
	};
private:
	// メンバ変数
	std::unique_ptr<Model> m_pModel; // ランナーのモデル

	// 静的メンバ変数
	static bool m_bOnBase[(int)RunnerKind::Max];
	static RunnerParam m_tRunnerParam[(int)RunnerKind::Max];		// インプレー前のランナーの状態
	static Collision::Info m_RunnerCollision[(int)RunnerKind::Max];	// ランナーの当たり判定

private:
	// 内部処理
	// ランナーの移動処理
	void RunnerMove(RunnerKind kind);
	// ランナーが移動しているかチェック
	void RunnerCheck();
	// ベース上に誰かいるかチェック
	void BaseStateCheck();
public:
	static RunnerParam GetRannerParam(RunnerKind kind);
	static void SetOut(RunnerKind kind);
	static void HomeRun();
	static Collision::Info GetCollision(int No);
	static bool GetOnBase(int No);
};
