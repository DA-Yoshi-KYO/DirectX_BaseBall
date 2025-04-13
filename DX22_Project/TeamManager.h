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
private:

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
		Pitcher,
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
		Trajectory,		// 弾道
		Meat,			// ミート
		Power,			// パワー
		Speed,			// 走力
		Throwing,		// 肩力
		Deiffence,		// 守備力
		Chatch,			// 捕球
		Position,		// ポジション
		SubPosition,	// サブポジション
	};

	struct PitcherState
	{
		std::wstring m_sName;
		FieldingNo m_eFieldingNo;	// 守備位置
		float m_fSpeed;		// 球速
		float m_fStamina;	// スタミナ
		float m_fControl;	// コントロール
		bool m_bLefty;		// 左投手か
		bool m_bEntry;		// 出場しているか
		bool m_bLeave;		// 試合から退いたか
		bool m_bBentch;		// ベンチ登録されているか	
	};

	struct BatterState
	{
		std::wstring m_sName;
		FieldingNo m_eFieldingNo;	// 守備位置
		int m_nLineupNo;	// 打順
		int m_nTrajectory;	// 弾道
		float m_fMeat;		// ミート
		float m_fPower;		// パワー
		float m_fSpeed;		// 走力
		float m_fThrowing;	// 肩力
		float m_fDeiffence;	// 守備力
		float m_fChatch;	// 捕球
		bool m_bLefty;		// 左打者か
		bool m_bEntry;		// 出場しているか
		bool m_bLeave;		// 試合から退いたか
	};
public:
	CTeamManager();
	virtual ~CTeamManager();
	virtual bool Load() = 0;
	
private:


protected:
	std::vector<PitcherState> m_tVecPitcherMember;
	std::vector<BatterState> m_tVecBatterMember;

	std::vector<PitcherState> GetPitcherState();
	std::vector<BatterState> GetBatterState();
};