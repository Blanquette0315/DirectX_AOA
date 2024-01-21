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

	// Player�� ��ũ��Ʈ�� ������ ü���� Ȯ���Ѵ�.
	m_iMaxHP = PlayerScript->GetMAXHP();
	m_iCurHP = PlayerScript->GetCURHP();

	// ü�¹ٸ� ǥ���ϱ� ���� �ۼ�Ʈ�� ���Ѵ�.
	float pRatio = (float)m_iCurHP / (float)m_iMaxHP;

	// ������ Shader�� �����Ѵ�.
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &pRatio);

	Vec3 vScale = PlayerScript->Transform()->GetRelativeScale();

	float y = vScale.y * (-0.5f);

	// ���� ���Ͱ� �ݴ븦 ���� ������ ü�¹ٸ� �ٽ� ������ �־�� �Ѵ�.
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
	// ü���� ��ġ�� �ڽ��� ���� Number Script���� �������ش�.
	int Num0 = m_iCurHP / 100;
	CNumberScript* pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"HPNum_0")->GetScript((UINT)NUMBERSCRIPT));
	// ���� ù�ڸ��� 0�̸� ����� �ȵǾ�� �ϹǷ� ����ó���� ���ش�.
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