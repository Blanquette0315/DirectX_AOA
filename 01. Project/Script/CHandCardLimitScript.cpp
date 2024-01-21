#include "pch.h"
#include "CHandCardLimitScript.h"

#include "CGameMgrScript.h"
#include "CUIMgrScript.h"
#include "CCardMgrScript.h"
#include "CPlayerScript.h"

#include "CNumberScript.h"

CHandCardLimitScript::CHandCardLimitScript()
	: CUIScript(HANDCARDLIMITSCRIPT)
{
}

CHandCardLimitScript::~CHandCardLimitScript()
{
}

void CHandCardLimitScript::begin()
{
	CUIScript::begin();
}

void CHandCardLimitScript::tick()
{
	CUIScript::tick();

	RenderNumber();
}

void CHandCardLimitScript::RenderNumber()
{
	int Num0 = m_pUIMgrScript->GetGameMgrScript()->GetCardMgrScript()->GetHandSize();
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"HandCardLimitNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);

	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"HandCardLimitNum_1")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(10);

	int Num2 = m_pUIMgrScript->GetGameMgrScript()->GetPlayerScript()->GetDrowCount();
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"HandCardLimitNum_2")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num2 + 1);
}