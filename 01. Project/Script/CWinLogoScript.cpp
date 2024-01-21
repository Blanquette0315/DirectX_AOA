#include "pch.h"
#include "CWinLogoScript.h"

#include <Engine/CDevice.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CRenderMgr.h>

CWinLogoScript::CWinLogoScript()
	: CUIScript(WINLOGOSCRIPT)
	, m_fAccTime(0.f)
{
}

CWinLogoScript::~CWinLogoScript()
{
}

void CWinLogoScript::begin()
{
	m_fAccTime = 0.f;
}

void CWinLogoScript::tick()
{
	m_fAccTime += DT;

	Vec3 vOriPos = Transform()->GetRelativePos();

	float ratio = 1.f;

	// �̵�
	if (m_fAccTime < 0.55f)
	{
		vOriPos.y -= 150.f * DT;
		Transform()->SetRelativePos(vOriPos);

		// ���� ������ ���� ����
		ratio = m_fAccTime / 0.55f;
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &ratio);
		int i = 10;
		MeshRender()->GetDynamicMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &i);
	}
	else if (m_fAccTime >= 0.55f)
	{
		// ���� ������ ����
		MeshRender()->GetSharedMaterial();
	}

	// ��Ʈ �־��ֱ�
	static wchar_t szFloat[50] = {};
	swprintf_s(szFloat, 50, L"�¸�");

	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 vScale = Transform()->GetRelativeScale();
	Vec2 vFontPos = {};
	vFontPos.x = (vResolution.x * 0.5) + (vPos.x) - (vScale.x * 0.5) + 70.f;
	vFontPos.y = (vResolution.y * 0.5) + (-1.f * vPos.y) - (vScale.y * 0.5) + 90.f;

	float alpha = 255 * ratio;

	tFont Font = {};
	Font.strFont = L"�¸�";
	Font.vecPos = vFontPos;
	Font.fSize = 100.f;
	Font.vecColor = Vec4(255, 255, 255, alpha);

	CRenderMgr::GetInst()->AddFont(Font);
}