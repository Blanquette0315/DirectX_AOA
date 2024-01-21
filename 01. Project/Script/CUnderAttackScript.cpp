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

	// 랜더링이 켜졌으면, 시간을 잼
	if (m_CurRender)
	{
		m_fAccTime += DT;

		// 0.55초를 지나면 꺼진다.
		// RenderOn호출시 누적시간을 다시 0으로 넣어줘서 마지막으로 호출된 시점부터 누적시간이 카운트 된다.
		if (m_fAccTime > 0.3f && m_fAccTime < 0.5f)
		{
			float Ratio = (0.5f - m_fAccTime) / 0.2;

			// 점차 옅어진다.
			MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, Vec4(1.f,0.f,0.f, Ratio));
			
		}
		else if (m_fAccTime > 0.5f)
		{
			m_bRenderOnOff = false;
		}
	}
}