#include "FieldDirecter.h"
#include "Main.h"
#include "Ball.h"
#include "Field.h"
#include "StrikeZone.h"
#include "Base.h"

CFieldDirecter::CFieldDirecter()
{
}

CFieldDirecter::~CFieldDirecter()
{
}

void CFieldDirecter::Init()
{
	CScene* pScene = GetScene();

	pScene->AddGameObject<CField>("Field", Tag::Field);
	pScene->AddGameObject<CBall>("Ball", Tag::GameObject);
	pScene->AddGameObject<CStrikeZone>("StrikeZone", Tag::UI);

	for (int i = 0; i < (int)BaseKind::Max; i++)
	{
		pScene->AddGameObject<CBase>("Base", Tag::GameObject)->Init(BaseKind(i));
	}

}

void CFieldDirecter::EndInplay()
{
}
