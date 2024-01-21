#include "pch.h"
#include "CActMonHPBarScript.h"

#include "CNumberScript.h"
#include "CMonsterScript.h"

CActMonHPBarScript::CActMonHPBarScript()
	: CUIScript(ACTMONHPBARSCRIPT)
	, m_iMaxHP(0)
	, m_iCurHP(0)
{
}

CActMonHPBarScript::~CActMonHPBarScript()
{
}

void CActMonHPBarScript::begin()
{
	//CUIScript::begin();
}

void CActMonHPBarScript::tick()
{
	//CUIScript::tick();

	CMonsterScript* MonScript = GetMonsterScript(GetOwner()->GetParent());

	// Monster�� ��ũ��Ʈ�� ������ ü���� Ȯ���Ѵ�.
	m_iMaxHP = MonScript->GetMonMaxHP();
	m_iCurHP = MonScript->GetMonCurHP();

	// ü�¹ٸ� ǥ���ϱ� ���� �ۼ�Ʈ�� ���Ѵ�.
	float pRatio = (float)m_iCurHP / (float)m_iMaxHP;

	// ������ Shader�� �����Ѵ�.
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &pRatio);

	Vec3 vScale = MonScript->Transform()->GetRelativeScale();

	float y = vScale.y * (-0.5f);

	// ���� ���Ͱ� �ݴ븦 ���� ������ ü�¹ٸ� �ٽ� ������ �־�� �Ѵ�.
	if (MonScript->GetMonRot())
	{
		Transform()->SetRelativeRotation(0.f, 3.14f, 0.f);
		Transform()->SetRelativePos(0.f, y, 1.f);
		//Transform()->SetRelativePos_Z(1.f);
	}
	else
	{
		Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
		Transform()->SetRelativePos(0.f, y, -1.f);
		//Transform()->SetRelativePos_Z(-1.f);
	}

	RenderNumber();
}

void CActMonHPBarScript::RenderNumber()
{
	// ü���� ��ġ�� �ڽ��� ���� Number Script���� �������ش�.
	int Num0 = m_iCurHP / 10;
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

	int Num1 = m_iCurHP - (Num0 * 10);
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"HPNum_1")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num1);
}