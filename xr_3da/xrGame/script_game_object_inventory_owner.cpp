////////////////////////////////////////////////////////////////////////////
// script_game_object_inventory_owner.�pp :	������� ��� inventory owner
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "script_space.h"
#include "script_game_object.h"
#include "script_game_object_impl.h"
#include "InventoryOwner.h"
#include "Pda.h"
#include "xrMessages.h"
#include "character_info.h"
#include "gametask.h"
#include "actor.h"
#include "level.h"
#include "date_time.h"
#include "uigamesp.h"
#include "hudmanager.h"
#include "restricted_object.h"
#include "script_engine.h"
#include "attachable_item.h"
#include "script_entity.h"
#include "string_table.h"
#include "alife_registry_wrappers.h"
#include "relation_registry.h"
#include "custommonster.h"
#include "movement_manager.h"
#include "actorcondition.h"
#include "level_navigation_graph.h"
#include "ui/UIMainIngameWnd.h"
#include "inventory.h"

bool CScriptGameObject::GiveInfoPortion(LPCSTR info_id)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return false;
	pInventoryOwner->TransferInfo(info_id, true);
	return			true;
}

bool CScriptGameObject::DisableInfoPortion(LPCSTR info_id)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return false;
	pInventoryOwner->TransferInfo(info_id, false);
	return true;
}


void  CScriptGameObject::SetNewsShowTime	(LPCSTR news, int show_time)
{
	if(HUD().GetUI())
		HUD().GetUI()->UIMainIngameWnd->SetDelayForPdaMessage(CStringTable().IndexById(news), show_time);
}
bool  CScriptGameObject::GiveGameNews		(LPCSTR news, LPCSTR texture_name, int x1, int y1, int x2, int y2, u32 delay)
{
	CActor* pActor = smart_cast<CActor*>(&object());
	if(!pActor) return false;

	GAME_NEWS_DATA news_data;
	news_data.news_text = CStringTable().IndexById(news);
	news_data.show_time = DEFAULT_NEWS_SHOW_TIME;
	
	if(xr_strlen(texture_name)>0)
	{
		news_data.texture_name = texture_name;
		news_data.x1 = x1;
		news_data.x2 = x2;
		news_data.y1 = y1;
		news_data.y2 = y2;
	}
	else
	{
		news_data.texture_name = NULL;
	}

	if (news_data.news_text == NO_STRING) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"no entrance found for news in string table", news);
		return								false;
	}
	
	if(delay==0)
		pActor->AddGameNews(news_data);
	else
		pActor->AddGameNews_deffered(news_data,delay);

	return true;
}
bool CScriptGameObject::GiveInfoPortionViaPda(LPCSTR info_id, CScriptGameObject* pFromWho)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return false;
	if(!pInventoryOwner->GetPDA()) return false;

	CInventoryOwner* pFromWhoInvOwner = smart_cast<CInventoryOwner*>(&pFromWho->object());
	if(!pFromWhoInvOwner) return false;
	if(!pFromWhoInvOwner->GetPDA()) return false;

	//���������� �� ������ PDA ����� ���������� � �������
	NET_Packet		P;
	object().u_EventGen(P,GE_PDA,pInventoryOwner->GetPDA()->ID());
	P.w_u16			(u16(pFromWhoInvOwner->GetPDA()->ID()));		//�����������
	P.w_s16			(ePdaMsgInfo);									
	P.w_s32			(CInfoPortion::IdToIndex(info_id));
	object().u_EventSend(P);
	return			true;
}


bool  CScriptGameObject::HasInfo				(LPCSTR info_id)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return false;
//	if(!pInventoryOwner->GetPDA()) return false;
	return pInventoryOwner->HasInfo(info_id);

}
bool  CScriptGameObject::DontHasInfo			(LPCSTR info_id)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return true;
//	if(!pInventoryOwner->GetPDA()) return true;
	return !pInventoryOwner->HasInfo(info_id);
}

xrTime CScriptGameObject::GetInfoTime			(LPCSTR info_id)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return xrTime(0);

	INFO_DATA info_data;
	if(pInventoryOwner->GetInfo(info_id, info_data))
		return xrTime(info_data.receive_time);
	else
		return xrTime(0);
}


bool CScriptGameObject::SendPdaMessage(EPdaMsg pda_msg, CScriptGameObject* pForWho)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return false;
	if(!pInventoryOwner->GetPDA()) return false;

	CInventoryOwner* pForWhoInvOwner = smart_cast<CInventoryOwner*>(&pForWho->object());
	if(!pForWhoInvOwner) return false;
	if(!pForWhoInvOwner->GetPDA()) return false;

	//���������� �� ������ PDA ����� ���������� � �������
	NET_Packet		P;
	object().u_EventGen(P,GE_PDA,pForWhoInvOwner->GetPDA()->ID());
	P.w_u16			(u16(pInventoryOwner->GetPDA()->ID()));		//�����������
	P.w_s16			((u16)pda_msg);
	P.w_stringZ		(NULL);
	object().u_EventSend(P);
	return			true;
}


bool CScriptGameObject::IsTalking()
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return false;
	return			pInventoryOwner->IsTalking();
}

void CScriptGameObject::StopTalk()
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return;
	pInventoryOwner->StopTalk();
}
void CScriptGameObject::EnableTalk()
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return;
	pInventoryOwner->EnableTalk();
}
void CScriptGameObject::DisableTalk()
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return;
	pInventoryOwner->DisableTalk();
}

bool CScriptGameObject::IsTalkEnabled()
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return false;
	return pInventoryOwner->IsTalkEnabled();
}

void CScriptGameObject::EnableTrade			()
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return;
	pInventoryOwner->EnableTrade();
}
void CScriptGameObject::DisableTrade		()
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return;
	pInventoryOwner->DisableTrade();
}
bool CScriptGameObject::IsTradeEnabled		()
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if(!pInventoryOwner) return false;
	return pInventoryOwner->IsTradeEnabled();
}

void CScriptGameObject::ForEachInventoryItems(const luabind::functor<void> &functor)
{
	CInventoryOwner* owner = smart_cast<CInventoryOwner*>(&object());
	if(!owner){
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CScriptGameObject::ForEachInventoryItems non-CInventoryOwner object !!!");
		return;
	}
	
	CInventory* pInv = &owner->inventory();
	TIItemContainer item_list;
	pInv->AddAvailableItems(item_list, true);

	TIItemContainer::iterator it;
	for(it =  item_list.begin(); item_list.end() != it; ++it) 
	{
		CGameObject* inv_go = smart_cast<CGameObject*>(*it);
		if( inv_go ){
			functor(inv_go->lua_game_object(),this);
		}
	}
}


void CScriptGameObject::DropItem(CScriptGameObject* pItem, Fvector pos)
{
	CInventoryOwner* owner = smart_cast<CInventoryOwner*>(&object());
	CInventoryItem* item = smart_cast<CInventoryItem*>(&pItem->object());
	if(!owner||!item){
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CScriptGameObject::DropItem non-CInventoryOwner object !!!");
		return;
	}

	NET_Packet						P;
	CGameObject::u_EventGen			(P,GE_OWNERSHIP_REJECT, object().ID());
	P.w_u16							(pItem->object().ID());
	CGameObject::u_EventSend		(P);

	NET_Packet						PP;
	CGameObject::u_EventGen			(PP,GE_CHANGE_POS, pItem->object().ID());
	PP.w_vec3						(pos);
	CGameObject::u_EventSend		(PP);

/*	CPHSynchronize* pSyncObj = NULL;
	pSyncObj = item->object().PHGetSyncItem(0);
	if (!pSyncObj) return;
	SPHNetState state;
	pSyncObj->get_State(state);
	state.position = pos;
	state.previous_position = pos;
	pSyncObj->set_State(state);

*/
}

//�������� ���� �� ������ ��������� � ��������� ��������
void CScriptGameObject::TransferItem(CScriptGameObject* pItem, CScriptGameObject* pForWho)
{
	if (!pItem || !pForWho) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"cannot transfer NULL item");
		return;
	}

	CInventoryItem* pIItem = smart_cast<CInventoryItem*>(&pItem->object());
//	VERIFY(pIItem);
	if (!pIItem) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"Cannot transfer not CInventoryItem item");
		return ;
	}

	// ��������� � ���� 
	NET_Packet						P;
	CGameObject::u_EventGen			(P,GE_OWNERSHIP_REJECT, object().ID());
	P.w_u16							(pIItem->object().ID());
	CGameObject::u_EventSend		(P);

	// ������ ��������
	CGameObject::u_EventGen			(P,GE_OWNERSHIP_TAKE, pForWho->object().ID());
	P.w_u16							(pIItem->object().ID());
	CGameObject::u_EventSend		(P);
}

u32 CScriptGameObject::Money	()
{
	CInventoryOwner* pOurOwner		= smart_cast<CInventoryOwner*>(&object()); VERIFY(pOurOwner);
	return pOurOwner->m_dwMoney;
}

void CScriptGameObject::TransferMoney(int money, CScriptGameObject* pForWho)
{
	if (!pForWho) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"cannot transfer money for NULL object");
		return;
	}
	CInventoryOwner* pOurOwner		= smart_cast<CInventoryOwner*>(&object()); VERIFY(pOurOwner);
	CInventoryOwner* pOtherOwner	= smart_cast<CInventoryOwner*>(&pForWho->object()); VERIFY(pOtherOwner);

	if (pOurOwner->m_dwMoney-money<0) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"Character does not have enought money");
		return;
	}
//	R_ASSERT3(pOurOwner->m_dwMoney-money>=0, "Character does not have enought money", pOurOwner->CharacterInfo().Name());

	pOurOwner->m_dwMoney	-= money;
	pOtherOwner->m_dwMoney	+= money;
}
//////////////////////////////////////////////////////////////////////////

int	CScriptGameObject::GetGoodwill(CScriptGameObject* pToWho)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"GetGoodwill available only for InventoryOwner");
		return 0;
	}
	return RELATION_REGISTRY().GetGoodwill(pInventoryOwner->object_id(), pToWho->object().ID());
}

void CScriptGameObject::SetGoodwill(int goodwill, CScriptGameObject* pWhoToSet)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"SetGoodwill available only for InventoryOwner");
		return ;
	}
	return RELATION_REGISTRY().SetGoodwill(pInventoryOwner->object_id(), pWhoToSet->object().ID(), goodwill);
}

void CScriptGameObject::ChangeGoodwill(int delta_goodwill, CScriptGameObject* pWhoToSet)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"ChangeGoodwill available only for InventoryOwner");
		return ;
	}
	RELATION_REGISTRY().ChangeGoodwill(pInventoryOwner->object_id(), pWhoToSet->object().ID(), delta_goodwill);
}

//////////////////////////////////////////////////////////////////////////

void CScriptGameObject::SetCommunityGoodwill(int goodwill, CScriptGameObject* pWhoToSet)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"SetCommunityGoodwill available only for InventoryOwner");
		return ;
	}
	RELATION_REGISTRY().SetCommunityGoodwill(pInventoryOwner->object_id(), pWhoToSet->object().ID(), goodwill);
}

int	CScriptGameObject::GetCommunityGoodwill(CScriptGameObject* pToWho)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"GetCommunityGoodwill available only for InventoryOwner");
		return 0;
	}
	return RELATION_REGISTRY().GetCommunityGoodwill(pInventoryOwner->object_id(), pToWho->object().ID());
}

void CScriptGameObject::ChangeCommunityGoodwill(int delta_goodwill, CScriptGameObject* pWhoToSet)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"ChangeCommunityGoodwill available only for InventoryOwner");
		return ;
	}
	RELATION_REGISTRY().ChangeCommunityGoodwill(pInventoryOwner->object_id(), pWhoToSet->object().ID(), delta_goodwill);
}

//////////////////////////////////////////////////////////////////////////

void CScriptGameObject::SetRelation(ALife::ERelationType relation, CScriptGameObject* pWhoToSet)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"SetRelation available only for InventoryOwner");
		return ;
	}

	CInventoryOwner* pOthersInventoryOwner = smart_cast<CInventoryOwner*>(&pWhoToSet->object());
	VERIFY(pOthersInventoryOwner);
	if (!pOthersInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"SetRelation available only for InventoryOwner");
		return ;
	}
	RELATION_REGISTRY().SetRelationType(pInventoryOwner, pOthersInventoryOwner, relation);
}

//////////////////////////////////////////////////////////////////////////

int	CScriptGameObject::GetAttitude			(CScriptGameObject* pToWho)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());VERIFY(pInventoryOwner);
	CInventoryOwner* pOthersInventoryOwner = smart_cast<CInventoryOwner*>(&pToWho->object());VERIFY(pOthersInventoryOwner);
	return RELATION_REGISTRY().GetAttitude(pInventoryOwner, pOthersInventoryOwner);
}


//////////////////////////////////////////////////////////////////////////

LPCSTR CScriptGameObject::ProfileName			()
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"ProfileName available only for InventoryOwner");
		return NULL;
	}
	
	PROFILE_ID profile_id =  pInventoryOwner->CharacterInfo().Profile();
	if(!profile_id || !profile_id.size() )
		return NULL;
	else
		return *profile_id;
}


LPCSTR CScriptGameObject::CharacterName			()
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CharacterName available only for InventoryOwner");
		return NULL;
	}
	return pInventoryOwner->CharacterInfo().Name();
}
int CScriptGameObject::CharacterRank			()
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CharacterRank available only for InventoryOwner");
		return 0;
	}
	return pInventoryOwner->Rank();
}
void CScriptGameObject::SetCharacterRank			(int char_rank)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"SetCharacterRank available only for InventoryOwner");
		return ;
	}
	return pInventoryOwner->SetRank(char_rank);
}

void CScriptGameObject::ChangeCharacterRank			(int char_rank)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"ChangeCharacterRank available only for InventoryOwner");
		return;
	}
	return pInventoryOwner->ChangeRank(char_rank);
}

int CScriptGameObject::CharacterReputation			()
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CharacterReputation available only for InventoryOwner");
		return 0;
	}
	return pInventoryOwner->Reputation();
}

void CScriptGameObject::SetCharacterReputation		(int char_rep)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"SetCharacterReputation available only for InventoryOwner");
		return ;
	}
	pInventoryOwner->SetReputation(char_rep);
}

void CScriptGameObject::ChangeCharacterReputation		(int char_rep)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"ChangeCharacterReputation available only for InventoryOwner");
		return ;
	}
	pInventoryOwner->ChangeReputation(char_rep);
}

LPCSTR CScriptGameObject::CharacterCommunity	()
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CharacterCommunity available only for InventoryOwner");
		return NULL;
	}
	return *pInventoryOwner->CharacterInfo().Community().id();
}

void CScriptGameObject::SetCharacterCommunity	(LPCSTR comm)
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"SetCharacterCommunity available only for InventoryOwner");
		return;
	}
	CHARACTER_COMMUNITY	community;
	community.set(comm);
	return pInventoryOwner->SetCommunity(community.index());
}

LPCSTR CScriptGameObject::snd_character_profile_sect () const
{
	CInventoryOwner* pInventoryOwner = smart_cast<CInventoryOwner*>(&object());
//	VERIFY(pInventoryOwner);
	if (!pInventoryOwner) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"snd_character_profile_sect available only for InventoryOwner");
		return NULL;
	}

	return pInventoryOwner->SpecificCharacter().SndConfigSect();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*
struct EGameTask{

};
xr_vector<>
CScriptGameObject::GetGameTaskState	(LPCSTR task_id, int objective_num)
{

}
*/
ETaskState CScriptGameObject::GetGameTaskState	(LPCSTR task_id, int objective_num)
{
	CActor* pActor = smart_cast<CActor*>(&object());
//	VERIFY(pActor);
	if (!pActor) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"GetGameTaskState available only for actor");
		return eTaskStateDummy;
	}

	const GAME_TASK_VECTOR* tasks =  pActor->game_task_registry->registry().objects_ptr();
	if(!tasks) 
		return eTaskStateDummy;

	GAME_TASK_VECTOR::const_iterator it = tasks->begin();
	for(; tasks->end() != it; it++)
	{
		if((*it).task_id == task_id) 
			break;
	}
	
	if(tasks->end() == it) 
		return eTaskStateDummy;

//	R_ASSERT3((std::size_t)objective_num < (*it).states.size(), "wrong objective num", task_id);
	if ((std::size_t)objective_num >= (*it).states.size()) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"wrong objective num", task_id);
		return eTaskStateDummy;
	}

	return (*it).states[objective_num];
}

void CScriptGameObject::SetGameTaskState	(ETaskState state, LPCSTR task_id, int objective_num)
{
	CActor* pActor = smart_cast<CActor*>(&object());
//	VERIFY(pActor);
	if (!pActor) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"SetGameTaskState available only for actor");
		return;
	}

//	TASK_INDEX task_index = CGameTask::IdToIndex(task_id);
//	R_ASSERT3(task_index != NO_TASK, "wrong task id", task_id);
	
	GAME_TASK_VECTOR& tasks =  pActor->game_task_registry->registry().objects();
	GAME_TASK_VECTOR::iterator it = tasks.begin();
	for(; tasks.end() != it; it++)
	{
		if((*it).task_id == task_id) 
			break;
	}

	if (tasks.end() == it) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"actor does not has task", task_id);
		return;
	}
	if ((std::size_t)objective_num >= (*it).states.size()) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"wrong objective num", task_id);
		return;
	}

//	R_ASSERT3(tasks.end() != it, "actor does not has task", task_id);
//	R_ASSERT3((std::size_t)objective_num < (*it).states.size(), "wrong objective num", task_id);
	(*it).states[objective_num] = state;
	
	if(0 == objective_num){//setState for task and all sub-tasks
		TASK_STATE_IT iit =(*it).states.begin();
		for(;iit!=(*it).states.end();++iit)
			if( (*iit)==eTaskStateInProgress )
				(*iit)=state;
	}

	//���� �� ������������� ��������� ��������� ��� ��������� �������, ��
	//��������� ����� ����������
	if(0 == objective_num && eTaskStateCompleted == state || eTaskStateFail == state)
	{
		(*it).finish_time = Level().GetGameTime();
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void  CScriptGameObject::SwitchToTrade		()
{
	CActor* pActor = smart_cast<CActor*>(&object());	if(!pActor) return;

	//������ ���� ��������� � ������ single
	CUIGameSP* pGameSP = smart_cast<CUIGameSP*>(HUD().GetUI()->UIGame());
	if(!pGameSP) return;

	if(pGameSP->TalkMenu.IsShown())
	{
		pGameSP->TalkMenu.SwitchToTrade();
	}
}
void  CScriptGameObject::SwitchToTalk		()
{
	CActor* pActor = smart_cast<CActor*>(&object());	if(!pActor) return;

	//������ ���� ��������� � ������ single
	CUIGameSP* pGameSP = smart_cast<CUIGameSP*>(HUD().GetUI()->UIGame());
	if(!pGameSP) return;

	if(pGameSP->TradeMenu.IsShown())
	{
		pGameSP->TradeMenu.SwitchToTalk();
	}
}

void  CScriptGameObject::RunTalkDialog			(CScriptGameObject* pToWho)
{
	CActor* pActor = smart_cast<CActor*>(&object());	
//	R_ASSERT2(pActor, "RunTalkDialog applicable only for actor");

	if (!pActor) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"RunTalkDialog applicable only for actor");
		return;
	}

	CInventoryOwner* pPartner = smart_cast<CInventoryOwner*>(&pToWho->object());	VERIFY(pPartner);
	pActor->RunTalkDialog(pPartner);
}

void  CScriptGameObject::ActorSleep			(int hours, int minutes)
{
	CActor* pActor = smart_cast<CActor*>(&object());	if(!pActor) return;
	pActor->conditions().GoSleep(generate_time(1,1,1,hours, minutes, 0, 0), true);
}

//////////////////////////////////////////////////////////////////////////

void construct_restriction_vector(shared_str restrictions, xr_vector<ALife::_OBJECT_ID> &result)
{
	result.clear();
	string64	temp;
	u32			n = _GetItemCount(*restrictions);
	for (u32 i=0; i<n; ++i) {
		CObject	*object = Level().Objects.FindObjectByName(_GetItem(*restrictions,i,temp));
		if (!object)
			continue;
		result.push_back(object->ID());
	}
}

void CScriptGameObject::add_restrictions		(LPCSTR out, LPCSTR in)
{
	CCustomMonster	*monster = smart_cast<CCustomMonster*>(&object());
	if (!monster) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CRestrictedObject : cannot access class member add_restrictions!");
		return;
	}
	
	xr_vector<ALife::_OBJECT_ID>			temp0;
	xr_vector<ALife::_OBJECT_ID>			temp1;

	construct_restriction_vector			(out,temp0);
	construct_restriction_vector			(in,temp1);
	
	monster->movement().restrictions().add_restrictions(temp0,temp1);
}

void CScriptGameObject::remove_restrictions		(LPCSTR out, LPCSTR in)
{
	CCustomMonster	*monster = smart_cast<CCustomMonster*>(&object());
	if (!monster) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CRestrictedObject : cannot access class member remove_restrictions!");
		return;
	}

	xr_vector<ALife::_OBJECT_ID>			temp0;
	xr_vector<ALife::_OBJECT_ID>			temp1;

	construct_restriction_vector			(out,temp0);
	construct_restriction_vector			(in,temp1);

	monster->movement().restrictions().remove_restrictions	(temp0,temp1);
}

void CScriptGameObject::remove_all_restrictions	()
{
	CCustomMonster	*monster = smart_cast<CCustomMonster*>(&object());
	if (!monster) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CRestrictedObject : cannot access class member remove_all_restrictions!");
		return;
	}
	monster->movement().restrictions().remove_all_restrictions	();
}

LPCSTR CScriptGameObject::in_restrictions	()
{
	CCustomMonster	*monster = smart_cast<CCustomMonster*>(&object());
	if (!monster) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CRestrictedObject : cannot access class member in_restrictions!");
		return								("");
	}
	return									(*monster->movement().restrictions().in_restrictions());
}

LPCSTR CScriptGameObject::out_restrictions	()
{
	CCustomMonster	*monster = smart_cast<CCustomMonster*>(&object());
	if (!monster) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CRestrictedObject : cannot access class member out_restrictions!");
		return								("");
	}
	return									(*monster->movement().restrictions().out_restrictions());
}

bool CScriptGameObject::accessible_position	(const Fvector &position)
{
	CCustomMonster	*monster = smart_cast<CCustomMonster*>(&object());
	if (!monster) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CRestrictedObject : cannot access class member accessible!");
		return								(false);
	}
	return									(monster->movement().restrictions().accessible(position));
}

bool CScriptGameObject::accessible_vertex_id(u32 level_vertex_id)
{
	CCustomMonster	*monster = smart_cast<CCustomMonster*>(&object());
	if (!monster) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CRestrictedObject : cannot access class member accessible!");
		return								(false);
	}
	THROW2									(ai().level_graph().valid_vertex_id(level_vertex_id),"Cannot check if level vertex id is accessible, because it is invalid");
	return									(monster->movement().restrictions().accessible(level_vertex_id));
}

u32	 CScriptGameObject::accessible_nearest	(const Fvector &position, Fvector &result)
{
	CCustomMonster	*monster = smart_cast<CCustomMonster*>(&object());
	if (!monster) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CRestrictedObject : cannot access class member accessible!");
		return								(u32(-1));
	}
	return									(monster->movement().restrictions().accessible_nearest(position,result));
}

bool CScriptGameObject::limping				() const
{
	CEntityAlive							*entity_alive = smart_cast<CEntityAlive*>(&object());
	if (!entity_alive) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CRestrictedObject : cannot access class member limping!");
		return								(false);
	}
	return									(entity_alive->conditions().IsLimping());
}

void CScriptGameObject::enable_attachable_item	(bool value)
{
	CAttachableItem							*attachable_item = smart_cast<CAttachableItem*>(&object());
	if (!attachable_item) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CAttachableItem : cannot access class member enable_attachable_item!");
		return;
	}
	attachable_item->enable					(value);
}

bool CScriptGameObject::attachable_item_enabled	() const
{
	CAttachableItem							*attachable_item = smart_cast<CAttachableItem*>(&object());
	if (!attachable_item) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"CAttachableItem : cannot access class member attachable_item_enabled!");
		return								(false);
	}
	return									(attachable_item->enabled());
}



void  CScriptGameObject::RestoreWeapon		()
{
	CActor* pActor = smart_cast<CActor*>(&object());	VERIFY(pActor);
	pActor->RestoreHidedWeapon(GEG_PLAYER_INVENTORYMENU_CLOSE);
}
void  CScriptGameObject::HideWeapon			()
{
	CActor* pActor = smart_cast<CActor*>(&object());	VERIFY(pActor);
	pActor->HideCurrentWeapon(GEG_PLAYER_INVENTORYMENU_OPEN);
}


STasks CScriptGameObject::GetAllGameTasks()
{
	STasks	tasks;
	CActor* pActor = smart_cast<CActor*>(&object());
	if (!pActor) {
		ai().script_engine().script_log		(ScriptStorage::eLuaMessageTypeError,"GetGameTaskState available only for actor");
		return tasks;
	}
	const GAME_TASK_VECTOR* tasks_ =  pActor->game_task_registry->registry().objects_ptr();
	if(!tasks_) 
		return tasks;

	for(GAME_TASK_VECTOR::const_iterator it = tasks_->begin(); tasks_->end() != it; ++it)
	{
		CGameTask gt;
		gt.Init(*it);
		tasks.m_all_tasks.resize(tasks.Size()+1);
		STask& t = tasks.m_all_tasks.back();
		t.m_name	= gt.Id();
		t.m_state	= gt.m_ObjectiveStates[0];
		u32 sub_num = gt.ObjectivesNum();
		for(u32 i=0; i<sub_num;++i){
			t.m_objectives.resize(t.m_objectives.size()+1);
			STaskObjective& to	= t.m_objectives.back();
			to.m_name			= gt.ObjectiveDesc(i);
			to.m_state			= gt.ObjectiveState(i);
		}
	}
	return tasks;
};