#pragma once

// インクルード部

// 前方宣言
class CGameObject;

/// <summary>
/// コンポーネントを取りまとめるベースクラス
/// </summary>
class CComponent
{
public:
    // コンポーネントはGameObjectに依存しているので
    // デフォルトのコンストラクタを無効にし、GameObjectに持たせる
	CComponent() = delete;
    CComponent(CGameObject* inGameObject);
	virtual ~CComponent();

	virtual void Init();
	virtual void Uninit();	
	virtual void Update();
	virtual void Draw();

	/// <summary>
	/// コンポーネントに紐付けられているGameObjectを取得する
	/// </summary>
	/// <returns>
    /// (CGameObject*)コンポーネントに紐付けられているGameObjectクラスポインタ
    /// </returns>
	CGameObject* GetGameObject() { return m_pGameObject; }
	
protected:
	CGameObject* m_pGameObject; // コンポーネントに紐付けるGameObject

};

