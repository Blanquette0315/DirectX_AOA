#include "pch.h"
#include "CDiscardPileScript.h"

#include "CGameMgrScript.h"
#include "CUIMgrScript.h"
#include "CCardMgrScript.h"

#include "CNumberScript.h"

CDiscardPileScript::CDiscardPileScript()
	: CUIScript(DISCARDPILESCRIPT)
	, m_iDiscardSize(0)
{
}

CDiscardPileScript::~CDiscardPileScript()
{
}

void CDiscardPileScript::begin()
{
	CUIScript::begin();
}

void CDiscardPileScript::tick()
{
	CUIScript::tick();

	m_iDiscardSize = m_pUIMgrScript->GetGameMgrScript()->GetCardMgrScript()->GetDiscardSize();

	RenderNumber();
}

void CDiscardPileScript::RenderNumber()
{
	// 묘지의 카드의 수를 출력해준다.
	int Num0 = m_iDiscardSize / 10;

	// 10의 자리의 수는 0일 경우 출력하지 않기 위해
	if (Num0 == 0)
	{
		Num0 = -1;
	}
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"DiscardPileNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);

	int Num1 = m_iDiscardSize % 10;
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"DiscardPileNum_1")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num1);
}