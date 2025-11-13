#pragma once

// インクルード部
#include "SpriteRendererBase.h"

/// <summary>
/// ビルボード描画を扱うコンポーネントクラス
/// </summary>
class CBillboardRenderer : public CSpriteRendererBase
{
public: 
	using CSpriteRendererBase::CSpriteRendererBase;
	~CBillboardRenderer();
	void Draw() override;

    /// <summary>
    /// 深度バッファを使用するかどうかの指定
    /// </summary>
    /// <param name="isDepth">
    /// true:深度バッファ使用 false:深度バッファ不使用
    /// </param>
    void SetDepth(bool isDepth) { m_bIsDepth = isDepth; }

private:
    bool m_bIsDepth = true;    // 深度バッファを使用するかどうか
};

