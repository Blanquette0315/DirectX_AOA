#include "pch.h"
#include "CPlayerScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CEventMgr.h>

#include <Engine/CMaterial.h>
#include <Engine/CAnimation2D.h>
#include <Engine/CRenderMgr.h>

#include <Engine/CLevel.h>
#include <Engine/CSound.h>

#include "CGameMgrScript.h"
#include "CCardMgrScript.h"
#include "CTileScript.h"
#include "CMonsterScript.h"

#include "CPlayerRightHandScript.h"
#include "CPlayerLeftHandScript.h"
#include "CPlayerHandScript.h"

#include "CMissileScript.h"
#include "CWaponScript.h"

#include "CUIMgrScript.h"
#include "CBloodEffectScript.h"
#include "CUnderAttackScript.h"

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
	, m_eObjType(OBJECT_TYPE::PLAYER)
	, m_pOwnerTile(nullptr)
	, m_pGameMgrScript(nullptr)
	, m_fAccTime(0.f)
	, m_fAccel(500.f)
	, m_fSpeed(320.f)
	, m_bAtteck(false)
	, m_bLockRot(false)
	, m_bRot(false)
	, m_bMove(false)
	, m_bHit(false)
	, m_pMoveTarget(nullptr)
	, m_vDifPos{}
	, m_fTargetDestance(0.f)
	, m_fAccDestance(0.f)
	, m_fSeta(0.f)
	, m_iStep(0)
	, m_bStepOnce(false)
	, m_iPlayerDebuf(0.f)
	, m_arrInven{}
	, m_bSelectedCard(false)
	, m_bOneMoretick(false)
	, m_OnceEffect(false)
	, m_bSlowROnce(false)
	, m_bArrowHit(false)
{
	// �÷��̾��� �ʱ� ���� �־��ֱ�
	m_tPlayerInfo.Max_HP = 100;
	m_tPlayerInfo.Cur_HP = m_tPlayerInfo.Max_HP;
	m_tPlayerInfo.RightAttack = 0;
	m_tPlayerInfo.LeftAttack = 0;
	m_tPlayerInfo.Deffence = 0;
	m_tPlayerInfo.Max_Acction = 3;
	m_tPlayerInfo.Cur_Acction = m_tPlayerInfo.Max_Acction;
	m_tPlayerInfo.DrowCount = 6;
	m_fSpeed = 320.f;

	// Player�� ���� �⺻ ī�带 �迭�� �־��ֱ�
	DefaultCardSetting();

	// �÷��̾��� �⺻ ���� ����
	// �޼�
	tPlayer_Wapon PlayerWapon = {};
	PlayerWapon.eWaponType = WAPON_TYPE::SWORD;
	PlayerWapon.eWaponDir = CARD_DIR_TYPE::LEFT;
	PlayerWapon.iATT = 4;
	PlayerWapon.iDff = 0;

	m_tLefetWapon = PlayerWapon;
	m_eLeftWaponType = m_tLefetWapon.eWaponType;

	//������
	PlayerWapon.eWaponType = WAPON_TYPE::SHIELD;
	PlayerWapon.eWaponDir = CARD_DIR_TYPE::RIGHT;
	PlayerWapon.iATT = 0;
	PlayerWapon.iDff = 2;

	m_tRightWapon = PlayerWapon;
	m_eRightWaponType = m_tRightWapon.eWaponType;

	// �÷��̾�� ȸ�� ������ �־��ֱ�
	tItem_Info PlayerItem = {};

	PlayerItem.eItemType = ITEM_TYPE::ITEM;

	tItem tempITem = {};
	tempITem.eItemType = ITEM_NAME::HPPOTION;

	PlayerItem.tItem = tempITem;
	PlayerItem.bExist = true;

	m_arrInven[0] = PlayerItem;

	// �ι�° ������ �־��ֱ�
	PlayerItem = {};
	PlayerWapon.eWaponDir = CARD_DIR_TYPE::RIGHT;
	PlayerWapon.eWaponType = WAPON_TYPE::SWORD;
	PlayerWapon.iATT = 4;
	PlayerWapon.iDff = 0;

	PlayerItem.tWapon = PlayerWapon;
	PlayerItem.bExist = true;

	m_arrInven[1] = PlayerItem;
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
	// UI���� Player ���� ����
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Player Move Speed ", &m_fSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Player Accel      ", &m_fAccel);

	AddScriptParam(SCRIPT_PARAM::INT, "Player State MAX_HP ", &m_tPlayerInfo.Max_HP);
	AddScriptParam(SCRIPT_PARAM::INT, "Player State CUR_HP ", &m_tPlayerInfo.Cur_HP);
	AddScriptParam(SCRIPT_PARAM::INT, "Player State RATTACK", &m_tPlayerInfo.RightAttack);
	AddScriptParam(SCRIPT_PARAM::INT, "Player State LATTACK", &m_tPlayerInfo.LeftAttack);
	AddScriptParam(SCRIPT_PARAM::INT, "Player State DEF    ", &m_tPlayerInfo.Deffence);
	AddScriptParam(SCRIPT_PARAM::INT, "Player State MAX_ACT", &m_tPlayerInfo.Max_Acction);
	AddScriptParam(SCRIPT_PARAM::INT, "Player State CUR_ACT", &m_tPlayerInfo.Cur_Acction);

	// GameMgr�� �÷��̾� ����ϱ�
	CGameObject* pGameMgr = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"GameMgr");
	m_pGameMgrScript = (CGameMgrScript*)(pGameMgr->GetScript(GAMEMGRSCRIPT));
	m_pGameMgrScript->RegisterPlayer(GetOwner());

	// �÷��̾� ������ �̴ϼ�
	PosInit();

	// �÷��̾ ��ġ�� Ÿ�� �������ֱ�
	CLevel* m_pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	SetOwnerTile(m_pCurLevel->FindObjectByName(L"Tile_0"));

	// Tile�� ��ġ�� �µ��� Position ����
	PosOnTile();

	// �ӽ÷� ���� ī�� ����� �ֱ� ���߿��� �Ʒ����� ������ ���� ����Ǿ�� �Ѵ�.
	// ī�� ������ ������ ���Կ��� �ؾ��Ѵ�.
	//CreateWaponCard();

	// ó�� �����ϸ� IDLE Animation�� �������ش�.
	IDLEAnimPlay();
}

void CPlayerScript::tick()
{
	// �ִϸ��̼� ��� Test�뵵
	// ���� �����ϴ� �ִϸ��̼��� �������� IDLE ���·� �ǵ��� �ش�.
	if (GetOwner()->Animator2D()->GetCurAnimation()->Is_End())
	{
		IDLEAnimPlay();
		m_bLockRot = false;
	}

	// ������ Ȱ��ȭ �Ǿ��ٸ�,
	if (m_bAtteck)
	{
		Attack_tick();
	}

	// �̵��� Ȱ��ȭ �Ǿ��ٸ�, �̵��� �����Ѵ�.
	if (m_bMove)
	{
		Move_tick();
	}

	// ������ ���� ���� �ƴ� ���� Hit�� ����Ǿ�� �Ѵ�.
	// RefBeetle�� �ݻ� ������� ���� �� ����.
	if (!m_bAtteck && m_bHit)
	{
		Hit_tick();
	}

	// Hit��� �׽�Ʈ��
	if (KEY_TAP(KEY::H))
	{
		m_bHit = true;
	}

	// ���콺 �����ǿ� ���� Player�� ȸ���� ī�尡 ���õȰ� ���� ���� �ؾ��Ѵ�.
	//if (m_bSelectedCard)
	//{
	//	// ���콺 ������ üũ �뵵
	//	Vec2 vMousePos = CKeyMgr::GetInst()->GetGWMousePos();
	//	Vec3 vPlayerPos = GetOwner()->Transform()->GetRelativePos();
	//	// �ٸ� �ִϸ��̼� ���� ���� ���� �����ϸ� �ȵ�.
	//	if (!m_bLockRot && !m_bAtteck)
	//	{
	//		if (vPlayerPos.x < vMousePos.x)
	//		{
	//			// ���콺�� �÷��̾�� ������ �ִٴ� ��
	//			PlayerRotRight();
	//		}
	//		else
	//		{
	//			PlayerRotLeft();
	//		}
	//	}
	//}
	//else
	//{
	//	PlayerRotLeft();
	//}

	// ���콺 ������ üũ �뵵
	Vec2 vMousePos = CKeyMgr::GetInst()->GetGWMousePos();
	Vec3 vPlayerPos = GetOwner()->Transform()->GetRelativePos();
	// �ٸ� �ִϸ��̼� ���� ���� ���� �����ϸ� �ȵ�.
	if (!m_bLockRot && !m_bAtteck)
	{
		if (m_bSelectedCard)
		{
			if (vPlayerPos.x < vMousePos.x)
			{
				// ���콺�� �÷��̾�� ������ �ִٴ� ��
				PlayerRotRight();
			}
			else
			{
				PlayerRotLeft();
			}
		}
	}
}




void CPlayerScript::SetOwnerTile(CGameObject* _Tile)
{
	m_pOwnerTile = _Tile;
	CTileScript* m_pTileScript = (CTileScript*)m_pOwnerTile->GetScript((int)TILESCRIPT);
	m_pTileScript->SetTargetObj(GetOwner(), m_eObjType);
}

void CPlayerScript::PosInit()
{
	// ������ ���� �������� ���� �������ֱ� (�ӽ÷� Scale�� 1/2�� �÷��ִ� �Ŷ� �ӽ� ����)
	Vec3 vPlayerPos = Vec3(0.f, 0.f, 100.f);
	Vec3 vPlayerScale = GetOwner()->Transform()->GetRelativeScale();
	vPlayerPos.y += vPlayerScale.y * 0.5f;

	GetOwner()->Transform()->SetRelativePos(vPlayerPos);
	m_vPlayerPos.x = vPlayerPos.x;
	m_vPlayerPos.y = vPlayerPos.y;
}

void CPlayerScript::PosOnTile()
{
	// Player�� ���콺 ��ġ�� ���� ȸ���ϵ��� Ǯ���ش�.
	m_bLockRot = false;

	Vec2 vPlayerPos = Vec2(m_vPlayerPos.x, m_vPlayerPos.y);
	Vec3 vTileWorldPos = m_pOwnerTile->Transform()->GetWorldPos();
	vPlayerPos.x += vTileWorldPos.x;
	vPlayerPos.y += vTileWorldPos.y;
	GetOwner()->Transform()->SetRelativePos_XY(vPlayerPos);
}

void CPlayerScript::PosOnTempTile(CTileScript* _TempTile)
{
	// Player�� ���콺 ��ġ�� ���� ȸ������ �ʵ��� ���´�.
	m_bLockRot = true;

	// �ӽ÷� �̵��Ǿ�� �� Ÿ���� ���� Ÿ�Ϻ��� �������� ���������� Ȯ���ؼ� �÷��̾ ���� ������ �����ش�.
	Vec3 OriginTilePos = m_pOwnerTile->Transform()->GetRelativePos();
	Vec3 TempTilePos = _TempTile->Transform()->GetRelativePos();

	// �ӽ� Ÿ���� ���� Ÿ�Ϻ��� ������ ���
	if (OriginTilePos.x < TempTilePos.x)
	{
		PlayerRotRight();
	}
	else
	{
		PlayerRotLeft();
	}

	Vec3 vPlayerPos = Vec3(m_vPlayerPos.x, m_vPlayerPos.y, 100);
	Vec3 vTileWorldPos = _TempTile->Transform()->GetWorldPos();
	vPlayerPos.x += vTileWorldPos.x;
	vPlayerPos.y += vTileWorldPos.y;
	GetOwner()->Transform()->SetRelativePos(vPlayerPos);
}

void CPlayerScript::IDLEAnimPlay()
{
	// ���� ������ ���Ⱑ īŸ�����, ����̶�� �ǹ��̴�.
	if (m_eRightWaponType == WAPON_TYPE::KATANA)
	{
		Animator2D()->Play(L"IDLE_Both_HandBase", true);
		Transform()->SetRelativeScale(120.f, 60.f, 1.f); // ��չ���� 120, 60�̴�.

		// ���� �� �ִϸ��̼� ����
		GetRightHandScript()->StopAnim();
		GetLeftHandScript()->StopAnim();
	
		// ��� IDLE �ִϸ��̼� ���
		GetBothHandScript()->IDLEAnimPlayer(m_eRightWaponType);
	}
	else
	{
		Animator2D()->Play(L"IDLE_SIG_HandBase", true);
		Transform()->SetRelativeScale(80.f, 60.f, 1.f); // �Ѽչ���� 80, 60�̴�.

		// ��� �ִϸ��̼� ����
		GetBothHandScript()->StopAnim();

		// ���� ���⺰ �ִϸ��̼� ����
		GetRightHandScript()->IDLEAnimPlayer(m_eRightWaponType);

		// ���� ���⺰ �ִϸ��̼� ����
		GetLeftHandScript()->IDLEAnimPlayer(m_eLeftWaponType);
	}

	PlayerRotLeft();
}

void CPlayerScript::AttackAnimPlay(CARD_DIR_TYPE _CardDir)
{
	// ���� ������ �˸°� �ִϸ��̼��� ������ �־�� �Ѵ�.
	// ��, ���� ������ ���� ���� ��������, ������ ���������� ���� �ٸ��� ��µǾ�� �Ѵ�. 
	
	// ���� ������ ���Ⱑ īŸ�����, ����̶�� �ǹ��̴�.
	if (m_eRightWaponType == WAPON_TYPE::KATANA)
	{
		Animator2D()->Play(L"Atteck_Both_HandBase", false);
		Transform()->SetRelativeScale(120.f, 60.f, 1.f); // ��չ���� 120, 60�̴�.

		// ���� �� �ִϸ��̼� ����
		GetRightHandScript()->StopAnim();
		GetLeftHandScript()->StopAnim();

		// īŸ�� �ִϸ��̼� ���
		GetBothHandScript()->AttackAnimPlay(m_eRightWaponType, _CardDir);
	}
	else
	{
		Animator2D()->Play(L"Atteck_SIG_HandBase", false);
		Transform()->SetRelativeScale(80.f, 60.f, 1.f); // �Ѽչ���� 80, 60�̴�.

		// ��� �ִϸ��̼� ����
		GetBothHandScript()->StopAnim();

		// ���� ���⺰ �ִϸ��̼� ����
		GetRightHandScript()->RAttackAnimPlay(m_eRightWaponType, _CardDir);
		
		// ���� ���⺰ �ִϸ��̼� ����
		GetLeftHandScript()->LAttackAnimPlay(m_eLeftWaponType, _CardDir);
	}
	
}

void CPlayerScript::Attack(CTileScript* _pTargetTile, CARD_DIR_TYPE _CardDir)
{
	CalculateTarget(_pTargetTile);

	AttackAnimPlay(_CardDir);
	m_eDir = _CardDir; // ���� ����Ʈ ����� �Ҷ� ����ϱ� ���� �޾Ƶд�.
	m_bAtteck = true;

	// ����Ʈ ����� �ѹ� �����ϱ� ����
	m_OnceEffect = true;
}

void CPlayerScript::Move(CTileScript* _pTargetTile)
{
	CalculateTarget(_pTargetTile);

	// �� �� ������ �Ÿ� ���ϱ�
	m_fTargetDestance = sqrt((m_vDifPos.x* m_vDifPos.x) + (m_vDifPos.y* m_vDifPos.y));

	m_bMove = true;
}

void CPlayerScript::Hit(int _iAtk)
{
	// �ǰ� ����� �����ϱ� ���� true�� �־��ش�.
	m_bHit = true;

	int iATT = _iAtk;
	// Player�� ü���� ����Ѵ�.
	// ���߿��� �� �ִ����� ���� Ȯ���ؼ� �� �ִٸ�, �� ���� ��� �Ѵ�.

	if (m_tPlayerInfo.Deffence != 0)
	{
		// ���� �����ִ� ���差 ���� ���ݷ��� ũ�ٸ�,
		if (m_tPlayerInfo.Deffence < _iAtk)
		{
			iATT -= m_tPlayerInfo.Deffence;
			m_tPlayerInfo.Deffence = 0;

			// HP ���
			if (m_tPlayerInfo.Cur_HP != 0)
			{
				if (m_tPlayerInfo.Cur_HP > iATT)
				{
					m_tPlayerInfo.Cur_HP -= iATT;

					// ȿ���� ���
					CreateHitSound();

					// UnderAttack ȿ�� ��û
					AddEventUnderAttack();
				}
				// �÷��̾��� ���� HP�� ���� ���ط� ���� �۴ٸ� 0�� �ش�.
				else
				{
					m_tPlayerInfo.Cur_HP = 0;

					// ȿ���� ���
					CreateHitSound();

					// UnderAttack ȿ�� ��û
					AddEventUnderAttack();
				}
			}
		}
		else
		{
			m_tPlayerInfo.Deffence -= iATT;
			iATT = 0;

			// ȿ���� ���
			CreateHitShieldSound();
		}
	}
	else
	{
		if (m_tPlayerInfo.Cur_HP != 0)
		{
			if (m_tPlayerInfo.Cur_HP > iATT)
			{
				m_tPlayerInfo.Cur_HP -= iATT;

				// ȿ���� ���
				CreateHitSound();

				// UnderAttack ȿ�� ��û
				AddEventUnderAttack();
			}
			// �÷��̾��� ���� HP�� ���� ���ط� ���� �۴ٸ� 0�� �ش�.
			else
			{
				m_tPlayerInfo.Cur_HP = 0;

				// ȿ���� ���
				CreateHitSound();

				// UnderAttack ȿ�� ��û
				AddEventUnderAttack();
			}
		}
	}

	// �ǰ� ����Ʈ�� ����Ѵ�.
	if (!m_bArrowHit)
	{
		CreateHitEff();
	}

	// �� ����Ʈ�� ����Ѵ�.
	CreateBloodEff();

	// DT ���ο� ��û
	tEvent tevt = {};
	tevt.eType = EVENT_TYPE::SLOWTIME;
	CEventMgr::GetInst()->AddEvent(tevt);

	m_bSlowROnce = true;
}

void CPlayerScript::Attack_tick()
{
	// ���콺 ��ġ�� ���� �÷��̾� ȸ���� ���� ���� true�� �ֱ�
	m_bLockRot = true;

	// �����ð� ���
	m_fAccTime += DT;

	Vec2 DTVec2 = CalculateDTPos();

	// �̵��� ���� �Ÿ�
	m_fAccDestance += sqrt((DTVec2.x * DTVec2.x) + (DTVec2.y * DTVec2.y));

	// 0.45�ʱ����� ���� �ݴ� �������� ����
	if (m_fAccTime < 0.45f)
	{
		// ��ǥ �Ÿ� ����
		m_fTargetDestance = 20.f;

		// �����Ÿ��� ���� �������� �ʾ�����,
		if (abs(m_fTargetDestance) > abs(m_fAccDestance))
		{
			// �ݴ� �������� ��ǥ �Ÿ� ���� �̵�
			MoveOppoDir(DTVec2.x, DTVec2.y);
		}
		// ��ǥ �̵��Ÿ��� �Ѿ� ���� ���
		else
		{
			// �ִ� �̵� �Ÿ� ����
			PosOnTile();
			MoveOppoDir(20.f * cos(m_fSeta), 20.f * sin(m_fSeta));
		}
	}
	// 0.55�ʱ����� ���
	else if (m_fAccTime >= 0.45f && m_fAccTime < 0.55f)
	{
		// ��ǥ �̵� �Ÿ� ������ (20 + 40)
		m_fTargetDestance = 60.f;

		// ���� �̵��Ÿ� �ʱ�ȭ
		m_fAccDestance = 0.f;
	}
	// ��� �ð��� ������, ���� �������� ���� ����
	else
	{
		if (m_fTargetDestance == 60.f)
		{
			// ���� ����Ʈ ���� : �ѹ��� �����Ǿ�� �Ѵ�.
			if (m_OnceEffect)
			{
				CreateAttackEff();
				m_OnceEffect = false;
			}

			if (abs(m_fTargetDestance) > abs(m_fAccDestance))
			{
				// ���� ���� �������� �̵�
				MoveOriginDir(DTVec2.x, DTVec2.y);
			}
			// ���� ���� ��ǥ �Ÿ����� �̵��� �ߴٸ�,
			else
			{
				// Player�� ��ǥ�� ����� �ʵ��� �̵� ����
				// ���� �������� �����ͼ�
				PosOnTile();
				MoveOriginDir(40.f * cos(m_fSeta), 40.f * sin(m_fSeta));

				// ��ǥ �̵� �Ÿ� ������ (40)
				m_fTargetDestance = 40.f;

				// ���� �̵��Ÿ� �ʱ�ȭ
				m_fAccDestance = 0.f;

				// ������ �ǰ� ��� ����
				m_pGameMgrScript->TargetMonHit();
			}
		}
		else if (m_fTargetDestance == 40.f)
		{
			if (abs(m_fTargetDestance) > abs(m_fAccDestance))
			{
				MoveOppoDir(DTVec2.x, DTVec2.y);
			}

			// ���� Player�� ���� �����ǰ� ������ ���� �����.
			else
			{
				PosOnTile();
				m_bLockRot = true;
				m_bAtteck = false;
				MoveInit();
			}
		}
	}
}

void CPlayerScript::Move_tick()
{
	// �̹� tick���� ��ǥ���� �̵��ؾ��� x,y���� ���
	Vec2 DTVec2 = CalculateDTPos();

	// �̵��� ���� �Ÿ�
	m_fAccDestance += sqrt((DTVec2.x * DTVec2.x) + (DTVec2.y * DTVec2.y));


	// ��ǥ �̵� �Ÿ��� DT���� �̵��Ÿ� �������� Ȯ���ؼ� ��� Transform�� �ű��� ���Ѵ�.
	// ���� �̹��� �̵��ϰ����� �Ÿ� �������� ��ǥ���� ũ�ٸ�,
	if (abs(m_fTargetDestance) < abs(m_fAccDestance))
	{
		// ��ǥ�������� Player�� ��ġ�� �ű��, �������� ������.
		// PlayerChangeTile()�� Ÿ�� ���� ����� Ÿ������ �������� ����ֵ��� �Ǿ��ִ�.
		m_pGameMgrScript->PlayerChangeTile(m_pMoveTarget);
		MoveInit();
		m_bMove = false;
	}
	// �ƴ϶��, DT��ŭ �̵��ؾ��� ���������� �̵��Ѵ�.
	else
	{
		MoveOriginDir(DTVec2.x, DTVec2.y);
	}
}

void CPlayerScript::Hit_tick()
{
	if (m_bOneMoretick)
	{
		m_bHit = false;
		m_bOneMoretick = false;
		return;
	}

	// ���콺 ��ġ�� ���� �÷��̾� ȸ���� ���� ���� true�� �ֱ�
	m_bLockRot = true;

	// �����ð� ���
	m_fAccTime += DT;

	// 0.1�ʰ� ������,DTSlow ȿ���� ������ �ش�.
	if (m_bSlowROnce && m_fAccTime > 0.1f)
	{
		CTimeMgr::GetInst()->ReleaseSlow();
		m_bSlowROnce = false;
	}

	// ������ ���� ���� �ִϸ��̼��� 0.55���̴�.
	if (m_fAccTime < 0.55f)
	{
		if (!m_bStepOnce)
		{
			m_bStepOnce = true;
			// ù ��° ����� ��,
			if (m_iStep == 0)
			{
				// 90�� �������� 5��ŭ �̵�
				m_fSeta = 1.571;
				m_fTargetDestance = 5;
				m_fAccDestance = 0;
			}
			// �� ��° ���
			else if (m_iStep == 1)
			{
				// -15�� �������� 15 �̵�
				m_fTargetDestance = 15;
				m_fSeta = (-0.2618);
				m_fAccDestance = 0;
			}
			// �� ��° ���
			else if (m_iStep == 2)
			{
				// -165�� �������� 30 �̵�
				m_fTargetDestance = 30;
				m_fSeta = (-2.88);
				m_fAccDestance = 0;
			}
			// �� ��° ���
			else if (m_iStep == 3)
			{
				// -15�� �������� 15 �̵�
				m_fTargetDestance = 15;
				m_fSeta = (-0.2618);
				m_fAccDestance = 0;
			}
			// �ټ� ��° ���
			else if (m_iStep == 4)
			{
				// 105�� �������� 15 �̵�
				m_fTargetDestance = 15;
				m_fSeta = 1.833;
				m_fAccDestance = 0;
			}
			// ���� ��° ���
			else if (m_iStep == 5)
			{
				// 0�� �������� 8 �̵�
				m_fTargetDestance = 8;
				m_fSeta = 0;
				m_fAccDestance = 0;
			}
			// �ϰ� ��° ���
			else if (m_iStep == 6)
			{
				// 180�� �������� 8 �̵�
				m_fTargetDestance = 8;
				m_fSeta = 3.142;
				m_fAccDestance = 0;
			}
			// ���� ��° ���
			else if (m_iStep == 7)
			{
				// 0�� �������� 8 �̵�
				m_fTargetDestance = 8;
				m_fSeta = 0;
				m_fAccDestance = 0;
			}
			// ��ȩ ��° ���
			else if (m_iStep == 8)
			{
				// -45�� �������� 10 �̵�
				m_fTargetDestance = 10;
				m_fSeta = (-0.7854);
				m_fAccDestance = 0;
			}
		}


		// �̹� tick���� ��ǥ���� �̵��ؾ��� x,y���� ���
		Vec2 DTVec2 = CalculateDTPos();

		// �̵��� ���� �Ÿ�
		m_fAccDestance += sqrt((DTVec2.x * DTVec2.x) + (DTVec2.y * DTVec2.y));
		
		// ���� �̵� �κ�
		if (m_fTargetDestance > m_fAccDestance)
		{
			MoveOriginDir(DTVec2.x, DTVec2.y);
		}
		else
		{	
			// ��ǥ ������ ���� ���ϰ� ���� �� ���� ������� ����
			PosOnTile();
			m_bLockRot = true;
			if (m_iStep == 0)
			{
				MoveOriginDir(5 * cos(m_fSeta), 5 * sin(m_fSeta));
				m_iStep = 1; // ���� ������� ����
			}
			else if (m_iStep == 1)
			{
				MoveOriginDir(15 * cos(m_fSeta), 15 * sin(m_fSeta));
				m_iStep = 2; // ���� ������� ����
			}
			else if (m_iStep == 2)
			{
				MoveOriginDir(30 * cos(m_fSeta), 30 * sin(m_fSeta));
				m_iStep = 3; // ���� ������� ����
			}
			else if (m_iStep == 3)
			{
				MoveOriginDir(15 * cos(m_fSeta), 15 * sin(m_fSeta));
				m_iStep = 4; // ���� ������� ����
			}
			else if (m_iStep == 4)
			{
				MoveOriginDir(15 * cos(m_fSeta), 15 * sin(m_fSeta));
				m_iStep = 5; // ���� ������� ����
			}
			else if (m_iStep == 5)
			{
				MoveOriginDir(8 * cos(m_fSeta), 8 * sin(m_fSeta));
				m_iStep = 6; // ���� ������� ����
			}
			else if (m_iStep == 6)
			{
				MoveOriginDir(8 * cos(m_fSeta), 8 * sin(m_fSeta));
				m_iStep = 7; // ���� ������� ����
			}
			else if (m_iStep == 7)
			{
				MoveOriginDir(8 * cos(m_fSeta), 8 * sin(m_fSeta));
				m_iStep = 8; // ���� ������� ����
			}
			else if (m_iStep == 8)
			{
				MoveOriginDir(8 * cos(m_fSeta), 8 * sin(m_fSeta));
				PosOnTile();
				m_bOneMoretick = true;
				MoveInit();
				m_iStep = 0;
				// CTimeMgr::GetInst()->ReleaseSlow();
			}

			m_bStepOnce = false;
		}
	}
}

void CPlayerScript::MoveInit()
{
	m_pMoveTarget = nullptr;
	m_vDifPos = Vec2(0.f,0.f);
	m_fTargetDestance = 0.f;
	m_fAccDestance = 0.f;
	m_fSeta = 0.f;

	m_fSpeed = 320.f;

	// ���� ��� ����� ���Ǵ� ���� �ð��ʱ�ȭ
	m_fAccTime = 0.f;
}

void CPlayerScript::MoveOriginDir(float _x, float _y)
{
	// �÷��̾��� �������� �����´�.
	Vec3 vPlayerPos = Transform()->GetRelativePos();
	// ������������ �̵��ϵ��� ����
	vPlayerPos.x += _x;
	vPlayerPos.y += _y;

	// �������� �Է����ش�.
	Transform()->SetRelativePos(vPlayerPos);
}

void CPlayerScript::MoveOppoDir(float _x, float _y)
{
	// �÷��̾��� �������� �����´�.
	Vec3 vPlayerPos = Transform()->GetRelativePos();
	// �ݴ�������� �̵��ϵ��� ����
	vPlayerPos.x -= _x;
	vPlayerPos.y -= _y;

	// �������� �Է����ش�.
	Transform()->SetRelativePos(vPlayerPos);
}

Vec2 CPlayerScript::CalculateDTPos()
{
	// (�ӵ� + ���ӵ� * DT) * DT = �̹� DT���� �̵� �Ÿ�
	m_fSpeed += m_fAccel * DT;
	float DestanceDT = m_fSpeed * DT;

	// �̹� DT���� �̵��� X�� = Rdt * cos(Seta)
	float xDT = DestanceDT * cos(m_fSeta);

	// �̹� DT���� �̵��� Y�� = Rdt * sin(Seta)
	float yDT = DestanceDT * sin(m_fSeta);

	return Vec2(xDT, yDT);
}

void CPlayerScript::CalculateTarget(CTileScript* _pTargetTile)
{
	m_pMoveTarget = _pTargetTile;
	Vec3 TargetPos = _pTargetTile->Transform()->GetWorldPos();
	// ���� �������� ��ġ�ؾ��� �÷��̾��� ��ǥ ��ǥ�� �����Ѵ�.
	TargetPos.x += m_vPlayerPos.x;
	TargetPos.y += m_vPlayerPos.y;

	Vec3 OriginPos = GetOwner()->Transform()->GetRelativePos();

	// ���� ���ϱ�
	m_vDifPos = Vec2(TargetPos.x - OriginPos.x, TargetPos.y - OriginPos.y);

	// �� �� ������ ���� ���ϱ�
	m_fSeta = atan2f(m_vDifPos.y, m_vDifPos.x);
}

// =========================
// =    ����Ʈ ���� �Լ�    =
// =========================

void CPlayerScript::CreateAttackEff()
{
	// ���� ���� ī�� ���ٸ�, ���� ��� �������� Ȯ���� �˸��� ���� ����Ʈ�� ������ش�.
	if (m_eDir == CARD_DIR_TYPE::RIGHT)
	{
		switch (m_eRightWaponType)
		{
		case WAPON_TYPE::KATANA:
		{
			CreateSlashEff();

			CreateSlashSound();
			break;
		}
			
		case WAPON_TYPE::SWORD:
		{
			CreateSlashEff();

			CreateSlashSound();
			break;
		}
			
		case WAPON_TYPE::SHIELD:
		{
			CreatePunchEff();

			// ��ġ ȿ���� ���
			CreatePunchSound();
			break;
		}
			
		}
	}
	else
	{
		switch (m_eLeftWaponType)
		{
		case WAPON_TYPE::KATANA:
		{
			CreateSlashEff();

			CreateSlashSound();
			break;
		}

		case WAPON_TYPE::SWORD:
		{
			CreateSlashEff();

			CreateSlashSound();
			break;
		}

		case WAPON_TYPE::SHIELD:
		{
			CreatePunchEff();
			
			CreatePunchSound();
			break;
		}

		}
	}
}

void CPlayerScript::CreatePunchSound()
{
	srand(time(nullptr));
	int iRand = rand() % 2;

	if (iRand == 0)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Punch\\Injured_Punch_4(bullet_impact_body_thump_03).wav");
		pSound->Play(1, 1.0f, true);
	}
	else
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Punch\\Injured_Punch_1(punch_grit_wet_impact_05).wav");
		pSound->Play(1, 1.0f, true);
	}
}

void CPlayerScript::CreateSlashSound()
{
	srand(time(nullptr));
	int iRand = rand() % 3;

	if (iRand == 0)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Slash\\Slash_Small_0(SH101_Sword Attack_Hollywood_Fienup_004).wav");
		pSound->Play(1, 1.0f, true);
	}
	else if(iRand == 1)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Slash\\Slash_Small_1(whoosh_weapon_knife_swing_01).wav");
		pSound->Play(1, 1.0f, true);
	}
	else
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Slash\\Slash_Small_2(whoosh_weapon_knife_swing_04).wav");
		pSound->Play(1, 1.0f, true);
	}
}

void CPlayerScript::CreateHitSound()
{
	srand(time(nullptr));
	int iRand = rand() % 4;

	if (iRand == 0)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\SwordHit\\Sword_Hit_Flesh_1.wav");
		pSound->Play(1, 1.0f, true);
	}
	else if (iRand == 1)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\SwordHit\\Sword_Hit_Flesh_2.wav");
		pSound->Play(1, 1.0f, true);
	}
	else if (iRand == 2)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\SwordHit\\Sword_Hit_Flesh_3.wav");
		pSound->Play(1, 1.0f, true);
	}
	else
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\SwordHit\\Sword_Hit_Flesh_4.wav");
		pSound->Play(1, 1.0f, true);
	}
}

void CPlayerScript::CreateHitShieldSound()
{
	srand(time(nullptr));
	int iRand = rand() % 2;

	if (iRand == 0)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\SwordHit\\sword_hit_impact_heavy_03.wav");
		pSound->Play(1, 1.0f, true);
	}
	else if (iRand == 1)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\SwordHit\\sword_hit_impact_heavy_05.wav");
		pSound->Play(1, 1.0f, true);
	}	
}

void CPlayerScript::CreateAddShieldSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Add\\collect_item_11.wav");
	pSound->Play(1, 1.0f, true);
}

void CPlayerScript::AddEventUnderAttack()
{
	CGameObject* pMainCam = CLevelMgr::GetInst()->GetCurLevel()->FindObjectByName(L"MainCamera");
	CUnderAttackScript* pEffScript = (CUnderAttackScript*)pMainCam->GetChildObject(L"Eff_UnderAttack")->GetScript((UINT)UNDERATTACKSCRIPT);
	pEffScript->RenderOn();
}

void CPlayerScript::MoveSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Slash\\Slash_Small_0(SH101_Sword Attack_Hollywood_Fienup_004).wav");
	pSound->Play(1, 1.0f, true);
}

void CPlayerScript::CreateHitEff()
{
	Ptr<CPrefab> pHitPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\HitEffect_Prefab_0.pref");

	CGameObject* pHitEffect = pHitPrefab->Instantiate();

	// ���� �� begin ȣ��
	pHitEffect->begin();

	// ������ ���� z �� ����
	pHitEffect->Transform()->SetRelativePos_Z(-3);

	// ������ z��ȸ�� �Է�
	srand(time(nullptr));
	int deg = rand() % 180;
	float rad = deg / XM_2PI;
	pHitEffect->Transform()->SetRelativeRotation(0.f, 0.f, rad);

	// �ڽ����� �����Ѵ�.
	GetOwner()->AddChild(pHitEffect);

	// ���̾� ����
	pHitEffect->SetAllLayer(11);
}

void CPlayerScript::CreateArrowHitEff(Vec3 TargetPos)
{
	m_bArrowHit = false;

	Ptr<CPrefab> pHitPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Eff_ArrowHit_Prefab_0.pref");

	CGameObject* pHitEffect = pHitPrefab->Instantiate();

	Vec3 OriginPos = Transform()->GetRelativePos();

	// ���� ���ϱ�
	Vec2 vDifPos = Vec2(TargetPos.x - OriginPos.x, TargetPos.y - OriginPos.y);

	// �� ���� ������ ���� ���ϱ�
	float fSeta = atan2f(vDifPos.y, vDifPos.x);

	// ������ŭ ����Ʈ ȸ��
	pHitEffect->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, fSeta));

	// ���� ���� ����
	Vec3 vPlayerPos = Transform()->GetRelativePos();
	vPlayerPos.x += 35.f * cos(fSeta);
	vPlayerPos.y += 35.f * sin(fSeta);

	pHitEffect->Transform()->SetRelativePos(vPlayerPos);
	pHitEffect->Transform()->SetRelativePos_Z(0.f);

	tEvent tEvnt = {};
	tEvnt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvnt.wParam = (DWORD_PTR)pHitEffect;
	tEvnt.lParam = (DWORD_PTR)11;

	CEventMgr::GetInst()->AddEvent(tEvnt);
	// ����Ʈ �ִϸ��̼� ����
	pHitEffect->Animator2D()->Play(L"Eff_ArrowHit", false);
}

void CPlayerScript::CreateBloodEff()
{
	Ptr<CPrefab> pEffPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Eff_Blood_Prefab_0.pref");

	CGameObject* pEffect = pEffPrefab->Instantiate();

	// �ڽ����� �����Ѵ�.
	GetOwner()->AddChild(pEffect);

	// ���̾� ����
	pEffect->SetAllLayer(11);

	// �� ȿ�� �ִϸ��̼� ���� ����
	CBloodEffectScript* pBloodScript = (CBloodEffectScript*)pEffect->GetScript((UINT)BLOODEFFECTSCRIPT);
	pBloodScript->PlayRandAnim();
}

void CPlayerScript::CreateSlashEff()
{
	Ptr<CPrefab> pEffPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Eff_Slash_Prefab_0.pref");

	CGameObject* pEffect = pEffPrefab->Instantiate();

	// ����Ʈ ȸ��
	Vec3 vRot = pEffect->Transform()->GetRelativeRotation();

	// y���� ȸ���� ������ �ش�.
	if (1.5707963268f > m_fSeta && m_fSeta > -1.5707963268f)
	{
		vRot.y = -3.1415926536f;
		vRot.z += m_fSeta;
	}

	// ������
	else
	{
		vRot.y = 0.f;
		vRot.z += m_fSeta;
	}

	pEffect->Transform()->SetRelativeRotation(vRot);

	// ����Ʈ ������ ����
	Vec3 vPos = m_pOwnerTile->Transform()->GetWorldPos();

	// ��ǥ �̵��� = 85
	// ��ǥ ���������� ��Ÿ�� ���ؼ� �����ؾ� �Ѵ�.
	if (m_fSeta == 0.f || m_fSeta == 3.14159274f)
	{
		vPos.x += cos(m_fSeta) * 35;
		vPos.y += sin(m_fSeta) * 35;
	}

	vPos.x += cos(m_fSeta) * 55;
	vPos.y += sin(m_fSeta) * 55;
	vPos.z = 0.f;

	pEffect->Transform()->SetRelativePos(vPos);
	

	tEvent tEvnt = {};
	tEvnt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvnt.wParam = (DWORD_PTR)pEffect;
	tEvnt.lParam = (DWORD_PTR)11;

	CEventMgr::GetInst()->AddEvent(tEvnt);

	// ����Ʈ �ִϸ��̼� ����
	pEffect->Animator2D()->Play(L"Eff_Slash", false);
}

void CPlayerScript::CreatePunchEff()
{
	Ptr<CPrefab> pEffPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Eff_Punch_Prefab_0.pref");

	CGameObject* pEffect = pEffPrefab->Instantiate();

	// ����Ʈ ȸ��
	Vec3 vRot = pEffect->Transform()->GetRelativeRotation();

	// y���� ȸ���� ������ �ش�.
	if (1.5707963268f > m_fSeta && m_fSeta > -1.5707963268f)
	{
		vRot.y = -3.1415926536f;
		vRot.z += m_fSeta;
	}

	// ������
	else
	{
		vRot.y = 0.f;
		vRot.z += m_fSeta;
	}

	pEffect->Transform()->SetRelativeRotation(vRot);

	// ����Ʈ ������ ����
	Vec3 vPos = m_pOwnerTile->Transform()->GetWorldPos();

	// ��ǥ �̵��� = 85
	// ��ǥ ���������� ��Ÿ�� ���ؼ� �����ؾ� �Ѵ�.
	if (m_fSeta == 0.f || m_fSeta == 3.14159274f)
	{
		vPos.x += cos(m_fSeta) * 35;
		vPos.y += sin(m_fSeta) * 35;
	}

	vPos.x += cos(m_fSeta) * 55;
	vPos.y += sin(m_fSeta) * 55;
	vPos.z = 0.f;

	pEffect->Transform()->SetRelativePos(vPos);


	tEvent tEvnt = {};
	tEvnt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvnt.wParam = (DWORD_PTR)pEffect;
	tEvnt.lParam = (DWORD_PTR)11;

	CEventMgr::GetInst()->AddEvent(tEvnt);

	// ����Ʈ �ִϸ��̼� ����
	pEffect->Animator2D()->Play(L"Eff_Punch", false);
}

void CPlayerScript::CreateShieldParticle()
{
	Ptr<CPrefab> pEffPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Par_Shield_Prefab_0.pref");

	CGameObject* pEffect = pEffPrefab->Instantiate();

	// ���� ���� �������ֱ�
	Vec3 vParitclePos = Transform()->GetRelativePos();

	vParitclePos.y -= 20.f;
	vParitclePos.z = 0.f;

	pEffect->Transform()->SetRelativePos(vParitclePos);

	tEvent tEvnt = {};
	tEvnt.eType = EVENT_TYPE::CREATE_OBJECT;
	tEvnt.wParam = (DWORD_PTR)pEffect;
	tEvnt.lParam = (DWORD_PTR)11;

	CEventMgr::GetInst()->AddEvent(tEvnt);
}

// =========================
// =    ����� ���� �Լ�    =
// =========================

void CPlayerScript::CreateWaponCard()
{
	Ptr<CPrefab> pWaponPrefab = nullptr;

	// īŸ���� ��� �����ո� �̿��� ����� �ȴ�.
	if (m_tRightWapon.eWaponType == WAPON_TYPE::KATANA)
	{
		pWaponPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_Katana_Prefab_0.pref");

		CGameObject* pRWaopon = pWaponPrefab->Instantiate();

		// ������ ���� z �� ����
		pRWaopon->Transform()->SetRelativePos_Z(-1);

		// UIMgr�� ������ �ڽ����� �����Ѵ�.
		m_pGameMgrScript->GetUIMgrScript()->GetOwner()->GetChildObject(L"RightWaponSlot")->AddChild(pRWaopon);

		// ���̾� ����
		pRWaopon->SetAllLayer(10);

		// �ش� ������ ��ũ��Ʈ �ּҸ� ������ �����صд�.
		if (nullptr != pRWaopon)
		{
			CWaponScript* pTargetScript = GetWaponScript(pRWaopon);

			if (nullptr != pTargetScript)
			{
				m_pRightWapon = pTargetScript;
			}
		}
		
		// ������ ���� ���� Ÿ�Կ� �ش� ������ Ÿ���� �����Ѵ�.
		m_eRightWaponType = m_tRightWapon.eWaponType;
		m_eLeftWaponType = m_tRightWapon.eWaponType;
	}
	else
	{
		// ���� ���� ���� ����
		switch (m_tRightWapon.eWaponType)
		{
		case WAPON_TYPE::SWORD:
		{
			pWaponPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_SwordBasic_Prefab_0.pref");
			break;
		}

		case WAPON_TYPE::SHIELD:
		{
			pWaponPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_ShieldBasic_Prefab_0.pref");
			break;
		}
		}

		// ���� ���� ��ü ����
		CGameObject* pRWaopon = pWaponPrefab->Instantiate();

		// ������ ���� z �� ����
		pRWaopon->Transform()->SetRelativePos_Z(-1);

		// UIMgr�� ������ �ڽ����� �����Ѵ�.
		m_pGameMgrScript->GetUIMgrScript()->GetOwner()->GetChildObject(L"RightWaponSlot")->AddChild(pRWaopon);

		// ���̾� ����
		pRWaopon->SetAllLayer(10);

		// �ش� ������ ��ũ��Ʈ �ּҸ� ������ �����صд�.
		if (nullptr != pRWaopon)
		{
			CWaponScript* pTargetScript = GetWaponScript(pRWaopon);

			if (nullptr != pTargetScript)
			{
				m_pRightWapon = pTargetScript;
			}
		}

		// ������ ���� ���� Ÿ�Կ� �ش� ������ Ÿ���� �����Ѵ�.
		m_eRightWaponType = m_tRightWapon.eWaponType;


		// �޼� ����
		switch (m_tLefetWapon.eWaponType)
		{
		case WAPON_TYPE::SWORD:
		{
			pWaponPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_SwordBasic_Prefab_0.pref");
			break;
		}

		case WAPON_TYPE::SHIELD:
		{
			pWaponPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Card_ShieldBasic_Prefab_0.pref");
			break;
		}
		}

		// ���� ���� ��ü ����
		CGameObject* pLWaopon = pWaponPrefab->Instantiate();

		// ������ ���� z �� ����
		pLWaopon->Transform()->SetRelativePos_Z(-1);

		// UIMgr�� ������ �ڽ����� �����Ѵ�.
		m_pGameMgrScript->GetUIMgrScript()->GetOwner()->GetChildObject(L"LeftWaponSlot")->AddChild(pLWaopon);

		// ���̾� ����
		pLWaopon->SetAllLayer(10);

		// �ش� ������ ��ũ��Ʈ �ּҸ� ������ �����صд�.
		if (nullptr != pLWaopon)
		{
			CWaponScript* pTargetScript = GetWaponScript(pLWaopon);

			if (nullptr != pTargetScript)
			{
				m_pLeftWapon = pTargetScript;
			}
		}

		// ������ ���� ���� Ÿ�Կ� �ش� ������ Ÿ���� �����Ѵ�.
		m_eLeftWaponType = m_tLefetWapon.eWaponType;
	}


}


// ȸ�� ������ ���� ȣ��
void CPlayerScript::RecoveryCurHP(int _Recovery)
{
	m_tPlayerInfo.Cur_HP += _Recovery;

	// �ִ� ü���� ���� ���ϵ��� ����
	if (m_tPlayerInfo.Cur_HP >= m_tPlayerInfo.Max_HP)
	{
		m_tPlayerInfo.Cur_HP = m_tPlayerInfo.Max_HP;
	}
}

// =========================
// =    ī�嵦 ���� �Լ�    =
// =========================

void CPlayerScript::DefaultCardSetting()
{
	// �⺻ ���� ī�� 3�� �ֱ� (�� ���� ����)
	tPlayerCardInfo CardInfo = {};
	CardInfo.CardName = CARD_TYPE::ATTACK;
	CardInfo.Reinforce = false;
	CardInfo.CardDir = CARD_DIR_TYPE::RIGHT;

	for (int i = 0; i < 3; ++i)
	{
		m_arrCardList.push_back(CardInfo);
	}

	CardInfo.CardDir = CARD_DIR_TYPE::LEFT;

	for (int i = 0; i < 3; ++i)
	{
		m_arrCardList.push_back(CardInfo);
	}

	// �⺻ ��� ī�� 3�� �ֱ� (�� ���� ����)
	CardInfo.CardName = CARD_TYPE::DEFFENCE;
	CardInfo.Reinforce = false;
	CardInfo.CardDir = CARD_DIR_TYPE::RIGHT;

	for (int i = 0; i < 3; ++i)
	{
		m_arrCardList.push_back(CardInfo);
	}

	CardInfo.CardDir = CARD_DIR_TYPE::LEFT;

	for (int i = 0; i < 3; ++i)
	{
		m_arrCardList.push_back(CardInfo);
	}
}

// =========================
// =    ����� ���� �Լ�    =
// =========================

void CPlayerScript::SetDebuf(DEBUF_TYPE _eDeBufType)
{
	switch (_eDeBufType)
	{
	case DEBUF_TYPE::RESTRAIN:
	{
		m_iPlayerDebuf |= RESTRAIN_DEBUF;
		break;
	}
	}
}

bool CPlayerScript::CheckDebuf(DEBUF_TYPE _eDeBufType)
{
	switch (_eDeBufType)
	{
	case DEBUF_TYPE::RESTRAIN:
	{
		if (m_iPlayerDebuf & RESTRAIN_DEBUF)
			return true;
		else
			return false;

		break;
	}
	}
}

void CPlayerScript::ReleaseDebuf(DEBUF_TYPE _eDeBufType)
{
	switch (_eDeBufType)
	{
	case DEBUF_TYPE::RESTRAIN:
	{
		m_iPlayerDebuf &= ~RESTRAIN_DEBUF;
		break;
	}
	}
}




void CPlayerScript::BeginOverlap(CCollider2D* _pOther)
{
	// ������ ȭ���� ��� �ǰ� ����
	if (nullptr != _pOther->GetOwner()->GetScript((UINT)MISSILESCRIPT))
	{
		m_bArrowHit = true;

		// ȭ���� �μ���, ������ �ǰݵǱ�
		CMissileScript* pArrowScript = (CMissileScript*)_pOther->GetOwner()->GetScript((UINT)MISSILESCRIPT);
		Hit(pArrowScript->GetATK());

		CreateArrowHitEff(_pOther->Transform()->GetRelativePos());

		_pOther->GetOwner()->Destroy();
	}
}

void CPlayerScript::Overlap(CCollider2D* _pOther)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _pOther)
{
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	// �θ��ʿ��� ������ �����Ͱ� ���� �� ������ ȣ�����ش�.
	CScript::SaveToFile(_File);

	fwrite(&m_fSpeed, sizeof(float), 1, _File);

	SaveResourceRef(m_Prefab, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);

	fread(&m_fSpeed, sizeof(float), 1, _File);
	LoadResourceRef(m_Prefab, _File);
}

