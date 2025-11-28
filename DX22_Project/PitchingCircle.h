#pragma once

#include "GameObject.h"
constexpr DirectX::XMFLOAT3 ce_fPitchingCircleFirstSize = { 300.0f,300.0f,0.0f };	// ピッチングサークルの最初のサイズ
constexpr DirectX::XMFLOAT2 ce_fPitchingCircleEndSize = { 20.0f,20.0f };		// ピッチングサークルの最後のサイズ
class CPitchingCircle : public CGameObject
{
public:
	CPitchingCircle();
	~CPitchingCircle();
	void Init() override;
	void Update() override;
	void Draw() override;
	void ResetInitSize();
	void StartScaleDown();
	void Pitched();
	void SetFuturePos(DirectX::XMFLOAT3 pos) { m_f3Future = pos; }

private:
	DirectX::XMFLOAT3 m_f3Future;
	bool m_bScaleDown;

};
