#include "pch.h"
#include "CCardDefNumScript.h"

#include "CNumberScript.h"
#include "CCardScript.h"

CCardDefNumScript::CCardDefNumScript()
	: CUIScript(CARDDEFNUMSCRIPT)
	, m_iDef(0)
{
}

CCardDefNumScript::~CCardDefNumScript()
{
}

void CCardDefNumScript::begin()
{
}

void CCardDefNumScript::tick()
{
	CCardScript* CardScript = GetCardScript(GetOwner()->GetParent());

	m_iDef = CardScript->GetCardState().Deffence;

	RenderNumber();
}

void CCardDefNumScript::RenderNumber()
{
	int Num0 = m_iDef / 10;
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"DefNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);

	int Num1 = m_iDef - (Num0 * 10);
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"DefNum_1")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num1);
}