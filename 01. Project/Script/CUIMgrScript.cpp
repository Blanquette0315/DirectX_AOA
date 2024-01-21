#include "pch.h"
#include "CUIMgrScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CGameMgrScript.h"

#include "CItemCardScript.h"

CUIMgrScript::CUIMgrScript()
	: CScript(UIMGRSCRIPT)
	, m_pGameMgrScript(nullptr)
	, m_pFocusedUI(nullptr)
	, m_pTargetUI(nullptr)
	, m_pSelectItemScript(nullptr)
	, m_bSelectedItem(false)
	, m_bNext(false)
{
}

CUIMgrScript::~CUIMgrScript()
{
}

void CUIMgrScript::begin()
{
	CGameObject* pGameMgr = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"GameMgr");
	m_pGameMgrScript = (CGameMgrScript*)(pGameMgr->GetScript(GAMEMGRSCRIPT));

	m_pGameMgrScript->RegisterUIMgrScript(this);
}

void CUIMgrScript::tick()
{
	if (m_bNext)
	{
		m_bSelectedItem = false;
		m_bNext = false;
	}
}

bool CUIMgrScript::IS_ItemVaild()
{
	if (m_bSelectedItem == true && m_bNext == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CUIMgrScript::ReleaseSelectedCard()
{
	m_pSelectItemScript = nullptr;
	m_bNext = true;
}

void CUIMgrScript::UseSelectedItemCard()
{
	m_pSelectItemScript->UseSelectedItemCard();

	ReleaseSelectedCard();
}