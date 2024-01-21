#include "pch.h"
#include "CDemonDogScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CEventMgr.h>

#include "CGameMgrScript.h"
#include "CTileMgrScript.h"

#include "CTileScript.h"
#include "CPlayerScript.h"

CDemonDogScript::CDemonDogScript()
	: CMonsterScript(DEMONDOGSCRIPT)
	, m_bRestrain(false)
{
	// 몬스터의 기본 스텟 넣어주기
	// 최대 체력은 12 ~ 16
	m_tMonsterInfo.Max_HP = 14;
	m_tMonsterInfo.Cur_HP = m_tMonsterInfo.Max_HP;
	m_tMonsterInfo.Attack = 6;
	m_tMonsterInfo.Deffence = 0;
	m_tMonsterInfo.Range = 1;
}

CDemonDogScript::~CDemonDogScript()
{
}

void CDemonDogScript::begin()
{
	CMonsterScript::begin();

	// 몬스터가 위치할 타일 정해주기
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_14"));

	// Tile의 위치에 맞도록 Position 변경
	PosOnTile();
}

void CDemonDogScript::tick()
{
	CMonsterScript::tick();
}



void CDemonDogScript::CreateBiteEff()
{
	Ptr<CPrefab> pEffPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Eff_Bite_Prefab_0.pref");

	CGameObject* pEffect = pEffPrefab->Instantiate();

	// 회전 해주기
	Vec3 TargetPos = GetGameMgrScript()->GetPlayerScript()->GetTile()->Transform()->GetWorldPos();
	TargetPos.y += Transform()->GetRelativeScale().y * 0.5f;

	Vec3 OriginPos = Transform()->GetRelativePos();

	// 차이 구하기
	Vec2 vDifPos = Vec2(TargetPos.x - OriginPos.x, TargetPos.y - OriginPos.y);

	// 두 지점 사이의 각도 구하기
	float fSeta = atan2f(vDifPos.y, vDifPos.x);

	// 각도만큼 이펙트 회전
	pEffect->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, fSeta));

	// 생성 지점 지정
	Vec3 vPlayerPos = GetGameMgrScript()->GetPlayerScript()->Transform()->GetRelativePos();
	vPlayerPos.x += cos(fSeta);
	vPlayerPos.y += sin(fSeta);


	pEffect->Transform()->SetRelativePos(vPlayerPos);
	pEffect->Transform()->SetRelativePos_Z(0.f);

	tEvent tEvnt = {};
	tEvnt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvnt.wParam = (DWORD_PTR)pEffect;
	tEvnt.lParam = (DWORD_PTR)11;

	CEventMgr::GetInst()->AddEvent(tEvnt);

	// 이펙트 애니메이션 실행
	pEffect->Animator2D()->Play(L"Eff_Bite", false);
}

void CDemonDogScript::CreateBiteSound()
{
	srand(time(nullptr));
	int iRand = rand() % 2;

	if (iRand == 0)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\DemonDog\\DogBite_0(troll_monster_attack_fast_02).wav");
		pSound->Play(1, 1.0f, true);
	}
	else
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\DemonDog\\DogBite_1(troll_monster_attack_fast_03).wav");
		pSound->Play(1, 1.0f, true);
	}
}

void CDemonDogScript::MosnsterAIPlay()
{
	// 이번 차례때 해야할일을 마치지 않았으니 다시 false를 넣어준다.
	m_bTurnEnd = false;
	m_ActionOnce = false;

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
		// 속박 패턴
		m_eMonsterPattern = MONSTER_PATTERN::RESTRAIN;

		// 만약 플레이어가 속박이 걸렸다면 공격을 한다.
		if (GetGameMgrScript()->GetPlayerScript()->CheckDebuf(DEBUF_TYPE::RESTRAIN))
		{
			m_eMonsterPattern = MONSTER_PATTERN::ATTACK;
		}
	}
}

void CDemonDogScript::Hit(int ATK, WAPON_TYPE _eWaponType)
{
	CMonsterScript::Hit(ATK, _eWaponType);

	// 만약 속박을 건 DemonDog가 죽으면 플레이어의 속박 상태이상을 바로 풀어주어야 한다.
	if (GetTempDead())
	{
		// 이때, 문제는 해당 속박 디버프를 2마리의 강아지가 걸었다면, 디버프가 풀리면 안된다.
		if (m_bRestrain)
		{
			GetGameMgrScript()->GetPlayerScript()->ReleaseDebuf(DEBUF_TYPE::RESTRAIN);
		}
	}
}

void CDemonDogScript::Move()
{
	PosOnTile();
	m_bMonMove = true;
}

void CDemonDogScript::Restrain()
{
	m_bRestrain = true;

	// 공격 애니메이션을 실행해 준다.
	PlayAttackAnim();

	// PlayerScript에 속박 디버프를 체크해 준다.
	GetGameMgrScript()->GetPlayerScript()->SetDebuf(DEBUF_TYPE::RESTRAIN);

	// 차례 넘김
	m_bAnimStart = true;
}

void CDemonDogScript::PlayIDLEAnim()
{
	Animator2D()->Play(L"IDLE_DemonDog", true);
}

void CDemonDogScript::PlayDefAnim()
{
	Animator2D()->Play(L"Def_DemonDog", true);
}

void CDemonDogScript::PlayAttackAnim()
{
	Animator2D()->Play(L"Attack_DemonDog", true);
	CreateBiteEff();

	// 효과음 재생
	CreateBiteSound();
}

void CDemonDogScript::PlayDeadAnim()
{
	Animator2D()->Play(L"Dead_DemonDog", true);
}



void CDemonDogScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CDemonDogScript::Overlap(CCollider2D* _pOther)
{
}

void CDemonDogScript::EndOverlap(CCollider2D* _pOther)
{
}

void CDemonDogScript::SaveToFile(FILE* _File)
{
}

void CDemonDogScript::LoadFromFile(FILE* _File)
{
}