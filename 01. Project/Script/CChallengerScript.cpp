#include "pch.h"
#include "CChallengerScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CGameMgrScript.h"
#include "CTileMgrScript.h"

#include "CTileScript.h"

CChallengerScript::CChallengerScript()
	: CMonsterScript(CHALLENGERSCRIPT)
	, m_bRunOnce(false)
{
	// ������ �⺻ ���� �־��ֱ�
	// HP�� 15~25
	m_tMonsterInfo.Max_HP = 20;
	m_tMonsterInfo.Cur_HP = m_tMonsterInfo.Max_HP;
	m_tMonsterInfo.Attack = 15;
	m_tMonsterInfo.Deffence = 10;
	m_tMonsterInfo.Range = 1;
}

CChallengerScript::~CChallengerScript()
{
}



void CChallengerScript::begin()
{
	CMonsterScript::begin();

	// ���Ͱ� ��ġ�� Ÿ�� �����ֱ�
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_23"));

	// Tile�� ��ġ�� �µ��� Position ����
	PosOnTile();
}

void CChallengerScript::tick()
{
	CMonsterScript::tick();
}


void CChallengerScript::CreateAddShieldSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Add\\collect_item_11.wav");
	pSound->Play(1, 0.8f, true);
}

void CChallengerScript::MosnsterAIPlay()
{
	// �̹� ���ʶ� �ؾ������� ��ġ�� �ʾ����� �ٽ� false�� �־��ش�.
	m_bTurnEnd = false;
	m_ActionOnce = false;

	// �� �� �������ٸ�,
	if (m_tMonsterInfo.Deffence == 0)
	{
		// �ֺ��� �÷��̾ �ִٸ�, 1ȸ ��������.
		if (!m_bRunOnce && GetOwnerTileScript()->FindAdjPlayer())
		{
			// �÷��̾� �ݴ� ������ ���ͼ� �̵��Ѵ�.
			CTileScript* NextTileScript = GetOwnerTileScript()->GetOppPlayerTile();
			// ��ȯ�� nullptr�� ��� �̵������� Ÿ���� ���ų� ������ �÷��̾ ���ٴ� �ǹ��̹Ƿ� �׳� �� �ø��� �ȴ�.
			if (NextTileScript != nullptr)
			{
				GetGameMgrScript()->GetTileMgrScript()->ObjectChangeTile(GetOwner(), NextTileScript, OBJECT_TYPE::MONSTER);
				m_eMonsterPattern = MONSTER_PATTERN::RUN;
			}
			else
			{
				m_eMonsterPattern = MONSTER_PATTERN::SHEILD;
			}
		}

		// ������ �ƴٸ�, �� �ø��� ������ �����Ѵ�.
		if (m_bRunOnce)
		{
			m_eMonsterPattern = MONSTER_PATTERN::SHEILD;
		}
	}
	else
	{
		// TileMgr���� Player���� �Ÿ��� ������ ��û�Ѵ�.
		m_tAstarInfo = GetGameMgrScript()->GetTileMgrScript()->FindAstar((CTileScript*)GetOwnerTile()->GetScript((UINT)TILESCRIPT));

		// �÷��̾���� �Ÿ��� Ȯ���� ��ȿ�Ÿ� �ȿ� ���Դ��� Ȯ���Ѵ�.
		// ���� �������� �÷��̾ �� �ָ� �ִٸ�,
		if (m_tMonsterInfo.Range < m_tAstarInfo.iTargetRange)
		{
			// �̵� ���� ������ ���� �ൿ ���ϱ�
			GetGameMgrScript()->GetTileMgrScript()->ObjectChangeTile(GetOwner(), m_tAstarInfo.pNextTile, OBJECT_TYPE::MONSTER);
			m_eMonsterPattern = MONSTER_PATTERN::MOVE;
		}
		// ���� ������ ���Դٸ�,
		else
		{
			// ���� ���� ������ ���� �ൿ ���ϱ�
			m_eMonsterPattern = MONSTER_PATTERN::ATTACK;
		}
	}
}

void CChallengerScript::Hit(int ATK, WAPON_TYPE _eWaponType)
{
	CMonsterScript::Hit(ATK, _eWaponType);
}

void CChallengerScript::Move()
{
	PosOnTile();
	m_bMonMove = true;
}

void CChallengerScript::Run()
{
	PosOnTile();
	m_bMonMove = true;
	m_bRunOnce = true;
}

void CChallengerScript::Sheild()
{
	PlayDefAnim();

	// ��� ����Ʈ ���
	CreateShieldParticle();

	// ���� ���� ȿ���� ����
	CreateAddShieldSound();

	// �ٽ� �� 20���� ���δ�.
	m_tMonsterInfo.Deffence = 10;

	m_bAnimStart = true;
	m_bRunOnce = false;
}

void CChallengerScript::PlayIDLEAnim()
{
	Animator2D()->Play(L"IDLE_Challenger", true);
}

void CChallengerScript::PlayDefAnim()
{
	Animator2D()->Play(L"Def_Challenger", true);
}

void CChallengerScript::PlayAttackAnim()
{
	Animator2D()->Play(L"Attack_Challenger", true);

	// ȿ���� ���
	CreateSlashSound();
}

void CChallengerScript::PlayDeadAnim()
{
	Animator2D()->Play(L"Dead_Challenger", true);
}

void CChallengerScript::PlayStunAnim()
{
	Animator2D()->Play(L"Stun_Challenger", true);
}




void CChallengerScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CChallengerScript::Overlap(CCollider2D* _pOther)
{
}

void CChallengerScript::EndOverlap(CCollider2D* _pOther)
{
}

void CChallengerScript::SaveToFile(FILE* _File)
{
}

void CChallengerScript::LoadFromFile(FILE* _File)
{
}
