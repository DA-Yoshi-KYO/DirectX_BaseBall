#pragma once

// インクルード部
#include "Component.h"
#include "Texture.h"
#include "Model.h"
#include "Shader.h"
#include <variant>

// オブジェクトの汎用パラメータ
struct RendererParam
{
	DirectX::XMFLOAT3 m_f3Pos;
	DirectX::XMFLOAT3 m_f3Size;
	DirectX::XMFLOAT3 m_f3Rotate;
	DirectX::XMFLOAT4 m_f4Color;
	DirectX::XMFLOAT2 m_f2UVPos;
	DirectX::XMFLOAT2 m_f2UVSize;
	D3D11_CULL_MODE m_eCulling;
};


/// <summary>
/// レンダラー用の仲介コンポーネントクラス
/// </summary>
class CRendererComponent : public CComponent
{
public:
	using CComponent::CComponent;
	virtual ~CRendererComponent();

    static void UnloadShader();

	// 各種パラメータの設定
	/// <summary>
	/// レンダラーの統合パラメータを設定
	/// </summary>
	/// <param name="inParam">
    /// 設定するレンダラーのパラメータ
    /// </param>
	void SetRendererParam(const RendererParam inParam);

	/// <summary>
	/// 表示するローカル座標を設定
	/// </summary>
	/// <param name="inPos">
    /// 設定する座標の値(DirectX::XMFLOAT3)
    /// </param>
	void SetPos(DirectX::XMFLOAT3 inPos);

	/// <summary>
	/// 表示する2D座標を設定
	/// </summary>
	/// <param name="inPos">
    /// 設定する座標の値(DirectX::XMFLOAT2)
    /// </param>
	void SetPos(DirectX::XMFLOAT2 inPos);

	/// <summary>
	/// 表示するローカルサイズを設定
	/// </summary>
	/// <param name="inSize">
    /// 設定するサイズの値(DirectX::XMFLOAT3)
    /// </param>
	void SetSize(DirectX::XMFLOAT3 inSize);

	/// <summary>
	/// 表示する2Dサイズを設定
	/// </summary>
	/// <param name="inSize">
    /// 設定するサイズの値(DirectX::XMFLOAT2)
    /// </param>
	void SetSize(DirectX::XMFLOAT2 inSize);

	/// <summary>
	/// 表示するローカル回転を設定
	/// </summary>
	/// <param name="inRotation">
    /// 設定する回転の値(DirectX::XMFLOAT3)
    /// </param>
	void SetRotation(DirectX::XMFLOAT3 inRotation);

	/// <summary>
	/// 表示する2D回転を設定
	/// </summary>
	/// <param name="inRotation">
    /// 設定する回転の値(DirectX::XMFLOAT2)
    /// </param>
	void SetRotation(float inRotation);

	/// <summary>
	/// 表示する色を設定
	/// </summary>
	/// <param name="inColor">
    /// 設定する色の値(DirectX::XMFLOAT4)
    /// </param>
	void SetColor(DirectX::XMFLOAT4 inColor);

	/// <summary>
	/// UV座標の位置を設定
	/// </summary>
	/// <param name="inUVPos">
    /// 設定するUV座標の位置(DirectX::XMFLOAT2)
    /// </param>
	void SetUVPos(DirectX::XMFLOAT2 inUVPos);

	/// <summary>
	/// UV座標のサイズを設定
	/// </summary>
	/// <param name="inUVSize">
    /// 設定するUV座標のサイズ(DirectX::XMFLOAT2)
    /// </param>
	void SetUVSize(DirectX::XMFLOAT2 inUVSize);

	/// <summary>
	/// 表現技法の変更
	/// </summary>
	/// <param name="inCulling">
	/// カリングモード
	/// </param>
	/// <param name="・D3D11_CULL_NONE">
	/// カリングしない(表裏どちらも描画)
	/// </param>
	/// <param name="・D3D11_CULL_FRONT">
	/// 表面カリング(裏面のみ描画)
	/// </param>
	/// <param name="・D3D11_CULL_BACK">
	/// 裏面カリング(表面のみ描画)
	/// </param>
	void SetCullingMode(D3D11_CULL_MODE inCulling);

	/// <summary>
	/// 描画用のパラメータを取得
	/// </summary>
	/// <returns>
	/// 描画用パラメータをまとめた構造体
	/// </returns>
	RendererParam GetRendererParam();

    void LoadVertexShader(std::string inPath);
    void LoadPixelShader(std::string inPath);

    VertexShader* GetVS();
    PixelShader* GetPS();

protected:
	RendererParam m_tParam;	// レンダラーの統合パラメータ
    static std::map<std::string, VertexShader*> m_pVSList;
    static std::map<std::string, PixelShader*> m_pPSList;
    std::string m_sVSKey;
    std::string m_sPSKey;

};
