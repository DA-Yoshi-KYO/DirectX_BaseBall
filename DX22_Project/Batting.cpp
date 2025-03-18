#include "Batting.h"

CBatting::CBatting()
	:  m_pCursor(nullptr)
{
}

CBatting::~CBatting()
{
	m_pCursor.release();
}

void CBatting::Update()
{
}

void CBatting::Draw()
{
}

void CBatting::SetCursor(CCursor* cursor)
{
	m_pCursor.reset(cursor);
}
