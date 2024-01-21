#include "pch.h"
#include "CPlayerLeftHandScript.h"

#include "CPlayerScript.h"

CPlayerLeftHandScript::CPlayerLeftHandScript()
	: CScript(PLAYERLEFTHANDSCRIPT)
	, m_bAttack(false)
	, m_bIDLE(false)
{
}

CPlayerLeftHandScript::~CPlayerLeftHandScript()
{
}

void CPlayerLeftHandScript::begin()
{
}

void CPlayerLeftHandScript::tick()
{
	// ���� ��� ���� z�� ������ �켱���� �����ֱ�
	if (m_bAttack)
	{
		switch (m_eWaponType)
		{

		case WAPON_TYPE::SWORD:
		{
			if (m_eAttackDir == CARD_DIR_TYPE::RIGHT)
			{
				RAttack_L_Sword_tick();
			}
			else
			{
				LAttack_L_Sword_tick();
			}

			break;
		}

		case WAPON_TYPE::SHIELD:
		{
			if (m_eAttackDir == CARD_DIR_TYPE::RIGHT)
			{
				RAttack_L_Shield_tick();
			}
			else
			{
				LAttack_L_Shield_tick();
			}

			break;
		}
		}
	}

	if (m_bIDLE)
	{
		if (GetPlayerScript()->Is_Rot())
			Transform()->SetRelativePos_Z(-1);
		else
			Transform()->SetRelativePos_Z(1);
	}
}

void CPlayerLeftHandScript::LAttackAnimPlay(WAPON_TYPE _WAPONTYPE, CARD_DIR_TYPE _DIRTYPE)
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
			Animator2D()->Play(L"RAttack_L_Sword", false);
		}
		else if (_DIRTYPE == CARD_DIR_TYPE::LEFT)
		{
			Animator2D()->Play(L"LAttack_L_Sword", false);
		}
	}
	else if (_WAPONTYPE == WAPON_TYPE::SHIELD)
	{
		if (_DIRTYPE == CARD_DIR_TYPE::RIGHT)
		{
			Animator2D()->Play(L"RAttack_L_Shield", false);
		}
		else if (_DIRTYPE == CARD_DIR_TYPE::LEFT)
		{
			Animator2D()->Play(L"LAttack_L_Shield", false);
		}
	}
}

void CPlayerLeftHandScript::RAttack_L_Sword_tick()
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

void CPlayerLeftHandScript::LAttack_L_Sword_tick()
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

void CPlayerLeftHandScript::RAttack_L_Shield_tick()
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

void CPlayerLeftHandScript::LAttack_L_Shield_tick()
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


void CPlayerLeftHandScript::IDLEAnimPlayer(WAPON_TYPE _WAPONTYPE)
{
	m_bAttack = false;
	m_bIDLE = true;

	Transform()->SetRelativeScale(1.f, 1.f, 1.f);

	if (_WAPONTYPE == WAPON_TYPE::SWORD)
	{
		Animator2D()->Play(L"IDLE_L_Sword", true);
	}
	else if (_WAPONTYPE == WAPON_TYPE::SHIELD)
	{
		Animator2D()->Play(L"IDLE_L_Shield", true);
	}
}

void CPlayerLeftHandScript::StopAnim()
{
	Animator2D()->Stop();

	// ũ�⵵ 0�����ؼ� �ƿ� ����� ���ϵ��� �����. (����Ÿ ���� Ƣ��� �� �ֱ� ����)
	Transform()->SetRelativeScale(0.f, 0.f, 1.f);
}

void CPlayerLeftHandScript::SaveToFile(FILE* _File)
{
	CScript::SaveToFile(_File);
}

void CPlayerLeftHandScript::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);
}