#pragma once

#include "RendererComponent.h"
#include "Model.h"
#include "Texture.h"

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
    void LoadTexture(std::string inPath);
    static void UnloadAll();

	void SetDepth(bool isDepth) { m_bIsDepth = isDepth; }
    Model* GetModel();

private:
	bool m_bIsDepth = true;    // 深度バッファを使用するかどうか
    static std::map<std::string, Model*> m_pModelList;
    static std::map<std::string, Texture*> m_pCustomTextureList;
    std::vector<Texture*> m_pUseTextureList;
    std::string m_sModelKey;

};
