#pragma once

#include "Defines.h"
#include "Cursor.h"

class CBatting
{
public:
	CBatting();
	~CBatting();
	void Update();
	void Draw();

	void SetCursor(CCursor* cursor);
private:
	std::unique_ptr<CCursor> m_pCursor;
};
