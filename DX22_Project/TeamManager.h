#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>
#include "Defines.h"

class CTeamManager
{
public:
	enum Teams
	{
		Bears,
		Rabbits,
		Tigers,
		Elephants,
		Monkeys,

		Max
	};

	enum Quality
	{
		G,
		F,
		E,
		D,
		C,
		B,
		A,
		S
	};

	
	enum FieldingNo
	{
		None = -1,	// 不出場
		Pitcher = 1,
		Chatcher,
		First,
		Second,
		Third,
		Short,
		Left,
		Center,
		Right,
		DH
	};

	enum csvData
	{
		Name,			// 名前
		PitcherBatter,	// 投手か野手か
		Handed,			// 利き腕
		BallSpeed,		// 球速
		Control,		// コントロール
		Stamina,		// スタミナ
		Slider,			// スライダー変化量
		Curve,			// カーブ変化量
		Split,			// フォーク変化量
		Sinker,			// シンカー変化量
		Shoot,			// シュート変化量
		Twoseam,		// ツーシーム変化量
		SliderKind,		// スライダー系球種
		CurveKind,		// カーブ系球種
		SplitKind,		// フォーク系球種
		SinkerKind,		// シンカー変化量系球種
		ShootKind,		// シュート系球種
		TwoseamKind,	// ツーシーム系球種
		StarterRelief,	// 先発か中継ぎか
		Trajectory,		// 弾道
		Meat,			// ミート
		Power,			// パワー
		Speed,			// 走力
		Throwing,		// 肩力
		Defence,		// 守備力
		Chatch,			// 捕球
		Position,		// ポジション
		SubPosition,	// サブポジション
		PositionEntry,	// 出場ポジション
		LineupNo		// 打順
	};

	struct PitcherState
	{
		std::wstring m_sName;
		FieldingNo m_eFieldingNo;	// 守備位置
		FieldingNo m_eAptitude;		// ポジション適性
		float m_fSpeed;		// 球速
		Quality m_eStamina;	// スタミナ
		Quality m_eControl;	// コントロール
		bool m_bLefty;		// 左投手か
		bool m_bEntry;		// 出場しているか
		bool m_bLeave;		// 試合から退いたか
		bool m_bBentch;		// ベンチ登録されているか	
		bool m_bStarter;	// 先発かどうか
		int m_nStarterNo;		// ベンチナンバー
		int m_nBenchNo;		// ベンチナンバー
	};

	struct BatterState
	{
		std::wstring m_sName;
		FieldingNo m_eFieldingNo;	// 守備位置
		FieldingNo m_eAptitude;		// ポジション適性
		FieldingNo m_eSubAptitude;	// サブポジション適性
		int m_nLineupNo;	// 打順
		int m_nTrajectory;	// 弾道
		Quality m_eMeat;		// ミート
		Quality m_ePower;		// パワー
		Quality m_eSpeed;		// 走力
		Quality m_eThrowing;	// 肩力
		Quality m_eDefence;	// 守備力
		Quality m_eChatch;	// 捕球
		bool m_bLefty;		// 左打者か
		bool m_bEntry;		// 出場しているか
		bool m_bLeave;		// 試合から退いたか
		int m_nBenchNo;		// ベンチナンバー
		int m_nBaseNo;		// どのベース上にいるか
	};

private:
	CTeamManager();

public:
	void Init();
	void Release(int teamNo);
	~CTeamManager();
	bool Load(Teams team);
	static std::shared_ptr<CTeamManager>& GetInstance(int teamNo);
	std::vector<PitcherState> GetPitcherState();
	std::vector<BatterState> GetBatterState();
	PitcherState GetTakingPitcherState();
	BatterState GetTakingBatterState();
	BatterState GetRunnerState(int BaseNo);
	BatterState GetFielderState(FieldingNo No);
	Teams GetTeam();
	void InitStarter();
	void ResetStarter(int StarterNo);
	void ResetLineup(int BenchNo,int LineupNo);
	void ResetLineupNo(int LineupNo1,int LineupNo2);
	void ResetFielding(FieldingNo LineupNo1, FieldingNo LineupNo2);
	void SetEntry(int No, bool isEntry, bool isPitcher);
	void SetBenchNo(int No, int BenchNo, bool isPitcher);
	void SetPosition(int No, FieldingNo position, bool isPitcher);
	void SetPitcherState(std::vector<PitcherState> state);
	void SetBatterState(std::vector<BatterState> state);
	void SetFourBall();
	void NextBatter();
private:
	std::vector<PitcherState> m_tVecPitcherMember;
	std::vector<BatterState> m_tVecBatterMember;
	Teams m_eTeamKind;
	static std::shared_ptr<CTeamManager> m_pTeam[Player::MaxPlayer];
	int m_nTakingNo;
};