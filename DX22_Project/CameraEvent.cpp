#include "CameraEvent.h"
#include "Defines.h"

CCameraEvent::CCameraEvent()
	:m_start(0.0f, 0.0f, 0.0f), m_end(0.0f, 0.0f, 0.0f), m_time(0.0f), m_totalTime(0.0f) 
{

}

CCameraEvent::~CCameraEvent()
{
}

void CCameraEvent::Update()
{
	if (!IsEvent()) { return; }


	float param = m_time / m_totalTime;
	param = pow(param, 3);

	m_look.x = m_start.x * (1 - param) + m_end.x * param + WORLD_AJUST;
	m_look.y = m_start.y * (1 - param) + m_end.y * param + WORLD_AJUST;
	m_look.z = m_start.z * (1 - param) + m_end.z * param + WORLD_AJUST;

	m_pos.x = m_look.x + 0.0f; 
	m_pos.y = m_look.y + 1.0f; 
	m_pos.z = m_look.z + 1.5f;
	m_time += 1.0f / 60.0f;
}

void CCameraEvent::SetEvent(DirectX::XMFLOAT3 startPos, DirectX::XMFLOAT3 endPos, float time)
{
	m_start = startPos;
	m_end = endPos;
	m_time = 0.0f;
	m_totalTime = time;
}

bool CCameraEvent::IsEvent()
{
	return m_time < m_totalTime;
}