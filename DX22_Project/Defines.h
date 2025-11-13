#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <assert.h>
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <DirectXMath.h>
#include <memory>
#include <vector>

// defines

static const int FPS = 60;
static const float fFPS = static_cast<float>(FPS);

// リソースパス
#define PATH_MODEL(fbx) ("Assets/Model/" fbx)
#define PATH_TEX(png) ("Assets/Texture/" png)
#define PATH_SHADER(png) ("Assets/Shader/" png)
#define PATH_BGM(mp3) ("Assets/Sound/BGM" mp3)
#define PATH_SE(mp3) ("Assets/Sound/SE" mp3)
#define PATH_FONT(ttf) (L"Assets\\Font\\" ttf)
#define PATH_DATA(data) (std::wstring(L"Assets/Data/") + (data))
#define ERROR_MESSAGE(asset) do{ MessageBox(NULL,asset,"LoadError",MB_OK); } while (0)
#define INFO_MESSAGE(information) do{ MessageBox(NULL,information,"Information",MB_OK); } while (0)
#define RANGEERROR_MESSAGE(value) do{ MessageBox(NULL,value,"Out of range",MB_OK); } while (0)



// 3D空間定義
#define CMETER(value) (value * 0.01f)
#define METER(value) (value * 1.0f)
#define KMETER(value) (value * 1000.0f)
#define MSEC(value) (value / fFPS)
#define CMSEC(value) MSEC(CMETER(value))
#define WORLD_AJUST 0.0f
static const float GRAVITY = 0.70f;

// タイトル
static const char* APP_TITLE = "DXBaseBall";

// 画面サイズ
static const int SCREEN_WIDTH = 1280;
static const int SCREEN_HEIGHT = 720;

// グリッドサイズ(デバッグ用
static const int	DEBUG_GRID_NUM = 10;			// グリッド中心から端までの線の本数
static const float	DEBUG_GRID_MARGIN = METER(1.0f);	// グリッド配置幅

enum Player
{
	One,
	Two,

	MaxPlayer
};

#endif // __DEFINES_H__