#ifndef D_SORT_TRI_PRIMITIVE_H
#define D_SORT_TRI_PRIMITIVE_H
#include "dTriCollideK.h"
#include "dSortTriPrimitive.h"
#include "dTriColliderCommon.h"
#include "dTriColliderMath.h"
#include "__aabb_tri.h"
#include "../MathUtils.h"
#ifdef DEBUG
#include "../PHDebug.h"
extern u32 dbg_tries_num;
#endif
static 	xr_vector<Triangle> pos_tries;
static 	xr_vector<Triangle> neg_tries;

template<class T>
int dSortTriPrimitiveCollide (	
							  dxGeom		*o1,		dxGeom			*o2,
							  int			flags,		dContactGeom	*contact,	int skip,
							  CDB::RESULT*	R_begin,	CDB::RESULT*	R_end ,
							  CDB::TRI*		T_array,	const Fvector*	V_array,
							  const Fvector&	AABB
							  )
{

	int			ret	=	0;
	Triangle	tri;
	dxGeomUserData* data=dGeomGetUserData(o1);
	Triangle* neg_tri=&(data->neg_tri);
	Triangle* b_neg_tri=&(data->b_neg_tri);
	dReal* last_pos=data->last_pos;

	bool* pushing_neg=&data->pushing_neg;
	bool* pushing_b_neg=&data->pushing_b_neg;
	bool spushing_neg=*pushing_neg;
	bool spushing_b_neg=*pushing_b_neg;
	pos_tries.clear	();
	dReal neg_depth=dInfinity,b_neg_depth=dInfinity;
	const dReal* p=dGeomGetPosition(o1);
	UINT	b_count		=0			;
	bool	intersect	=	false	;
	bool	no_last_pos	=last_pos[0]==-dInfinity;


#ifdef DEBUG
	if(ph_dbg_draw_mask.test(phDbgDrawTriTestAABB))
		DBG_DrawAABB(cast_fv(p),AABB,D3DCOLOR_XRGB(0,0,255));
#endif

	if(*pushing_neg){
		dReal sidePr=T::Proj(o1,neg_tri->norm);
		neg_tri->dist=dDOT(p,neg_tri->norm)-neg_tri->pos;
		neg_tri->depth=sidePr-neg_tri->dist;
#ifdef DEBUG
		if(ph_dbg_draw_mask.test(phDbgDrawSavedTries))
			DBG_DrawTri(neg_tri->T,V_array,D3DCOLOR_XRGB(255,0,0));
#endif
		if(neg_tri->dist<0.f)
			neg_depth=neg_tri->depth;
		else
		{
			*pushing_neg=false;

		}
	}

	if(*pushing_b_neg){
		dReal sidePr=T::Proj(o1,b_neg_tri->norm);
		b_neg_tri->dist=dDOT(p,b_neg_tri->norm)-b_neg_tri->pos;
		b_neg_tri->depth=sidePr-b_neg_tri->dist;
#ifdef DEBUG
		if(ph_dbg_draw_mask.test(phDbgDrawSavedTries))
			DBG_DrawTri(b_neg_tri->T,V_array,D3DCOLOR_XRGB(0,0,255));
#endif
		if(b_neg_tri->dist<0.f)
			b_neg_depth=b_neg_tri->depth;
		else{
			*pushing_b_neg=false;

		}
	}

	bool b_pushing=*pushing_neg||*pushing_b_neg;


	for (CDB::RESULT* Res=R_begin; Res!=R_end; ++Res)
	{
#ifdef DEBUG
		if(ph_dbg_draw_mask.test(phDBgDrawIntersectedTries))
										DBG_DrawTri(Res,D3DCOLOR_XRGB(0,255,0));
		dbg_tries_num++;
#endif

		CDB::TRI* T = T_array + Res->id;

		tri.side0[0]=Res->verts[1].x-Res->verts[0].x;
		tri.side0[1]=Res->verts[1].y-Res->verts[0].y;
		tri.side0[2]=Res->verts[1].z-Res->verts[0].z;

		tri.side1[0]=Res->verts[2].x-Res->verts[1].x;
		tri.side1[1]=Res->verts[2].y-Res->verts[1].y;
		tri.side1[2]=Res->verts[2].z-Res->verts[1].z;

		tri.T=T;
		dCROSS(tri.norm,=,tri.side0,tri.side1);
		dNormalize3(tri.norm);
		
		tri.pos=dDOT((dReal*)&Res->verts[0],tri.norm);
		tri.dist=dDOT(p,tri.norm)-tri.pos;
		
		Point vertices[3]={Point((dReal*)&Res->verts[0]),Point((dReal*)&Res->verts[1]),Point((dReal*)&Res->verts[2])};
		if(tri.dist<0.f){
#ifdef DEBUG
			if(ph_dbg_draw_mask.test(phDBgDrawNegativeTries))
				DBG_DrawTri(Res,D3DCOLOR_XRGB(0,0,255));
#endif
			float last_pos_dist=dDOT(last_pos,tri.norm)-tri.pos;
			if((!(last_pos_dist<0.f))||b_pushing)
				if(__aabb_tri(Point(p),Point((float*)&AABB),vertices))
				{
#ifdef DEBUG
					if(ph_dbg_draw_mask.test(phDBgDrawTriesChangesSign))
						DBG_DrawTri(Res,D3DCOLOR_XRGB(0,255,0));
#endif
					if(!b_pushing)
					{
						if(!no_last_pos)
						{
#ifdef DEBUG
							if(ph_dbg_draw_mask.test(phDbgDrawTriTrace))
								DBG_DrawLine(cast_fv(last_pos),cast_fv(p),D3DCOLOR_XRGB(255,0,255));
#endif
							dVector3 tri_point;
							PlanePoint(tri,last_pos,p,last_pos_dist,tri_point);
#ifdef DEBUG
							if(ph_dbg_draw_mask.test(phDbgDrawTriPoint))
								DBG_DrawPoint(cast_fv(tri_point),0.01f,D3DCOLOR_XRGB(255,0,255));
#endif
							intersect=intersect||TriContainPoint(	(dReal*)&Res->verts[0],
								(dReal*)&Res->verts[1],
								(dReal*)&Res->verts[2],
								tri.norm,tri.side0,
								tri.side1,tri_point);
						}
						else
						{
							intersect=true;
						}
					}
					else
					{
						intersect=true;
					}

					if(TriContainPoint((dReal*)&Res->verts[0],(dReal*)&Res->verts[1],(dReal*)&Res->verts[2],
						tri.norm,tri.side0,
						tri.side1,p)

						){
							dReal sidePr=T::Proj(o1,tri.norm);
							tri.depth=sidePr-tri.dist;
							if(neg_depth>tri.depth&&(!(*pushing_neg||spushing_neg)||dDOT(neg_tri->norm,tri.norm)>-M_SQRT1_2)&&(!(*pushing_b_neg||spushing_b_neg)||dDOT(b_neg_tri->norm,tri.norm)>-M_SQRT1_2))//exclude switching on opposite side &&(!*pushing_b_neg||dDOT(b_neg_tri->norm,tri.norm)>-M_SQRT1_2)
							{
								neg_depth=tri.depth;
								(*neg_tri)=tri;
								ret=0;
								//if(intersect)*pushing_neg=true;
							}


						}
					else{
						++b_count;
						dReal sidePr=T::Proj(o1,tri.norm);
						tri.depth=sidePr-tri.dist;
						if(b_neg_depth>tri.depth&&(!(*pushing_b_neg||spushing_b_neg)||dDOT(b_neg_tri->norm,tri.norm)>-M_SQRT1_2)&&((!*pushing_neg||!spushing_neg)||dDOT(neg_tri->norm,tri.norm)>-M_SQRT1_2)){//exclude switching on opposite side &&(!*pushing_neg||dDOT(neg_tri->norm,tri.norm)>-M_SQRT1_2)
							b_neg_depth=tri.depth;
							(*b_neg_tri)=tri;
							ret=0;
							//if(intersect)*pushing_b_neg=true;
						}
					}
				}
		}
		else{
#ifdef DEBUG
			if(ph_dbg_draw_mask.test(phDBgDrawPositiveTries))
				DBG_DrawTri(Res,D3DCOLOR_XRGB(255,0,0));
#endif	
			if(ret>10) continue;
				if(!b_pushing&&(!intersect||no_last_pos))
					ret+=T::Collide(
					(const dReal*)&V_array[T->verts[0]],
					(const dReal*)&V_array[T->verts[1]],
					(const dReal*)&V_array[T->verts[2]],
					&tri,
					o1,
					o2,
					3,
					CONTACT(contact, ret * skip),   skip);
				if(no_last_pos)
					pos_tries.push_back(tri);
		}
	}

	//if(intersect) ret=0;
	xr_vector<Triangle>::iterator i;


	if(neg_depth<dInfinity&&intersect)
	{
		bool include = true;
		if(no_last_pos)
			for(i=pos_tries.begin();pos_tries.end() != i;++i)
			{
				if(TriContainPoint(
					(dReal*)&V_array[i->T->verts[0]],
					(dReal*)&V_array[i->T->verts[1]],
					(dReal*)&V_array[i->T->verts[2]],
					i->norm,i->side0,
					i->side1,p))
					if(
						!((dDOT(neg_tri->norm,(dReal*)&V_array[i->T->verts[0]])-neg_tri->pos)>0.f)||
						!((dDOT(neg_tri->norm,(dReal*)&V_array[i->T->verts[1]])-neg_tri->pos)>0.f)||
						!((dDOT(neg_tri->norm,(dReal*)&V_array[i->T->verts[2]])-neg_tri->pos)>0.f)
						){
							include=false;
							break;
						}
			};

		if(include){		
			ret+=T::CollidePlain(
				neg_tri->side0,neg_tri->side1,neg_tri->norm,
				neg_tri->T,
				neg_tri->dist,
				o1,o2,flags,
				CONTACT(contact, ret * skip),
				skip);	
			*pushing_neg=!!ret;
		}

	}


	//for(i=pos_tries.begin();pos_tries.end() != i;++i){
	//	CDB::TRI* T=i->T;
	//	ret+=dTriPrimitive(
	//		(const dReal*)&V_array[T->verts[0]],
	//		(const dReal*)&V_array[T->verts[1]],
	//		(const dReal*)&V_array[T->verts[2]],
	//		T,
	//		o1,
	//		o2,
	//		3,
	//		CONTACT(contact, ret * skip),   skip);

	//}



	if(b_neg_depth<dInfinity&&intersect){

		bool include = true;
		if(no_last_pos)
			for(i=pos_tries.begin();pos_tries.end() != i;++i){
				if(
					!((dDOT(b_neg_tri->norm,(dReal*)&V_array[i->T->verts[0]])-b_neg_tri->pos)>0.f)||
					!((dDOT(b_neg_tri->norm,(dReal*)&V_array[i->T->verts[1]])-b_neg_tri->pos)>0.f)||
					!((dDOT(b_neg_tri->norm,(dReal*)&V_array[i->T->verts[2]])-b_neg_tri->pos)>0.f)

					){
						include=false;
						break;
					}
			};

		if(include)	
		{	
			ret+=T::CollidePlain(
				b_neg_tri->side0,
				b_neg_tri->side1,
				b_neg_tri->norm,
				b_neg_tri->T,
				b_neg_tri->dist,
				o1,o2,flags,
				CONTACT(contact, ret * skip),
				skip);	
			*pushing_b_neg=!!ret;
		}

	}
	dVectorSet(last_pos,p);
	return ret;
}
#endif