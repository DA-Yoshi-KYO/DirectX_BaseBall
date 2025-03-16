#pragma once

#include "imgui.h"
#include <DirectXMath.h>
#include <string>

//#define _IMGUI

void SpriteParamDebug(DirectX::XMFLOAT2* pos, DirectX::XMFLOAT2* size, std::string name, DirectX::XMFLOAT2 guipos = { 20.0f,20.0f });
void ValueDebug(int* value,std::string name, DirectX::XMFLOAT2 guipos = { 20.0f,20.0f });
void BoolDebug(bool* value, std::string name, DirectX::XMFLOAT2 guipos = { 20.0f,20.0f });