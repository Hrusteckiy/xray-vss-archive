#include "stdafx.h"
#pragma hdrstop

#include "Blender_deffer_flat.h"

CBlender_deffer_flat::CBlender_deffer_flat	()	{	description.CLS		= B_DEFAULT;	}
CBlender_deffer_flat::~CBlender_deffer_flat	()	{	}

void	CBlender_deffer_flat::Save	(	IWriter& FS )
{
	CBlender::Save	(FS);
}
void	CBlender_deffer_flat::Load	(	IReader& FS, WORD version )
{
	CBlender::Load	(FS,version);
}

class cl_chpos	: public R_constant_setup 
{
	u32			dwFrame;
	Fvector4	c_hpos;

	virtual void setup (R_constant* C) 
	{ 
		Fvector& P		= Device.vCameraPosition;
		RCache.set_c	(C,P.x,P.y,P.z,0);
	}
};
static cl_chpos	binder_chpos;

void	CBlender_deffer_flat::Compile(CBlender_Compile& C)
{
	CBlender::Compile		(C);

	switch(C.iElement) 
	{
	case 0: 	// deffer
		C.r2_Pass			("r2_deffer_base_flat","r2_deffer_base_flat");
		C.r2_Sampler		("s_base",C.L_textures[0]);
		C.r2_End			();
		break;
	case 1:		// smap-direct
		C.r2_Pass			("r2_shadow_direct_base","r2_shadow_direct_base");
		C.r2_Sampler		("s_base",C.L_textures[0]);
		C.r2_End			();
		break;
	case 2:		// smap-point
		C.r2_Pass			("r2_shadow_point_base","r2_shadow_point_base");
		C.r2_Sampler		("s_base",C.L_textures[0]);
		C.r2_End			();
		break;
	}
}
