#include "pch.h"
#include "CPlayerHPBarScript.h"

#include "CNumberScript.h"
#include "CPlayerScript.h"

CPlayerHPBarScript::CPlayerHPBarScript()
	:CUIScript(PLAYERHPBARSCRIPT)
	, m_iMaxHP(0)
	, m_iCurHP(0)
{
}

CPlayerHPBarScript::~CPlayerHPBarScript()
{
}

void CPlayerHPBarScript::begin()
{
}

void CPlayerHPBarScript::tick()
{
	CPlayerScript* PlayerScript = (CPlayerScript*)GetOwner()->GetParent()->GetScript((UINT)PLAYERSCRIPT);

	// Player의 스크립트를 가져와 체력을 확인한다.
	m_iMaxHP = PlayerScript->GetMAXHP();
	m_iCurHP = PlayerScript->GetCURHP();

	// 체력바를 표기하기 위해 퍼센트를 구한다.
	float pRatio = (float)m_iCurHP / (float)m_iMaxHP;

	// 비율을 Shader에 전달한다.
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &pRatio);

	Vec3 vScale = PlayerScript->Transform()->GetRelativeScale();

	float y = vScale.y * (-0.5f);

	// 만약 몬스터가 반대를 보고 있으면 체력바를 다시 뒤집에 주어야 한다.
	if (PlayerScript->Is_Rot())
	{
		Transform()->SetRelativeRotation(0.f, 3.14f, 0.f);
		Transform()->SetRelativePos(0.f, y, 2.f);
	}
	else
	{
		Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
		Transform()->SetRelativePos(0.f, y, -2.f);
	}

	RenderNumber();
}

void CPlayerHPBarScript::RenderNumber()
{
	// 체력의 수치를 자식이 지닌 Number Script에게 전달해준다.
	int Num0 = m_iCurHP / 100;
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"HPNum_0")->GetScript((UINT)NUMBERSCRIPT));
	// 제일 첫자리가 0이면 출력이 안되어야 하므로 예외처리를 해준다.
	if (Num0 == 0)
	{
		pNumScript->SetNumber(-1);
	}
	else
	{
		pNumScript->SetNumber(Num0);
	}

	int Num1 = (m_iCurHP - (Num0 * 100)) / 10;
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"HPNum_1")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num1);

	int Num2 = ((m_iCurHP - (Num0 * 100)) - (10 * Num1));
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"HPNum_2")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num2);
}