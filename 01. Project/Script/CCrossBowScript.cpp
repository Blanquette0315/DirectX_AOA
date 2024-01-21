#include "pch.h"
#include "CCrossBowScript.h"

#include <Engine/CEventMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CGameMgrScript.h"
#include "CTileMgrScript.h"

#include "CTileScript.h"
#include "CMissileScript.h"
#include "CPlayerScript.h"

CCrossBowScript::CCrossBowScript()
	: CMonsterScript(CROSSBOWSCRIPT)
	, m_Prefab(nullptr)
	, m_pTargetTileScript(nullptr)
	, m_bBowLoading(true)
{
	// 몬스터의 기본 스텟 넣어주기
	//HP 12~18
	m_tMonsterInfo.Max_HP = 12;
	m_tMonsterInfo.Cur_HP = m_tMonsterInfo.Max_HP;
	m_tMonsterInfo.Attack = 10;
	m_tMonsterInfo.Deffence = 0;
	m_tMonsterInfo.Range = 2;
}

CCrossBowScript::~CCrossBowScript()
{
}

void CCrossBowScript::begin()
{
	CMonsterScript::begin();

	// 몬스터가 위치할 타일 정해주기
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_3"));

	// Tile의 위치에 맞도록 Position 변경
	PosOnTile();

	// 화살 프리펩 찾아서 주소 저장하기
	m_Prefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Arrow_Prefab_0.pref");
}

void CCrossBowScript::tick()
{
	CMonsterScript::tick();
}




void CCrossBowScript::CreateArrowShootEff()
{
	Ptr<CPrefab> pEffPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Eff_ArrowShoot_Prefab_0.pref");

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
	Vec3 vMonPos = Transform()->GetRelativePos();
	vMonPos.x += 75.f * cos(fSeta);
	vMonPos.y += 75.f * sin(fSeta);


	pEffect->Transform()->SetRelativePos(vMonPos);
	pEffect->Transform()->SetRelativePos_Z(0.f);

	tEvent tEvnt = {};
	tEvnt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvnt.wParam = (DWORD_PTR)pEffect;
	tEvnt.lParam = (DWORD_PTR)11;

	CEventMgr::GetInst()->AddEvent(tEvnt);

	// 이펙트 애니메이션 실행
	pEffect->Animator2D()->Play(L"Eff_ArrowShoot", false);
}

void CCrossBowScript::CreateArrowReloadSound()
{
	srand(time(nullptr));
	int iRand = rand() % 2;

	if (iRand == 0)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\CrossBow\\bow_crossbow_arrow_draw_stretch2_08.wav");
		pSound->Play(1, 1.0f, true);
	}
	else
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\CrossBow\\bow_crossbow_arrow_draw_stretch2_09.wav");
		pSound->Play(1, 1.0f, true);
	}
}

void CCrossBowScript::CreateArrowShootSound()
{
	srand(time(nullptr));
	int iRand = rand() % 2;

	if (iRand == 0)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\CrossBow\\bow_crossbow_arrow_shoot_type1_03.wav");
		pSound->Play(1, 1.0f, true);
	}
	else
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\CrossBow\\bow_crossbow_arrow_shoot_type1_04.wav");
		pSound->Play(1, 1.0f, true);
	}
}

void CCrossBowScript::MosnsterAIPlay()
{
	// 이번 차례때 해야할일을 마치지 않았으니 다시 false를 넣어준다.
	m_bTurnEnd = false;
	m_ActionOnce = false;

	// 플레이어가 범위 안에 들어왔는지 확인한다.
	// 반환받은 타일은 화살이 진행해야하는 타일.
	m_pTargetTileScript = GetOwnerTileScript()->FindPlayerinRange(m_tMonsterInfo.Range);
	if (m_pTargetTileScript != nullptr)
	{
		// 화살이 장전이 되어있는지 확인한다.
		if (m_bBowLoading)
		{
			// 공격 패턴 실행을 위한 행동 정하기
			m_eMonsterPattern = MONSTER_PATTERN::ATTACK;
		}
		else
		{
			// 재장전 하기
			m_eMonsterPattern = MONSTER_PATTERN::RELOAD;
		}
		
	}
	else
	{
		// TileMgr에게 Player와의 거리와 방향을 요청한다.
		m_tAstarInfo = GetGameMgrScript()->GetTileMgrScript()->FindAstar((CTileScript*)GetOwnerTile()->GetScript((UINT)TILESCRIPT));

		// 플레이어와의 거리를 확인해 유효거리 안에 들어왔는지 확인한다.
		// 공격 범위보다 플레이어가 더 멀리 있다면,
		if (1 < m_tAstarInfo.iTargetRange)
		{
			// 이동 패턴 실행을 위한 행동 정하기
			GetGameMgrScript()->GetTileMgrScript()->ObjectChangeTile(GetOwner(), m_tAstarInfo.pNextTile, OBJECT_TYPE::MONSTER);
			m_eMonsterPattern = MONSTER_PATTERN::MOVE;
		}
	}
}

void CCrossBowScript::Hit(int ATK, WAPON_TYPE _eWaponType)
{
	CMonsterScript::Hit(ATK, _eWaponType);
}

void CCrossBowScript::Move()
{
	PosOnTile();
	m_bMonMove = true;
}

void CCrossBowScript::Attack()
{
	PlayAttackAnim();

	// 효과음 넣기
	CreateArrowShootSound();

	CreateArrowShootEff();

	// 화살 프리펩 이니시에이트
	CGameObject* pArrow = m_Prefab->Instantiate();
	pArrow->Transform()->SetRelativePos(Transform()->GetRelativePos());
	// 화살에게 자신이 목표할 타일 설정해주기
	CMissileScript* pMScript = (CMissileScript*)(pArrow->GetScript((UINT)MISSILESCRIPT));
	pMScript->SetTargetTile(m_pTargetTileScript);
	pMScript->SetOwnerPos(Transform()->GetRelativeScale());
	pMScript->SetATK(m_tMonsterInfo.Attack);
	pMScript->CalculateTarget();

	tEvent tEVN = {};
	tEVN.eType = EVENT_TYPE::CREATE_OBJECT;
	tEVN.wParam = (DWORD_PTR)pArrow;
	tEVN.lParam = 4;

	CEventMgr::GetInst()->AddEvent(tEVN);

	m_bBowLoading = false;
	m_bAnimStart = true;
}

void CCrossBowScript::Reload()
{
	PlayDefAnim();

	// 효과음 넣기
	CreateArrowReloadSound();

	m_bBowLoading = true;
	m_bAnimStart = true;
}

void CCrossBowScript::PlayIDLEAnim()
{
	Animator2D()->Play(L"IDLE_CrossBow", true);
}

void CCrossBowScript::PlayDefAnim()
{
	Animator2D()->Play(L"Def_CrossBow", true);
}

void CCrossBowScript::PlayAttackAnim()
{
	Animator2D()->Play(L"Attack_CrossBow", true);
}

void CCrossBowScript::PlayDeadAnim()
{
	Animator2D()->Play(L"Dead_CrossBow", true);
}




void CCrossBowScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CCrossBowScript::Overlap(CCollider2D* _pOther)
{
}

void CCrossBowScript::EndOverlap(CCollider2D* _pOther)
{
}

void CCrossBowScript::SaveToFile(FILE* _File)
{
}

void CCrossBowScript::LoadFromFile(FILE* _File)
{
}
