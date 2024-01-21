#include "pch.h"
#include "CBigCostScript.h"

#include "CGameMgrScript.h"
#include "CUIMgrScript.h"

#include "CPlayerScript.h"
#include "CNumberScript.h"

CBigCostScript::CBigCostScript()
	: CUIScript(BIGCOSTSCRIPT)
	, m_pPlayerScript(nullptr)
{
}

CBigCostScript::~CBigCostScript()
{
}

void CBigCostScript::begin()
{
	CUIScript::begin();
}

void CBigCostScript::tick()
{
	CUIScript::tick();

	// Player�� ��ũ��Ʈ�� ������ �����صд�.
	m_pPlayerScript = m_pUIMgrScript->GetGameMgrScript()->GetPlayerScript();

	RenderNumber();
}

void CBigCostScript::RenderNumber()
{
	// ü���� ��ġ�� �ڽ��� ���� Number Script���� �������ش�.
	int Num0 = m_pPlayerScript->GetCurAct();
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"BigCostNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);

	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"BigCostNum_1")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(10);

	int Num2 = m_pPlayerScript->GetMaxAct();
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"BigCostNum_2")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num2);
}