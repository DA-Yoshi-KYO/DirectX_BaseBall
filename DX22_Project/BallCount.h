// ==============================
//    インクルード部
// ==============================
#pragma once
#include "Defines.h"
#include "Texture.h"

// ==============================
//    定数定義
// ==============================
#define MAX_BALL_COUNT 4	// ボールのカウントの最大数
#define MAX_STRIKE_COUNT 3	// ストライクのカウントの最大数
#define MAX_OUT_COUNT 3		// アウトのカウントの最大数
#define MAX_BASE_COUNT 4	// ベースの数の最大数
#define MAX_SCORE 99		// スコアの最大値
#define MAX_INNING 12		// イニングの最大数

class CBallCount
{
public:
	enum class InningHalf
	{
		Top,
		Bottom
	};

private:
	CBallCount();
public:
	~CBallCount();
	void Init(InningHalf Player1Harf);
	void Update();
	void Draw();

public:
	/// <summary> AddBallCount:ボールのカウントを1増やす </summary>
	void AddBallCount();
	/// <summary> AddStrikeCount:ストライクのカウントを1増やす </summary>
	void AddStrikeCount(bool isFoul = false);
	/// <summary> AddOutCount:アウトのカウントを1増やす </summary>
	void AddOutCount();
	/// <summary> AddScore:攻撃チームのスコアを1増やす </summary>
	void AddScore();
	/// <summary> SetBaseState:引数番目のベース状況を変える </summary>
	/// <param name="base:"> ベース番号(0 ～ 3) </param>
	/// <param name="state:"> 塁状況(true:ランナー有,false:ランナー無) </param>
	void SetBaseState(int base, bool state);
	/// <summary> SetBaseState:引数番目のベース状況を取得する </summary>
	/// <param name="base:"> ベース番号(0 ～ 3) </param>
	///  <returns> 塁状況(true:ランナー有,false:ランナー無) </returns>
	bool GetBaseState(int base);
	/// <summary> ResetCount:ストライク・ボールのカウントを0にリセットする </summary>
	void ResetCount();
	/// <summary> ChangeInning:どちらかのチームの攻撃が終わったら呼び出し、次のイニングの準備をする </summary>
	void ChangeInning();
	/// <summary> IsEnd:試合が終わったかどうか </summary>
	/// <returns> true:試合終了,false:試合中 </returns>
	bool IsEnd();

	// オモテ・ウラ
	enum class Team
	{
		Player1,
		Player2
	};

	// ゲームの進行要素
	struct GameState 
	{
		InningHalf half;
		Team offense;
		Team defense;
	}m_tGameState;
	Team GetOffenseTeam();
	Team GetDefenseTeam();

	// インプレー修了条件要素
	enum class InplayElement
	{
		HoldBall,
		Running,
		Max
	};
	bool m_bInplay[(int)InplayElement::Max]; // インプレー修了条件

	/// <summary> SetEndInplay:Inplay状態を終了していいかをセットする </summary>
	/// <param name="ElemEndInplay:"> Inplay状態を継続している要素 </param>
	/// <param name="state:"> true:Inplay中,false:終了 </param>
	void SetEndInplay(InplayElement ElemEndInplay,bool state);
	/// <summary> SetEndInplay:Inplay状態を終了していいかを取得する </summary>
	/// <returns> true:Inplay終了,false:Inplay中 </returns>
	bool GetEndInplay();

	/// <summary> シングルトンインスタンスの取得 </summary>
	static std::unique_ptr <CBallCount>& GetInstance();

private:
	// コンポジション
	std::unique_ptr<Texture> m_pSheet;
	std::unique_ptr<Texture> m_pBack;
	// スプライトのパラメータ
	SpriteParam m_tSheetParam;
	SpriteParam m_tBackParam;
	// 各種状況
	struct TCount
	{
		int m_nBallCount;
		int m_nStrikeCount;
		int m_nOutCount;
		bool m_bBaseState[4];
		int m_nScore[2];
		int m_nInning;
		bool m_bEnd;
	}m_tCount;

	bool m_bPlayer1Top;

private:
	// 内部処理
	// ボールカウントの描画
	void DrawBallCount();
	// ベース状況の描画
	void DrawBaseCount();
	// 得点の描画
	void DrawScore();
	// イニングの描画
	void DrawInning();
};