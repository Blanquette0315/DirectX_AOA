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

	// 목표 거리 구하기
	m_fTargetDestance = sqrt((m_vDifPos.x * m_vDifPos.x) + (m_vDifPos.y * m_vDifPos.y));

	Vec2 DTVec2 = CalculateDTPos();

	// 이동한 누적 거리
	m_fAccDestance += sqrt((DTVec2.x * DTVec2.x) + (DTVec2.y * DTVec2.y));

	// 목표 이동 거리에 도달하지 못했다면, 화살은 날라간다.
	if (abs(m_fTargetDestance) > abs(m_fAccDestance))
	{
		// 플레이어의 포지션을 가져온다.
		Vec3 vArrowPos = Transform()->GetRelativePos();
		// 원래방향으로 이동하도록 지정
		vArrowPos.x += DTVec2.x;
		vArrowPos.y += DTVec2.y;

		// 포지션을 입력해준다.
		Transform()->SetRelativePos(vArrowPos);
	}
	// 목표거리를 넘어서거나 도달하면,
	else
	{
		// 화살을 지운다.
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
	// (속도 + 가속도 * DT) * DT = 이번 DT동안 이동 거리
	m_fSpeed += m_fAccel * DT;
	float DestanceDT = m_fSpeed * DT;

	// 이번 DT동안 이동할 X축 = Rdt * cos(Seta)
	float xDT = DestanceDT * cos(m_fSeta);

	// 이번 DT동안 이동할 Y축 = Rdt * sin(Seta)
	float yDT = DestanceDT * sin(m_fSeta);

	return Vec2(xDT, yDT);
}

void CMissileScript::CalculateTarget()
{
	Vec3 TargetPos = m_TargetScript->Transform()->GetWorldPos();
	// 실제 발판위에 위치해야할 화살의 목표 좌표로 변경한다.
	TargetPos.y += (m_pOwnerScale.y * 0.5);

	Vec3 OriginPos = Transform()->GetRelativePos();

	// 차이 구하기
	m_vDifPos = Vec2(TargetPos.x - OriginPos.x, TargetPos.y - OriginPos.y);

	// 두 점 사이의 각도 구하기
	m_fSeta = atan2f(m_vDifPos.y, m_vDifPos.x);

	// 각도만큼 화살 돌려주기
	Transform()->SetRelativeRotation(Vec3(0.f, 0.f, m_fSeta));
}