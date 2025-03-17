#pragma once

#include "Defines.h"
#include "Texture.h"
#include "StrikeZone.h"

constexpr DirectX::XMFLOAT2 ce_fCursorPos = { 0.0f,-100.0f };

class CCursor
{
public:
	CCursor();
	~CCursor();
	void Update(int play);
	void Draw(int play);
	void SetStrikeZone(CStrikeZone* zone);
	DirectX::XMFLOAT2 GetPos(int play);
	DirectX::XMFLOAT2 GetSize(int play);
	void SetPos(int play,DirectX::XMFLOAT2);
	void SetMove(int play, bool isMove);
private:
	std::unique_ptr<Texture> m_pTexture[2];
	std::unique_ptr<CStrikeZone> m_pStrikeZone;
	SpriteParam m_tParam[2];

	bool m_bMove[2];
};
