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
	// 몬스터의 기본 스텟 넣어주기
	// HP는 15~25
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

	// 몬스터가 위치할 타일 정해주기
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_23"));

	// Tile의 위치에 맞도록 Position 변경
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
	// 이번 차례때 해야할일을 마치지 않았으니 다시 false를 넣어준다.
	m_bTurnEnd = false;
	m_ActionOnce = false;

	// 방어가 다 떨어졌다면,
	if (m_tMonsterInfo.Deffence == 0)
	{
		// 주변에 플레이어가 있다면, 1회 도망간다.
		if (!m_bRunOnce && GetOwnerTileScript()->FindAdjPlayer())
		{
			// 플레이어 반대 방향을 얻어와서 이동한다.
			CTileScript* NextTileScript = GetOwnerTileScript()->GetOppPlayerTile();
			// 반환이 nullptr일 경우 이동가능한 타일이 없거나 주위에 플레이어가 없다는 의미이므로 그냥 방어를 올리면 된다.
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

		// 도망을 쳤다면, 방어를 올리는 패턴을 실행한다.
		if (m_bRunOnce)
		{
			m_eMonsterPattern = MONSTER_PATTERN::SHEILD;
		}
	}
	else
	{
		// TileMgr에게 Player와의 거리와 방향을 요청한다.
		m_tAstarInfo = GetGameMgrScript()->GetTileMgrScript()->FindAstar((CTileScript*)GetOwnerTile()->GetScript((UINT)TILESCRIPT));

		// 플레이어와의 거리를 확인해 유효거리 안에 들어왔는지 확인한다.
		// 공격 범위보다 플레이어가 더 멀리 있다면,
		if (m_tMonsterInfo.Range < m_tAstarInfo.iTargetRange)
		{
			// 이동 패턴 실행을 위한 행동 정하기
			GetGameMgrScript()->GetTileMgrScript()->ObjectChangeTile(GetOwner(), m_tAstarInfo.pNextTile, OBJECT_TYPE::MONSTER);
			m_eMonsterPattern = MONSTER_PATTERN::MOVE;
		}
		// 공격 범위에 들어왔다면,
		else
		{
			// 공격 패턴 실행을 위한 행동 정하기
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

	// 방어 이펙트 출력
	CreateShieldParticle();

	// 방어력 증가 효과음 진행
	CreateAddShieldSound();

	// 다시 방어를 20으로 높인다.
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

	// 효과음 재생
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
