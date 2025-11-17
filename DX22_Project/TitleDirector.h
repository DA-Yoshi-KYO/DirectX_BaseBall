#pragma once

#include <array>

class CTitleDirector
{
public:
	CTitleDirector();
	~CTitleDirector();
	void Init();
	void Update();

private:
	class CTitleBackGround* m_pBackGround;
	class CTitleBall* m_pBall;
	std::array<class CTitleBat*, 2> m_pBat;
	class CTitleStartButton* m_pStartButton;
	class CTitleEndButton* m_pEndButton;
	class CTitleSelectCursor* m_pSelectCursor;
	class CTitleLogo* m_pLogo;

	enum TitlePhase
	{
		Animation,
		Select
	}m_ePhase;

	void UpdateAnimation();
	void UpdateSelect();

};
