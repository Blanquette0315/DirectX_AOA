#include "pch.h"
#include "CResMgr.h"

#include "CSound.h"

void CResMgr::init()
{
	InitSound();

	CreateDefaultMesh();

	CreateDefaultTexture();

	CreateDefaultGrapicsShader();

	CreateDefaultComputeShader();

	CreateDefaultMaterial();
}

void CResMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}

void CResMgr::CreateDefaultMesh()
{
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;
	CMesh* pMesh = nullptr;

	// Point Mesh
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	UINT idx = 0;
	
	pMesh = new CMesh(true);
	pMesh->Create(&v, 1, &idx, 1);
	AddRes<CMesh>(L"PointMesh", pMesh);

	// 사각형 메쉬 만들기

	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"RectMesh", pMesh);
	vecIdx.clear();

	// DebugRectIndex
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"RectMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();

	//원형 메쉬 만들기
	// 중심점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);

	vecVtx.push_back(v);

	int iSlice = 40;
	float fRadius = 0.5f;
	float fTheta = XM_2PI / (float)iSlice;

	for (int i = 0; i < iSlice; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 1.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, 1.f - (v.vPos.y + 0.5f));
		vecVtx.push_back(v);
	}

	for (UINT i = 0; i < iSlice; ++i)
	{
		vecIdx.push_back(0);
		if (i == iSlice - 1)
		{
			vecIdx.push_back(1);
		}
		else
		{
			vecIdx.push_back(i + 2);
		}
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CircleMesh", pMesh);
	vecIdx.clear();

	// Circle_debug
	for (int i = 0; i < vecVtx.size(); ++i)
	{
		vecIdx.push_back(i + 1);
	}
	vecIdx.push_back(1);
	
	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CircleMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();
}

void CResMgr::CreateDefaultTexture()
{
	// 로딩
	// Texture 로딩 
	//CResMgr::GetInst()->Load<CTexture>(L"Player", L"texture\\CharmanderUI.png");
	//CResMgr::GetInst()->Load<CTexture>(L"Smoke", L"texture\\smokeparticle.png");
	//
	//Load<CTexture>(L"SmokeParticle", L"texture\\particle\\smokeparticle.png");
	//Load<CTexture>(L"Bubbles50px", L"texture\\particle\\Bubbles50px.png");
	//
	//Load<CTexture>(L"TILEAtlas", L"texture\\TILE.bmp");
	//Load<CTexture>(L"TerrainTiles", L"texture\\terrainTiles_default.png");
	//Load<CTexture>(L"Alina_Bandit", L"texture\\Alina_Bandit.png");
	//
	//// 애니메이션 로딩 
	//CResMgr::GetInst()->Load<CTexture>(L"link", L"texture\\link.png");
	//Load<CTexture>(L"CardUpgradeEffAtlas", L"texture\\CardUpgradeEffAtlas.png"); // 36, 3, 12 size 164 178
	//Load<CTexture>(L"MonsterAtlas", L"texture\\MonsterAtlas.png");
	//Load<CTexture>(L"KillBossAnim", L"texture\\KillBossAnim.png"); // 130 13 10 size 198 90 
	//
	// NoiseTexture
	// Noise_01은 생성 타입이 원일 시 고르게 분포하지 않는 문제가 있다.
	Load<CTexture>(L"texture\\noise\\noise_01.png", L"texture\\noise\\noise_01.png");
	Load<CTexture>(L"texture\\noise\\noise_02.png", L"texture\\noise\\noise_02.png");
	Load<CTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_03.jpg");
	
	Load<CTexture>(L"texture\\noise\\TestNoise.png", L"texture\\noise\\TestNoise.png");

	// 생성
	// Texture 생성
	// D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS 로 한 이유
	// : 텍스쳐를 하나 만들어서 최초 검은색으로 출력하다가 ComputeShader를 이용해 색을 바꿔 출력해보기 위함이다.
	CreateTexture(L"UAVTex", 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
}

void CResMgr::CreateDefaultGrapicsShader()
{
	AddInputLayout(DXGI_FORMAT_R32G32B32_FLOAT, "POSITION");
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "COLOR");
	AddInputLayout(DXGI_FORMAT_R32G32_FLOAT, "TEXCOORD");

	CGrapicsShader* pShader = nullptr;

	// std2D Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddScalarParam(INT_0, "Test Int");
	pShader->AddScalarParam(FLOAT_2, "Test Float");
	pShader->AddScalarParam(VEC2_3, "Test Vec2");
	pShader->AddScalarParam(VEC4_2, "Test Vec4");
	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGrapicsShader>(L"Std2DShader", pShader);

	// std2DAlphaBlend Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_AlphaBlend");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	pShader->AddScalarParam(INT_0, "Test Int");
	pShader->AddScalarParam(FLOAT_2, "Test Float");
	pShader->AddScalarParam(VEC2_3, "Test Vec2");
	pShader->AddScalarParam(VEC4_2, "Test Vec4");
	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGrapicsShader>(L"Std2DAlphaBlendShader", pShader);

	// Grid Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\editor.fx", "VS_Grid");
	pShader->CreatePixelShader(L"shader\\editor.fx", "PS_Grid");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGrapicsShader>(L"GridShader", pShader);

	// DebufDraw Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\debugdraw.fx", "VS_DebugDraw");
	pShader->CreatePixelShader(L"shader\\debugdraw.fx", "PS_DebugDraw");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGrapicsShader>(L"DebugDrawShader", pShader);

	// ParticleRenderShader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\particlerender.fx", "VS_ParticleRender");
	pShader->CreateGeometryShader(L"shader\\particlerender.fx", "GS_ParticleRender");
	pShader->CreatePixelShader(L"shader\\particlerender.fx", "PS_ParticleRender");

	// 이제 점 Mesh를 사용하기 때문에 Topology를 POINTLIST로 전달해준다. 
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGrapicsShader>(L"ParticleRenderShader", pShader);

	// postprocess
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_PostProcess");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_PostProcess");
	
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	//pShader->SetRSType(RS_TYPE::CULL_NONE);
	//pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POST_PROCESS);
	
	AddRes<CGrapicsShader>(L"PostProcessShader", pShader);

	// TileMap Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
	pShader->SetRSType(RS_TYPE::CULL_NONE);

	AddRes<CGrapicsShader>(L"TileMapShader", pShader);


	// 컨텐츠를 제작하면서 추가한 Shader
	// TileAlphaBlend Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\TileAlphaBlendShader.fx", "VS_TileAlphaBlend");
	pShader->CreatePixelShader(L"shader\\TileAlphaBlendShader.fx", "PS_TileAlphaBlend");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	
	pShader->AddScalarParam(VEC4_0, "Color Vec4");
	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGrapicsShader>(L"TileAlphaBlendShader", pShader);

	// stdCard Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\stdCardShader.fx", "VS_stdCard");
	pShader->CreatePixelShader(L"shader\\stdCardShader.fx", "PS_stdCard");

	pShader->SetRSType(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddScalarParam(VEC4_0, "Color Vec4 (No Alpha)");
	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGrapicsShader>(L"stdCardShader", pShader);

	// stdAlphaCard Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\stdCardShader.fx", "VS_stdCard");
	pShader->CreatePixelShader(L"shader\\stdCardShader.fx", "PS_stdCard");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	pShader->AddScalarParam(VEC4_0, "Color Vec4 (No Alpha)");
	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGrapicsShader>(L"stdAlphaCardShader", pShader);


	// std2DUI Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\std2DUI.fx", "VS_std2DUI");
	pShader->CreatePixelShader(L"shader\\std2DUI.fx", "PS_std2DUI");

	pShader->SetRSType(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGrapicsShader>(L"std2DUIShader", pShader);

	// PlayerHPBar Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\HPBarUIShader.fx", "VS_HPBarUI");
	pShader->CreatePixelShader(L"shader\\HPBarUIShader.fx", "PS_HPBarUI");

	pShader->SetRSType(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGrapicsShader>(L"PlayerHPBarShader", pShader);

	// CostUI Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\CostUIShader.fx", "VS_CostUI");
	pShader->CreatePixelShader(L"shader\\CostUIShader.fx", "PS_CostUI");

	pShader->SetRSType(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGrapicsShader>(L"CostUIShader", pShader);

	// TurnEndBtnUI Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\TurnEndBtn.fx", "VS_TurnEndBtnUI");
	pShader->CreatePixelShader(L"shader\\TurnEndBtn.fx", "PS_TurnEndBtnUI");

	pShader->SetRSType(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGrapicsShader>(L"TurnEndBtnUIShader", pShader);

	// NumUI Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\NumUI.fx", "VS_NumUI");
	pShader->CreatePixelShader(L"shader\\NumUI.fx", "PS_NumUI");

	pShader->SetRSType(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGrapicsShader>(L"NumUIShader", pShader);


	// ReverseEffectSahder : a값을 뒤집어 출력해주는 Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\ReverseEffect.fx", "VS_reverseEff");
	pShader->CreatePixelShader(L"shader\\ReverseEffect.fx", "PS_reverseEff");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	//pShader->SetDSType(DS_TYPE::LESS);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	pShader->AddScalarParam(VEC4_0, "Color Vec4 (No Alpha)");
	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGrapicsShader>(L"ReverseEffectShader", pShader);

	// stdEffect Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\stdEffectShader.fx", "VS_stdEffect");
	pShader->CreatePixelShader(L"shader\\stdEffectShader.fx", "PS_stdEffect");
	pShader->SetRSType(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGrapicsShader>(L"StdEffectShader", pShader);

	// ActorHPBarUI Sahder
	// PlayerHPBar Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\ActorHPBarUIShader.fx", "VS_ActorHPBarUI");
	pShader->CreatePixelShader(L"shader\\ActorHPBarUIShader.fx", "PS_ActorHPBarUI");

	pShader->SetRSType(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGrapicsShader>(L"ActorHPBarUIShader", pShader);


	// WinLogo Shader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\WinLogoSahder.fx", "VS_WinLogo");
	pShader->CreatePixelShader(L"shader\\WinLogoSahder.fx", "PS_WinLogo");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	pShader->AddTexureParam(TEX_0, "Output Texture 1");
	pShader->AddTexureParam(TEX_1, "Output Texture 2");
	pShader->AddTexureParam(TEX_2, "Output Texture 3");

	AddRes<CGrapicsShader>(L"WinLogoShader", pShader);


	// 파티클을 만들 때 사용할 Shader들

	// ShieldParticleRenderShader
	pShader = new CGrapicsShader;
	pShader->CreateVertexShader(L"shader\\particlerender.fx", "VS_ParticleRender");
	pShader->CreateGeometryShader(L"shader\\particlerender.fx", "GS_ParticleRender");
	pShader->CreatePixelShader(L"shader\\particlerender.fx", "PS_ShieldParticleRender");

	// 이제 점 Mesh를 사용하기 때문에 Topology를 POINTLIST로 전달해준다. 
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	//pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	pShader->AddTexureParam(TEX_0, "Output Texture 1");

	AddRes<CGrapicsShader>(L"ShieldParticleRenderShader", pShader);
}

#include "CPaintShader.h"
#include "CParticleUpdateShader.h"
void CResMgr::CreateDefaultComputeShader()
{
	CComputeShader* pShader = nullptr;

	pShader = new CPaintShader;
	pShader->CreateComputeShader(L"shader\\compute.fx", "CS_Paint");
	AddRes<CComputeShader>(L"PaintShader", pShader);

	// Particle Update Shader
	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\particleupdate.fx", "CS_ParticleUpdate");
	AddRes<CComputeShader>(L"ParticleUpdateShader", pShader);

	// ShieldEff Update Shader
	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\ShieldEffupdate.fx", "CS_ShieldEffUpdate");
	AddRes<CComputeShader>(L"ShieldEffUpdateShader", pShader);
}

void CResMgr::CreateDefaultMaterial()
{
	CMaterial* pMtrl = nullptr;

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGrapicsShader>(L"Std2DShader"));
	AddRes<CMaterial>(L"Std2DMtrl", pMtrl);

	//Load<CMaterial>(L"Std2DMtrl", L"material\\mtrl.mtrl");

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGrapicsShader>(L"Std2DAlphaBlendShader"));
	AddRes<CMaterial>(L"Std2DAlphaBlendMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGrapicsShader>(L"GridShader"));
	AddRes<CMaterial>(L"GridMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGrapicsShader>(L"DebugDrawShader"));
	AddRes<CMaterial>(L"DebugDrawMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGrapicsShader>(L"ParticleRenderShader"));
	AddRes<CMaterial>(L"ParticleRenderMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGrapicsShader>(L"PostProcessShader"));
	AddRes<CMaterial>(L"PostProcessMtrl", pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindRes<CGrapicsShader>(L"TileMapShader"));
	AddRes<CMaterial>(L"TileMapMtrl", pMtrl);
}

void CResMgr::AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName)
{
	D3D11_INPUT_ELEMENT_DESC LayoutDesc = {};
	LayoutDesc.AlignedByteOffset = m_iLayoutOffset;
	LayoutDesc.Format = _eFormat;
	LayoutDesc.InputSlot = 0;
	LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc.SemanticName = _strSemanticName;
	LayoutDesc.SemanticIndex = 0;
	m_vecLayoutInfo.push_back(LayoutDesc);

	m_iLayoutOffset += GetSizeofFormat(_eFormat);
}

bool CResMgr::DeleteRes(RES_TYPE _Type, const wstring& _strKey)
{
	map<wstring, Ptr<CRes>>::iterator iter = m_arrRes[(UINT)_Type].find(_strKey);

	if (m_arrRes[(UINT)_Type].end() != iter)
	{
		m_arrRes[(UINT)_Type].erase(iter);
		m_bChanged = true;
		return true;
	}

	return false;
}
