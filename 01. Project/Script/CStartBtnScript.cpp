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
	// ������ ���콺 üũ ��ũ��Ʈ�� �ּҸ� �����صд�.
	m_pMouseChScript = (CMouseCheckScript*)(GetOwner()->GetScript((UINT)MOUSECHECKSCRIPT));
}

void CStartBtnScript::tick()
{
	// �׽�Ʈ ���� ����
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::T))
	{
		// ���� ���̴� BGM����
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\BGM\\BGM_01_Theme_v1.wav");
		pSound->Stop();

		// ���� ��ȯ
		CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\TLevel_0.lv");
		ChangeLevelAndPlay(Level);
	}


	// ���� �ش� ��ư�� ���ȴٸ�,
	if (m_bLbtnPressed)
	{
		// ���� Level�� �������־�� �Ѵ�.
		//CLevel* Level = CLevelMgr::GetInst()->FindLevel(L"level\\Level_0.lv");

		//ChangeLevelAndPlay(Level);

		m_bSoundOnce = false;
		m_bSelected = true;
		m_bLbtnPressed = false;
	}

	if (!m_bSoundOnce)
	{
		// ȿ���� ����
		CreateBtnSound();

		// Main BGM ���ֱ�
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

	// ���콺�� �ش� UI���� �ö� �ִٸ�, Shader���� Tex_0�� �ƴ� Tex_1�� ����ؾ� �Ѵٰ� �����Ѵ�.
	MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::INT_3, &m_bMouseOn);

	// ��Ʈ �־��ֱ�
	static wchar_t szFloat[50] = {};
	swprintf_s(szFloat, 50, L"���� ����");

	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 vScale = Transform()->GetRelativeScale();
	Vec2 vFontPos = {};
	vFontPos.x = (vResolution.x * 0.5) + (vPos.x) - (vScale.x * 0.5) + 60;
	vFontPos.y = (vResolution.y * 0.5) + (-1.f * vPos.y) - (vScale.y * 0.5) + 10;

	tFont Font = {};
	Font.strFont = L"���� ����";
	Font.vecPos = vFontPos;
	Font.fSize = 30.f;
	Font.vecColor = Vec4(255, 255, 255, 255);

	CRenderMgr::GetInst()->AddFont(Font);
}