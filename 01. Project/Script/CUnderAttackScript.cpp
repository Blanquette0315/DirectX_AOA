#include "pch.h"
#include "CUnderAttackScript.h"

#include <Engine/CScript.h>

CUnderAttackScript::CUnderAttackScript()
	: CScript(UNDERATTACKSCRIPT)
	, m_bRenderOnOff(false)
	, m_fAccTime(0.f)
{
}

CUnderAttackScript::~CUnderAttackScript()
{
}

void CUnderAttackScript::begin()
{
	GetOwner()->MeshRender()->Deactivate();
}

void CUnderAttackScript::tick()
{
	if (m_bRenderOnOff)
	{
		GetOwner()->MeshRender()->Activate();
		m_CurRender = true;
	}
	else
	{
		GetOwner()->MeshRender()->Deactivate();
		m_CurRender = false;
	}

	// �������� ��������, �ð��� ��
	if (m_CurRender)
	{
		m_fAccTime += DT;

		// 0.55�ʸ� ������ ������.
		// RenderOnȣ��� �����ð��� �ٽ� 0���� �־��༭ ���������� ȣ��� �������� �����ð��� ī��Ʈ �ȴ�.
		if (m_fAccTime > 0.3f && m_fAccTime < 0.5f)
		{
			float Ratio = (0.5f - m_fAccTime) / 0.2;

			// ���� ��������.
			MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1.f,0.f,0.f, Ratio));
			
		}
		else if (m_fAccTime > 0.5f)
		{
			m_bRenderOnOff = false;
		}
	}
}