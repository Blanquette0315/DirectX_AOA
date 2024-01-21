#include "pch.h"
#include "CChargeScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CGameMgrScript.h"
#include "CTileMgrScript.h"

#include "CTileScript.h"

CChargeScript::CChargeScript()
	: CMonsterScript(CHARGESCRIPT)
{
	// ������ �⺻ ���� �־��ֱ�
	// HP�� 20~30
	m_tMonsterInfo.Max_HP = 3;
	m_tMonsterInfo.Cur_HP = m_tMonsterInfo.Max_HP;
	m_tMonsterInfo.Attack = 15;
	m_tMonsterInfo.Deffence = 0;
	m_tMonsterInfo.Range = 2;
}

CChargeScript::~CChargeScript()
{
}


void CChargeScript::begin()
{
	CMonsterScript::begin();

	// ���Ͱ� ��ġ�� Ÿ�� �����ֱ�
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_23"));

	// Tile�� ��ġ�� �µ��� Position ����
	PosOnTile();
}

void CChargeScript::tick()
{
	CMonsterScript::tick();
}



void CChargeScript::MosnsterAIPlay()
{
	// �̹� ���ʶ� �ؾ������� ��ġ�� �ʾ����� �ٽ� false�� �־��ش�.
	m_bTurnEnd = false;
	m_ActionOnce = false;

	// ���� �غ����� �ƴ� ���� �����ؾ� �Ѵ�.
	if (!m_bCharge)
	{
		// ���� ����� ���� ��Ÿ��� �����ؼ� �÷��̾ �����ϴ��� Ȯ���Ѵ�.
		m_eTargetDIR = GetOwnerTileScript()->FindPlayerinRange_DIR(m_tMonsterInfo.Range);
		// ��ȯ�� nullptr�� ��� �������� �÷��̾ ���ٴ� ���̴�.
		if (m_eTargetDIR != TILE_DIR::END)
		{
			// �÷��̾ ���� ���� �־��ٸ�, ������ �غ��Ѵ�.
			m_eMonsterPattern = MONSTER_PATTERN::CHARGE;
		}
		// ���� ���� �÷��̾ ���ٸ�, �̵��Ѵ�.
		else
		{
			// TileMgr���� Player���� �Ÿ��� ������ ��û�Ѵ�.
			m_tAstarInfo = GetGameMgrScript()->GetTileMgrScript()->FindAstar((CTileScript*)GetOwnerTile()->GetScript((UINT)TILESCRIPT));

			// �÷��̾���� �Ÿ��� Ȯ���� ��ȿ�Ÿ� �ȿ� ���Դ��� Ȯ���Ѵ�.
			// ���� �������� �÷��̾ �� �ָ� �ִٸ�,
			if (m_tMonsterInfo.Range < 1)
			{
				// �̵� ���� ������ ���� �ൿ ���ϱ�
				GetGameMgrScript()->GetTileMgrScript()->ObjectChangeTile(GetOwner(), m_tAstarInfo.pNextTile, OBJECT_TYPE::MONSTER);
				m_eMonsterPattern = MONSTER_PATTERN::MOVE;
			}
		}
	}
	// ���� �غ� ���̾��ٸ�, ������ �����Ѵ�.
	else
	{
		// ���� ���� ������ ���� �ൿ ���ϱ�
		m_eMonsterPattern = MONSTER_PATTERN::RUSH;
	}
}

void CChargeScript::Hit(int ATK, WAPON_TYPE _eWaponType)
{
	CMonsterScript::Hit(ATK, _eWaponType);
}

void CChargeScript::Move()
{
	PosOnTile();
	m_bTurnEnd = true;
}

void CChargeScript::Charge()
{
	// ��� ����� ��¡�� ��Ʈ �ΰ��� ����� �����̴�.
	PlayDefAnim();
	m_bTurnEnd = true;
}

void CChargeScript::Rush()
{
	PlayAttackAnim();
}

void CChargeScript::PlayIDLEAnim()
{
	Animator2D()->Play(L"IDLE_Charge", true);
}

void CChargeScript::PlayDefAnim()
{
	Animator2D()->Play(L"Def_Charge", true);
}

void CChargeScript::PlayAttackAnim()
{
	Animator2D()->Play(L"Attack_Charge", true);
}

void CChargeScript::PlayDeadAnim()
{
	Animator2D()->Play(L"Dead_Charge", true);
}

void CChargeScript::PlayStunAnim()
{
	Animator2D()->Play(L"Stun_Charge", true);
}



void CChargeScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CChargeScript::Overlap(CCollider2D* _pOther)
{
}

void CChargeScript::EndOverlap(CCollider2D* _pOther)
{
}

void CChargeScript::SaveToFile(FILE* _File)
{
}

void CChargeScript::LoadFromFile(FILE* _File)
{
}
