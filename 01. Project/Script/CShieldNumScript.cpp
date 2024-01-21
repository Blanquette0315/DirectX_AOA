#include "pch.h"
#include "CShieldNumScript.h"

#include "CMonsterScript.h"
#include "CNumberScript.h"

CShieldNumScript::CShieldNumScript()
	: CUIScript(SHIELDNUMSCRIPT)
	, m_iCurDef(0)
{
}

CShieldNumScript::~CShieldNumScript()
{
}

void CShieldNumScript::begin()
{
}

void CShieldNumScript::tick()
{
	CMonsterScript* MonScript = GetMonsterScript(GetOwner()->GetParent()->GetParent());

	// Monster의 스크립트를 가져와 체력을 확인한다.
	m_iCurDef = MonScript->GetMonCurDef();

	RenderNumber();
}

void CShieldNumScript::RenderNumber()
{
	int Num0 = m_iCurDef / 10;
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"DefNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);

	int Num1 = m_iCurDef - (Num0 * 10);
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"DefNum_1")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num1);
}