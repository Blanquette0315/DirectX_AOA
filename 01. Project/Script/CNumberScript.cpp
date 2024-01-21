#include "pch.h"
#include "CNumberScript.h"


CNumberScript::CNumberScript()
	: CScript(NUMBERSCRIPT)
	, m_iNumber(-1)
{
}

CNumberScript::~CNumberScript()
{
}

void CNumberScript::begin()
{
}

void CNumberScript::tick()
{
	RenderNum();
}


void CNumberScript::RenderNum()
{
	Vec2 LeftTopUV = { 0.f, 0.f };
	Vec2 Slice = {};
	Slice.x = 5.f / 75.f;
	Slice.y = 7.f / 7.f;

	Vec2 FullSize = {};
	FullSize.x = 5.f / 75.f;
	FullSize.y = 7.f / 7.f;

	switch (m_iNumber)
	{
	// -1일 경우 아무것도 출력하지 않는다.
	case -1:
	{
		LeftTopUV.x = -1.f;
		LeftTopUV.y = -1.f;
		break;
	}

	case 0:
	{
		LeftTopUV.x = 0.f;
		LeftTopUV.y = 0.f;
		break;
	}
	case 1:
	{
		LeftTopUV.x = 5.f / 75.f;
		break;
	}
	case 2:
	{
		LeftTopUV.x = 10.f / 75.f;
		break;
	}
	case 3:
	{
		LeftTopUV.x = 15.f / 75.f;
		break;
	}
	case 4:
	{
		LeftTopUV.x = 20.f / 75.f;
		break;
	}
	case 5:
	{
		LeftTopUV.x = 25.f / 75.f;
		break;
	}
	case 6:
	{
		LeftTopUV.x = 30.f / 75.f;
		break;
	}
	case 7:
	{
		LeftTopUV.x = 35.f / 75.f;
		break;
	}
	case 8:
	{
		LeftTopUV.x = 40.f / 75.f;
		break;
	}
	case 9:
	{
		LeftTopUV.x = 45.f / 75.f;
		break;
	}

	// 10일 경우 /를 출력 한다.
	case 10:
	{
		LeftTopUV.x = 65.f / 75.f;
		break;
	}
	}

	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, &LeftTopUV);
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, &Slice);
	MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_2, &FullSize);
}