#include "pch.h"
#include "CPlayerHPScript.h"

#include "CGameMgrScript.h"
#include "CUIMgrScript.h"

#include "CPlayerScript.h"
#include "CNumberScript.h"

CPlayerHPScript::CPlayerHPScript()
	: CUIScript(PLAYERHPSCRIPT)
	, m_pPlayerScript(nullptr)
	, m_iMaxHP(0)
	, m_iCurHP(0)
{
}

CPlayerHPScript::~CPlayerHPScript()
{
}

void CPlayerHPScript::begin()
{
	CUIScript::begin();
}

void CPlayerHPScript::tick()
{
	CUIScript::tick();

	// Player�� ��ũ��Ʈ�� ������ �����صд�.
	m_pPlayerScript = m_pUIMgrScript->GetGameMgrScript()->GetPlayerScript();
	
	// Player�� ü���� Ȯ���Ѵ�.
	m_iMaxHP = m_pPlayerScript->GetMAXHP();
	m_iCurHP = m_pPlayerScript->GetCURHP();

	// ü�¹ٸ� ǥ���ϱ� ���� �ۼ�Ʈ�� ���Ѵ�.
	float pRatio = (float)m_iCurHP / (float)m_iMaxHP;

	Vec4 RectUV = {};
	RectUV.x = 4.f / 44.f;
	RectUV.y = 4.f / 16.f;
	RectUV.z = 40.f / 44.f;
	RectUV.w = 11.f / 16.f;

	// ������ Shader�� �����Ѵ�.
	MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &pRatio);
	MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, &RectUV);

	RenderNumber();
}

void CPlayerHPScript::SaveToFile(FILE* _File)
{
}

void CPlayerHPScript::LoadFromFile(FILE* _File)
{
}



void CPlayerHPScript::RenderNumber()
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

	int Num2 = m_iCurHP - ((Num0 * 100) + (Num1 * 10));
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"HPNum_2")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num2);

	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"HPNum_3")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(10);

	int Num4 = m_iMaxHP / 100;
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"HPNum_4")->GetScript((UINT)NUMBERSCRIPT));
	// ���� ù�ڸ��� 0�̸� ����� �ȵǾ�� �ϹǷ� ����ó���� ���ش�.
	if (Num4 == 0)
	{
		pNumScript->SetNumber(-1);
	}
	else
	{
		pNumScript->SetNumber(Num4);
	}

	int Num5 = (m_iMaxHP - (Num4 * 100)) / 10;
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"HPNum_5")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num5);

	int Num6 = m_iMaxHP - ((Num4 * 100) + (Num5 * 10));
	pNumScript = (CNumberScript*)(GetOwner()->GetChildObject(L"HPNum_6")->GetScript((UINT)NUMBERSCRIPT));
	pNumScript->SetNumber(Num6);
}