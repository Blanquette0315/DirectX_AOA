#include "pch.h"
#include "CCardDeffenceScript.h"

#include "CCardMgrScript.h"
#include "CGameMgrScript.h"
#include "CPlayerScript.h"

#include "CWaponScript.h"

CCardDeffenceScript::CCardDeffenceScript()
	: CCardScript(CARDDEFFENCESCRIPT, CARD_TYPE::DEFFENCE)
{
	m_tCardState.Attack = 0;
	m_tCardState.Cost = 1;
	m_tCardState.Deffence = 1;
	m_tCardState.Range = 0;
}

CCardDeffenceScript::~CCardDeffenceScript()
{
}

void CCardDeffenceScript::begin()
{
	CCardScript::begin();
}

void CCardDeffenceScript::tick()
{
	// 플레이어가 지닌 무기로 부터 공격력을 가져와 더해주기
	CPlayerScript* PlayerScript = GetCardMgrScript()->GetGameMgrScript()->GetPlayerScript();
	
	int WaponDFF = 0;
	
	if (m_eCardDirType == CARD_DIR_TYPE::RIGHT)
	{
		WaponDFF = PlayerScript->GetRightWaponInfo().iDff;
	}
	else
	{
		WaponDFF = PlayerScript->GetLeftWaponInfo().iDff;
	}
	
	m_tCardState.Deffence = 3 + WaponDFF;

	CCardScript::tick();
}



void CCardDeffenceScript::SaveToFile(FILE* _File)
{
	CCardScript::SaveToFile(_File);
}

void CCardDeffenceScript::LoadFromFile(FILE* _File)
{
	CCardScript::LoadFromFile(_File);
}