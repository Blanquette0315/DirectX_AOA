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
	// ������ ���콺 üũ ��ũ��Ʈ�� �ּҸ� �����صд�.
	m_pMouseChScript = (CMouseCheckScript*)(GetOwner()->GetScript((UINT)MOUSECHECKSCRIPT));
	
	// UIMgrScript�� �ּҸ� �����صд�.
	m_pUIMgrScript = (CUIMgrScript*)GetOwner()->GetParent()->GetScript((UINT)UIMGRSCRIPT);
}

void CUIScript::tick()
{
	// ���콺 üũ
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
	// ���콺 üũ
	if (m_pMouseChScript->Is_MouseOverlap()
		&& !m_bLbtnPressed)
	{
		m_bMouseOn = true;
		// ���콺�� ������ ���� �ְ�, ��Ŭ������ ������ �Ǿ��� ��,
		if (KEY_TAP(KEY::LBTN))
		{
			m_bLbtnPressed = true;
			// �ش� ������ false�� ������ ���� ������ ��ġ���� �������־�� �Ѵ�.
		}
	}
	else
	{
		m_bMouseOn = false;
	}
}