////////////////////////////////////////////////////////////////////////////
//	Module 		: alife_simulator_script.cpp
//	Created 	: 25.12.2002
//  Modified 	: 13.05.2004
//	Author		: Dmitriy Iassenev
//	Description : ALife Simulator script export
////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "alife_simulator.h"
#include "script_space.h"
#include "ai_space.h"
#include "alife_object_registry.h"
#include "alife_story_registry.h"
#include "script_engine.h"
#include "xrServer_Objects_ALife_Monsters.h"
#include "restriction_space.h"
#include "alife_graph_registry.h"
#include "alife_spawn_registry.h"
#include "alife_registry_container.h"

using namespace luabind;

typedef xr_vector<std::pair<shared_str,int> >	STORY_PAIRS;
typedef STORY_PAIRS								SPAWN_STORY_PAIRS;
extern											LPCSTR GAME_CONFIG;
LPCSTR											_INVALID_STORY_ID		= "INVALID_STORY_ID";
LPCSTR											_INVALID_SPAWN_STORY_ID	= "INVALID_SPAWN_STORY_ID";
STORY_PAIRS										story_ids;
SPAWN_STORY_PAIRS								spawn_story_ids;

CALifeSimulator *alife			()
{
	return			(const_cast<CALifeSimulator*>(ai().get_alife()));
}

CSE_ALifeDynamicObject *alife_object		(const CALifeSimulator *self, ALife::_OBJECT_ID object_id)
{
	VERIFY			(self);
	return			(self->objects().object(object_id,true));
}

bool valid_object_id						(const CALifeSimulator *self, ALife::_OBJECT_ID object_id)
{
	VERIFY			(self);
	return			(object_id != 0xffff);
}

CSE_ALifeDynamicObject *alife_object		(const CALifeSimulator *self, LPCSTR name)
{
	VERIFY			(self);
	
	for (CALifeObjectRegistry::OBJECT_REGISTRY::const_iterator it = self->objects().objects().begin(); it != self->objects().objects().end(); it++) {
		CSE_ALifeDynamicObject *obj = it->second;
		if (xr_strcmp(obj->name_replace(),name) == 0)
			return it->second;
	}
	
	return			(0);
}


CSE_ALifeDynamicObject *alife_object		(const CALifeSimulator *self, ALife::_OBJECT_ID id, bool no_assert)
{
	VERIFY			(self);
	return			(self->objects().object(id,no_assert));
}

CSE_ALifeDynamicObject *alife_story_object	(const CALifeSimulator *self, ALife::_STORY_ID id)
{
	return			(self->story_objects().object(id,true));
}

template <typename _id_type>
void generate_story_ids		(
	STORY_PAIRS &result,
	_id_type	INVALID_ID,
	LPCSTR		section_name,
	LPCSTR		INVALID_ID_STRING,
	LPCSTR		invalid_id_description,
	LPCSTR		invalid_id_redefinition,
	LPCSTR		duplicated_id_description
)
{
	result.clear			();

    CInifile				*Ini = 0;
    string_path				game_ltx;
    FS.update_path			(game_ltx,"$game_config$",GAME_CONFIG);
    R_ASSERT3				(FS.exist(game_ltx),"Couldn't find file",game_ltx);
    Ini						= xr_new<CInifile>(game_ltx);
    
    LPCSTR					N,V;
	u32 					k;
	shared_str				temp;
    LPCSTR					section = section_name;
    R_ASSERT				(Ini->section_exist(section));

	for (k = 0; Ini->r_line(section,k,&N,&V); ++k) {
		temp				= Ini->r_string_wb(section,N);
		
		R_ASSERT3			(!strchr(*temp,' '),invalid_id_description,*temp);
		R_ASSERT2			(xr_strcmp(*temp,INVALID_ID_STRING),invalid_id_redefinition);
		
		STORY_PAIRS::const_iterator	I = result.begin();
		STORY_PAIRS::const_iterator	E = result.end();
		for ( ; I != E; ++I)
			R_ASSERT3		((*I).first != temp,duplicated_id_description,*temp);
		
		result.push_back	(std::make_pair(*temp,atoi(N)));
	}

	result.push_back		(std::make_pair(INVALID_ID_STRING,INVALID_ID));

    xr_delete				(Ini);
}

void kill_entity0			(CALifeSimulator *alife, CSE_ALifeMonsterAbstract *monster, const GameGraph::_GRAPH_ID &game_vertex_id)
{
	alife->kill_entity		(monster,game_vertex_id,0);
}

void kill_entity1			(CALifeSimulator *alife, CSE_ALifeMonsterAbstract *monster)
{
	alife->kill_entity		(monster,monster->m_tGraphID,0);
}

void add_in_restriction		(CALifeSimulator *alife, CSE_ALifeMonsterAbstract *monster, ALife::_OBJECT_ID id)
{
	alife->add_restriction	(monster->ID,id,RestrictionSpace::eRestrictorTypeIn);
}

void add_out_restriction	(CALifeSimulator *alife, CSE_ALifeMonsterAbstract *monster, ALife::_OBJECT_ID id)
{
	alife->add_restriction	(monster->ID,id,RestrictionSpace::eRestrictorTypeOut);
}

void remove_in_restriction	(CALifeSimulator *alife, CSE_ALifeMonsterAbstract *monster, ALife::_OBJECT_ID id)
{
	alife->remove_restriction	(monster->ID,id,RestrictionSpace::eRestrictorTypeIn);
}

void remove_out_restriction	(CALifeSimulator *alife, CSE_ALifeMonsterAbstract *monster, ALife::_OBJECT_ID id)
{
	alife->remove_restriction	(monster->ID,id,RestrictionSpace::eRestrictorTypeOut);
}

u32 get_level_id(CALifeSimulator *self)
{
	return						(self->graph().level().level_id());
}

CSE_ALifeDynamicObject *CALifeSimulator__create	(CALifeSimulator *self, ALife::_SPAWN_ID spawn_id)
{
	const CALifeSpawnRegistry::SPAWN_GRAPH::CVertex	*vertex = ai().alife().spawns().spawns().vertex(spawn_id);
	THROW2								(vertex,"Invalid spawn id!");

	CSE_ALifeDynamicObject				*spawn = smart_cast<CSE_ALifeDynamicObject*>(&vertex->data()->object());
	THROW								(spawn);

	CSE_ALifeDynamicObject				*object;
	self->create						(object,spawn,spawn_id);

	return								(object);
}

CSE_Abstract *CALifeSimulator__spawn_item		(CALifeSimulator *self, LPCSTR section, const Fvector &position, u32 level_vertex_id, GameGraph::_GRAPH_ID game_vertex_id)
{
	THROW								(self);
	return								(self->spawn_item(section,position,level_vertex_id,game_vertex_id,ALife::_OBJECT_ID(-1)));
}

ALife::_SPAWN_ID CALifeSimulator__spawn_id		(CALifeSimulator *self, ALife::_SPAWN_STORY_ID spawn_story_id)
{
	return								(((const CALifeSimulator *)self)->spawns().spawn_id(spawn_story_id));
}

LPCSTR get_level_name							(const CALifeSimulator *self, int level_id)
{
	return								(*ai().game_graph().header().level(level_id).name());
}

CSE_ALifeCreatureActor *get_actor				(const CALifeSimulator *self)
{
	THROW								(self);
	return								(self->graph().actor());
}

KNOWN_INFO_VECTOR *registry						(const CALifeSimulator *self, const ALife::_OBJECT_ID &id)
{
	THROW								(self);
	return								(self->registry(info_portions).object(id, true));
}

class CFindByIDPred
{
public:
	CFindByIDPred(INFO_ID element_to_find) {element = element_to_find;}
	bool operator () (const INFO_DATA& data) const {return data.info_id == element;}
private:
	INFO_ID element;
};

bool has_info									(const CALifeSimulator *self, const ALife::_OBJECT_ID &id, LPCSTR info_id)
{
	const KNOWN_INFO_VECTOR				*known_info = registry(self,id);
	if (!known_info)
		return							(false);

	if (std::find_if(known_info->begin(), known_info->end(), CFindByIDPred(info_id)) == known_info->end())
		return							(false);

	return								(true);
}

bool dont_has_info								(const CALifeSimulator *self, const ALife::_OBJECT_ID &id, LPCSTR info_id)
{
	THROW								(self);
	// absurdly, but only because of scriptwriters needs
	return								(!has_info(self,id,info_id));
}

//void disable_info_portion						(const CALifeSimulator *self, const ALife::_OBJECT_ID &id)
//{
//	THROW								(self);
//}

//void give_info_portion							(const CALifeSimulator *self, const ALife::_OBJECT_ID &id)
//{
//	THROW								(self);
//}

void CALifeSimulator::script_register			(lua_State *L)
{
	module(L)
	[
		class_<CALifeSimulator>("alife_simulator")
			.def("valid_object_id",			&valid_object_id)
			.def("level_id",				&get_level_id)
			.def("level_name",				&get_level_name)
			.def("object",					(CSE_ALifeDynamicObject *(*) (const CALifeSimulator *,ALife::_OBJECT_ID))(alife_object))
			.def("object",					(CSE_ALifeDynamicObject *(*) (const CALifeSimulator *,LPCSTR))(alife_object))
			.def("object",					(CSE_ALifeDynamicObject *(*) (const CALifeSimulator *,ALife::_OBJECT_ID, bool))(alife_object))
			.def("story_object",			(CSE_ALifeDynamicObject *(*) (const CALifeSimulator *,ALife::_STORY_ID))(alife_story_object))
			.def("set_switch_online",		(void (CALifeSimulator::*) (ALife::_OBJECT_ID,bool))(CALifeSimulator::set_switch_online))
			.def("set_switch_offline",		(void (CALifeSimulator::*) (ALife::_OBJECT_ID,bool))(CALifeSimulator::set_switch_offline))
			.def("set_interactive",			(void (CALifeSimulator::*) (ALife::_OBJECT_ID,bool))(CALifeSimulator::set_interactive))
			.def("kill_entity",				&CALifeSimulator::kill_entity)
			.def("kill_entity",				&kill_entity0)
			.def("kill_entity",				&kill_entity1)
			.def("add_in_restriction",		&add_in_restriction)
			.def("add_out_restriction",		&add_out_restriction)
			.def("remove_in_restriction",	&remove_in_restriction)
			.def("remove_out_restriction",	&remove_out_restriction)
			.def("remove_all_restrictions",	&CALifeSimulator::remove_all_restrictions)
			.def("create",					&CALifeSimulator__create)
			.def("create",					&CALifeSimulator::spawn_item)
			.def("create",					&CALifeSimulator__spawn_item)
			.def("spawn_id",				&CALifeSimulator__spawn_id)
			.def("actor",					&get_actor)
			.def("has_info",				&has_info)
			.def("dont_has_info",			&dont_has_info)
//			.def("disable_info_portion",	&disable_info_portion)
//			.def("give_info_portion",		&give_info_portion)

		,def("alife",						&alife)
	];

	{
		VERIFY					(story_ids.empty());
		generate_story_ids		(
			story_ids,
			INVALID_STORY_ID,
			"story_ids",
			"INVALID_STORY_ID",
			"Invalid story id description (contains spaces)!",
			"INVALID_STORY_ID redifinition!",
			"Duplicated story id description!"
		);

		luabind::class_<class_exporter<CALifeSimulator> >	instance("story_ids");

		STORY_PAIRS::const_iterator	I = story_ids.begin();
		STORY_PAIRS::const_iterator	E = story_ids.end();
		for ( ; I != E; ++I)
			instance.enum_		("_story_ids")[luabind::value(*(*I).first,(*I).second)];

		luabind::module			(L)[instance];
	}

	{
		VERIFY					(spawn_story_ids.empty());
		generate_story_ids		(
			spawn_story_ids,
			INVALID_SPAWN_STORY_ID,
			"spawn_story_ids",
			"INVALID_SPAWN_STORY_ID",
			"Invalid spawn story id description (contains spaces)!",
			"INVALID_SPAWN_STORY_ID redifinition!",
			"Duplicated spawn story id description!"
		);

		luabind::class_<class_exporter<class_exporter<CALifeSimulator> > >	instance("spawn_story_ids");

		SPAWN_STORY_PAIRS::const_iterator	I = spawn_story_ids.begin();
		SPAWN_STORY_PAIRS::const_iterator	E = spawn_story_ids.end();
		for ( ; I != E; ++I)
			instance.enum_		("_spawn_story_ids")[luabind::value(*(*I).first,(*I).second)];

		luabind::module			(L)[instance];
	}
}
