////////////////////////////////////////////////////////////////////////////
//	Module 		: game_path_manager_inline.h
//	Created 	: 02.10.2001
//  Modified 	: 12.11.2003
//	Author		: Dmitriy Iassenev
//	Description : Game path manager inline functions
////////////////////////////////////////////////////////////////////////////

#pragma once

#define TEMPLATE_SPECIALIZATION template <\
	typename _VertexEvaluator,\
	typename _vertex_id_type,\
	typename _index_type\
>

#define CGameManagerTemplate CBasePathManager<CGameGraph,_VertexEvaluator,_vertex_id_type,_index_type>

TEMPLATE_SPECIALIZATION
IC	bool CGameManagerTemplate::actual(const _vertex_id_type start_vertex_id, const _vertex_id_type dest_vertex_id) const
{
	return		(inherited::actual(start_vertex_id,dest_vertex_id));
}
