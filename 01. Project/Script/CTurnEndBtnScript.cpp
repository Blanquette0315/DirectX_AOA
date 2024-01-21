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
	// 만약 해당 버튼이 눌렸다면,
	if (m_bLbtnPressed)
	{
		// 사운드 출력
		CreateTurnEndSound();

		// GameMgr에게 턴을 넘긴다고 알려야 한다.
		m_pUIMgrScript->GetGameMgrScript()->SetCardDisCTurn();
		m_bLbtnPressed = false;
	}

	CUIScript::tick();

	// 마우스가 해당 UI위에 올라가 있다면, Shader에게 Tex_0이 아닌 Tex_1을 출력해야 한다고 전달한다.
	MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::INT_3, &m_bMouseOn);

	// 폰트 넣어주기
	static wchar_t szFloat[50] = {};
	swprintf_s(szFloat, 50, L"턴 종료");
	
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 vScale = Transform()->GetRelativeScale();
	Vec2 vFontPos = {};
	vFontPos.x = (vResolution.x * 0.5) + (vPos.x) - (vScale.x * 0.5) + 25;
	vFontPos.y = (vResolution.y * 0.5) + (- 1.f * vPos.y) - (vScale.y * 0.5) + 10;

	tFont Font = {};
	Font.strFont = L"턴 종료";
	Font.vecPos = vFontPos;
	Font.fSize = 24.f;
	Font.vecColor = Vec4(255, 255, 255, 255);

	CRenderMgr::GetInst()->AddFont(Font);
}