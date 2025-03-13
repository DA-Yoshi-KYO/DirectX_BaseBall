#pragma once

#include <DirectXMath.h>
#include <fstream>
#include <string>
#include <iostream>
#include "Defines.h"
#include "Model.h"
#include "Geometory.h"
#include "ShaderList.h"
#include "Camera.h"
#include "Collision.h"

class CGameObject
{
public:
	/*---------------四大処理---------------*/
	CGameObject();				// 初期化処理
	virtual ~CGameObject();		// 更新処理
	virtual void Update() = 0;	// 更新処理
	virtual void Draw() = 0;	// 描画処理
	virtual void SetModel(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 size, DirectX::XMFLOAT3 rotate, int ModelType = 0);	// モデルのセット＆描画
	virtual void OnCollision(Collision::Result collision);
	virtual Collision::Info GetCollision();
public:
	/*----------------getter---------------*/
	DirectX::XMFLOAT3 GetPos() { return m_pos; }
	DirectX::XMFLOAT3 GetSize() { return m_size; }
	DirectX::XMFLOAT3 GetRotation() { return m_rotate; }
protected:
	/*---------------行列要素---------------*/
	DirectX::XMFLOAT3 m_pos;
	DirectX::XMFLOAT3 m_size;
	DirectX::XMFLOAT3 m_rotate;
	/*--------------当たり判定--------------*/
	Collision::Info m_Collision;
};