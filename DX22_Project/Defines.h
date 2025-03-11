#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <assert.h>
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <DirectXMath.h>

using namespace std;

// defines
#define MAP_X (19)	// txtファイルのマップ横幅
#define MAP_Y (19)	// txtファイルのマップ縦幅

static const int FPS = 60;
static const float fFPS = static_cast<float>(FPS);
// リソースパス
#define ASSET(path)	"Assets/"path
#define PASS(fbx) ("Assets/Model/KayKitMini/" fbx)
#define LOWPASS(fbx) ("Assets/Model/LowPolyNature/" fbx)
#define TXTPASS(txt) ("Assets/txt/" txt)
#define TEXPASS(png) ("Assets/Texture/" png)

// 3D空間定義
#define CMETER(value) (value * 0.01f)
#define METER(value) (value * 1.0f)
#define MSEC(value) (value / fFPS)
#define CMSEC(value) MSEC(CMETER(value))
#define WORLD_AJUST 10000.0f
static const float GRAVITY = 0.98f;

// タイトル
static const char* APP_TITLE = "DX22_Golf";

// 画面サイズ
static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;

// グリッドサイズ(デバッグ用
static const int	DEBUG_GRID_NUM = 10;			// グリッド中心から端までの線の本数
static const float	DEBUG_GRID_MARGIN = METER(1.0f);	// グリッド配置幅

struct SpriteParam
{
	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 size;
	float rotate;
	DirectX::XMFLOAT4 color;
	DirectX::XMFLOAT2 uvPos;
	DirectX::XMFLOAT2 uvSize;
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProj;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 proj;
};

#endif // __DEFINES_H__