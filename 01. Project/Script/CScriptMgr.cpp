#include "pch.h"
#include "CScriptMgr.h"

#include "CActMonHPBarScript.h"
#include "CArenaBGMScript.h"
#include "CAttackNumScript.h"
#include "CBagSlot0Script.h"
#include "CBagSlot1Script.h"
#include "CBigCostScript.h"
#include "CBloodEffectScript.h"
#include "CCardAttackScript.h"
#include "CCardAttNumScript.h"
#include "CCardCostNumScript.h"
#include "CCardDeffenceScript.h"
#include "CCardDefNumScript.h"
#include "CCardMgrScript.h"
#include "CCardMoveScript.h"
#include "CCardScript.h"
#include "CChallengerScript.h"
#include "CChargeScript.h"
#include "CCrossBowScript.h"
#include "CDemonDogScript.h"
#include "CDiscardPileScript.h"
#include "CDrawHandCntScript.h"
#include "CDrawPileScript.h"
#include "CEndingSoundScript.h"
#include "CExhasusPileScript.h"
#include "CFastScript.h"
#include "CGameMgrScript.h"
#include "CHandCardLimitScript.h"
#include "CHitEffectScript.h"
#include "CHPPotionScript.h"
#include "CICardAttNumScript.h"
#include "CICardDefNumScript.h"
#include "CIceReaperScript.h"
#include "CItemCardScript.h"
#include "CLeftWaponSlotScript.h"
#include "CLv0MonSpawnScript.h"
#include "CLv1MonSpawnScript.h"
#include "CLvChanger0Script.h"
#include "CLvChanger1Script.h"
#include "CMainBGMScript.h"
#include "CMissileScript.h"
#include "CMouseCheckScript.h"
#include "CNormalKatanaScript.h"
#include "CNumberScript.h"
#include "COldShieldScript.h"
#include "COldSwordScript.h"
#include "CPersonFBase0Script.h"
#include "CPlayerHandScript.h"
#include "CPlayerHPBarScript.h"
#include "CPlayerHPScript.h"
#include "CPlayerLeftHandScript.h"
#include "CPlayerRightHandScript.h"
#include "CPlayerScript.h"
#include "CPlayerShieldScript.h"
#include "CRefBeetleScript.h"
#include "CRightWaponSlotScript.h"
#include "CShieldNumScript.h"
#include "CStartBtnScript.h"
#include "CstdAttackEffScript.h"
#include "CstdParticleScript.h"
#include "CTileMgrScript.h"
#include "CTileScript.h"
#include "CTLv0MonSpawnScript.h"
#include "CTLv1MonSpawnScript.h"
#include "CTLv2MonSpawnScript.h"
#include "CTLv3MonSpawnScript.h"
#include "CTLv4MonSpawnScript.h"
#include "CTLv5MonSpawnScript.h"
#include "CTurnEndBtnScript.h"
#include "CUIMgrScript.h"
#include "CUnderAttackScript.h"
#include "CWinLogoScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CActMonHPBarScript");
	_vec.push_back(L"CArenaBGMScript");
	_vec.push_back(L"CAttackNumScript");
	_vec.push_back(L"CBagSlot0Script");
	_vec.push_back(L"CBagSlot1Script");
	_vec.push_back(L"CBigCostScript");
	_vec.push_back(L"CBloodEffectScript");
	_vec.push_back(L"CCardAttackScript");
	_vec.push_back(L"CCardAttNumScript");
	_vec.push_back(L"CCardCostNumScript");
	_vec.push_back(L"CCardDeffenceScript");
	_vec.push_back(L"CCardDefNumScript");
	_vec.push_back(L"CCardMgrScript");
	_vec.push_back(L"CCardMoveScript");
	_vec.push_back(L"CCardScript");
	_vec.push_back(L"CChallengerScript");
	_vec.push_back(L"CChargeScript");
	_vec.push_back(L"CCrossBowScript");
	_vec.push_back(L"CDemonDogScript");
	_vec.push_back(L"CDiscardPileScript");
	_vec.push_back(L"CDrawHandCntScript");
	_vec.push_back(L"CDrawPileScript");
	_vec.push_back(L"CEndingSoundScript");
	_vec.push_back(L"CExhasusPileScript");
	_vec.push_back(L"CFastScript");
	_vec.push_back(L"CGameMgrScript");
	_vec.push_back(L"CHandCardLimitScript");
	_vec.push_back(L"CHitEffectScript");
	_vec.push_back(L"CHPPotionScript");
	_vec.push_back(L"CICardAttNumScript");
	_vec.push_back(L"CICardDefNumScript");
	_vec.push_back(L"CIceReaperScript");
	_vec.push_back(L"CItemCardScript");
	_vec.push_back(L"CLeftWaponSlotScript");
	_vec.push_back(L"CLv0MonSpawnScript");
	_vec.push_back(L"CLv1MonSpawnScript");
	_vec.push_back(L"CLvChanger0Script");
	_vec.push_back(L"CLvChanger1Script");
	_vec.push_back(L"CMainBGMScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMouseCheckScript");
	_vec.push_back(L"CNormalKatanaScript");
	_vec.push_back(L"CNumberScript");
	_vec.push_back(L"COldShieldScript");
	_vec.push_back(L"COldSwordScript");
	_vec.push_back(L"CPersonFBase0Script");
	_vec.push_back(L"CPlayerHandScript");
	_vec.push_back(L"CPlayerHPBarScript");
	_vec.push_back(L"CPlayerHPScript");
	_vec.push_back(L"CPlayerLeftHandScript");
	_vec.push_back(L"CPlayerRightHandScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPlayerShieldScript");
	_vec.push_back(L"CRefBeetleScript");
	_vec.push_back(L"CRightWaponSlotScript");
	_vec.push_back(L"CShieldNumScript");
	_vec.push_back(L"CStartBtnScript");
	_vec.push_back(L"CstdAttackEffScript");
	_vec.push_back(L"CstdParticleScript");
	_vec.push_back(L"CTileMgrScript");
	_vec.push_back(L"CTileScript");
	_vec.push_back(L"CTLv0MonSpawnScript");
	_vec.push_back(L"CTLv1MonSpawnScript");
	_vec.push_back(L"CTLv2MonSpawnScript");
	_vec.push_back(L"CTLv3MonSpawnScript");
	_vec.push_back(L"CTLv4MonSpawnScript");
	_vec.push_back(L"CTLv5MonSpawnScript");
	_vec.push_back(L"CTurnEndBtnScript");
	_vec.push_back(L"CUIMgrScript");
	_vec.push_back(L"CUnderAttackScript");
	_vec.push_back(L"CWinLogoScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CActMonHPBarScript" == _strScriptName)
		return new CActMonHPBarScript;
	if (L"CArenaBGMScript" == _strScriptName)
		return new CArenaBGMScript;
	if (L"CAttackNumScript" == _strScriptName)
		return new CAttackNumScript;
	if (L"CBagSlot0Script" == _strScriptName)
		return new CBagSlot0Script;
	if (L"CBagSlot1Script" == _strScriptName)
		return new CBagSlot1Script;
	if (L"CBigCostScript" == _strScriptName)
		return new CBigCostScript;
	if (L"CBloodEffectScript" == _strScriptName)
		return new CBloodEffectScript;
	if (L"CCardAttackScript" == _strScriptName)
		return new CCardAttackScript;
	if (L"CCardAttNumScript" == _strScriptName)
		return new CCardAttNumScript;
	if (L"CCardCostNumScript" == _strScriptName)
		return new CCardCostNumScript;
	if (L"CCardDeffenceScript" == _strScriptName)
		return new CCardDeffenceScript;
	if (L"CCardDefNumScript" == _strScriptName)
		return new CCardDefNumScript;
	if (L"CCardMgrScript" == _strScriptName)
		return new CCardMgrScript;
	if (L"CCardMoveScript" == _strScriptName)
		return new CCardMoveScript;
	if (L"CCardScript" == _strScriptName)
		return new CCardScript;
	if (L"CChallengerScript" == _strScriptName)
		return new CChallengerScript;
	if (L"CChargeScript" == _strScriptName)
		return new CChargeScript;
	if (L"CCrossBowScript" == _strScriptName)
		return new CCrossBowScript;
	if (L"CDemonDogScript" == _strScriptName)
		return new CDemonDogScript;
	if (L"CDiscardPileScript" == _strScriptName)
		return new CDiscardPileScript;
	if (L"CDrawHandCntScript" == _strScriptName)
		return new CDrawHandCntScript;
	if (L"CDrawPileScript" == _strScriptName)
		return new CDrawPileScript;
	if (L"CEndingSoundScript" == _strScriptName)
		return new CEndingSoundScript;
	if (L"CExhasusPileScript" == _strScriptName)
		return new CExhasusPileScript;
	if (L"CFastScript" == _strScriptName)
		return new CFastScript;
	if (L"CGameMgrScript" == _strScriptName)
		return new CGameMgrScript;
	if (L"CHandCardLimitScript" == _strScriptName)
		return new CHandCardLimitScript;
	if (L"CHitEffectScript" == _strScriptName)
		return new CHitEffectScript;
	if (L"CHPPotionScript" == _strScriptName)
		return new CHPPotionScript;
	if (L"CICardAttNumScript" == _strScriptName)
		return new CICardAttNumScript;
	if (L"CICardDefNumScript" == _strScriptName)
		return new CICardDefNumScript;
	if (L"CIceReaperScript" == _strScriptName)
		return new CIceReaperScript;
	if (L"CItemCardScript" == _strScriptName)
		return new CItemCardScript;
	if (L"CLeftWaponSlotScript" == _strScriptName)
		return new CLeftWaponSlotScript;
	if (L"CLv0MonSpawnScript" == _strScriptName)
		return new CLv0MonSpawnScript;
	if (L"CLv1MonSpawnScript" == _strScriptName)
		return new CLv1MonSpawnScript;
	if (L"CLvChanger0Script" == _strScriptName)
		return new CLvChanger0Script;
	if (L"CLvChanger1Script" == _strScriptName)
		return new CLvChanger1Script;
	if (L"CMainBGMScript" == _strScriptName)
		return new CMainBGMScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMouseCheckScript" == _strScriptName)
		return new CMouseCheckScript;
	if (L"CNormalKatanaScript" == _strScriptName)
		return new CNormalKatanaScript;
	if (L"CNumberScript" == _strScriptName)
		return new CNumberScript;
	if (L"COldShieldScript" == _strScriptName)
		return new COldShieldScript;
	if (L"COldSwordScript" == _strScriptName)
		return new COldSwordScript;
	if (L"CPersonFBase0Script" == _strScriptName)
		return new CPersonFBase0Script;
	if (L"CPlayerHandScript" == _strScriptName)
		return new CPlayerHandScript;
	if (L"CPlayerHPBarScript" == _strScriptName)
		return new CPlayerHPBarScript;
	if (L"CPlayerHPScript" == _strScriptName)
		return new CPlayerHPScript;
	if (L"CPlayerLeftHandScript" == _strScriptName)
		return new CPlayerLeftHandScript;
	if (L"CPlayerRightHandScript" == _strScriptName)
		return new CPlayerRightHandScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPlayerShieldScript" == _strScriptName)
		return new CPlayerShieldScript;
	if (L"CRefBeetleScript" == _strScriptName)
		return new CRefBeetleScript;
	if (L"CRightWaponSlotScript" == _strScriptName)
		return new CRightWaponSlotScript;
	if (L"CShieldNumScript" == _strScriptName)
		return new CShieldNumScript;
	if (L"CStartBtnScript" == _strScriptName)
		return new CStartBtnScript;
	if (L"CstdAttackEffScript" == _strScriptName)
		return new CstdAttackEffScript;
	if (L"CstdParticleScript" == _strScriptName)
		return new CstdParticleScript;
	if (L"CTileMgrScript" == _strScriptName)
		return new CTileMgrScript;
	if (L"CTileScript" == _strScriptName)
		return new CTileScript;
	if (L"CTLv0MonSpawnScript" == _strScriptName)
		return new CTLv0MonSpawnScript;
	if (L"CTLv1MonSpawnScript" == _strScriptName)
		return new CTLv1MonSpawnScript;
	if (L"CTLv2MonSpawnScript" == _strScriptName)
		return new CTLv2MonSpawnScript;
	if (L"CTLv3MonSpawnScript" == _strScriptName)
		return new CTLv3MonSpawnScript;
	if (L"CTLv4MonSpawnScript" == _strScriptName)
		return new CTLv4MonSpawnScript;
	if (L"CTLv5MonSpawnScript" == _strScriptName)
		return new CTLv5MonSpawnScript;
	if (L"CTurnEndBtnScript" == _strScriptName)
		return new CTurnEndBtnScript;
	if (L"CUIMgrScript" == _strScriptName)
		return new CUIMgrScript;
	if (L"CUnderAttackScript" == _strScriptName)
		return new CUnderAttackScript;
	if (L"CWinLogoScript" == _strScriptName)
		return new CWinLogoScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ACTMONHPBARSCRIPT:
		return new CActMonHPBarScript;
		break;
	case (UINT)SCRIPT_TYPE::ARENABGMSCRIPT:
		return new CArenaBGMScript;
		break;
	case (UINT)SCRIPT_TYPE::ATTACKNUMSCRIPT:
		return new CAttackNumScript;
		break;
	case (UINT)SCRIPT_TYPE::BAGSLOT0SCRIPT:
		return new CBagSlot0Script;
		break;
	case (UINT)SCRIPT_TYPE::BAGSLOT1SCRIPT:
		return new CBagSlot1Script;
		break;
	case (UINT)SCRIPT_TYPE::BIGCOSTSCRIPT:
		return new CBigCostScript;
		break;
	case (UINT)SCRIPT_TYPE::BLOODEFFECTSCRIPT:
		return new CBloodEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::CARDATTACKSCRIPT:
		return new CCardAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::CARDATTNUMSCRIPT:
		return new CCardAttNumScript;
		break;
	case (UINT)SCRIPT_TYPE::CARDCOSTNUMSCRIPT:
		return new CCardCostNumScript;
		break;
	case (UINT)SCRIPT_TYPE::CARDDEFFENCESCRIPT:
		return new CCardDeffenceScript;
		break;
	case (UINT)SCRIPT_TYPE::CARDDEFNUMSCRIPT:
		return new CCardDefNumScript;
		break;
	case (UINT)SCRIPT_TYPE::CARDMGRSCRIPT:
		return new CCardMgrScript;
		break;
	case (UINT)SCRIPT_TYPE::CARDMOVESCRIPT:
		return new CCardMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CARDSCRIPT:
		return new CCardScript;
		break;
	case (UINT)SCRIPT_TYPE::CHALLENGERSCRIPT:
		return new CChallengerScript;
		break;
	case (UINT)SCRIPT_TYPE::CHARGESCRIPT:
		return new CChargeScript;
		break;
	case (UINT)SCRIPT_TYPE::CROSSBOWSCRIPT:
		return new CCrossBowScript;
		break;
	case (UINT)SCRIPT_TYPE::DEMONDOGSCRIPT:
		return new CDemonDogScript;
		break;
	case (UINT)SCRIPT_TYPE::DISCARDPILESCRIPT:
		return new CDiscardPileScript;
		break;
	case (UINT)SCRIPT_TYPE::DRAWHANDCNTSCRIPT:
		return new CDrawHandCntScript;
		break;
	case (UINT)SCRIPT_TYPE::DRAWPILESCRIPT:
		return new CDrawPileScript;
		break;
	case (UINT)SCRIPT_TYPE::ENDINGSOUNDSCRIPT:
		return new CEndingSoundScript;
		break;
	case (UINT)SCRIPT_TYPE::EXHASUSPILESCRIPT:
		return new CExhasusPileScript;
		break;
	case (UINT)SCRIPT_TYPE::FASTSCRIPT:
		return new CFastScript;
		break;
	case (UINT)SCRIPT_TYPE::GAMEMGRSCRIPT:
		return new CGameMgrScript;
		break;
	case (UINT)SCRIPT_TYPE::HANDCARDLIMITSCRIPT:
		return new CHandCardLimitScript;
		break;
	case (UINT)SCRIPT_TYPE::HITEFFECTSCRIPT:
		return new CHitEffectScript;
		break;
	case (UINT)SCRIPT_TYPE::HPPOTIONSCRIPT:
		return new CHPPotionScript;
		break;
	case (UINT)SCRIPT_TYPE::ICARDATTNUMSCRIPT:
		return new CICardAttNumScript;
		break;
	case (UINT)SCRIPT_TYPE::ICARDDEFNUMSCRIPT:
		return new CICardDefNumScript;
		break;
	case (UINT)SCRIPT_TYPE::ICEREAPERSCRIPT:
		return new CIceReaperScript;
		break;
	case (UINT)SCRIPT_TYPE::ITEMCARDSCRIPT:
		return new CItemCardScript;
		break;
	case (UINT)SCRIPT_TYPE::LEFTWAPONSLOTSCRIPT:
		return new CLeftWaponSlotScript;
		break;
	case (UINT)SCRIPT_TYPE::LV0MONSPAWNSCRIPT:
		return new CLv0MonSpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::LV1MONSPAWNSCRIPT:
		return new CLv1MonSpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::LVCHANGER0SCRIPT:
		return new CLvChanger0Script;
		break;
	case (UINT)SCRIPT_TYPE::LVCHANGER1SCRIPT:
		return new CLvChanger1Script;
		break;
	case (UINT)SCRIPT_TYPE::MAINBGMSCRIPT:
		return new CMainBGMScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MOUSECHECKSCRIPT:
		return new CMouseCheckScript;
		break;
	case (UINT)SCRIPT_TYPE::NORMALKATANASCRIPT:
		return new CNormalKatanaScript;
		break;
	case (UINT)SCRIPT_TYPE::NUMBERSCRIPT:
		return new CNumberScript;
		break;
	case (UINT)SCRIPT_TYPE::OLDSHIELDSCRIPT:
		return new COldShieldScript;
		break;
	case (UINT)SCRIPT_TYPE::OLDSWORDSCRIPT:
		return new COldSwordScript;
		break;
	case (UINT)SCRIPT_TYPE::PERSONFBASE0SCRIPT:
		return new CPersonFBase0Script;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERHANDSCRIPT:
		return new CPlayerHandScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERHPBARSCRIPT:
		return new CPlayerHPBarScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERHPSCRIPT:
		return new CPlayerHPScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERLEFTHANDSCRIPT:
		return new CPlayerLeftHandScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERRIGHTHANDSCRIPT:
		return new CPlayerRightHandScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSHIELDSCRIPT:
		return new CPlayerShieldScript;
		break;
	case (UINT)SCRIPT_TYPE::REFBEETLESCRIPT:
		return new CRefBeetleScript;
		break;
	case (UINT)SCRIPT_TYPE::RIGHTWAPONSLOTSCRIPT:
		return new CRightWaponSlotScript;
		break;
	case (UINT)SCRIPT_TYPE::SHIELDNUMSCRIPT:
		return new CShieldNumScript;
		break;
	case (UINT)SCRIPT_TYPE::STARTBTNSCRIPT:
		return new CStartBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::STDATTACKEFFSCRIPT:
		return new CstdAttackEffScript;
		break;
	case (UINT)SCRIPT_TYPE::STDPARTICLESCRIPT:
		return new CstdParticleScript;
		break;
	case (UINT)SCRIPT_TYPE::TILEMGRSCRIPT:
		return new CTileMgrScript;
		break;
	case (UINT)SCRIPT_TYPE::TILESCRIPT:
		return new CTileScript;
		break;
	case (UINT)SCRIPT_TYPE::TLV0MONSPAWNSCRIPT:
		return new CTLv0MonSpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::TLV1MONSPAWNSCRIPT:
		return new CTLv1MonSpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::TLV2MONSPAWNSCRIPT:
		return new CTLv2MonSpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::TLV3MONSPAWNSCRIPT:
		return new CTLv3MonSpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::TLV4MONSPAWNSCRIPT:
		return new CTLv4MonSpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::TLV5MONSPAWNSCRIPT:
		return new CTLv5MonSpawnScript;
		break;
	case (UINT)SCRIPT_TYPE::TURNENDBTNSCRIPT:
		return new CTurnEndBtnScript;
		break;
	case (UINT)SCRIPT_TYPE::UIMGRSCRIPT:
		return new CUIMgrScript;
		break;
	case (UINT)SCRIPT_TYPE::UNDERATTACKSCRIPT:
		return new CUnderAttackScript;
		break;
	case (UINT)SCRIPT_TYPE::WINLOGOSCRIPT:
		return new CWinLogoScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ACTMONHPBARSCRIPT:
		return L"CActMonHPBarScript";
		break;

	case SCRIPT_TYPE::ARENABGMSCRIPT:
		return L"CArenaBGMScript";
		break;

	case SCRIPT_TYPE::ATTACKNUMSCRIPT:
		return L"CAttackNumScript";
		break;

	case SCRIPT_TYPE::BAGSLOT0SCRIPT:
		return L"CBagSlot0Script";
		break;

	case SCRIPT_TYPE::BAGSLOT1SCRIPT:
		return L"CBagSlot1Script";
		break;

	case SCRIPT_TYPE::BIGCOSTSCRIPT:
		return L"CBigCostScript";
		break;

	case SCRIPT_TYPE::BLOODEFFECTSCRIPT:
		return L"CBloodEffectScript";
		break;

	case SCRIPT_TYPE::CARDATTACKSCRIPT:
		return L"CCardAttackScript";
		break;

	case SCRIPT_TYPE::CARDATTNUMSCRIPT:
		return L"CCardAttNumScript";
		break;

	case SCRIPT_TYPE::CARDCOSTNUMSCRIPT:
		return L"CCardCostNumScript";
		break;

	case SCRIPT_TYPE::CARDDEFFENCESCRIPT:
		return L"CCardDeffenceScript";
		break;

	case SCRIPT_TYPE::CARDDEFNUMSCRIPT:
		return L"CCardDefNumScript";
		break;

	case SCRIPT_TYPE::CARDMGRSCRIPT:
		return L"CCardMgrScript";
		break;

	case SCRIPT_TYPE::CARDMOVESCRIPT:
		return L"CCardMoveScript";
		break;

	case SCRIPT_TYPE::CARDSCRIPT:
		return L"CCardScript";
		break;

	case SCRIPT_TYPE::CHALLENGERSCRIPT:
		return L"CChallengerScript";
		break;

	case SCRIPT_TYPE::CHARGESCRIPT:
		return L"CChargeScript";
		break;

	case SCRIPT_TYPE::CROSSBOWSCRIPT:
		return L"CCrossBowScript";
		break;

	case SCRIPT_TYPE::DEMONDOGSCRIPT:
		return L"CDemonDogScript";
		break;

	case SCRIPT_TYPE::DISCARDPILESCRIPT:
		return L"CDiscardPileScript";
		break;

	case SCRIPT_TYPE::DRAWHANDCNTSCRIPT:
		return L"CDrawHandCntScript";
		break;

	case SCRIPT_TYPE::DRAWPILESCRIPT:
		return L"CDrawPileScript";
		break;

	case SCRIPT_TYPE::ENDINGSOUNDSCRIPT:
		return L"CEndingSoundScript";
		break;

	case SCRIPT_TYPE::EXHASUSPILESCRIPT:
		return L"CExhasusPileScript";
		break;

	case SCRIPT_TYPE::FASTSCRIPT:
		return L"CFastScript";
		break;

	case SCRIPT_TYPE::GAMEMGRSCRIPT:
		return L"CGameMgrScript";
		break;

	case SCRIPT_TYPE::HANDCARDLIMITSCRIPT:
		return L"CHandCardLimitScript";
		break;

	case SCRIPT_TYPE::HITEFFECTSCRIPT:
		return L"CHitEffectScript";
		break;

	case SCRIPT_TYPE::HPPOTIONSCRIPT:
		return L"CHPPotionScript";
		break;

	case SCRIPT_TYPE::ICARDATTNUMSCRIPT:
		return L"CICardAttNumScript";
		break;

	case SCRIPT_TYPE::ICARDDEFNUMSCRIPT:
		return L"CICardDefNumScript";
		break;

	case SCRIPT_TYPE::ICEREAPERSCRIPT:
		return L"CIceReaperScript";
		break;

	case SCRIPT_TYPE::ITEMCARDSCRIPT:
		return L"CItemCardScript";
		break;

	case SCRIPT_TYPE::LEFTWAPONSLOTSCRIPT:
		return L"CLeftWaponSlotScript";
		break;

	case SCRIPT_TYPE::LV0MONSPAWNSCRIPT:
		return L"CLv0MonSpawnScript";
		break;

	case SCRIPT_TYPE::LV1MONSPAWNSCRIPT:
		return L"CLv1MonSpawnScript";
		break;

	case SCRIPT_TYPE::LVCHANGER0SCRIPT:
		return L"CLvChanger0Script";
		break;

	case SCRIPT_TYPE::LVCHANGER1SCRIPT:
		return L"CLvChanger1Script";
		break;

	case SCRIPT_TYPE::MAINBGMSCRIPT:
		return L"CMainBGMScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MOUSECHECKSCRIPT:
		return L"CMouseCheckScript";
		break;

	case SCRIPT_TYPE::NORMALKATANASCRIPT:
		return L"CNormalKatanaScript";
		break;

	case SCRIPT_TYPE::NUMBERSCRIPT:
		return L"CNumberScript";
		break;

	case SCRIPT_TYPE::OLDSHIELDSCRIPT:
		return L"COldShieldScript";
		break;

	case SCRIPT_TYPE::OLDSWORDSCRIPT:
		return L"COldSwordScript";
		break;

	case SCRIPT_TYPE::PERSONFBASE0SCRIPT:
		return L"CPersonFBase0Script";
		break;

	case SCRIPT_TYPE::PLAYERHANDSCRIPT:
		return L"CPlayerHandScript";
		break;

	case SCRIPT_TYPE::PLAYERHPBARSCRIPT:
		return L"CPlayerHPBarScript";
		break;

	case SCRIPT_TYPE::PLAYERHPSCRIPT:
		return L"CPlayerHPScript";
		break;

	case SCRIPT_TYPE::PLAYERLEFTHANDSCRIPT:
		return L"CPlayerLeftHandScript";
		break;

	case SCRIPT_TYPE::PLAYERRIGHTHANDSCRIPT:
		return L"CPlayerRightHandScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PLAYERSHIELDSCRIPT:
		return L"CPlayerShieldScript";
		break;

	case SCRIPT_TYPE::REFBEETLESCRIPT:
		return L"CRefBeetleScript";
		break;

	case SCRIPT_TYPE::RIGHTWAPONSLOTSCRIPT:
		return L"CRightWaponSlotScript";
		break;

	case SCRIPT_TYPE::SHIELDNUMSCRIPT:
		return L"CShieldNumScript";
		break;

	case SCRIPT_TYPE::STARTBTNSCRIPT:
		return L"CStartBtnScript";
		break;

	case SCRIPT_TYPE::STDATTACKEFFSCRIPT:
		return L"CstdAttackEffScript";
		break;

	case SCRIPT_TYPE::STDPARTICLESCRIPT:
		return L"CstdParticleScript";
		break;

	case SCRIPT_TYPE::TILEMGRSCRIPT:
		return L"CTileMgrScript";
		break;

	case SCRIPT_TYPE::TILESCRIPT:
		return L"CTileScript";
		break;

	case SCRIPT_TYPE::TLV0MONSPAWNSCRIPT:
		return L"CTLv0MonSpawnScript";
		break;

	case SCRIPT_TYPE::TLV1MONSPAWNSCRIPT:
		return L"CTLv1MonSpawnScript";
		break;

	case SCRIPT_TYPE::TLV2MONSPAWNSCRIPT:
		return L"CTLv2MonSpawnScript";
		break;

	case SCRIPT_TYPE::TLV3MONSPAWNSCRIPT:
		return L"CTLv3MonSpawnScript";
		break;

	case SCRIPT_TYPE::TLV4MONSPAWNSCRIPT:
		return L"CTLv4MonSpawnScript";
		break;

	case SCRIPT_TYPE::TLV5MONSPAWNSCRIPT:
		return L"CTLv5MonSpawnScript";
		break;

	case SCRIPT_TYPE::TURNENDBTNSCRIPT:
		return L"CTurnEndBtnScript";
		break;

	case SCRIPT_TYPE::UIMGRSCRIPT:
		return L"CUIMgrScript";
		break;

	case SCRIPT_TYPE::UNDERATTACKSCRIPT:
		return L"CUnderAttackScript";
		break;

	case SCRIPT_TYPE::WINLOGOSCRIPT:
		return L"CWinLogoScript";
		break;

	}
	return nullptr;
}