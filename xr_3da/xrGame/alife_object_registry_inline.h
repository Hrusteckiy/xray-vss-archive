////////////////////////////////////////////////////////////////////////////
//	Module 		: alife_object_registry_������.h
//	Created 	: 15.01.2003
//  Modified 	: 12.05.2004
//	Author		: Dmitriy Iassenev
//	Description : ALife object registry inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

IC	void CALifeObjectRegistry::add		(CSE_ALifeDynamicObject *object)
{
	if (objects().find(object->ID) != objects().end()) {
		THROW2					((*(objects().find(object->ID))).second == object,"The specified object is already presented in the Object Registry!");
		THROW2					((*(objects().find(object->ID))).second != object,"Object with the specified ID is already presented in the Object Registry!");
	}

	m_objects.insert			(std::make_pair(object->ID,object));
}

IC	void CALifeObjectRegistry::remove	(const ALife::_OBJECT_ID &id, bool no_assert)
{
	OBJECT_REGISTRY::iterator	I = m_objects.find(id);
	if (I == m_objects.end()) {
		THROW2					(no_assert,"The specified object hasn't been found in the Object Registry!");
		return;
	}

	m_objects.erase				(I);
}

IC	CSE_ALifeDynamicObject *CALifeObjectRegistry::object	(const ALife::_OBJECT_ID &id, bool no_assert) const
{
	OBJECT_REGISTRY::const_iterator	I = objects().find(id);

	if (objects().end() == I) {
#ifdef DEBUG
		if (!no_assert)
			Msg					("There is no object with id %d!",id);
#endif
		THROW2					(no_assert,"Specified object hasn't been found in the object registry!");
		return					(0);
	}

	return						((*I).second);
}

IC	const CALifeObjectRegistry::OBJECT_REGISTRY &CALifeObjectRegistry::objects	() const
{
	return						(m_objects);
}


template <typename _predicate>
void CALifeObjectRegistry::load				(IReader &file_stream, const _predicate &predicate)
{ 
	Msg							("* Loading objects...");
	R_ASSERT2					(file_stream.find_chunk(OBJECT_CHUNK_DATA),"Can't find chunk OBJECT_CHUNK_DATA!");
	m_objects.clear				();
	u32							count = file_stream.r_u32();
	for (u32 i=0; i<count; ++i) {
		CSE_ALifeDynamicObject	*object = get_object(file_stream);
		add						(object);
	}

	ALife::D_OBJECT_P_MAP::const_iterator	I = objects().begin();
	ALife::D_OBJECT_P_MAP::const_iterator	E = objects().end();
	for ( ; I != E; ++I)
		predicate				((*I).second);

	Msg							("%d objects are successfully loaded",count);
}
