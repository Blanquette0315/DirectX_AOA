#include "pch.h"
#include "CCardAttNumScript.h"

#include "CNumberScript.h"
#include "CCardScript.h"

CCardAttNumScript::CCardAttNumScript()
	: CUIScript(CARDATTNUMSCRIPT)
	, m_iAttack(0)
{
}

CCardAttNumScript::~CCardAttNumScript()
{
}

void CCardAttNumScript::begin()
{
}

void CCardAttNumScript::tick()
{
	CCardScript* CardScript = GetCardScript(GetOwner()->GetParent());

	m_iAttack = CardScript->GetCardState().Attack;

	RenderNumber();
}

void CCardAttNumScript::RenderNumber()
{
	int Num0 = m_iAttack / 10;
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"AttNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);

	int Num1 = m_iAttack - (Num0 * 10);
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"AttNum_1")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num1);
}