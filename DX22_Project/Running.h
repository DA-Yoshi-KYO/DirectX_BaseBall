#pragma once
#include "GameObject.h"
#include "Defines.h"
#include "Field.h"

class CRunning
{
public:
	CRunning();
	~CRunning();
	void Update(int AttackPlayer);
	void Draw();
	
public:
	//// インプレー前のランナーの種類
	//enum class RunnerKind
	//{
	//	FirstRunner,	// 一塁ランナー
	//	SecondRunner,	// 二塁ランナー
	//	ThirdRunner,	// 三塁ランナー
	//	BatterRunner,	// バッターランナー

	//	Max,
	//};

	//// ランナーの進行方向
	//enum class Direction
	//{
	//	Forward,	// 進塁
	//	Backward,	// 後退
	//	Stop,		// ベース上で停止
	//	BaseBetween	// ベース間で停止 
	//};

	//// ランナーの統合情報
	//struct RunnerParam
	//{
	//	ModelParam m_tModelParam;	// モデルの統合情報
	//	bool m_bAlive;				// 自分が走者として生きているか
	//	bool m_bStayPrevBase;		// インプレーが始まる前の自分の塁に着いているか
	//	bool m_bRunning;			// 走塁中かどうか
	//	CField::BaseKind m_eArriveKind;		// インプレー中に到達した塁
	//	CField::BaseKind m_ePrevArriveKind;	// インプレー中に到達した塁
	//	Direction m_eDirection;		// 進行方向
	//	float m_fSpeed;				// 走力
	//};

public:

	// ------------Setter------------
	/// <summary> SetOut:引数のランナーをアウトにする </summary>
	/// <param name="kind"> インプレー前のランナーの種類 </param>
	//static void SetOut(RunnerKind kind);

	// ------------Getter------------
	/// <summary> GetRannerParam:ランナーの情報を取得する </summary>
	/// <param name="kind"> インプレー前のランナーの種類 </param>
	/// <returns> 引数のランナーの統合情報 </returns>
	//static RunnerParam GetRannerParam(RunnerKind kind);
	/// <summary> GetCollision:ランナーの当たり判定を取得する </summary>
	/// <param name="kind"> インプレー前のランナーの種類 </param>
	/// <returns> 引数のランナーの当たり判定情報(Box) </returns>
	//static Collision::Info GetCollision(RunnerKind kind);
	/// <summary> GetOnBase:ランナーがいずれかのベース上にいるかを取得する </summary>
	/// <param name="kind"> インプレー前のランナーの種類 </param>
	/// <returns> 引数のランナーが true:いずれかのベース上にいる,false:いずれのベース上にもいない </returns>
	//static bool GetOnBase(RunnerKind kind);

public:
	/// <summary> HomeRun:ホームラン時の処理 </summary>
	//static void HomeRun();
private:
	// メンバ変数
	std::unique_ptr<Model> m_pModel; // ランナーのモデル

	// 静的メンバ変数
	//static bool m_bOnBase[(int)RunnerKind::Max];
	//static RunnerParam m_tRunnerParam[(int)RunnerKind::Max];		// インプレー前のランナーの状態
	//static Collision::Info m_RunnerCollision[(int)RunnerKind::Max];	// ランナーの当たり判定

private:
	// 内部処理
	// ランナーの移動処理
	//void RunnerMove(RunnerKind kind, int AttackPlayer);
	// ランナーが移動しているかチェック
	//void RunnerCheck();
	// ベース上に誰かいるかチェック
	//void BaseStateCheck();

};
