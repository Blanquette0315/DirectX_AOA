#include "pch.h"
#include "CHPPotionScript.h"

CHPPotionScript::CHPPotionScript()
	: CScript(HPPOTIONSCRIPT)
{
}

CHPPotionScript::~CHPPotionScript()
{
}

void CHPPotionScript::CreatePotionUseSound()
{
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Potion\\UsePotion(potions_mixing_alchemy_01 + Potion - Boiling).wav");
	pSound->Play(1, 1.f, true);
}

void CHPPotionScript::begin()
{
}

void CHPPotionScript::tick()
{

}