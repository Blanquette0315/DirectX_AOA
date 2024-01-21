#include "pch.h"
#include "CExhasusPileScript.h"

#include "CGameMgrScript.h"
#include "CUIMgrScript.h"
#include "CCardMgrScript.h"

#include "CNumberScript.h"

CExhasusPileScript::CExhasusPileScript()
	: CUIScript(EXHASUSPILESCRIPT)
	, m_iExhasusSize(0)
{
}

CExhasusPileScript::~CExhasusPileScript()
{
}

void CExhasusPileScript::begin()
{
	CUIScript::begin();
}

void CExhasusPileScript::tick()
{
	CUIScript::tick();

	m_iExhasusSize = m_pUIMgrScript->GetGameMgrScript()->GetCardMgrScript()->GetExhust();

	RenderNumber();
}

void CExhasusPileScript::RenderNumber()
{
	// 소멸 묘지에 남은 카드의 수를 출력해준다.
	int Num0 = m_iExhasusSize / 10;

	// 10의 자리의 수는 0일 경우 출력하지 않기 위해
	if (Num0 == 0)
	{
		Num0 = -1;
	}
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"ExhustPileNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);

	int Num1 = m_iExhasusSize % 10;
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"ExhustPileNum_1")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num1);
}