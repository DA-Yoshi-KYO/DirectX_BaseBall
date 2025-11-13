#pragma once

// インクルード部
#include "SpriteRendererBase.h"

/// <summary>
/// 2Dスプライト描画を扱うコンポーネントクラス
/// </summary>
class CSpriteRenderer : public CSpriteRendererBase
{
public:
	using CSpriteRendererBase::CSpriteRendererBase;
	virtual ~CSpriteRenderer();
	void Draw() override;
};

