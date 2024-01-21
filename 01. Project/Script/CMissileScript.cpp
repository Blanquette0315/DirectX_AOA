#include "pch.h"
#include "CMissileScript.h"

#include "CTileScript.h"

CMissileScript::CMissileScript()
	: CScript(MISSILESCRIPT)
	, m_TargetScript(nullptr)
	, m_fSpeed(300.f)
	, m_fAccel(600.f)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::tick()
{
	//CalculateTarget();

	// ��ǥ �Ÿ� ���ϱ�
	m_fTargetDestance = sqrt((m_vDifPos.x * m_vDifPos.x) + (m_vDifPos.y * m_vDifPos.y));

	Vec2 DTVec2 = CalculateDTPos();

	// �̵��� ���� �Ÿ�
	m_fAccDestance += sqrt((DTVec2.x * DTVec2.x) + (DTVec2.y * DTVec2.y));

	// ��ǥ �̵� �Ÿ��� �������� ���ߴٸ�, ȭ���� ���󰣴�.
	if (abs(m_fTargetDestance) > abs(m_fAccDestance))
	{
		// �÷��̾��� �������� �����´�.
		Vec3 vArrowPos = Transform()->GetRelativePos();
		// ������������ �̵��ϵ��� ����
		vArrowPos.x += DTVec2.x;
		vArrowPos.y += DTVec2.y;

		// �������� �Է����ش�.
		Transform()->SetRelativePos(vArrowPos);
	}
	// ��ǥ�Ÿ��� �Ѿ�ų� �����ϸ�,
	else
	{
		// ȭ���� �����.
		Destroy();
	}
}

void CMissileScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CMissileScript::Overlap(CCollider2D* _pOther)
{
}

void CMissileScript::EndOverlap(CCollider2D* _pOther)
{
}

Vec2 CMissileScript::CalculateDTPos()
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

void CMissileScript::CalculateTarget()
{
	Vec3 TargetPos = m_TargetScript->Transform()->GetWorldPos();
	// ���� �������� ��ġ�ؾ��� ȭ���� ��ǥ ��ǥ�� �����Ѵ�.
	TargetPos.y += (m_pOwnerScale.y * 0.5);

	Vec3 OriginPos = Transform()->GetRelativePos();

	// ���� ���ϱ�
	m_vDifPos = Vec2(TargetPos.x - OriginPos.x, TargetPos.y - OriginPos.y);

	// �� �� ������ ���� ���ϱ�
	m_fSeta = atan2f(m_vDifPos.y, m_vDifPos.x);

	// ������ŭ ȭ�� �����ֱ�
	Transform()->SetRelativeRotation(Vec3(0.f, 0.f, m_fSeta));
}