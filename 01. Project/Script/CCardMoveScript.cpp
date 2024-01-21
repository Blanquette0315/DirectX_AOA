#include "pch.h"
#include "CCardMoveScript.h"

#include "CGameMgrScript.h"
#include "CCardMgrScript.h"
#include "CTileMgrScript.h"

#include "CPlayerScript.h"
#include "CTileScript.h"

CCardMoveScript::CCardMoveScript()
	: CCardScript(CARDMOVESCRIPT, CARD_TYPE::MOVE)
{
	m_tCardState.Attack = 0;
	m_tCardState.Cost = 0;
	m_tCardState.Deffence = 0;
	m_tCardState.Range = 1;

}

CCardMoveScript::~CCardMoveScript()
{
}


void CCardMoveScript::begin()
{
	CCardScript::begin();
}

void CCardMoveScript::tick()
{
	CCardScript::tick();

}



void CCardMoveScript::SaveToFile(FILE* _File)
{
	CCardScript::SaveToFile(_File);
}

void CCardMoveScript::LoadFromFile(FILE* _File)
{
	CCardScript::LoadFromFile(_File);
}


//void CCardMoveScript::PlayerMove(CTileScript* _TargetTile)
//{
//	CCardMgrScript* pCardMgrScript = (CCardMgrScript*)(GetOwner()->GetParent()->GetScript((UINT)CARDMGRSCRIPT));
//	CGameMgrScript* pGameMgrScript = pCardMgrScript->GetGameMgrScript();
//	pGameMgrScript->GetTileMgrScript()->ObjectChangeTile(GetPlayer(), _TargetTile);
//}