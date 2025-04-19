#include "EffectGoal.h"
#include "Defines.h"

// エミッター
class CEffectGoalEmitter : public Emitter
{
public:
	CEffectGoalEmitter()
		// 画像のパス,生成間隔(秒),生存時間(秒)
		: Emitter(PATH_TEX("Star.png"), {10, 0.1f, 1.0f})
	{

	}
	
	virtual ~CEffectGoalEmitter() override {}
	
protected:
	void Spawn(Particle* particle, const DirectX::XMFLOAT3& rootPos) override 
	{
		// パーティクルの生存時間
		particle->life = (rand() % 10) * 0.1f + 1.5f;
		
		// パーティクルを出す位置
		particle->pos.value = rootPos;
		// パーティクルの移動先とスピード
		particle->pos.add = DirectX::XMFLOAT3(
			(rand() % 21 - 10) * 0.1f,
			(rand() % 21 - 10) * 0.1f,
			(rand() % 21 - 10) * 0.1f
		);
		
		// 時間経過でだんだんゆっくりにする
		particle->pos.accele = DirectX::XMFLOAT3(0.97f,0.97f,0.97f);
		// パーティクルのサイズ
		particle->size.value = DirectX::XMFLOAT3(
			(rand() % 11) * 0.1f + 0.1f,
			(rand() % 11) * 0.1f + 0.1f, 
			(rand() % 11) * 0.1f + 0.1f
		);
		// パーティクルの透明度を徐々に下げる
		particle->alpha.add = DirectX::XMFLOAT3(-0.005f,0.0f,0.0f);
	}
};

// エフェクト
CEffectGoal::CEffectGoal()
	: m_pCamera(nullptr)
{
	Emitter* pEmitter = new CEffectGoalEmitter();
	AddEmitter(pEmitter);
}

CEffectGoal::~CEffectGoal()
{

}

void CEffectGoal::Draw()
{
	// カメラがなかったら処理をしない
	if (!m_pCamera) { return; }

	// ビルボード行列の計算
	DirectX::XMFLOAT4X4 billboard = m_pCamera->GetViewMatrix(false);
	billboard._41 = billboard._42 = billboard._43 = 0.0f;
	DirectX::XMMATRIX mInvView = DirectX::XMLoadFloat4x4(&billboard);
	mInvView = DirectX::XMMatrixInverse(nullptr,mInvView);
	DirectX::XMStoreFloat4x4(&billboard,mInvView);

	// 基底クラスの描画関数を呼び出し
	DirectX::XMFLOAT4X4 view = m_pCamera->GetViewMatrix();
	DirectX::XMFLOAT4X4 proj = m_pCamera->GetProjectionMatrix();
	SetBillboardMatrix(billboard);
	Effect::Draw(view, proj);
}

void CEffectGoal::SetCamera(CCamera* pCamera)
{
	m_pCamera = pCamera;
}
