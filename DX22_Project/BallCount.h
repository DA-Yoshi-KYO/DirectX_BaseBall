/*+===================================================================
	File: BallCount.h
	Summary: スコアボード内の処理
	Author: 吉田京志郎
	Date:	2025/03/16	初回作成
						スコアボード、カウントの描画
			2025/03/17	塁状況、イニングの描画
						各種処理を描画に反映
						ボールカウントの処理実装
===================================================================+*/

// ==============================
//    インクルード部
// ==============================
#pragma once
#include "Defines.h"
#include "Texture.h"

// ==============================
//    定数定義
// ==============================
#define MAX_BALL_COUNT 4
#define MAX_STRIKE_COUNT 3
#define MAX_OUT_COUNT 3
#define MAX_BASE_COUNT 4
#define MAX_SCORE 99
#define MAX_INNING 12

class CBallCount
{
private:
	CBallCount();
public:
	~CBallCount();
	void Init();
	void Update();
	void Draw();

	/// <summary> AddBallCount:ボールのカウントを1増やす </summary>
	void AddBallCount();
	/// <summary> AddStrikeCount:ストライクのカウントを1増やす </summary>
	void AddStrikeCount(bool isFoul = false);
	/// <summary> AddOutCount:アウトのカウントを1増やす </summary>
	void AddOutCount();
	/// <summary> AddScore:引数のチームのスコアを1増やす </summary>
	/// <param name="No:"> どちらのチームか(0 or 1) </param>
	void AddScore(int No);
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
	enum class Inning
	{
		TOP,
		BOTTOM
	};
	Inning GetInning();

	// 先行チーム・後攻チーム
	enum class Team
	{
		TOP,
		BOTTOM
	};

	enum class InplayElement
	{
		HoldBall,
		Running,
		Max
	};
	bool m_bInplay[(int)InplayElement::Max];
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
		bool m_bTop;
		bool m_bEnd;
	}m_tCount;

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