#include "pch.h"
#include "CRenderMgr.h"

#include "GlobalComponent.h"
#include "CResMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CFontMgr.h"

CRenderMgr::CRenderMgr()
	: m_EditorCam(nullptr)
	, m_pLight2DBuffer(nullptr)
{
	// ������ ����� ����ȭ ���� �����
	m_pLight2DBuffer = new CStructuredBuffer();
	// �ش� ����ȭ ���ۿ� ��� push_back�� �߰��� ����� �� �ִ� �����̱� ������ �켱�� Element�� 2���� ��� �������.
	m_pLight2DBuffer->Create(sizeof(tLightInfo), 2, SB_TYPE::SRV_ONLY, nullptr, true);
}

CRenderMgr::~CRenderMgr()
{
	// �������ִ� ���� ����ȭ ���� ����
	if (nullptr != m_pLight2DBuffer)
		delete m_pLight2DBuffer;
}

void CRenderMgr::init()
{
	Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();

	// ��ó�� �뵵, ���� Ÿ�� �����
	m_RTCopyTex = CResMgr::GetInst()->CreateTexture(L"RTCopyTex"
		, (UINT)vRenderResol.x, (UINT)vRenderResol.y
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	m_RTCopyTex->UpdateData(60, PIPELINE_STAGE::PS);
}

void CRenderMgr::tick()
{
	m_vecCam.clear();
	m_vecLight2D.clear();
	m_arrFont.clear();
}

void CRenderMgr::render()
{
	// ���� Ÿ�� ����
	static Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	static Ptr<CTexture> pDSTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	// ������ �ؽ�ó ������Ʈ
	UpdateNoiseTexture();

	// 2D ���� ������Ʈ
	UpdateLight2D();

	// 3D ���� ������Ʈ
	//UpdateLight3D();

	// GlobalData ������Ʈ
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_global);
	pGlobalCB->UpdateData(PIPELINE_STAGE::ALL_STAGE);
	pGlobalCB->UpdateData_CS(); // CS������ DT�� ����ϱ� ������ ���ε��� �ɾ��־�� Shader�ڵ� �ʿ����� �� �� �ִ�.

	// ������ ���¿� ���� ������ �б� ó��
	// �� ���� ����� ���� ���ӳ��� ī�޶�, ������ ����� ���� �����Ϳ� ī�޶�� �������� �̷������ �Ѵ�.
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	if (LEVEL_STATE::PLAY == pCurLevel->GetState())
	{
		render_game();
	}
	else
	{
		render_editor();
	}

	// ��Ʈ ���
	for (size_t i = 0; i < m_arrFont.size(); ++i)
	{
		wchar_t szFloat[50] = {};
		swprintf_s(szFloat, 50, m_arrFont[i].strFont.c_str());
		CFontMgr::GetInst()->DrawFont(szFloat, m_arrFont[i].vecPos.x,
									m_arrFont[i].vecPos.y, m_arrFont[i].fSize,
									FONT_RGBA(m_arrFont[i].vecColor.x, m_arrFont[i].vecColor.y,
									m_arrFont[i].vecColor.z, m_arrFont[i].vecColor.w));
	}
}

void CRenderMgr::render_game()
{
	// �������� ������ ī�޶�� ����
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->render();
	}
}

void CRenderMgr::render_editor()
{
	assert(m_EditorCam);

	m_EditorCam->render();
}

void CRenderMgr::UpdateNoiseTexture()
{
	Ptr<CTexture> NoiseTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\noise\\TestNoise.png");
	NoiseTex->UpdateData(15, PIPELINE_STAGE::ALL_STAGE);
	NoiseTex->UpdateData_CS(15, true);

	g_global.vNoiseResolution = Vec2(NoiseTex->GetWidth(), NoiseTex->GetHeight());
}

void CRenderMgr::UpdateLight2D()
{
	// ���ۿ� ������ ���ڶ� ��� ���Ҵ��Ͽ� ũ�⸦ �÷��ش�.
	if (m_pLight2DBuffer->GetElementCount() < (UINT)m_vecLight2D.size())
	{
		// ������ �������� CRenderMgr�� ������ �ִ� 2D������ �� ���ٸ�, 2D������ ������ŭ ���� ũ�⸦ �÷��� �����Ѵ�.
		m_pLight2DBuffer->Create(m_pLight2DBuffer->GetElementSize(), (UINT)m_vecLight2D.size(), SB_TYPE::SRV_ONLY, nullptr, true);
	}

	// ���� ������ ����ȭ ���۷� �ű��.
	m_pLight2DBuffer->SetData(m_vecLight2D.data(), m_vecLight2D.size());

	// ����ȭ ���۸� t13 �������Ϳ� ���ε�
	m_pLight2DBuffer->UpdateData(13, PIPELINE_STAGE::VS | PIPELINE_STAGE::PS);

	// ���� ���� �۷ι� �����Ϳ� ������Ʈ
	g_global.iLight2DCount = (UINT)m_vecLight2D.size();
}

void CRenderMgr::CopyRenderTarget()
{
	// static�� ����ϸ� ó�� �ѹ��� ResMgr�� FindRes�� ȣ���� �ѹ��� ã�� �����Ѵ�.
	static Ptr<CTexture> RTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");

	// ���ε� ����
	ID3D11ShaderResourceView* SRV = nullptr;
	CONTEXT->PSSetShaderResources(60, 1, &SRV);

	// ����Ÿ�� ���� �ޱ�
	// CopyResource�� �� �ؽ�ó�� �ػ󵵰� ���� �ٸ��� ������ �߻��ϴ� �����ؾ� �Ѵ�.
	// ù ���ڴ� ������, �ι�° ���ڴ� ����̴�.
	CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), RTTex->GetTex2D().Get());

	// t60 �������� ���ε�
	m_RTCopyTex->UpdateData(60, PIPELINE_STAGE::PS);
}

CCamera* CRenderMgr::GetMainCam()
{
	if (LEVEL_STATE::PLAY == CLevelMgr::GetInst()->GetLevelState())
	{
		if (m_vecCam.empty())
			return nullptr;

		return m_vecCam[0];
	}
	else
	{
		return m_EditorCam;
	}
}