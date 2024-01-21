#include "pch.h"
#include "CMouseCheckScript.h"

#include <Engine/CKeyMgr.h>

CMouseCheckScript::CMouseCheckScript()
	:CScript(MOUSECHECKSCRIPT)
	, m_bMouseOverlap(false)
{
}

CMouseCheckScript::~CMouseCheckScript()
{
}

void CMouseCheckScript::begin()
{
}

void CMouseCheckScript::tick()
{

}



void CMouseCheckScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CMouseCheckScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}



bool CMouseCheckScript::Is_MouseOverlap()
{
	// KeyMgr�� ���� ���콺 ������ �޾ƿ���
	m_vMousePos = CKeyMgr::GetInst()->GetGWMousePos();

	// ���콺�� ������Ʈ �ȿ� ���� �ִ��� üũ�Ѵ�.
	RECT ObjRect = GetOwner()->Transform()->GetRectCoord();
	if (ObjRect.left <= m_vMousePos.x && ObjRect.right >= m_vMousePos.x
		&& ObjRect.top >= m_vMousePos.y && ObjRect.bottom <= m_vMousePos.y)
	{
		m_bMouseOverlap = true;
	}
	else
	{
		if (m_bMouseOverlap)
			m_bMouseOverlap = false;
	}

	return m_bMouseOverlap;
}