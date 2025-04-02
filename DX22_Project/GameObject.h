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

class CGameObject
{
public:
	/*---------------四大処理---------------*/
	CGameObject();				// 初期化処理
	virtual ~CGameObject();		// 更新処理
	virtual void Update() = 0;	// 更新処理
	virtual void Draw() = 0;	// 描画処理
	virtual void SetModel(ModelParam param, Model* model, bool isAnime = false) = 0;	// モデルのセット＆描画
protected:
};