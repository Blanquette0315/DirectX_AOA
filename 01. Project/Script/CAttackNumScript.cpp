#include "pch.h"
#include "CAttackNumScript.h"

#include "CNumberScript.h"
#include "CMonsterScript.h"

CAttackNumScript::CAttackNumScript()
	: CUIScript(ATTACKNUMSCRIPT)
	, m_iAttack(0)
{
}

CAttackNumScript::~CAttackNumScript()
{
}

void CAttackNumScript::begin()
{
}

void CAttackNumScript::tick()
{
	CMonsterScript* MonScript = GetMonsterScript(GetOwner()->GetParent()->GetParent());

	// Monster�� ��ũ��Ʈ�� ������ ü���� Ȯ���Ѵ�.
	m_iAttack = MonScript->GetMonAtt();

	RenderNumber();
}

void CAttackNumScript::RenderNumber()
{
	int Num0 = m_iAttack / 10;
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"ATTNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);

	int Num1 = m_iAttack - (Num0 * 10);
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"ATTNum_1")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num1);
}