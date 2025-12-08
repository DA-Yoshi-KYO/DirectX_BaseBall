#include "EffectBase.h"
#include"Shader.h"
#include"Oparation.h"
#include"2DPlayerBase.h"

#include"ColliderSquare.h"

void CEffectBase::Init()
{
	AddComponent<CShader>()->Load("shader\\unlitTextureVS.cso", "shader\\unlitTexturePS.cso");

	for (int i = 0; i < m_nMaxParticle; ++i)
	{
		m_tParticleParam[i].fPos = DirectX::XMFLOAT3(0.0f, -100.0f, 0.0f);
		m_tParticleParam[i].fSize = DirectX::XMFLOAT3(10.0f, 10.0f, 0.0f);
		m_tParticleParam[i].fColor = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_tParticleParam[i].fVelocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_tParticleParam[i].fCreatePos = m_tParticleParam[i].fPos;
		m_tParticleParam[i].fLifeTime = 10.0f;
		m_tParticleParam[i].fIntervalTime = 0.1f * i;
		m_tParticleParam[i].bUse = true;
	}

	m_eBlendState = BlendStateKind::AlphaBlend;

	m_fColliderSquareSize = DirectX::XMFLOAT3(10.0f, 10.0f, 0.0f);
	AddComponent<CColliderSquare>()->SetCenter(m_tParticleParam[0].fPos);
	GetComponent<CColliderSquare>()->SetHalfSize(m_fColliderSquareSize * 0.5f);
	GetComponent<CColliderSquare>()->SetTag("Effect");
}

void CEffectBase::Update()
{
	m_fTime += 1.0f / fFPS;

	//すべて終了してるか判定
	bool End = true;

	for (int i = 0; i < m_nMaxParticle; ++i)
	{
		if (!m_tParticleParam[i].bUse)continue;

		End = false;

		if (m_tParticleParam[i].fIntervalTime > m_fTime)continue;

		m_tParticleParam[i].fLifeTime -= 1.0f / fFPS;

		m_tParticleParam[i].fPos += m_tParticleParam[i].fVelocity;

		if (m_tParticleParam[i].fLifeTime < 0.0f)
		{
			m_tParticleParam[i].bUse = false;
		}
	}

	//すべて終了したら削除
	if (End)
	{
		Destroy();
	}

	GetComponent<CColliderSquare>()->SetCenter(m_tParticleParam[0].fPos);
}

void CEffectBase::OnColliderHit(CColliderBase* other)
{
	//すべて終了してるか判定
	bool End = true;

	if (other->GetTag() == "2DEnemy")
	{
		for (int i = 0; i < m_nMaxParticle; ++i)
		{
			if (!m_tParticleParam[i].bUse)continue;
			End = false;
			m_tParticleParam[i].bUse = false;

			//すべて終了したら削除
			if (End)
			{
				Destroy();
			}
		}
	}
}

float CEffectBase::RandomCalculation(int min, int max, unsigned int DecimalPlaces)
{
	if (min > max)
		return 0.0f;

	float fCalculation;

	if (DecimalPlaces != 1)
	{
		fCalculation = rand() % int(max - min) + min;

		float fCalcu = 0.0f;

		for (int i = 1; i < DecimalPlaces; ++i)
		{
			fCalcu = rand() % 10;

			fCalcu /= powf(10, i);

			fCalculation += fCalcu;
		}
	}
	else
	{
		fCalculation = rand() % int(max - min) + min;
	}

	return fCalculation;
}

void CEffectBase::SetPos(DirectX::XMFLOAT3 pos)
{
	for (int i = 0; i < m_nMaxParticle; ++i)
	{
		m_tParticleParam[i].fPos = pos;
		m_tParticleParam[i].fCreatePos = m_tParticleParam[i].fPos;
	}
}

void CEffectBase::SetVelocityX(float velocityX)
{
	for (int i = 0; i < m_nMaxParticle; ++i)
	{
		m_tParticleParam[i].fVelocity.x = velocityX;
	}
}

void CEffectBase::SetVelocityY(float velocityY)
{
	for (int i = 0; i < m_nMaxParticle; ++i)
	{
		m_tParticleParam[i].fVelocity.y = velocityY;
	}
}

void CEffectBase::SetVelocityZ(float velocityZ)
{
	for (int i = 0; i < m_nMaxParticle; ++i)
	{
		m_tParticleParam[i].fVelocity.z = velocityZ;
	}
}

void  CEffectBase::SetCalculationSize(float CalculationSize)
{
	for (int i = 0; i < m_nMaxParticle; ++i)
	{
		m_tParticleParam[i].fSize.x += m_fTime * CalculationSize;
		m_tParticleParam[i].fSize.y += m_fTime * CalculationSize;
		if (m_tParticleParam[i].fSize.x < 0.0f || m_tParticleParam[i].fSize.y < 0.0f)
		{
			m_tParticleParam[i].bUse = false;
		}
	}
}

void CEffectBase::SetColor(DirectX::XMFLOAT4 color)
{
	for (int i = 0; i < m_nMaxParticle; ++i)
	{
		m_tParticleParam[i].fColor = color;
	}
}

void CEffectBase::SetLifeTime(float life)
{
	for (int i = 0; i < m_nMaxParticle; ++i)
	{
		m_tParticleParam[i].fLifeTime = life;
	}
}
