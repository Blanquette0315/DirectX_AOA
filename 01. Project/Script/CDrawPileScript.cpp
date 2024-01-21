#include "pch.h"
#include "CDrawPileScript.h"

#include "CGameMgrScript.h"
#include "CUIMgrScript.h"
#include "CCardMgrScript.h"

#include "CNumberScript.h"

CDrawPileScript::CDrawPileScript()
	: CUIScript(DRAWPILESCRIPT)
	, m_iDeckSize(0)
{
}

CDrawPileScript::~CDrawPileScript()
{
}

void CDrawPileScript::begin()
{
	CUIScript::begin();
}

void CDrawPileScript::tick()
{
	CUIScript::tick();
	m_iDeckSize = m_pUIMgrScript->GetGameMgrScript()->GetCardMgrScript()->GetDrawPileSize();

	RenderNumber();
}


void CDrawPileScript::RenderNumber()
{
	// 덱에 남은 카드의 수를 출력해준다.
	int Num0 = m_iDeckSize / 10;

	// 10의 자리의 수는 0일 경우 출력하지 않기 위해
	if (Num0 == 0)
	{
		Num0 = -1;
	}
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"DrawPileNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);

	int Num1 = m_iDeckSize % 10;
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"DrawPileNum_1")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num1);
}