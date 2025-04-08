// ==============================
//    インクルード部
// ==============================
#pragma once
#include "GameObject.h"
#include "Defines.h"
#include "Ball.h"

// ==============================
//    定数定義
// ==============================
constexpr float ce_fGroundY = -5.0f;	// グラウンドの見た目上の高さ
constexpr float ce_fFenceHeight = ce_fGroundY + 15.0f;	// フェンスの高さ
constexpr float ce_fInOutBorderZ = WORLD_AJUST;	// 内外野の境目
constexpr DirectX::XMFLOAT3 ce_fPitcherPos = { WORLD_AJUST, WORLD_AJUST - 8.0f, WORLD_AJUST + 60.0f };	// グラウンドにいるピッチャーの位置(基準値)

class CField : public CGameObject
{
private:
	CField();

public:
	~CField();
	void Update() override;
	void Draw() override;
	void SetModel(ModelParam param, Model* model, bool isAnime = false) override;

public:
	// ベースの種類
	enum class BaseKind
	{
		First,	// ファーストベース
		Second,	// セカンドベース
		Third,	// サードベース
		Home,	// ホームベース

		Max
	};

public:	
	// ==============================
	//    アクセサ
	// ==============================
	// 
	// ------------Getter------------
	/// <summary> グラウンドの座標を取得する</summary>
	/// <returns> グラウンドの座標 </returns>
	DirectX::XMFLOAT3 GetPos();
	/// <summary> グラウンドのサイズを取得する</summary>
	/// <returns> グラウンドのサイズ </returns>
	DirectX::XMFLOAT3 GetSize();
	/// <summary> ベースの座標を取得する </summary>
	/// <param name="No"> ベースの種類 </param>
	/// <returns> 引数の種類のベースの座標 </returns>
	DirectX::XMFLOAT3 GetBasePos(BaseKind No);
	/// <summary> ベースのサイズを取得する </summary>
	/// <param name="No"> ベースの種類 </param>
	/// <returns> 引数の種類のベースのサイズ </returns>
	DirectX::XMFLOAT3 GetBaseSize(BaseKind No);
	/// <summary> CFieldクラスのシングルトンインスタンスを取得する</summary>
	/// <returns> CFieldクラスのインスタンス </returns>
	static std::unique_ptr<CField>& GetInstance();

public:
	/// <summary> OnCollision:グラウンドに当たった時の処理 </summary>
	/// <param name="collision"> 当たった結果 </param>
	void OnCollision(Collision::Result collision);

private:
	// コンポジション
	std::unique_ptr<Model> m_pField;
	std::unique_ptr<Model> m_pSkydome;
	std::unique_ptr<Model> m_pBase;
	std::unique_ptr<Model> m_pHomeBase;

	// モデルのパラメータ
	ModelParam m_tFieldParam;
	ModelParam m_tSkydomeParam;
	ModelParam m_tBaseParam[(int)BaseKind::Max];

	// 当たり判定情報
	Collision::Info m_Ground;
	std::vector<Collision::Info> m_FirstBaseLine;	// 一塁線
	std::vector<Collision::Info> m_ThirdBaseLine;	// 三塁線
	std::vector<Collision::Info> m_LeftFoul;		// レフト側ファールゾーン
	std::vector<Collision::Info> m_RightFoul;		// ライト側ファールゾーン
	std::vector<Collision::Info> m_HomeRunZone;		// ホームランゾーン

private:
	// 内部処理
	// モデルの読み込み
	void InitModel();
	// 当たり判定情報の初期化
	void InitCollision();

};
