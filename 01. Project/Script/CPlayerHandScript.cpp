#include "pch.h"
#include "CPlayerHandScript.h"
#include <Engine/CAnimation2D.h>

#include "CPlayerScript.h"

CPlayerHandScript::CPlayerHandScript()
	: CScript(PLAYERHANDSCRIPT)
	, m_bAttack(false)
	, m_bIDLE(false)
{
}

CPlayerHandScript::~CPlayerHandScript()
{
}

void CPlayerHandScript::begin()
{
}

void CPlayerHandScript::tick()
{
	// 공격 모션 도중 z값 관리로 우선순위 정해주기
	if (m_bAttack)
	{
		if (m_eWaponType == WAPON_TYPE::KATANA)
		{
			Attack_Katana_tick();
		}
	}

	if (m_bIDLE)
	{
		if (GetPlayerScript()->Is_Rot())
			Transform()->SetRelativePos_Z(1);
		else
			Transform()->SetRelativePos_Z(-1);
	}
}


void CPlayerHandScript::AttackAnimPlay(WAPON_TYPE _WAPONTYPE, CARD_DIR_TYPE _DIRTYPE)
{
	m_bAttack = true;
	m_bIDLE = false;

	m_eWaponType = _WAPONTYPE;
	m_eAttackDir = _DIRTYPE;

	Transform()->SetRelativeScale(1.f, 1.f, 1.f);

	if (_WAPONTYPE == WAPON_TYPE::KATANA)
	{
		Animator2D()->Play(L"Atteck_Katana", false);
	}
}

void CPlayerHandScript::IDLEAnimPlayer(WAPON_TYPE _WAPONTYPE)
{
	m_bAttack = false;
	m_bIDLE = true;

	m_eWaponType = _WAPONTYPE;

	if (_WAPONTYPE == WAPON_TYPE::KATANA)
	{
		Animator2D()->Play(L"IDLE_Katana", true);
	}
}

void CPlayerHandScript::StopAnim()
{
	Animator2D()->Stop();

	// 크기도 0으로해서 아에 출력을 안하도록 만든다. (마젠타 색이 튀어나올 수 있기 때문)
	Transform()->SetRelativeScale(0.f, 0.f, 1.f);
}



void CPlayerHandScript::Attack_Katana_tick()
{
	if (Animator2D()->GetCurAnimation() != nullptr)
	{
		int iCurAnimIdx = Animator2D()->GetCurAnimIdx();
		if (iCurAnimIdx == 0)
		{
			if (GetPlayerScript()->Is_Rot())
				Transform()->SetRelativePos_Z(-1);
			else
				Transform()->SetRelativePos_Z(1);
		}
		else if (iCurAnimIdx == 1)
		{
			if (GetPlayerScript()->Is_Rot())
				Transform()->SetRelativePos_Z(1);
			else
				Transform()->SetRelativePos_Z(-1);
		}
	}
}


void CPlayerHandScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CPlayerHandScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}