#include "pch.h"
#include "CCardAttackScript.h"

#include "CGameMgrScript.h"
#include "CCardMgrScript.h"
#include "CPlayerScript.h"

#include "CWaponScript.h"

CCardAttackScript::CCardAttackScript()
	: CCardScript(CARDATTACKSCRIPT, CARD_TYPE::ATTACK)
{
	m_tCardState.Attack = 1;
	m_tCardState.Cost = 1;
	m_tCardState.Deffence = 0;
	m_tCardState.Range = 1;
}

CCardAttackScript::~CCardAttackScript()
{
}

void CCardAttackScript::begin()
{
	CCardScript::begin();
}

void CCardAttackScript::tick()
{
	// �÷��̾ ���� ����� ���� ���ݷ��� ������ �����ֱ�
	CPlayerScript* PlayerScript = GetCardMgrScript()->GetGameMgrScript()->GetPlayerScript();

	int WaponATT = 0;

	if (m_eCardDirType == CARD_DIR_TYPE::RIGHT)
	{
		WaponATT = PlayerScript->GetRightWaponInfo().iATT;
	}
	else
	{
		WaponATT = PlayerScript->GetLeftWaponInfo().iATT;
	}

	m_tCardState.Attack = 1 + WaponATT;

	CCardScript::tick();
}



void CCardAttackScript::SaveToFile(FILE* _File)
{
	CCardScript::SaveToFile(_File);
}

void CCardAttackScript::LoadFromFile(FILE* _File)
{
	CCardScript::LoadFromFile(_File);
}