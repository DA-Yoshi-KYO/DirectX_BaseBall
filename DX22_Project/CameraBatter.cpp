#include "CameraBatter.h"
#include "Defines.h"

CCameraBatter::CCameraBatter()
{
}

CCameraBatter::~CCameraBatter()
{
}

void CCameraBatter::Update()
{
	m_pos = { WORLD_AJUST,WORLD_AJUST + 10.0f, WORLD_AJUST + 170.0f };
	m_look = { WORLD_AJUST,WORLD_AJUST + 0.0f, WORLD_AJUST - 10.0f };
}
