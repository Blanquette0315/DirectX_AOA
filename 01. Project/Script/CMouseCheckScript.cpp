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
	// KeyMgr로 부터 마우스 포지션 받아오기
	m_vMousePos = CKeyMgr::GetInst()->GetGWMousePos();

	// 마우스가 오브젝트 안에 들어와 있는지 체크한다.
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