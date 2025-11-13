// ==============================
//    インクルード部
// ==============================
#pragma once
#include "Defines.h"
#include "Texture.h"

// ==============================
//    定数定義
// ==============================
#define MAX_BALL_COUNT 4	// ボールのカウントの最大数
#define MAX_STRIKE_COUNT 3	// ストライクのカウントの最大数
#define MAX_OUT_COUNT 3		// アウトのカウントの最大数
#define MAX_BASE_COUNT 4	// ベースの数の最大数
#define MAX_SCORE 99		// スコアの最大値
#define MAX_INNING 12		// イニングの最大数

class CGameManager
{
private:
	CGameManager();
	
public:
	~CGameManager();
	void Init();
	void Update();
	void Draw();

public:


	// オモテ・ウラ
	enum class Team
	{
		Player1,
		Player2
	};


	// インプレー修了条件要素
	enum class InplayElement
	{
		HoldBall,
		Running,
		Max
	};
	bool m_bInplay[(int)InplayElement::Max]; // インプレー修了条件

	/// <summary> SetEndInplay:Inplay状態を終了していいかをセットする </summary>
	/// <param name="ElemEndInplay:"> Inplay状態を継続している要素 </param>
	/// <param name="state:"> true:Inplay中,false:終了 </param>
	void SetEndInplay(InplayElement ElemEndInplay,bool state);
	/// <summary> SetEndInplay:Inplay状態を終了していいかを取得する </summary>
	/// <returns> true:Inplay終了,false:Inplay中 </returns>
	bool GetEndInplay();

	/// <summary> シングルトンインスタンスの取得 </summary>
	static CGameManager* GetInstance();

private:
	static CGameManager* m_pInstance;

private:
};