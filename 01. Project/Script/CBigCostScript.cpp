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

	// Player의 스크립트를 가져와 저장해둔다.
	m_pPlayerScript = m_pUIMgrScript->GetGameMgrScript()->GetPlayerScript();

	RenderNumber();
}

void CBigCostScript::RenderNumber()
{
	// 체력의 수치를 자식이 지닌 Number Script에게 전달해준다.
	int Num0 = m_pPlayerScript->GetCurAct();
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"BigCostNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);

	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"BigCostNum_1")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(10);

	int Num2 = m_pPlayerScript->GetMaxAct();
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"BigCostNum_2")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num2);
}