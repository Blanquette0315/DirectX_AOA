#include "pch.h"
#include "CDrawHandCntScript.h"

#include "CGameMgrScript.h"
#include "CUIMgrScript.h"
#include "CPlayerScript.h"

#include "CNumberScript.h"

CDrawHandCntScript::CDrawHandCntScript()
	: CUIScript(DRAWHANDCNTSCRIPT)
{
}

CDrawHandCntScript::~CDrawHandCntScript()
{
}

void CDrawHandCntScript::begin()
{
	CUIScript::begin();
}

void CDrawHandCntScript::tick()
{
	CUIScript::tick();

	RenderNumber();
}

void CDrawHandCntScript::RenderNumber()
{
	int Num0 = m_pUIMgrScript->GetGameMgrScript()->GetPlayerScript()->GetDrowCount();
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"DrawHandCntNum_0")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num0);
}