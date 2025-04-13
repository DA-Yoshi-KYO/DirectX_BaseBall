#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <assert.h>
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <DirectXMath.h>
#include <memory>

// defines

static const int FPS = 60;
static const float fFPS = static_cast<float>(FPS);

// リソースパス
#define PATH_MODEL(fbx) ("Assets/Model/" fbx)
#define PATH_TEX(png) ("Assets/Texture/" png)
#define PATH_FONT(ttf) (L"Assets\\Font\\" ttf)
#define PATH_DATA(data) (L"Assets/Data/" data)
#define ERROR_MESSAGE(asset) do{ MessageBox(NULL,asset,"LoadError",MB_OK); } while (0)
#define INFO_MESSAGE(information) do{ MessageBox(NULL,information,"Information",MB_OK); } while (0)
#define RANGEERROR_MESSAGE(value) do{ MessageBox(NULL,value,"Out of range",MB_OK); } while (0)

constexpr int ce_nSheetSplit = 5;

// 3D空間定義
#define CMETER(value) (value * 0.01f)
#define METER(value) (value * 1.0f)
#define KMETER(value) (value * 1000.0f)
#define MSEC(value) (value / fFPS)
#define CMSEC(value) MSEC(CMETER(value))
#define WORLD_AJUST 10000.0f
static const float GRAVITY = 0.70f;

// タイトル
static const char* APP_TITLE = "DXBaseBall";

// 画面サイズ
static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;

// グリッドサイズ(デバッグ用
static const int	DEBUG_GRID_NUM = 10;			// グリッド中心から端までの線の本数
static const float	DEBUG_GRID_MARGIN = METER(1.0f);	// グリッド配置幅

struct SpriteParam
{
	DirectX::XMFLOAT2 pos = {0.0f,0.0f};
	DirectX::XMFLOAT2 offsetPos = {0.0f,0.0f};
	DirectX::XMFLOAT2 size = {100.0f,100.0f};
	float rotate = 0.0f;
	DirectX::XMFLOAT4 color = {1.0f,1.0f,1.0f,1.0f};
	DirectX::XMFLOAT2 uvPos = { 0.0f,0.0f };
	DirectX::XMFLOAT2 uvSize = {1.0f,1.0f};
	DirectX::XMFLOAT4X4 world = {};
	DirectX::XMFLOAT4X4 view = {};
	DirectX::XMFLOAT4X4 proj = {};
};

struct ModelParam
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 size;
	DirectX::XMFLOAT3 rotate;
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProj;
	DirectX::XMFLOAT4X4 wvp[3];
};

#endif // __DEFINES_H__