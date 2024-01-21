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
	// ������ �⺻ ���� �־��ֱ�
	// �ִ� ü���� 12 ~ 16
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

	// ���Ͱ� ��ġ�� Ÿ�� �����ֱ�
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_14"));

	// Tile�� ��ġ�� �µ��� Position ����
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

	// ȸ�� ���ֱ�
	Vec3 TargetPos = GetGameMgrScript()->GetPlayerScript()->GetTile()->Transform()->GetWorldPos();
	TargetPos.y += Transform()->GetRelativeScale().y * 0.5f;

	Vec3 OriginPos = Transform()->GetRelativePos();

	// ���� ���ϱ�
	Vec2 vDifPos = Vec2(TargetPos.x - OriginPos.x, TargetPos.y - OriginPos.y);

	// �� ���� ������ ���� ���ϱ�
	float fSeta = atan2f(vDifPos.y, vDifPos.x);

	// ������ŭ ����Ʈ ȸ��
	pEffect->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, fSeta));

	// ���� ���� ����
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

	// ����Ʈ �ִϸ��̼� ����
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
	// �̹� ���ʶ� �ؾ������� ��ġ�� �ʾ����� �ٽ� false�� �־��ش�.
	m_bTurnEnd = false;
	m_ActionOnce = false;

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
		// �ӹ� ����
		m_eMonsterPattern = MONSTER_PATTERN::RESTRAIN;

		// ���� �÷��̾ �ӹ��� �ɷȴٸ� ������ �Ѵ�.
		if (GetGameMgrScript()->GetPlayerScript()->CheckDebuf(DEBUF_TYPE::RESTRAIN))
		{
			m_eMonsterPattern = MONSTER_PATTERN::ATTACK;
		}
	}
}

void CDemonDogScript::Hit(int ATK, WAPON_TYPE _eWaponType)
{
	CMonsterScript::Hit(ATK, _eWaponType);

	// ���� �ӹ��� �� DemonDog�� ������ �÷��̾��� �ӹ� �����̻��� �ٷ� Ǯ���־�� �Ѵ�.
	if (GetTempDead())
	{
		// �̶�, ������ �ش� �ӹ� ������� 2������ �������� �ɾ��ٸ�, ������� Ǯ���� �ȵȴ�.
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

	// ���� �ִϸ��̼��� ������ �ش�.
	PlayAttackAnim();

	// PlayerScript�� �ӹ� ������� üũ�� �ش�.
	GetGameMgrScript()->GetPlayerScript()->SetDebuf(DEBUF_TYPE::RESTRAIN);

	// ���� �ѱ�
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

	// ȿ���� ���
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