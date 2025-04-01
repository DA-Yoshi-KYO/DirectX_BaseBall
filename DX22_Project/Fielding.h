// ==============================
//    インクルード部
// ==============================
#pragma once
#include "Defines.h"
#include "GameObject.h"
#include "Collision.h"
#include "Field.h"
#include "Running.h"

class CFielding : public CGameObject
{
public:
	CFielding();
	virtual ~CFielding()override;
	virtual void Update()override;
	virtual void Draw()override;
	virtual void SetModel(ModelParam param, Model* model, bool isAnime = false) override;

public:
	// キャッチの仕方
	enum class ChatchPattern
	{
		Grounder,	// ゴロ捕球
		Fry,		// フライ捕球
		NotChatch	// まだキャッチしていない
	};

private:
	// 守備位置
	enum class FieldMember
	{
		Pitcher,	// 投手 
		Chatcher,	// 捕手
		First,		// 一塁手
		Second,		// 二塁手
		Third,		// 三塁手
		Short,		// 遊撃手
		Left,		// 左翼手
		Center,		// 中堅手
		Right,		// 右翼手

		Max
	};
public:
	// ==============================
	//    アクセサ
	// ==============================
	// 
	// ------------Getter------------
	/// <summary> 守備選手の当たり判定を取得する </summary>
	/// <param name="kind"> 守備選手の種類 </param>
	/// <returns> 引数の守備選手の当たり判定情報(Box) </returns>
	Collision::Info GetCollision(FieldMember Member);
	/// <summary> キャッチの仕方を取得する </summary>
	/// <returns> ゴロ捕球かフライ捕球か未捕球か </returns>
	static ChatchPattern GetChatchPattern();

private:
	// コンポジション
	std::unique_ptr<Model> m_pFieldMember[(int)FieldMember::Max];

	// モデルパラメータ
	ModelParam m_tParam[(int)FieldMember::Max];

	// メンバ変数
	int m_nOperationNo;	// 操作しているキャラの番号
	int m_nBaseNearNo[(int)CField::BaseKind::Max];	// ベースに一番近いキャラの番号
	bool m_bHold;	// ボールを持っているか
	bool m_bBaseCovered[(int)CField::BaseKind::Max];	// 守備選手がベースを踏んでいるか
	DirectX::XMFLOAT3 m_fThrowDirection;	// 投げる方向

	// 静的メンバ変数
	static ChatchPattern m_eChatch;	// キャッチの仕方

	// 当たり判定情報
	Collision::Info m_Collision[(int)FieldMember::Max];

private:
	// 内部処理
	// フォースプレイアウトの処理
	void OutProcess(CRunning::RunnerParam* RunnerParam, CField::BaseKind kind);
	// ボールに一番近い選手を探索する処理
	int OperationSearch();
	// 引数のベースに一番近い選手を操作選手以外から探索する処理
	int BaseSearch(CField::BaseKind kind);
	// ベースに一番近い選手がベースカバーしに行く処理
	void BaseCover();
	// 送球処理
	void Throwing(CField::BaseKind kind);

};