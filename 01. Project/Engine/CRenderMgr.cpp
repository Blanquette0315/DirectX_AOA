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
	// 광원에 사용할 구조화 버퍼 만들기
	m_pLight2DBuffer = new CStructuredBuffer();
	// 해당 구조화 버퍼에 계속 push_back해 추가해 사용할 수 있는 구조이기 때문에 우선은 Element를 2개로 잡고 만들었다.
	m_pLight2DBuffer->Create(sizeof(tLightInfo), 2, SB_TYPE::SRV_ONLY, nullptr, true);
}

CRenderMgr::~CRenderMgr()
{
	// 가지고있던 광원 구조화 버퍼 삭제
	if (nullptr != m_pLight2DBuffer)
		delete m_pLight2DBuffer;
}

void CRenderMgr::init()
{
	Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();

	// 후처리 용도, 렌더 타겟 복사용
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
	// 렌더 타겟 지정
	static Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	static Ptr<CTexture> pDSTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	// 노이즈 텍스처 업데이트
	UpdateNoiseTexture();

	// 2D 광원 업데이트
	UpdateLight2D();

	// 3D 광원 업데이트
	//UpdateLight3D();

	// GlobalData 업데이트
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_global);
	pGlobalCB->UpdateData(PIPELINE_STAGE::ALL_STAGE);
	pGlobalCB->UpdateData_CS(); // CS에서도 DT를 사용하기 때문에 바인딩을 걸어주어야 Shader코드 쪽에서도 알 수 있다.

	// 레벨의 상태에 따른 랜더링 분기 처리
	// ㄴ 게임 모드일 때는 게임내의 카메라가, 에디터 모드일 때는 에디터용 카메라로 랜더링이 이루어져야 한다.
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	if (LEVEL_STATE::PLAY == pCurLevel->GetState())
	{
		render_game();
	}
	else
	{
		render_editor();
	}

	// 폰트 출력
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
	// 렌더링의 기준을 카메라로 설정
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
	// 버퍼에 공간이 모자랄 경우 재할당하여 크기를 늘려준다.
	if (m_pLight2DBuffer->GetElementCount() < (UINT)m_vecLight2D.size())
	{
		// 버퍼의 공간보다 CRenderMgr가 가지고 있는 2D광원이 더 많다면, 2D광원의 갯수만큼 버퍼 크기를 늘려서 생성한다.
		m_pLight2DBuffer->Create(m_pLight2DBuffer->GetElementSize(), (UINT)m_vecLight2D.size(), SB_TYPE::SRV_ONLY, nullptr, true);
	}

	// 광원 정보를 구조화 버퍼로 옮긴다.
	m_pLight2DBuffer->SetData(m_vecLight2D.data(), m_vecLight2D.size());

	// 구조화 버퍼를 t13 레지스터에 바인딩
	m_pLight2DBuffer->UpdateData(13, PIPELINE_STAGE::VS | PIPELINE_STAGE::PS);

	// 광원 개수 글로벌 데이터에 업데이트
	g_global.iLight2DCount = (UINT)m_vecLight2D.size();
}

void CRenderMgr::CopyRenderTarget()
{
	// static을 사용하면 처음 한번만 ResMgr의 FindRes를 호출해 한번만 찾아 저장한다.
	static Ptr<CTexture> RTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");

	// 바인딩 해제
	ID3D11ShaderResourceView* SRV = nullptr;
	CONTEXT->PSSetShaderResources(60, 1, &SRV);

	// 렌더타겟 복사 받기
	// CopyResource의 두 텍스처의 해상도가 서로 다르면 오류가 발생하니 주의해야 한다.
	// 첫 인자는 목적지, 두번째 인자는 대상이다.
	CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), RTTex->GetTex2D().Get());

	// t60 레지스터 바인딩
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