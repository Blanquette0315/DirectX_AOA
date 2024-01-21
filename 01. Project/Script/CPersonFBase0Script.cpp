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
	// ���� ������ �޾ƿ���
	Vec3 vPos = Transform()->GetRelativePos();

	// �̵��� ���
	float fMove = m_fSpeed * DT;

	// ���� �ö󰡾��� ��,
	if (m_bUp)
	{
		if (m_fAccDis < m_fDistance)
		{
			// �̵� ������ ���
			m_fAccDis += fMove;

			// ���� �̹� ���� �̵��� 10���� Ŀ����, 10���� ����
			if (m_fAccDis >= m_fDistance)
			{
				vPos.y = m_vOriginPos.y + m_fDistance;

				// �������� �ٽ� �Ʒ��� ����������.
				// ���� ���� �̵��� �ʱ�ȭ
				m_fAccDis = 0.f;
				m_bUp = false;
			}
			// �װ� �ƴ϶�� ���������� ����
			else
			{
				vPos.y += fMove;
			}

			// ������ �̵� ����
			Transform()->SetRelativePos(vPos);
		}
	}
	// �Ʒ��� ���������� ��,
	else
	{
		if (m_fAccDis < m_fDistance)
		{
			// �̵� ������ ���
			m_fAccDis += fMove;

			// ���� �̹� ���� �̵��� 10���� Ŀ����, 10���� ����
			if (m_fAccDis >= m_fDistance)
			{
				vPos.y = m_vOriginPos.y;

				// �������� �ٽ� �Ʒ��� ����������.
				// ���� ���� �̵��� �ʱ�ȭ
				m_fAccDis = 0.f;
				m_bUp = true;
			}
			// �װ� �ƴ϶�� ���������� ����
			else
			{
				vPos.y -= fMove;
			}

			// ������ �̵� ����
			Transform()->SetRelativePos(vPos);
		}
	}
	
}

void CPersonFBase0Script::SaveToFile(FILE* _File)
{
	// �θ��ʿ��� ������ �����Ͱ� ���� �� ������ ȣ�����ش�.
	CScript::SaveToFile(_File);

	// �ӵ�, ��ǥ �Ÿ� ����
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
	fwrite(&m_fDistance, sizeof(float), 1, _File);
}

void CPersonFBase0Script::LoadFromFile(FILE* _File)
{
	CScript::LoadFromFile(_File);

	fread(&m_fSpeed, sizeof(float), 1, _File);
	fread(&m_fDistance, sizeof(float), 1, _File);
}