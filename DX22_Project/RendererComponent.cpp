// インクルード部
#include "RendererComponent.h"

std::map<std::string, VertexShader*> CRendererComponent::m_pVSList = {};
std::map<std::string, PixelShader*> CRendererComponent::m_pPSList = {};

CRendererComponent::~CRendererComponent()
{

}

void CRendererComponent::UnloadShader()
{
    for (auto itr : m_pVSList)
    {
        if (itr.second)
        {
            delete itr.second;
            itr.second = nullptr;
        }
    }

    for (auto itr : m_pPSList)
    {
        if (itr.second)
        {
            delete itr.second;
            itr.second = nullptr;
        }
    }
}

void CRendererComponent::SetRendererParam(const RendererParam inParam)
{
	m_tParam = inParam;
}

void CRendererComponent::SetPos(const DirectX::XMFLOAT3 inPos)
{
	m_tParam.m_f3Pos = inPos;
}

void CRendererComponent::SetPos(DirectX::XMFLOAT2 inPos)
{
	m_tParam.m_f3Pos.x = inPos.x;
	m_tParam.m_f3Pos.y = inPos.y;
	m_tParam.m_f3Pos.z = 0.0f;
}

void CRendererComponent::SetSize(DirectX::XMFLOAT3 inSize)
{
	m_tParam.m_f3Size = inSize;
}

void CRendererComponent::SetSize(DirectX::XMFLOAT2 inSize)
{
	m_tParam.m_f3Size.x = inSize.x;
	m_tParam.m_f3Size.y = inSize.y;
	m_tParam.m_f3Size.z = 1.0f;
}

void CRendererComponent::SetRotation(DirectX::XMFLOAT3 inRotation)
{
	m_tParam.m_f3Rotate = inRotation;
}

void CRendererComponent::SetRotation(float inRotation)
{
	m_tParam.m_f3Rotate.x = 0.0f;
	m_tParam.m_f3Rotate.y = 0.0f;
	m_tParam.m_f3Rotate.z = inRotation;
}

void CRendererComponent::SetColor(DirectX::XMFLOAT4 inColor)
{
	m_tParam.m_f4Color = inColor;
}

void CRendererComponent::SetUVPos(DirectX::XMFLOAT2 inUVPos)
{
	m_tParam.m_f2UVPos = inUVPos;
}

void CRendererComponent::SetUVSize(DirectX::XMFLOAT2 inUVSize)
{
	m_tParam.m_f2UVSize = inUVSize;
}

void CRendererComponent::SetCulling(D3D11_CULL_MODE inCulling)
{
	m_tParam.m_eCulling = inCulling;
}

RendererParam CRendererComponent::GetRendererParam()
{
	return m_tParam;
}

void CRendererComponent::LoadVertexShader(std::string inPath)
{
    m_sVSKey = inPath;

    auto itr = m_pVSList.find(m_sVSKey);
    if (itr != m_pVSList.end())
    {
        return;
    }

    VertexShader* pVS = new VertexShader();
    if (FAILED(pVS->Load(m_sVSKey.c_str())))
    {
        MessageBox(NULL, m_sVSKey.c_str(), "Load failed.", MB_OK);
    }

    m_pVSList[m_sVSKey] = pVS;
}

void CRendererComponent::LoadPixelShader(std::string inPath)
{
    m_sPSKey = inPath;

    auto itr = m_pPSList.find(m_sPSKey);
    if (itr != m_pPSList.end())
    {
        return;
    }

    PixelShader* pPS = new PixelShader();
    if (FAILED(pPS->Load(m_sPSKey.c_str())))
    {
        MessageBox(NULL, m_sPSKey.c_str(), "Load failed.", MB_OK);
    }

    m_pPSList[m_sPSKey] = pPS;
}

VertexShader* CRendererComponent::GetVS()
{
    return m_pVSList[m_sVSKey];
}

PixelShader* CRendererComponent::GetPS()
{
    return m_pPSList[m_sVSKey];
}
