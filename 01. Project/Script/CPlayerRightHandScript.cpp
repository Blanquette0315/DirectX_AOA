#include "pch.h"
#include "CPlayerRightHandScript.h"
#include <Engine/CAnimation2D.h>

#include "CPlayerScript.h"

CPlayerRightHandScript::CPlayerRightHandScript()
	: CScript(PLAYERRIGHTHANDSCRIPT)
	, m_bAttack(false)
	, m_bIDLE(false)
{
}

CPlayerRightHandScript::~CPlayerRightHandScript()
{
}

void CPlayerRightHandScript::begin()
{
}

void CPlayerRightHandScript::tick()
{
	// 공격 모션 도중 z값 관리로 우선순위 정해주기
	if (m_bAttack)
	{
		switch (m_eWaponType)
		{

		case WAPON_TYPE::SWORD:
		{
			if (m_eAttackDir == CARD_DIR_TYPE::RIGHT)
			{
				RAttack_R_Sword_tick();
			}
			else
			{
				LAttack_R_Sword_tick();
			}

			break;
		}
			
		case WAPON_TYPE::SHIELD:
		{
			if (m_eAttackDir == CARD_DIR_TYPE::RIGHT)
			{
				RAttack_R_Shield_tick();
			}
			else
			{
				LAttack_R_Shield_tick();
			}

			break;
		}
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


void CPlayerRightHandScript::RAttackAnimPlay(WAPON_TYPE _WAPONTYPE, CARD_DIR_TYPE _DIRTYPE)
{
	m_bAttack = true;
	m_bIDLE = false;

	m_eWaponType = _WAPONTYPE;
	m_eAttackDir = _DIRTYPE;

	Transform()->SetRelativeScale(1.f, 1.f, 1.f);

	if (_WAPONTYPE == WAPON_TYPE::SWORD)
	{
		if (_DIRTYPE == CARD_DIR_TYPE::RIGHT)
		{
			Animator2D()->Play(L"RAttack_R_Sword", false);
		}
		else if (_DIRTYPE == CARD_DIR_TYPE::LEFT)
		{
			Animator2D()->Play(L"LAttack_R_Sword", false);
		}
	}
	else if (_WAPONTYPE == WAPON_TYPE::SHIELD)
	{
		if (_DIRTYPE == CARD_DIR_TYPE::RIGHT)
		{
			Animator2D()->Play(L"RAttack_R_Shield", false);
		}
		else if (_DIRTYPE == CARD_DIR_TYPE::LEFT)
		{
			Animator2D()->Play(L"LAttack_R_Shield", false);
		}
	}
}

void CPlayerRightHandScript::RAttack_R_Sword_tick()
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

void CPlayerRightHandScript::LAttack_R_Sword_tick()
{
	if (Animator2D()->GetCurAnimation() != nullptr)
	{
		int iCurAnimIdx = Animator2D()->GetCurAnimIdx();
		if (iCurAnimIdx == 0)
		{
			if (GetPlayerScript()->Is_Rot())
				Transform()->SetRelativePos_Z(1);
			else
				Transform()->SetRelativePos_Z(-1);
		}
		else if (iCurAnimIdx == 1)
		{
			if (GetPlayerScript()->Is_Rot())
				Transform()->SetRelativePos_Z(-1);
			else
				Transform()->SetRelativePos_Z(1);
		}
	}
}

void CPlayerRightHandScript::RAttack_R_Shield_tick()
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

void CPlayerRightHandScript::LAttack_R_Shield_tick()
{
	if (Animator2D()->GetCurAnimation() != nullptr)
	{
		int iCurAnimIdx = Animator2D()->GetCurAnimIdx();
		if (iCurAnimIdx == 0)
		{
			if (GetPlayerScript()->Is_Rot())
				Transform()->SetRelativePos_Z(1);
			else
				Transform()->SetRelativePos_Z(-1);
		}
		else if (iCurAnimIdx == 1)
		{
			if (GetPlayerScript()->Is_Rot())
				Transform()->SetRelativePos_Z(-1);
			else
				Transform()->SetRelativePos_Z(1);
		}
	}
}


void CPlayerRightHandScript::IDLEAnimPlayer(WAPON_TYPE _WAPONTYPE)
{
	m_bAttack = false;
	m_bIDLE = true;

	m_eWaponType = _WAPONTYPE;

	Transform()->SetRelativeScale(1.f, 1.f, 1.f);

	if (_WAPONTYPE == WAPON_TYPE::SWORD)
	{
		Animator2D()->Play(L"IDLE_R_Sword", true);
	}
	else if (_WAPONTYPE == WAPON_TYPE::SHIELD)
	{
		Animator2D()->Play(L"IDLE_R_Shield", true);
	}
}

void CPlayerRightHandScript::StopAnim()
{
	Animator2D()->Stop();

	// 크기도 0으로해서 아에 출력을 안하도록 만든다. (마젠타 색이 튀어나올 수 있기 때문)
	Transform()->SetRelativeScale(0.f, 0.f, 1.f);
}




void CPlayerRightHandScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CPlayerRightHandScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}