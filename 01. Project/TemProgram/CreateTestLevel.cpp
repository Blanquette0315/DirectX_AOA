#include "pch.h"
#include "CreateTestLevel.h"

#include <Engine/CEventMgr.h>
#include <Engine/CResMgr.h>
#include <Engine/CCollisionMgr.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/GlobalComponent.h>

#include <Engine/CPaintShader.h>

#include "CCameraScript.h"

#include "CSaveLoadMgr.h"

void CreateTestLevel()
{
#ifdef _DEBUG
	CLevel* Level = CSaveLoadMgr::GetInst()->LoadLevel(L"level\\Test Level.lv");
	CLevelMgr::GetInst()->RegisterLevel(Level->GetRelativePath(), Level);
	ChangeLevel(Level);
	return;
#else
	CLevel* Level = CSaveLoadMgr::GetInst()->LoadLevel(L"level\\MainLevel.lv");
	CLevelMgr::GetInst()->RegisterLevel(Level->GetRelativePath(), Level);
	ChangeLevelAndPlay(Level);
	return;
#endif

	CLevel* pLevel = new CLevel;

	// Layer �̸� ����
	pLevel->GetLayer(1)->SetName(L"Player");
	pLevel->GetLayer(2)->SetName(L"PlayerProjectile");
	pLevel->GetLayer(3)->SetName(L"Monster");
	pLevel->GetLayer(4)->SetName(L"MonsterProjectile");

	// ComputeShader �׽�Ʈ
	//Ptr<CPaintShader> pComputeShader = (CPaintShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"PaintShader").Get();
	//pComputeShader->SetTexture(CResMgr::GetInst()->FindRes<CTexture>(L"UAVTex"));
	//pComputeShader->SetColor(Vec4(0.f, 0.f, 1.f, 1.f));
	//pComputeShader->Execute();

	// Camera Object �߰�
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");

	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);

	pCamObj->Camera()->SetLayerMaskAll();
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHICS);

	pLevel->AddGameObject(pCamObj, 0);

	// Directional Light �߰�
	// ���� ���⼺ ������ ���϶��� 0.7, �㿡�� 0.2������ �ΰ� ����Ѵ�.
	// ������ �׷��� �ʿ䰡 ���� ������ CRenderMesh�� �߰����� �ʴ´�.
	// Directional Light�� ���� ������ �ʿ� ���� ������ ���� �ʾҴ�.
	CGameObject* pDirLight = new CGameObject;
	pDirLight->SetName(L"DirectionalLight");

	pDirLight->AddComponent(new CTransform);
	pDirLight->AddComponent(new CLight2D);

	pDirLight->Light2D()->SetLightColor(Vec3(0.7f, 0.7f, 0.7f));
	pDirLight->Light2D()->SetLightEmbient(Vec3(0.2f, 0.2f, 0.2f));
	pDirLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);

	pLevel->AddGameObject(pDirLight, 0);


	// Point Light �߰�
	//CGameObject* pPointLight = new CGameObject;
	//pPointLight->SetName(L"PointLight");
	//
	//pPointLight->AddComponent(new CTransform);
	//pPointLight->AddComponent(new CLight2D);
	//
	//pPointLight->Transform()->SetRelativePos(Vec3(-200.f, 0.f, 0.f));
	////DebugDrawCircle(Vec4(0.f, 1.f, 0.f, 1.f), Vec3(-200.f, 0.f, 0.f), 500.f, 100.f);
	//
	//pPointLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pPointLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pPointLight->Light2D()->SetRadius(500.f);
	//
	//pLevel->AddGameObject(pPointLight, 0);

	// Spot Light �߰�
	/*CGameObject* pSpotLight = new CGameObject;
	pSpotLight->SetName(L"SpotLight");

	pSpotLight->AddComponent(new CTransform);
	pSpotLight->AddComponent(new CLight2D);

	pSpotLight->Transform()->SetRelativePos(Vec3(-200.f, 0.f, 0.f));
	pSpotLight->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, -45.f * (XM_PI / 180)));
	DebugDrawCircle(Vec4(0.f, 1.f, 0.f, 1.f), Vec3(-200.f, 0.f, 0.f), 900.f, 100.f);

	pSpotLight->Light2D()->SetLightType(LIGHT_TYPE::SPOT);
	pSpotLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pSpotLight->Light2D()->SetRadius(500.f);
	pSpotLight->Light2D()->SetAngle(45);

	pLevel->AddGameObject(pSpotLight, 0);*/


	/*CGameObject* pChild = new CGameObject;
	pChild->SetName(L"Child");

	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CMeshRender);
	pChild->AddComponent(new CCollider2D);

	pChild->Transform()->SetIgnoreParentScale(true);
	pChild->Transform()->SetRelativePos(Vec3(512.f, 0.f, 100.f));
	pChild->Transform()->SetRelativeScale(Vec3(256.f, 256.f, 1.f));

	pChild->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pChild->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pChild->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Player"));*/

	/*Ptr<CPrefab> prefab = new CPrefab(pChild);
	CResMgr::GetInst()->AddRes<CPrefab>(L"ChildObject", prefab.Get());
	prefab->Save(L"prefab//ChildObject.pref");*/

	//pObject->AddChild(pChild);
	//pLevel->AddGameObject(pChild, 3);

	// GameObject �ʱ�ȭ
	/*CGameObject* pObject = nullptr;

	pObject = new CGameObject;
	pObject->SetName(L"Player");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CPlayerScript);
	pObject->AddComponent(new CAnimator2D);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 800.f));
	pObject->Transform()->SetRelativeScale(Vec3(256.f, 256.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pObject->Animator2D()->CreateAnimation(L"LeftWalk", CResMgr::GetInst()->FindRes<CTexture>(L"link"), Vec2(0.f, 650.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"Test", CResMgr::GetInst()->FindRes<CTexture>(L"link"), Vec2(0.f, 650.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	pObject->Animator2D()->Play(L"LeftWalk", true);

	Vec4 vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);
	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Player"));
	
	pLevel->AddGameObject(pObject, 1);*/

	// TileMap Object
	/*CGameObject* pTileMapObj = new CGameObject;
	pTileMapObj->SetName(L"TileMap");
	pTileMapObj->AddComponent(new CTransform);
	pTileMapObj->AddComponent(new CTileMap);

	pTileMapObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 1000.f));
	pTileMapObj->Transform()->SetRelativeScale(Vec3(800.f, 800.f, 1.f));

	pTileMapObj->TileMap()->SetTileAtlas(CResMgr::GetInst()->FindRes<CTexture>(L"TILEAtlas"));
	pTileMapObj->TileMap()->SetTileCount(10, 10);

	pLevel->AddGameObject(pTileMapObj, 0);*/

	// Particle Object
	/*CGameObject* pParticle = new CGameObject;
	pParticle->SetName(L"Particle");
	pParticle->AddComponent(new CTransform);
	pParticle->AddComponent(new CParticleSystem);
	
	pParticle->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	
	pParticle->ParticleSystem()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));
	pParticle->ParticleSystem()->SetCS((CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get());
	pParticle->ParticleSystem()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Bubbles50px"));

	pParticle->ParticleSystem()->SetMinMaxLifeTime(Vec2(0.2f, 1.f));
	pParticle->ParticleSystem()->SetMinMaxSpeed(Vec2(100.f, 300.f));
	pParticle->ParticleSystem()->Set_SE_Color(Vec4(0.24f, 0.28f, 0.8f, 1.f), Vec4(0.78f, 0.74f, 0.9f, 1.f));
	pParticle->ParticleSystem()->Set_SE_Scale(Vec4(50.f, 50.f, 1.f, 0.f), Vec4(10.f, 10.f, 1.f, 0.f));
	pParticle->ParticleSystem()->SetWorldSpawn(true);
	
	pParticle->ParticleSystem()->SetFrequency(5.f);
	pParticle->ParticleSystem()->SetSpawnRange(100.f);
	
	pLevel->AddGameObject(pParticle, 0);*/

	// PostProcess Object
	/*CGameObject* pPostProcess = new CGameObject;
	pPostProcess->SetName(L"PostProcess");

	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);

	pPostProcess->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));
	pPostProcess->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PostProcessMtrl"));

	pLevel->AddGameObject(pPostProcess, 0);*/


	// �浹 ���̾� ����
	//CCollisionMgr::GetInst()->CollisionLayerCheck(1, 3);

	// Level ����
	//pLevel->SetName(L"Test Level");
	//CSaveLoadMgr::GetInst()->SaveLevel(pLevel);
	CLevelMgr::GetInst()->RegisterLevel(pLevel->GetRelativePath(), pLevel);

	// ���� ����
	// ���� �׽�Ʈ ������ ���� ������ ��ȯ
	ChangeLevel(pLevel);
}
