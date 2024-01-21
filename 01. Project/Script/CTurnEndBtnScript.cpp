#include "pch.h"
#include "CTurnEndBtnScript.h"

#include <Engine/CDevice.h>
#include <Engine/CRenderMgr.h>

#include "CUIMgrScript.h"
#include "CGameMgrScript.h"

CTurnEndBtnScript::CTurnEndBtnScript()
	: CUIScript(TURNENDBTNSCRIPT)
{
}

CTurnEndBtnScript::~CTurnEndBtnScript()
{
}

void CTurnEndBtnScript::CreateTurnEndSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Turn\\metal_object_small_move_impact_03.wav");
	pSound->Play(1, 1.f, true);
}

void CTurnEndBtnScript::begin()
{
	CUIScript::begin();
}

void CTurnEndBtnScript::tick()
{
	// ���� �ش� ��ư�� ���ȴٸ�,
	if (m_bLbtnPressed)
	{
		// ���� ���
		CreateTurnEndSound();

		// GameMgr���� ���� �ѱ�ٰ� �˷��� �Ѵ�.
		m_pUIMgrScript->GetGameMgrScript()->SetCardDisCTurn();
		m_bLbtnPressed = false;
	}

	CUIScript::tick();

	// ���콺�� �ش� UI���� �ö� �ִٸ�, Shader���� Tex_0�� �ƴ� Tex_1�� ����ؾ� �Ѵٰ� �����Ѵ�.
	MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::INT_3, &m_bMouseOn);

	// ��Ʈ �־��ֱ�
	static wchar_t szFloat[50] = {};
	swprintf_s(szFloat, 50, L"�� ����");
	
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 vScale = Transform()->GetRelativeScale();
	Vec2 vFontPos = {};
	vFontPos.x = (vResolution.x * 0.5) + (vPos.x) - (vScale.x * 0.5) + 25;
	vFontPos.y = (vResolution.y * 0.5) + (- 1.f * vPos.y) - (vScale.y * 0.5) + 10;

	tFont Font = {};
	Font.strFont = L"�� ����";
	Font.vecPos = vFontPos;
	Font.fSize = 24.f;
	Font.vecColor = Vec4(255, 255, 255, 255);

	CRenderMgr::GetInst()->AddFont(Font);
}