#include "stdafx.h"
#include "..\resourcemanager.h"
#include "blender_light_occq.h"
#include "blender_light_mask.h"
#include "blender_light_direct.h"
#include "blender_light_point.h"
#include "blender_light_spot.h"
#include "blender_light_reflected.h"
#include "blender_combine.h"
#include "blender_bloom_build.h"
#include "blender_luminance.h"

void	CRenderTarget::u_setrt			(const ref_rt& _1, const ref_rt& _2, const ref_rt& _3, IDirect3DSurface9* zb)
{
	VERIFY									(_1);
	dwWidth									= _1->dwWidth;
	dwHeight								= _1->dwHeight;
	if (_1) RCache.set_RT(_1->pRT,	0); else RCache.set_RT(NULL,0);
	if (_2) RCache.set_RT(_2->pRT,	1); else RCache.set_RT(NULL,1);
	if (_3) RCache.set_RT(_3->pRT,	2); else RCache.set_RT(NULL,2);
	RCache.set_ZB							(zb);
	RImplementation.rmNormal				();
}

void	CRenderTarget::u_setrt			(u32 W, u32 H, IDirect3DSurface9* _1, IDirect3DSurface9* _2, IDirect3DSurface9* _3, IDirect3DSurface9* zb)
{
	VERIFY									(_1);
	dwWidth									= W;
	dwHeight								= H;
	VERIFY									(_1);
	RCache.set_RT							(_1,	0);
	RCache.set_RT							(_2,	1);
	RCache.set_RT							(_3,	2);
	RCache.set_ZB							(zb);
	RImplementation.rmNormal				();
}

void	CRenderTarget::u_stencil_optimize	()
{
	VERIFY	(RImplementation.o.nvstencil);
	RCache.set_ColorWriteEnable				(FALSE);
	u32		Offset;
	float	_w					= float(Device.dwWidth);
	float	_h					= float(Device.dwHeight);
	u32		C					= D3DCOLOR_RGBA	(255,255,255,255);
	FVF::TL* pv					= (FVF::TL*) RCache.Vertex.Lock	(4,g_combine->vb_stride,Offset);
	pv->set						(EPS,			float(_h+EPS),	EPS_S,	1.f, C, 0, 0);	pv++;
	pv->set						(EPS,			EPS,			EPS_S,	1.f, C, 0, 0);	pv++;
	pv->set						(float(_w+EPS),	float(_h+EPS),	EPS_S,	1.f, C, 0, 0);	pv++;
	pv->set						(float(_w+EPS),	EPS,			EPS_S,	1.f, C, 0, 0);	pv++;
	RCache.Vertex.Unlock		(4,g_combine->vb_stride);
	RCache.set_Stencil			(TRUE,D3DCMP_EQUAL,0x1,0xff,0xff);	// keep/keep/keep
	RCache.set_Element			(s_occq->E[1]	);
	RCache.set_Geometry			(g_combine		);
	RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
}

u8		fpack			(float v)				{
	s32	_v	= iFloor	(((v+1)*.5f)*255.f + .5f);
	clamp	(_v,0,255);
	return	u8(_v);
}
u8		fpackZ			(float v)				{
	s32	_v	= iFloor	(_abs(v)*255.f + .5f);
	clamp	(_v,0,255);
	return	u8(_v);
}
Fvector	vunpack			(s32 x, s32 y, s32 z)	{
	Fvector	pck;
	pck.x	= (float(x)/255.f - .5f)*2.f;
	pck.y	= (float(y)/255.f - .5f)*2.f;
	pck.z	= -float(z)/255.f;
	return	pck;
}
Fvector	vunpack			(Ivector src)			{
	return	vunpack	(src.x,src.y,src.z);
}
Ivector	vpack			(Fvector src)
{
	Fvector			_v;
	int	bx			= fpack	(src.x);
	int by			= fpack	(src.y);
	int bz			= fpackZ(src.z);
	// dumb test
	float	e_best	= flt_max;
	int		r=bx,g=by,b=bz;
#ifdef DEBUG
	int		d=0;
#else
	int		d=3;
#endif
	for (int x=_max(bx-d,0); x<=_min(bx+d,255); x++)
	for (int y=_max(by-d,0); y<=_min(by+d,255); y++)
	for (int z=_max(bz-d,0); z<=_min(bz+d,255); z++)
	{
		_v				= vunpack(x,y,z);
		float	m		= _v.magnitude();
		float	me		= _abs(m-1.f);
		if	(me>0.03f)	continue;
		_v.div	(m);
		float	e		= _abs(src.dotproduct(_v)-1.f);
		if (e<e_best)	{
			e_best		= e;
			r=x,g=y,b=z;
		}
	}
	Ivector		ipck;
	ipck.set	(r,g,b);
	return		ipck;
}

void	CRenderTarget::OnDeviceCreate	()
{
	dwAccumulatorClearMark			= 0;
	Device.Resources->Evict			();

	// Blenders
	b_occq							= xr_new<CBlender_light_occq>			();
	b_accum_mask					= xr_new<CBlender_accum_direct_mask>	();
	b_accum_direct					= xr_new<CBlender_accum_direct>			();
	b_accum_point					= xr_new<CBlender_accum_point>			();
	b_accum_spot					= xr_new<CBlender_accum_spot>			();
	b_accum_reflected				= xr_new<CBlender_accum_reflected>		();
	b_bloom							= xr_new<CBlender_bloom_build>			();
	b_luminance						= xr_new<CBlender_luminance>			();
	b_combine						= xr_new<CBlender_combine>				();

	//	NORMAL
	{
		u32		w=Device.dwWidth, h=Device.dwHeight;
		BOOL	Xdepth				= RImplementation.o.mrtmixdepth;
		rt_Position.create			(r2_RT_P,		w,h,D3DFMT_A16B16G16R16F);
		rt_Normal.create			(r2_RT_N,		w,h,D3DFMT_A16B16G16R16F);
		if (Xdepth) rt_Color.create	(r2_RT_albedo,	w,h,D3DFMT_A8R8G8B8);
		else		rt_Color.create	(r2_RT_albedo,	w,h,D3DFMT_A16B16G16R16F);
		rt_Accumulator.create		(r2_RT_accum,	w,h,D3DFMT_A16B16G16R16F);
		
		rt_Generic_0.create			(r2_RT_generic0,w,h,D3DFMT_A8R8G8B8		);
		rt_Generic_1.create			(r2_RT_generic1,w,h,D3DFMT_A8R8G8B8		);
	}

	// OCCLUSION
	s_occq.create					(b_occq,		"r2\\occq");

	// DIRECT (spot)
	if (RImplementation.o.HW_smap)
	{
		u32	size					=RImplementation.o.smapsize;
		rt_smap_depth.create		(r2_RT_smap_depth,			size,size,D3DFMT_D24X8	);
		rt_smap_surf.create			(r2_RT_smap_surf,			size,size,D3DFMT_R5G6B5	);
		rt_smap_ZB					= NULL;
		s_accum_mask.create			(b_accum_mask,				"r2\\accum_mask");
		s_accum_direct.create		(b_accum_direct,			"r2\\accum_direct");
	}
	else
	{
		u32	size					=RImplementation.o.smapsize;
		rt_smap_surf.create			(r2_RT_smap_surf,			size,size,D3DFMT_R32F);
		rt_smap_depth				= NULL;
		R_CHK						(HW.pDevice->CreateDepthStencilSurface	(size,size,D3DFMT_D24X8,D3DMULTISAMPLE_NONE,0,TRUE,&rt_smap_ZB,NULL));
		s_accum_mask.create			(b_accum_mask,				"r2\\accum_mask");
		s_accum_direct.create		(b_accum_direct,			"r2\\accum_direct");
	}

	// POINT
	{
		s_accum_point.create		(b_accum_point,				"r2\\accum_point_s");
		accum_point_geom_create		();
		g_accum_point.create		(D3DFVF_XYZ,				g_accum_point_vb, g_accum_point_ib);
		accum_omnip_geom_create		();
		g_accum_omnipart.create		(D3DFVF_XYZ,				g_accum_omnip_vb, g_accum_omnip_ib);
	}

	// SPOT
	{
		s_accum_spot.create			(b_accum_spot,				"r2\\accum_spot_s",	"lights\\lights_spot01");
		accum_spot_geom_create		();
		g_accum_spot.create			(D3DFVF_XYZ,				g_accum_spot_vb, g_accum_spot_ib);
	}

	// REFLECTED
	{
		s_accum_reflected.create	(b_accum_reflected,			"r2\\accum_refl");
	}

	// BLOOM
	{
		D3DFORMAT	fmt				= D3DFMT_A8R8G8B8;			//D3DFMT_A16B16G16R16F;		// D3DFMT_X8R8G8B8
		u32	w=BLOOM_size_X, h=BLOOM_size_Y;
		u32 fvf_build				= D3DFVF_XYZRHW|D3DFVF_TEX4|D3DFVF_TEXCOORDSIZE2(0)|D3DFVF_TEXCOORDSIZE2(1)|D3DFVF_TEXCOORDSIZE2(2)|D3DFVF_TEXCOORDSIZE2(3);
		u32 fvf_filter				= (u32)D3DFVF_XYZRHW|D3DFVF_TEX8|D3DFVF_TEXCOORDSIZE4(0)|D3DFVF_TEXCOORDSIZE4(1)|D3DFVF_TEXCOORDSIZE4(2)|D3DFVF_TEXCOORDSIZE4(3)|D3DFVF_TEXCOORDSIZE4(4)|D3DFVF_TEXCOORDSIZE4(5)|D3DFVF_TEXCOORDSIZE4(6)|D3DFVF_TEXCOORDSIZE4(7);
		rt_Bloom_1.create			(r2_RT_bloom1,	w,h,		fmt);
		rt_Bloom_2.create			(r2_RT_bloom2,	w,h,		fmt);
		g_bloom_build.create		(fvf_build,		RCache.Vertex.Buffer(), RCache.QuadIB);
		g_bloom_filter.create		(fvf_filter,	RCache.Vertex.Buffer(), RCache.QuadIB);
		s_bloom_dbg_1.create		("effects\\screen_set",		r2_RT_bloom1);
		s_bloom_dbg_2.create		("effects\\screen_set",		r2_RT_bloom2);
		s_bloom.create				(b_bloom,					"r2\\bloom");
		f_bloom_factor				= 0.5f;
	}

	// TONEMAP
	{
		rt_LUM_64.create			(r2_RT_luminance_t64,	64, 64,	D3DFMT_A16B16G16R16F	);
		rt_LUM_8.create				(r2_RT_luminance_t8,	8,	8,	D3DFMT_A16B16G16R16F	);
		rt_LUM_result.create		(r2_RT_luminance,		1,	1,	D3DFMT_A16B16G16R16F	);	// need blending here
		s_luminance.create			(b_luminance,				"r2\\luminance");
		f_luminance_adapt			= 0.5f;
	}

	// COMBINE
	{
		s_combine.create					(b_combine,					"r2\\combine");
		s_combine_dbg_0.create				("effects\\screen_set",		r2_RT_luminance_t64	);
		s_combine_dbg_1.create				("effects\\screen_set",		r2_RT_luminance_t8	);
		s_combine_dbg_Accumulator.create	("effects\\screen_set",		r2_RT_accum			);
		g_combine.create					(FVF::F_TL,		RCache.Vertex.Buffer(), RCache.QuadIB);

		u32 fvf_aa_blur				= D3DFVF_XYZRHW|D3DFVF_TEX4|D3DFVF_TEXCOORDSIZE2(0)|D3DFVF_TEXCOORDSIZE2(1)|D3DFVF_TEXCOORDSIZE2(2)|D3DFVF_TEXCOORDSIZE2(3);
		g_aa_blur.create			(fvf_aa_blur,	RCache.Vertex.Buffer(), RCache.QuadIB);

		u32 fvf_aa_AA				= D3DFVF_XYZRHW|D3DFVF_TEX7|D3DFVF_TEXCOORDSIZE2(0)|D3DFVF_TEXCOORDSIZE2(1)|D3DFVF_TEXCOORDSIZE2(2)|D3DFVF_TEXCOORDSIZE2(3)|D3DFVF_TEXCOORDSIZE2(4)|D3DFVF_TEXCOORDSIZE4(5)|D3DFVF_TEXCOORDSIZE4(6);
		g_aa_AA.create				(fvf_aa_AA,		RCache.Vertex.Buffer(), RCache.QuadIB);
	}

	// Build textures
	{
		// Build material(s)
		{
			// Surface
			R_CHK						(D3DXCreateVolumeTexture(HW.pDevice,TEX_material_LdotN,TEX_material_LdotH,4,1,0,D3DFMT_A8L8,D3DPOOL_MANAGED,&t_material_surf));
			t_material					= Device.Resources->_CreateTexture(r2_material);
			t_material->surface_set		(t_material_surf);

			// Fill it (addr: x=dot(L,N),y=dot(L,H))
			D3DLOCKED_BOX				R;
			R_CHK						(t_material_surf->LockBox	(0,&R,0,0));
			for (u32 slice=0; slice<4; slice++)
			{
				for (u32 y=0; y<TEX_material_LdotH; y++)
				{
					for (u32 x=0; x<TEX_material_LdotN; x++)
					{
						u16*	p	=	(u16*)		(LPBYTE (R.pBits) + slice*R.SlicePitch + y*R.RowPitch + x*2);
						float	ld	=	float(x)	/ float	(TEX_material_LdotN-1);
						float	ls	=	float(y)	/ float	(TEX_material_LdotH-1);
						ls			*=	powf(ld,1/32.f);
						float	fd,fs;

						switch	(slice)
						{
						case 0:	{ // looks like OrenNayar
							fd	= _sqrt(ld);
							fs	= powf(ls,16.f)*.5f;
								}	break;
						case 1:	{// looks like Blinn
							fd	= ld;
							fs	= powf(ls,24.f);
								}	break;
						case 2:	{ // looks like Phong
							fd	= ld;
							fs	= powf(ls,128.f);
								}	break;
						case 3:	{ // looks like Metal
							float	s0	=	_abs	(1-_abs	(0.05f*_sin(33.f*ld)+ld-ls));
							float	s1	=	_abs	(1-_abs	(0.05f*_cos(33.f*ld*ls)+ld-ls));
							float	s2	=	_abs	(1-_abs	(ld-ls));
							fd		=	ld;
							fs		=	powf	(_max(_max(s0,s1),s2), 24.f);
							fs		*=	powf	(ld,1/8.f);
								}	break;
						default:
							fd	= fs = 0;
						}
						s32		_d	=	clampr	(iFloor	(fd*255.5f),	0,255);
						s32		_s	=	clampr	(iFloor	(fs*255.5f),	0,255);
						*p			=	u16		(_s*256 + _d);
					}
				}
			}
			R_CHK		(t_material_surf->UnlockBox	(0));
			// #ifdef DEBUG
			// R_CHK	(D3DXSaveTextureToFile	("x:\\r2_material.dds",D3DXIFF_DDS,t_material_surf,0));
			// #endif
		}

		// Build encode table - RG
		/*
		if (0)
		{
			// Surface
			R_CHK						(D3DXCreateTexture(HW.pDevice,TEX_float2rgb,TEX_float2rgb,1,0,D3DFMT_X8R8G8B8,D3DPOOL_MANAGED,&t_encodeRG_surf));
			t_encodeRG					= Device.Resources->_CreateTexture(r2_float2RG);
			t_encodeRG->surface_set		(t_encodeRG_surf);

			// Fill it, scale = (1,255,65536)
			D3DLOCKED_RECT				R;
			R_CHK						(t_encodeRG_surf->LockRect	(0,&R,0,0));
			for (u32 y=0; y<TEX_float2rgb; y++)
			{
				for (u32 x=0; x<TEX_float2rgb; x++)
				{
					u32*	p	=	(u32*)		(LPBYTE (R.pBits) + y*R.Pitch + x*4);
					*p			=	color_rgba	(x,y,0,0);
				}
			}
			R_CHK						(t_encodeRG_surf->UnlockRect	(0));
		}

		// Build encode table - B
		if (0)
		{
			// Surface
			R_CHK						(D3DXCreateTexture(HW.pDevice,TEX_float2rgb,1,1,0,D3DFMT_X8R8G8B8,D3DPOOL_MANAGED,&t_encodeB_surf));
			t_encodeB					= Device.Resources->_CreateTexture(r2_float2B);
			t_encodeB->surface_set		(t_encodeB_surf);

			// Fill it, scale = (1,255,65536)
			D3DLOCKED_RECT				R;
			R_CHK						(t_encodeB_surf->LockRect	(0,&R,0,0));
			for (u32 x=0; x<TEX_float2rgb; x++)
			{
				u32*	p	=	(u32*)		(LPBYTE (R.pBits) + x*4);
				*p			=	color_rgba	(0,0,x,0);
			}
			R_CHK						(t_encodeB_surf->UnlockRect	(0));
		}
		*/
	}

	// 
	dwWidth		= Device.dwWidth;
	dwHeight	= Device.dwHeight;
}

void	CRenderTarget::OnDeviceDestroy	()
{
	// Textures
	t_material->surface_set		(NULL);
	_RELEASE					(rt_smap_ZB				);

	accum_spot_geom_destroy		();
	accum_omnip_geom_destroy	();
	accum_point_geom_destroy	();

	// Blenders
	xr_delete					(b_combine				);
	xr_delete					(b_luminance			);
	xr_delete					(b_bloom				);
	xr_delete					(b_accum_reflected		);
	xr_delete					(b_accum_spot			);
	xr_delete					(b_accum_point			);
	xr_delete					(b_accum_direct			);
	xr_delete					(b_accum_mask			);
	xr_delete					(b_occq					);
}
