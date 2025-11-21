#pragma once

// インクルード部
#include "Defines.h"
#include "Component.h"
#include "imgui.h"
#include "RendererComponent.h"
#include <list>
#include "CollisionBase.h"

// オブジェクトタグ
enum class Tag
{
	None,
    SkyBox,    
    Field,     
    Sound,     
	GameObject,
	PostEffect,
	UI,

    Max
};

// オブジェクト識別用ID
struct ObjectID
{
    std::string m_sName;    // オブジェクトの名前
    int m_nSameCount;       // 同オブジェクトの数
};

/// <summary>
/// ゲームオブジェクトをまとめるベースクラス
/// </summary>
class CGameObject
{
public:
    // 初期化処理時に値を参照したい時は、その値をコンストラクタで初期化する
    // ※ここでのコンストラクタが各種Initより先に呼ばれる為
    CGameObject();
    virtual ~CGameObject();
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void WriteBuffer();
	virtual void Draw();

    /// <summary>
    /// オブジェクトが破棄された時の処理
    /// </summary>
    virtual void OnDestroy();

    /// <summary>
    /// オブジェクトが衝突された時の処理
    /// </summary>
    virtual void OnCollision(CCollisionBase* other, std::string thisTag, Collision::Result result);

    /// <summary>
    /// オブジェクトを破棄する
    /// </summary>
    virtual void Destroy();

    /// <summary>
    /// オブジェクトが破棄予定かどうかを取得
    /// </summary>
    /// <returns>
    /// true:破棄予定 false:破棄されていない
    /// </returns>
    bool IsDestroy();

    /// <summary>
    /// IMGUIインスペクターの表示
    /// </summary>
    /// <param name="isEnd">
    /// true:ImGui::End()を呼び出す false:呼び出さない
    /// </param>
    /// <returns>
    /// (int)この関数内で使用したChildの数
    /// </returns>
    virtual int Inspecter(bool isEnd = true);

    /// <summary>
    /// オブジェクトの前方向ベクトルの取得
    /// </summary>
    /// <returns>
    /// (DirectX::XMFLOAT3)前方向単位ベクトル
    /// </returns>
    DirectX::XMFLOAT3 GetForward();

    /// <summary>
    /// オブジェクトの右方向ベクトルの取得
    /// </summary>
    /// <returns>
    /// (DirectX::XMFLOAT3)右方向単位ベクトル
    /// </returns>
    DirectX::XMFLOAT3 GetRight();

    /// <summary>
    /// オブジェクトの上方向ベクトルの取得
    /// </summary>
    /// <returns>
    /// (DirectX::XMFLOAT3)上方向単位ベクトル
    /// </returns>
    DirectX::XMFLOAT3 GetUp();

public:
	/// <summary>
	/// コンポーネントの追加
	/// </summary>
	/// <param name="T">
    /// 追加するCComponent型のコンポーネントクラス
    /// </param>
	/// <returns>
    /// 追加したコンポーネントクラスのポインタ
    /// </returns>
	template<typename T = CComponent>
	T* AddComponent()
	{
        // コンポーネントをインスタンス化し、自身を紐付ける
		T* pComponent = new T(this);

        // コンポーネントのリストに追加する
		m_pComponent_List.push_back(pComponent);

        // 初期化処理
		((CComponent*)pComponent)->Init();

		return pComponent;
	}

    /// <summary>
    /// コンポーネントの取得
    /// </summary>
    /// <param name="T">
    /// 取得するCComponent型のコンポーネントクラス
    /// </param>
    /// <returns>
    /// 一番最初に見つかったCComponent型のポインタ、見つからなかった場合はnullptr
    /// </returns>
	template<typename T = CComponent>
	T* GetComponent()
	{
        // 自身を紐付けている全てのコンポーネントを探索
		for (CComponent* pComponent : m_pComponent_List)
		{
            // T*型のコンポーネントが見つかった場合はその値を返す
			T* pRet = dynamic_cast<T*>(pComponent);
			if (pRet != nullptr) return pRet;
		}

        // 見つからなかった場合はnullptrを返す
		return nullptr;
	}

    /// <summary>
    /// コンポーネントの取得
    /// </summary>
    /// <param name="T">
    /// 取得するCComponent型のコンポーネントクラス
    /// </param>
    /// <returns>
    /// CComponent型のポインタリスト、見つからなかった場合は要素数0のポインタリスト
    /// </returns>
    template<typename T = CComponent>
    std::list<T*> GetSameComponents()
    {
        std::list<T*> componentList;
        componentList.clear();

        // 自身を紐付けている全てのコンポーネントを探索
        for (CComponent* pComponent : m_pComponent_List)
        {
            // T*型のコンポーネントが見つかった場合はその値をリストに格納する
            T* pRet = dynamic_cast<T*>(pComponent);
            if (pRet != nullptr) componentList.push_back(pRet);
        }

        // 探索した結果のリストを返す
        return componentList;
    }

public:
    /// <summary>
    /// オブジェクトの座標をセット
    /// </summary>
    /// <param name="inPos">
    /// オブジェクトの座標
    /// </param>
    void SetPos(DirectX::XMFLOAT3 inPos) { m_tParam.m_f3Pos = inPos; }

    /// <summary>
    /// オブジェクトの座標を取得
    /// </summary>
    /// <returns>
    /// (DirectX::XMFLOAT3)オブジェクトの座標
    /// </returns>
    DirectX::XMFLOAT3 GetPos() { return m_tParam.m_f3Pos; }

    /// <summary>
    /// オブジェクトのサイズをセット
    /// </summary>
    /// <param name="inSize">
    /// オブジェクトのサイズ
    /// </param>
    void SetSize(DirectX::XMFLOAT3 inSize) { m_tParam.m_f3Size = inSize; }

    /// <summary>
    /// オブジェクトのサイズを取得
    /// </summary>
    /// <returns>
    /// (DirectX::XMFLOAT3)オブジェクトのサイズ
    /// </returns>
    DirectX::XMFLOAT3 GetSize() { return m_tParam.m_f3Size; }

    /// <summary>
    /// オブジェクトの回転をセット
    /// </summary>
    /// <param name="inRotate">
    /// オブジェクトの回転
    /// </param>
    void SetRotate(DirectX::XMFLOAT3 inRotate) { m_tParam.m_f3Rotate = inRotate; }

    /// <summary>
    /// オブジェクトの回転を取得
    /// </summary>
    /// <returns>
    /// (DirectX::XMFLOAT3)オブジェクトの回転
    /// </returns>
    DirectX::XMFLOAT3 GetRotate() { return m_tParam.m_f3Rotate; }

    /// <summary>
    /// オブジェクトの色(RGBA)をセット
    /// </summary>
    /// <returns>
    /// オブジェクトの色
    /// </returns>
    void SetColor(DirectX::XMFLOAT4 inColor) { m_tParam.m_f4Color = inColor; }

    /// <summary>
    /// オブジェクトの色(RGBA)を取得
    /// </summary>
    /// <returns>
    /// (DirectX::XMFLOAT4)オブジェクトの色
    /// </returns>
    DirectX::XMFLOAT4 GetColor() { return m_tParam.m_f4Color; }

    /// <summary>
    /// オブジェクトのタグをセット
    /// </summary>
    /// <param name="inTag">
    /// オブジェクトのタグ
    /// </param>
    void SetTag(Tag inTag) { m_eTag = inTag; }

    /// <summary>
    /// オブジェクトのタグを取得
    /// </summary>
    /// <returns>
    /// (Tag)オブジェクトのタグ
    /// </returns>
    Tag GetTag() { return m_eTag; }

    /// <summary>
    /// オブジェクトの識別用IDをセット
    /// </summary>
    /// <param name="inTag">
    ///オブジェクトの識別用ID
    /// </param>
    void SetID(ObjectID inTag) { m_tID = inTag; }

    /// <summary>
    /// オブジェクトの識別用IDを取得
    /// </summary>
    /// <returns>
    /// (ObjectID)識別用ID構造体
    /// </returns>
    ObjectID GetID() { return m_tID; }

    void SetActive(bool isActive) { m_bActive = isActive; }
    bool GetActive() { return m_bActive; }

    /// <summary>
    /// オブジェクトのワールド行列を取得
    /// </summary>
    /// <returns>
    /// (DirectX::XMFLOAT4X4*)オブジェクトのワールド行列参照
    /// </returns>
    DirectX::XMFLOAT4X4* GetWorld();

public:
    std::list<CComponent*> m_pComponent_List;   // コンポーネントのリスト

protected:
    RendererParam m_tParam;         // 描画パラメータ
    DirectX::XMFLOAT3 m_f3OldPos;   // 前フレームの位置
    bool m_bDestroy;                // オブジェクトが破棄されているかのフラグ
    bool m_bActive;                 // 破棄はしないが、更新や描画等の対象外としたいときに使用
    Tag m_eTag;                     // オブジェクトのタグ
    ObjectID m_tID;                 // オブジェクト識別用ID
    std::vector<PixelShader*> m_pDebugPSList;

};

