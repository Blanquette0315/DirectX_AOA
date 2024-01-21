#include "pch.h"
#include "CCardCostNumScript.h"

#include "CNumberScript.h"
#include "CCardScript.h"

CCardCostNumScript::CCardCostNumScript()
	: CUIScript(CARDCOSTNUMSCRIPT)
	, m_iCost(0)
{
}

CCardCostNumScript::~CCardCostNumScript()
{
}

void CCardCostNumScript::begin()
{
}

void CCardCostNumScript::tick()
{
	CCardScript* CardScript = GetCardScript(GetOwner()->GetParent());
	
	m_iCost = CardScript->GetCardState().Cost;

	RenderNumber();
}

void CCardCostNumScript::RenderNumber()
{
	int Num0 = m_iCost;
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"CostNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);
}