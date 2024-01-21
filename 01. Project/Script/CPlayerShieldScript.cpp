#include "pch.h"
#include "CPlayerShieldScript.h"

#include "CPlayerScript.h"
#include "CNumberScript.h"

CPlayerShieldScript::CPlayerShieldScript()
	: CUIScript(PLAYERSHIELDSCRIPT)
	, m_iCurDef(0)
{
}

CPlayerShieldScript::~CPlayerShieldScript()
{
}

void CPlayerShieldScript::begin()
{
}

void CPlayerShieldScript::tick()
{
	CPlayerScript* PlayerScript = (CPlayerScript*)GetOwner()->GetParent()->GetParent()->GetScript((UINT)PLAYERSCRIPT);

	m_iCurDef = PlayerScript->GetCurDef();

	RenderNumber();
}

void CPlayerShieldScript::RenderNumber()
{
	int Num0 = m_iCurDef / 10;
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"DefNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);

	int Num1 = m_iCurDef - (Num0 * 10);
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"DefNum_1")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num1);
}