#pragma once

#include "xr_collide_defs.h"
#include "render.h"

class ENGINE_API IRender_Sector;
class ENGINE_API CObject;

namespace Feel
{
	const float fuzzy_update_vis	= 1000.f;		// speed of fuzzy-logic desisions
	const float fuzzy_update_novis	= 1000.f;		// speed of fuzzy-logic desisions
	const float fuzzy_guaranteed	= 1.f;			// distance which is supposed 100% visible
	const float lr_granularity		= 0.1f;			// assume similar positions

	class ENGINE_API Vision
	{
	private:
		xr_vector<CObject*>			seen;
		xr_vector<CObject*>			query;
		xr_vector<CObject*>			diff;

		void						o_new	(CObject* E);
		void						o_delete(CObject* E);
		void						o_trace	(Fvector& P, float dt, float vis_threshold);
	public:
		struct feel_visible_Item 
		{
			float				fuzzy;		// note range: (-1[no]..1[yes])
			CObject*			O;
			collide::ray_cache	Cache;
			float				Cache_vis;
			Fvector				cp_LP;
			Fvector				cp_LR_src;
			Fvector				cp_LR_dst;
			Fvector				cp_LAST;	// last point found to be visible
		};
		xr_vector<feel_visible_Item>	feel_visible;
	public:
		void						feel_vision_clear		();
		void						feel_vision_query		(Fmatrix& mFull,	Fvector& P);
		void						feel_vision_update		(CObject* parent,	Fvector& P, float dt, float vis_threshold);
		void						feel_vision_get			(xr_vector<CObject*>& R)		{
			R.clear					();
			xr_vector<feel_visible_Item>::iterator I=feel_visible.begin(),E=feel_visible.end();
			for (; I!=E; I++)	if (positive(I->fuzzy)) R.push_back(I->O);
		}
		Fvector						feel_vision_get_vispoint(CObject* _O)					{
			xr_vector<feel_visible_Item>::iterator I=feel_visible.begin(),E=feel_visible.end();
			for (; I!=E; I++)		if (_O == I->O) {
				VERIFY	(positive(I->fuzzy));
				return	I->cp_LAST;
			}
			VERIFY2		(0, "There is no such object in the potentially visible list" );
			return		Fvector().set(0,0,0).div(0);
		}
		virtual		BOOL			feel_vision_isRelevant	(CObject* O)					= 0;
		virtual		float			feel_vision_mtl_transp	(u32 element)					= 0;	
	};
};
