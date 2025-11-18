#include "MemberSelectDirector.h"
#include "Main.h"
#include "MemberSelectBackGround.h"

CMemberSelectDirector::CMemberSelectDirector()
{

}

CMemberSelectDirector::~CMemberSelectDirector()
{

}

void CMemberSelectDirector::Init()
{
	CScene* pScene = GetScene();

	pScene->AddGameObject<CMemberSelectBackGround>("BackGround", Tag::UI);
}
