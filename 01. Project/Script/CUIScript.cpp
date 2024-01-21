#include "pch.h"
#include "CUIScript.h"

#include <Engine/CKeyMgr.h>

#include "CGameMgrScript.h"
#include "CUIMgrScript.h"
#include "CMouseCheckScript.h"

CUIScript::CUIScript(int _Script)
	: CScript(_Script)
	, m_bMouseOn(false)
	, m_bLbtnPressed(false)
	, m_pMouseChScript(nullptr)
	, m_pUIMgrScript(nullptr)
{
}

CUIScript::~CUIScript()
{
}

void CUIScript::begin()
{
	// 본인의 마우스 체크 스크립트의 주소를 저장해둔다.
	m_pMouseChScript = (CMouseCheckScript*)(GetOwner()->GetScript((UINT)MOUSECHECKSCRIPT));
	
	// UIMgrScript의 주소를 저장해둔다.
	m_pUIMgrScript = (CUIMgrScript*)GetOwner()->GetParent()->GetScript((UINT)UIMGRSCRIPT);
}

void CUIScript::tick()
{
	// 마우스 체크
	MouseCheck();
}

void CUIScript::SaveToFile(FILE* _File)
{
}

void CUIScript::LoadFromFile(FILE* _File)
{
}

void CUIScript::MouseCheck()
{
	// 마우스 체크
	if (m_pMouseChScript->Is_MouseOverlap()
		&& !m_bLbtnPressed)
	{
		m_bMouseOn = true;
		// 마우스가 본인의 위에 있고, 좌클릭으로 선택이 되었을 때,
		if (KEY_TAP(KEY::LBTN))
		{
			m_bLbtnPressed = true;
			// 해당 변수의 false로 해제는 각각 할일을 마치고나서 해제해주어야 한다.
		}
	}
	else
	{
		m_bMouseOn = false;
	}
}