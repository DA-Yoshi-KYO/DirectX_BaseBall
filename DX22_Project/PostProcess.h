#pragma once

#include "Shader.h"
#include "Defines.h"
#include <DirectXMath.h>
#include "RendererComponent.h"

class CPostProcess
{
public:
    CPostProcess();
    virtual ~CPostProcess();

    virtual void Init() = 0;
    virtual void Write();
    virtual void ParameterSetting(bool isEnd = true);

    void Load(std::string inPath);

    bool GetIsActive() { return m_bIsActive; }
    PixelShader* GetPS() { return m_pPS; };
    std::string GetName() { return m_sName; }
    void SetOrderNo(int inNo) { m_nOrderNo = inNo; }
    int GetOrderNo() { return m_nOrderNo; }

protected:
    bool m_bIsActive; 
    PixelShader* m_pPS;
    std::string m_sName;
    int m_nOrderNo;

};
