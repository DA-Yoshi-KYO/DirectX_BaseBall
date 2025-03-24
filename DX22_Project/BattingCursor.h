#pragma once

#include "Defines.h"
#include "Texture.h"
#include "StrikeZone.h"
#include "Collision.h"

constexpr DirectX::XMFLOAT2 ce_fBattingCursorPos = { 0.0f,-150.0f };

class CBattingCursor
{
public:
	CBattingCursor();
	~CBattingCursor();
	void Update();
	void Draw();
	void SetStrikeZone(CStrikeZone* zone);
	DirectX::XMFLOAT2 GetPos();
	DirectX::XMFLOAT2 GetSize();
	void SetPos(DirectX::XMFLOAT2);
	void SetMove(bool isMove);
	static Collision::Info2D GetCollision(bool isCursorOnry, Collision::Type2D type = Collision::Type2D::eCircle);
private:
	std::unique_ptr<Texture> m_pTexture;
	std::unique_ptr<CStrikeZone> m_pStrikeZone;
	SpriteParam m_tParam;
	bool m_bMove;
	static Collision::Info2D m_Collision;
};
