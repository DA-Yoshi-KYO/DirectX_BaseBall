#pragma once

#include "RendererComponent.h"
#include "Model.h"

/// <summary>
/// モデル描画を扱うコンポーネントクラス
/// </summary>
class CModelRenderer : public CRendererComponent
{
public:
	using CRendererComponent::CRendererComponent;
	~CModelRenderer();
    void Init() override;
	void Draw() override;
    void Load(std::string inPath, float inScale = 1.0f, Model::Flip inFlip = Model::Flip::ZFlip);
    static void UnloadAll();

	void SetDepth(bool isDepth) { m_bIsDepth = isDepth; }
    Model* GetModel();

private:
	bool m_bIsDepth = true;    // 深度バッファを使用するかどうか
    static std::map<std::string, Model*> m_pModelList;
    std::string m_sModelKey;

};
