#include "stdafx.h"
#include "..\igame_persistent.h"
#include "..\environment.h"

#define STENCIL_CULL 0

float	hclip(float v, float dim)		{ return 2.f*v/dim - 1.f; }
void	CRenderTarget::phase_combine	()
{
	u32			Offset					= 0;
	Fvector2	p0,p1;

	//*** exposure-pipeline
	u32			gpu_id	= Device.dwFrame%2;
	{
		t_LUM_src->surface_set		(rt_LUM_pool[gpu_id*2+0]->pSurface);
		t_LUM_dest->surface_set		(rt_LUM_pool[gpu_id*2+1]->pSurface);
	}

	// low/hi RTs
	u_setrt				( rt_Generic_0,rt_Generic_1,0,HW.pBaseZB );
	RCache.set_CullMode	( CULL_NONE );
	RCache.set_Stencil	( FALSE		);

	// 
	if (RImplementation.o.bug)	{
		RCache.set_Stencil					(TRUE,D3DCMP_LESSEQUAL,0x01,0xff,0x00);	// stencil should be >= 1
		if (RImplementation.o.nvstencil)	{
			u_stencil_optimize				(FALSE);
			RCache.set_ColorWriteEnable		();
		}
	}

	// calc m-blur matrices
	Fmatrix		m_previous, m_current;
	Fvector2	m_blur_scale;
	{
		static Fmatrix		m_saved_viewproj;
		
		// (new-camera) -> (world) -> (old_viewproj)
		Fmatrix	m_invview;	m_invview.invert	(Device.mView);
		m_previous.mul		(m_saved_viewproj,m_invview);
		m_current.set		(Device.mProject)		;
		m_saved_viewproj.set(Device.mFullTransform)	;
		float	scale		= ps_r2_mblur/2.f;
		m_blur_scale.set	(scale,-scale).div(12.f);
	}

	// Draw full-screen quad textured with our scene image
	{
		// Compute params
		Fmatrix		m_v2w;			m_v2w.invert				(Device.mView		);
		CEnvDescriptor&		envdesc	= g_pGamePersistent->Environment.CurrentEnv;
		const float minamb			= 0.001f;
		Fvector4	ambclr			= { _max(envdesc.ambient.x*2,minamb),	_max(envdesc.ambient.y*2,minamb),			_max(envdesc.ambient.z*2,minamb),	0	};
		Fvector4	envclr			= { envdesc.sky_color.x*2+EPS,	envdesc.sky_color.y*2+EPS,	envdesc.sky_color.z*2+EPS,	envdesc.weight					};
		Fvector4	fogclr			= { envdesc.fog_color.x,	envdesc.fog_color.y,	envdesc.fog_color.z,		0	};
					envclr.x		*= 2*ps_r2_sun_lumscale_hemi; 
					envclr.y		*= 2*ps_r2_sun_lumscale_hemi; 
					envclr.z		*= 2*ps_r2_sun_lumscale_hemi; 

		// Fill VB
		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);
		p0.set						(.5f/_w, .5f/_h);
		p1.set						((_w+.5f)/_w, (_h+.5f)/_h );

		// Fill vertex buffer
		Fvector4* pv				= (Fvector4*)	RCache.Vertex.Lock	(4,g_combine_VP->vb_stride,Offset);
		pv->set						(hclip(EPS,		_w),	hclip(_h+EPS,	_h),	p0.x, p1.y);	pv++;
		pv->set						(hclip(EPS,		_w),	hclip(EPS,		_h),	p0.x, p0.y);	pv++;
		pv->set						(hclip(_w+EPS,	_w),	hclip(_h+EPS,	_h),	p1.x, p1.y);	pv++;
		pv->set						(hclip(_w+EPS,	_w),	hclip(EPS,		_h),	p1.x, p0.y);	pv++;
		RCache.Vertex.Unlock		(4,g_combine_VP->vb_stride);

		// Setup textures
		IDirect3DBaseTexture9*	e0	= envdesc.sky_r_textures_env[0].second->surface_get();
		IDirect3DBaseTexture9*	e1	= envdesc.sky_r_textures_env[1].second->surface_get();
		t_envmap_0->surface_set		(e0);	_RELEASE(e0);
		t_envmap_1->surface_set		(e1);	_RELEASE(e1);
	
		// Draw
		RCache.set_Element			(s_combine->E[0]		);
		RCache.set_c				("m_v2w",		m_v2w	);
		RCache.set_c				("L_ambient",	ambclr	);
		RCache.set_c				("env_color",	envclr	);
		RCache.set_c				("fog_color",	fogclr	);
		RCache.set_Geometry			(g_combine_VP			);
		RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}

	// Draw skybox
	{
		RCache.set_Stencil			(TRUE,D3DCMP_EQUAL,0x00,0xff,0x00);
		if (RImplementation.o.nvstencil)	{
			u_stencil_optimize				(FALSE);
			RCache.set_ColorWriteEnable		();
		}
		g_pGamePersistent->Environment.RenderFirst	();
	}

	// Forward rendering
	{
		u_setrt							(rt_Generic_0,0,0,HW.pBaseZB);		// LDR RT
		RCache.set_CullMode				(CULL_CCW);
		RCache.set_Stencil				(FALSE);
		RCache.set_ColorWriteEnable		();
		RImplementation.render_forward	();
	}


	// Perform blooming filter and distortion if needed
	RCache.set_Stencil	(FALSE);
	phase_bloom			( );												// HDR RT invalidated here

	// Distortion filter
	BOOL	bDistort	= RImplementation.o.distortion_enabled;				// This can be modified
	{
		if (0==RImplementation.mapDistort.size())	bDistort	= FALSE;
		if (bDistort)		{
			u_setrt						(rt_Generic_1,0,0,HW.pBaseZB);		// Now RT is a distortion mask
			RCache.set_CullMode			(CULL_CCW);
			RCache.set_Stencil			(FALSE);
			RCache.set_ColorWriteEnable	();
			CHK_DX(HW.pDevice->Clear	( 0L, NULL, D3DCLEAR_TARGET, color_rgba(127,127,127,127), 1.0f, 0L));
			RImplementation.r_dsgraph_render_distort	();
		}
	}

	// Combine everything + perform AA
	u_setrt				( Device.dwWidth,Device.dwHeight,HW.pBaseRT,NULL,NULL,HW.pBaseZB);
	RCache.set_CullMode	( CULL_NONE );
	RCache.set_Stencil	( FALSE		);
	if (1)	
	{
		// 
		struct v_aa	{
			Fvector4	p;
			Fvector2	uv0;
			Fvector2	uv1;
			Fvector2	uv2;
			Fvector2	uv3;
			Fvector2	uv4;
			Fvector4	uv5;
			Fvector4	uv6;
		};

		float	_w					= float(Device.dwWidth);
		float	_h					= float(Device.dwHeight);
		float	ddw					= 1.f/_w;
		float	ddh					= 1.f/_h;
		p0.set						(.5f/_w, .5f/_h);
		p1.set						((_w+.5f)/_w, (_h+.5f)/_h );

		// Fill vertex buffer
		v_aa* pv					= (v_aa*) RCache.Vertex.Lock	(4,g_aa_AA->vb_stride,Offset);
		pv->p.set(EPS,			float(_h+EPS),	EPS,1.f); pv->uv0.set(p0.x, p1.y);pv->uv1.set(p0.x-ddw,p1.y-ddh);pv->uv2.set(p0.x+ddw,p1.y+ddh);pv->uv3.set(p0.x+ddw,p1.y-ddh);pv->uv4.set(p0.x-ddw,p1.y+ddh);pv->uv5.set(p0.x-ddw,p1.y,p1.y,p0.x+ddw);pv->uv6.set(p0.x,p1.y-ddh,p1.y+ddh,p0.x);pv++;
		pv->p.set(EPS,			EPS,			EPS,1.f); pv->uv0.set(p0.x, p0.y);pv->uv1.set(p0.x-ddw,p0.y-ddh);pv->uv2.set(p0.x+ddw,p0.y+ddh);pv->uv3.set(p0.x+ddw,p0.y-ddh);pv->uv4.set(p0.x-ddw,p0.y+ddh);pv->uv5.set(p0.x-ddw,p0.y,p0.y,p0.x+ddw);pv->uv6.set(p0.x,p0.y-ddh,p0.y+ddh,p0.x);pv++;
		pv->p.set(float(_w+EPS),float(_h+EPS),	EPS,1.f); pv->uv0.set(p1.x, p1.y);pv->uv1.set(p1.x-ddw,p1.y-ddh);pv->uv2.set(p1.x+ddw,p1.y+ddh);pv->uv3.set(p1.x+ddw,p1.y-ddh);pv->uv4.set(p1.x-ddw,p1.y+ddh);pv->uv5.set(p1.x-ddw,p1.y,p1.y,p1.x+ddw);pv->uv6.set(p1.x,p1.y-ddh,p1.y+ddh,p1.x);pv++;
		pv->p.set(float(_w+EPS),EPS,			EPS,1.f); pv->uv0.set(p1.x, p0.y);pv->uv1.set(p1.x-ddw,p0.y-ddh);pv->uv2.set(p1.x+ddw,p0.y+ddh);pv->uv3.set(p1.x+ddw,p0.y-ddh);pv->uv4.set(p1.x-ddw,p0.y+ddh);pv->uv5.set(p1.x-ddw,p0.y,p0.y,p1.x+ddw);pv->uv6.set(p1.x,p0.y-ddh,p0.y+ddh,p1.x);pv++;
		RCache.Vertex.Unlock		(4,g_aa_AA->vb_stride);

		// Draw COLOR
		if (ps_r2_ls_flags.test(R2FLAG_AA))			RCache.set_Element	(s_combine->E[bDistort?3:1]);	// look at blender_combine.cpp
		else										RCache.set_Element	(s_combine->E[bDistort?4:2]);	// look at blender_combine.cpp
		RCache.set_c				("e_barrier",	ps_r2_aa_barier.x,	ps_r2_aa_barier.y,	ps_r2_aa_barier.z,	0);
		RCache.set_c				("e_weights",	ps_r2_aa_weight.x,	ps_r2_aa_weight.y,	ps_r2_aa_weight.z,	0);
		RCache.set_c				("e_kernel",	ps_r2_aa_kernel,	ps_r2_aa_kernel,	ps_r2_aa_kernel,	0);
		RCache.set_c				("m_current",	m_current);
		RCache.set_c				("m_previous",	m_previous);
		RCache.set_c				("m_blur",		m_blur_scale.x,m_blur_scale.y, 0,0);
		RCache.set_Geometry			(g_aa_AA);
		RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
	}

	//	Re-adapt luminance
	RCache.set_Stencil		(FALSE);

	//*** exposure-pipeline-clear
	{
		std::swap					(rt_LUM_pool[gpu_id*2+0],rt_LUM_pool[gpu_id*2+1]);
		t_LUM_src->surface_set		(NULL);
		t_LUM_dest->surface_set		(NULL);
	}

#ifdef DEBUG
	RCache.set_CullMode	( CULL_CCW );
	static	xr_vector<Fplane>		saved_dbg_planes;
	if (bDebug)		saved_dbg_planes= dbg_planes;
	else			dbg_planes		= saved_dbg_planes;
	if (1) for (u32 it=0; it<dbg_planes.size(); it++)
	{
		Fplane&		P	=	dbg_planes[it];
		Fvector		zero	;
		zero.mul	(P.n,P.d);
		
		Fvector             L_dir,L_up=P.n,L_right;
		L_dir.set           (0,0,1);                if (_abs(L_up.dotproduct(L_dir))>.99f)  L_dir.set(1,0,0);
		L_right.crossproduct(L_up,L_dir);           L_right.normalize       ();
		L_dir.crossproduct  (L_right,L_up);         L_dir.normalize         ();

		Fvector				p0,p1,p2,p3;
		float				sz	= 100.f;
		p0.mad				(zero,L_right,sz).mad	(L_dir,sz);
		p1.mad				(zero,L_right,sz).mad	(L_dir,-sz);
		p2.mad				(zero,L_right,-sz).mad	(L_dir,-sz);
		p3.mad				(zero,L_right,-sz).mad	(L_dir,+sz);
		RCache.dbg_DrawTRI	(Fidentity,p0,p1,p2,0xffffffff);
		RCache.dbg_DrawTRI	(Fidentity,p2,p3,p0,0xffffffff);
	}

	static	xr_vector<dbg_line_t>	saved_dbg_lines;
	if (bDebug)		saved_dbg_lines	= dbg_lines;
	else			dbg_lines		= saved_dbg_lines;
	if (1) for (u32 it=0; it<dbg_lines.size(); it++)
	{
		RCache.dbg_DrawLINE		(Fidentity,dbg_lines[it].P0,dbg_lines[it].P1,dbg_lines[it].color);
	}
#endif

	// ********************* Debug
	/*
	if (0)		{
		u32		C					= color_rgba	(255,255,255,255);
		float	_w					= float(Device.dwWidth)/3;
		float	_h					= float(Device.dwHeight)/3;

		// draw light-spheres
#ifdef DEBUG
		if (0) for (u32 it=0; it<dbg_spheres.size(); it++)
		{
			Fsphere				S	= dbg_spheres[it].first;
			Fmatrix				M;	
			u32				ccc		= dbg_spheres[it].second.get();
			M.scale					(S.R,S.R,S.R);
			M.translate_over		(S.P);
			RCache.dbg_DrawEllipse	(M,ccc);
			RCache.dbg_DrawAABB		(S.P,.05f,.05f,.05f,ccc);
		}
#endif
		// Draw quater-screen quad textured with our direct-shadow-map-image
		if (1) 
		{
			u32							IX=0,IY=1;
			p0.set						(.5f/_w, .5f/_h);
			p1.set						((_w+.5f)/_w, (_h+.5f)/_h );

			// Fill vertex buffer
			FVF::TL* pv					= (FVF::TL*) RCache.Vertex.Lock	(4,g_combine->vb_stride,Offset);
			pv->set						((IX+0)*_w+EPS,	(IY+1)*_h+EPS,	EPS,	1.f, C, p0.x, p1.y);	pv++;
			pv->set						((IX+0)*_w+EPS,	(IY+0)*_h+EPS,	EPS,	1.f, C, p0.x, p0.y);	pv++;
			pv->set						((IX+1)*_w+EPS,	(IY+1)*_h+EPS,	EPS,	1.f, C, p1.x, p1.y);	pv++;
			pv->set						((IX+1)*_w+EPS,	(IY+0)*_h+EPS,	EPS,	1.f, C, p1.x, p0.y);	pv++;
			RCache.Vertex.Unlock		(4,g_combine->vb_stride);

			// Draw COLOR
			RCache.set_Shader			(s_combine_dbg_0);
			RCache.set_Geometry			(g_combine);
			RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
		}

		// Draw quater-screen quad textured with our accumulator
		if (0)
		{
			u32							IX=1,IY=1;
			p0.set						(.5f/_w, .5f/_h);
			p1.set						((_w+.5f)/_w, (_h+.5f)/_h );

			// Fill vertex buffer
			FVF::TL* pv					= (FVF::TL*) RCache.Vertex.Lock	(4,g_combine->vb_stride,Offset);
			pv->set						((IX+0)*_w+EPS,	(IY+1)*_h+EPS,	EPS,	1.f, C, p0.x, p1.y);	pv++;
			pv->set						((IX+0)*_w+EPS,	(IY+0)*_h+EPS,	EPS,	1.f, C, p0.x, p0.y);	pv++;
			pv->set						((IX+1)*_w+EPS,	(IY+1)*_h+EPS,	EPS,	1.f, C, p1.x, p1.y);	pv++;
			pv->set						((IX+1)*_w+EPS,	(IY+0)*_h+EPS,	EPS,	1.f, C, p1.x, p0.y);	pv++;
			RCache.Vertex.Unlock		(4,g_combine->vb_stride);

			// Draw COLOR
			RCache.set_Shader			(s_combine_dbg_1);
			RCache.set_Geometry			(g_combine);
			RCache.Render				(D3DPT_TRIANGLELIST,Offset,0,4,0,2);
		}
	}
	*/
#ifdef DEBUG
	dbg_spheres.clear	();
	dbg_lines.clear		();
	dbg_planes.clear	();
#endif
}

void CRenderTarget::phase_wallmarks		()
{
	// Targets
	u_setrt								(rt_Color,NULL,NULL,HW.pBaseZB);
	// Stencil	- draw only where stencil >= 0x1
	RCache.set_Stencil					(TRUE,D3DCMP_LESSEQUAL,0x01,0xff,0x00);
	RCache.set_CullMode					(CULL_CCW);
	RCache.set_ColorWriteEnable			(D3DCOLORWRITEENABLE_RED|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_BLUE);
}
