#include "SceneMemberselect.h"
#include "Main.h"

CSceneMemberselect::CSceneMemberselect(CTeamManager::Teams player1, CTeamManager::Teams player2)
{
	if(!CTeamManager::GetInstance(Player::One)->Load(player1)) SetEnd(true);
	if(!CTeamManager::GetInstance(Player::Two)->Load(player2)) SetEnd(true);
}

CSceneMemberselect::~CSceneMemberselect()
{

}

void CSceneMemberselect::Update()
{

}

void CSceneMemberselect::Draw()
{

}
