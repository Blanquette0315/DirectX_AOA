#include "pch.h"
#include "CStartBtnScript.h"

#include <Engine/CDevice.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CEventMgr.h>

#include <Engine/CLevel.h>

#include "CUIMgrScript.h"
#include "CGameMgrScript.h"

CStartBtnScript::CStartBtnScript()
	: CUIScript(STARTBTNSCRIPT)
	, m_fAccTime(0.f)
	, m_bSoundOnce(true)
	, m_bSelected(false)
{
}

CStartBtnScript::~CStartBtnScript()
{
}

void CStartBtnScript::CreateBtnSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Btn\\Click(Button3).wav");
	pSound->Play(1, 1.f, true);
}

void CStartBtnScript::begin()
{
	// 본인의 마우스 체크 스크립트의 주소를 저장해둔다.
	m_pMouseChScript = (CMouseCheckScript*)(GetOwner()->GetScript((UINT)MOUSECHECKSCRIPT));
}

void CStartBtnScript::tick()
{
	// 테스트 레벨 진입
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::T))
	{
		// 진행 중이던 BGM끄기
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\BGM\\BGM_01_Theme_v1.wav");
		pSound->Stop();

		// 레벨 전환
		CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\TLevel_0.lv");
		ChangeLevelAndPlay(Level);
	}


	// 만약 해당 버튼이 눌렸다면,
	if (m_bLbtnPressed)
	{
		// 게임 Level을 변경해주어야 한다.
		//CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\Level_0.lv");

		//ChangeLevelAndPlay(Level);

		m_bSoundOnce = false;
		m_bSelected = true;
		m_bLbtnPressed = false;
	}

	if (!m_bSoundOnce)
	{
		// 효과음 진행
		CreateBtnSound();

		// Main BGM 꺼주기
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\BGM\\BGM_01_Theme_v1.wav");
		pSound->Stop();

		m_bSoundOnce = true;
	}

	if (m_bSelected)
	{
		if (m_fAccTime < 0.5f)
		{
			m_fAccTime += DT;
		}
		else if (m_fAccTime > 0.5f)
		{
			CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\Level_0.lv");
			ChangeLevelAndPlay(Level);
		}
	}


	CUIScript::tick();

	// 마우스가 해당 UI위에 올라가 있다면, Shader에게 Tex_0이 아닌 Tex_1을 출력해야 한다고 전달한다.
	MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::INT_3, &m_bMouseOn);

	// 폰트 넣어주기
	static wchar_t szFloat[50] = {};
	swprintf_s(szFloat, 50, L"게임 시작");

	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 vScale = Transform()->GetRelativeScale();
	Vec2 vFontPos = {};
	vFontPos.x = (vResolution.x * 0.5) + (vPos.x) - (vScale.x * 0.5) + 60;
	vFontPos.y = (vResolution.y * 0.5) + (-1.f * vPos.y) - (vScale.y * 0.5) + 10;

	tFont Font = {};
	Font.strFont = L"게임 시작";
	Font.vecPos = vFontPos;
	Font.fSize = 30.f;
	Font.vecColor = Vec4(255, 255, 255, 255);

	CRenderMgr::GetInst()->AddFont(Font);
}