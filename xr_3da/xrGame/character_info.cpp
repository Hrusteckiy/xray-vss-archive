//////////////////////////////////////////////////////////////////////////
// character_info.cpp			������� ���������� ��� ���������� � ����
// 
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "character_info.h"
#include "PhraseDialog.h"
#include "ui/xrXMLParser.h"

#include "alife_registry_container_composition.h"

//////////////////////////////////////////////////////////////////////////

SRelation::SRelation()
{
	m_eRelationType = ALife::eRelationTypeDummy;
}
SRelation::~SRelation()
{
}

ALife::ERelationType SRelation::RelationType() const 
{
	return m_eRelationType;
}
void SRelation::SetRelationType(ALife::ERelationType relation) 
{
	m_eRelationType = relation;
}

int SRelation::Goodwill() const
{
	return m_iGoodwill;
}
void SRelation::SetGoodwill(int new_goodwill)
{
	m_iGoodwill = new_goodwill;
}


//////////////////////////////////////////////////////////////////////////
SCharacterProfile::SCharacterProfile()
{
	m_iCharacterIndex = NO_SPECIFIC_CHARACTER;
	m_Community		= NO_COMMUNITY;
	m_Rank			= NO_RANK;
	m_Reputation	= NO_REPUTATION;
}

SCharacterProfile::~SCharacterProfile()
{
}



//////////////////////////////////////////////////////////////////////////

CCharacterInfo::CCharacterInfo()
{
	m_iProfileIndex = NO_PROFILE;
	m_iSpecificCharacterIndex = NO_SPECIFIC_CHARACTER;

	m_CurrentRank = NO_RANK;
	m_CurrentReputation = NO_REPUTATION;
	m_CurrentCommunity = NO_COMMUNITY;

}


CCharacterInfo::~CCharacterInfo()
{
}


void CCharacterInfo::InitXmlIdToIndex()
{
	if(!id_to_index::tag_name)
		id_to_index::tag_name = "character";
	if(!id_to_index::file_str)
		id_to_index::file_str = pSettings->r_string("profiles", "files");
}

void CCharacterInfo::Load(PROFILE_ID id)
{
	Load(id_to_index::IdToIndex(id));
}

void CCharacterInfo::Load(PROFILE_INDEX index)
{
	m_iProfileIndex = index;
	inherited_shared::load_shared(m_iProfileIndex, NULL);
}

void CCharacterInfo::InitSpecificCharacter (SPECIFIC_CHARACTER_INDEX new_index)
{
	R_ASSERT(new_index != NO_SPECIFIC_CHARACTER);
	m_iSpecificCharacterIndex = new_index;

	m_SpecificCharacter.Load(m_iSpecificCharacterIndex);
	if(Rank() == NO_RANK)
		SetRank(m_SpecificCharacter.Rank());
	if(Reputation() == NO_REPUTATION)
		SetReputation(m_SpecificCharacter.Reputation());
	if(Community() == NO_COMMUNITY)
		SetCommunity(m_SpecificCharacter.Community());
}

void CCharacterInfo::load_shared	(LPCSTR)
{
	CUIXml uiXml;
	const id_to_index::ITEM_DATA& item_data = id_to_index::GetByIndex(m_iProfileIndex);

	string128 xml_file_full;
	strconcat(xml_file_full, *shared_str(item_data.file_name), ".xml");

	bool xml_result = uiXml.Init("$game_data$", xml_file_full);
	R_ASSERT3(xml_result, "xml file not found", xml_file_full);

	//loading from XML
	XML_NODE* item_node = uiXml.NavigateToNode(id_to_index::tag_name, item_data.pos_in_file);
	R_ASSERT3(item_node, "profile id=", *shared_str(item_data.id));

	uiXml.SetLocalRoot(item_node);

	//������� ��� ���������
	data()->m_Community		= uiXml.Read("team", 0, *NO_COMMUNITY);
	data()->m_Rank			= uiXml.ReadInt("rank", 0, NO_RANK);
	data()->m_Reputation	= uiXml.ReadInt("reputation", 0, NO_REPUTATION);

	 LPCSTR spec_char = uiXml.Read("specific_character", 0, NULL);
	 if(!spec_char)
         data()->m_iCharacterIndex = NO_SPECIFIC_CHARACTER;
	 else
		 data()->m_iCharacterIndex = CSpecificCharacter::IdToIndex(spec_char);
}


LPCSTR CCharacterInfo::Name() const
{
	return	m_SpecificCharacter.Name();
}

LPCSTR CCharacterInfo::Bio() const
{
	return 	m_SpecificCharacter.Bio();
}
CHARACTER_RANK CCharacterInfo::Rank() const
{
	return	m_CurrentRank;
}
CHARACTER_COMMUNITY CCharacterInfo::Community() const
{
	return	m_CurrentCommunity;
}

CHARACTER_REPUTATION CCharacterInfo::Reputation() const
{
	return m_CurrentReputation;
}

void CCharacterInfo::SetRank (CHARACTER_RANK rank)
{
	m_CurrentRank = rank;
}
void CCharacterInfo::SetReputation (CHARACTER_REPUTATION reputation)
{
	m_CurrentReputation = reputation;
}

void CCharacterInfo::SetCommunity	(CHARACTER_COMMUNITY community)
{
	m_CurrentCommunity = community;
}

ALife::ERelationType  CCharacterInfo::GetRelationType	(u16 person_id) const 
{
	const RELATION_MAP* relation_map = relation_registry.objects_ptr();

	if(relation_map)
	{
		RELATION_MAP::const_iterator it = relation_map->find(person_id);
		if(relation_map->end() != it)
		{
			const SRelation& relation = (*it).second;
			return relation.RelationType();
		}
	}
	return ALife::eRelationTypeDummy;
}
void CCharacterInfo::SetRelationType	(u16 person_id, ALife::ERelationType new_relation)
{
	RELATION_MAP& relation_map = relation_registry.objects();
	relation_map[person_id].SetRelationType(new_relation);
}


int  CCharacterInfo::GetGoodwill (u16 person_id) const 
{
	const RELATION_MAP* relation_map = relation_registry.objects_ptr();

	if(relation_map)
	{
		RELATION_MAP::const_iterator it = relation_map->find(person_id);
		if(relation_map->end() != it)
		{
			const SRelation& relation = (*it).second;
			return relation.Goodwill();
		}
	}
		return NO_GOODWILL;
}
void CCharacterInfo::SetGoodwill	(u16 person_id, int goodwill)
{
	RELATION_MAP& relation_map = relation_registry.objects();
	relation_map[person_id].SetGoodwill(goodwill);
}


void CCharacterInfo::ClearRelations	()
{
	const RELATION_MAP* relation_map = relation_registry.objects_ptr();
	if(relation_map)
	{
		relation_registry.objects().clear();
	}
}

int	 CCharacterInfo::TradeIconX() const
{
	return m_SpecificCharacter.TradeIconX();
}
int	 CCharacterInfo::TradeIconY() const
{
	return m_SpecificCharacter.TradeIconY();
}
int	 CCharacterInfo::MapIconX()	 const
{
	return m_SpecificCharacter.MapIconX();
}
int	 CCharacterInfo::MapIconY()	 const
{
	return m_SpecificCharacter.MapIconY();
}

PHRASE_DIALOG_INDEX	CCharacterInfo::StartDialog	()	const
{
	return m_SpecificCharacter.data()->m_iStartDialog;
}
const DIALOG_INDEX_VECTOR&	CCharacterInfo::ActorDialogs	()	const
{
	return m_SpecificCharacter.data()->m_ActorDialogs;
}

void CCharacterInfo::load	(IReader& stream)
{
	m_SpecificCharacter.data()->m_iStartDialog = stream.r_s16();
}
void CCharacterInfo::save	(NET_Packet& stream)
{
	stream.w_s16((s16)StartDialog());
}