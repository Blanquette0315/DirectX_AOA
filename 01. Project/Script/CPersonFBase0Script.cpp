#include "pch.h"
#include "CPersonFBase0Script.h"

#include <Engine/CTimeMgr.h>

CPersonFBase0Script::CPersonFBase0Script()
	: CScript(PERSONFBASE0SCRIPT)
	, m_fSpeed(5.f)
	, m_fDistance(10.f)
	, m_fAccDis(0.f)
	, m_bUp(true)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Move Speed    ", &m_fSpeed);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Move Distance ", &m_fDistance);
}

CPersonFBase0Script::~CPersonFBase0Script()
{
}

void CPersonFBase0Script::begin()
{
	m_vOriginPos = Transform()->GetRelativePos();
}

void CPersonFBase0Script::tick()
{
	// 현재 포지션 받아오기
	Vec3 vPos = Transform()->GetRelativePos();

	// 이동량 계산
	float fMove = m_fSpeed * DT;

	// 위로 올라가야할 때,
	if (m_bUp)
	{
		if (m_fAccDis < m_fDistance)
		{
			// 이동 누적량 계산
			m_fAccDis += fMove;

			// 만약 이번 누적 이동이 10보다 커지면, 10으로 제한
			if (m_fAccDis >= m_fDistance)
			{
				vPos.y = m_vOriginPos.y + m_fDistance;

				// 다음에는 다시 아래로 내려가야함.
				// 따라서 누적 이동량 초기화
				m_fAccDis = 0.f;
				m_bUp = false;
			}
			// 그게 아니라면 정상적으로 진행
			else
			{
				vPos.y += fMove;
			}

			// 포지션 이동 지정
			Transform()->SetRelativePos(vPos);
		}
	}
	// 아래로 내려가야할 때,
	else
	{
		if (m_fAccDis < m_fDistance)
		{
			// 이동 누적량 계산
			m_fAccDis += fMove;

			// 만약 이번 누적 이동이 10보다 커지면, 10으로 제한
			if (m_fAccDis >= m_fDistance)
			{
				vPos.y = m_vOriginPos.y;

				// 다음에는 다시 아래로 내려가야함.
				// 따라서 누적 이동량 초기화
				m_fAccDis = 0.f;
				m_bUp = true;
			}
			// 그게 아니라면 정상적으로 진행
			else
			{
				vPos.y -= fMove;
			}

			// 포지션 이동 지정
			Transform()->SetRelativePos(vPos);
		}
	}
	
}

void CPersonFBase0Script::SaveToFile(FILE* _File)
{
	// 부모쪽에서 저장할 데이터가 있을 수 있으니 호출해준다.
	CScript::SaveToFile(_File);

	// 속도, 목표 거리 저장
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
	fwrite(&m_fDistance, sizeof(float), 1, _File);
}

void CPersonFBase0Script::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);

	fread(&m_fSpeed, sizeof(float), 1, _File);
	fread(&m_fDistance, sizeof(float), 1, _File);
}