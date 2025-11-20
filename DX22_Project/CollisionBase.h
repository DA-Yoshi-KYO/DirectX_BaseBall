#pragma once

// インクルード部
#include "Component.h"
#include <string>
#include "Collision.h"

/// <summary>
/// 衝突判定をまとめるベースクラス
/// </summary>
class CCollisionBase : public CComponent
{
public:
    using CComponent::CComponent;
    virtual ~CCollisionBase();

    /// <summary>
    /// 衝突が起きたかどうかを取得
    /// </summary>
    /// <param name="other">
    /// 衝突先
    /// </param>
    /// <returns>
    /// true:衝突 false:非衝突
    /// </returns>
    virtual Collision::Result IsHit(CCollisionBase* other);

    /// <summary>
    /// 衝突識別用タグの指定
    /// </summary>
    /// <param name="inTag">
    /// 識別用タグ
    /// </param>
    void SetTag(const std::string& inTag) { m_sTag = inTag; }

    /// <summary>
    /// 衝突識別用タグの取得
    /// </summary>
    /// <param name="inTag">
    /// 識別用タグ
    /// </param>
    const std::string GetTag() { return m_sTag; }

    /// <summary>
    /// 衝突判定を有効にするか設定
    /// </summary>
    /// <param name="inActive">
    /// true:有効 false:無効
    /// </param>
    void SetActive(const bool inActive) { m_bActive = inActive; }

    /// <summary>
    /// 衝突判定が有効かどうか取得
    /// </summary>
    /// <returns>
    /// true:有効 false:無効
    /// </returns>
    const bool GetActive() const { return m_bActive; }

    void SetInfo(Collision::Info info) { m_tCollisionInfo = info; }
    Collision::Info GetInfo() { return m_tCollisionInfo; }

protected:
    bool m_bActive;     // 衝突判定が有効かどうか
    std::string m_sTag; // 衝突判定識別用タグ
    Collision::Info m_tCollisionInfo;

};
