//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma hdrstop

#include "..\psystem.h"
#include "ParticleEffect.h"

#ifdef _EDITOR
	#include "UI_Tools.h"
#endif

using namespace PAPI;
using namespace PS;

static int action_list_handle=-1;

//------------------------------------------------------------------------------
// class CParticleEffectDef
//------------------------------------------------------------------------------
CPEDef::CPEDef()
{                                         
    strcpy				(m_Name,"unknown");
    m_ShaderName		= 0;
    m_TextureName		= 0;
    m_Frame.InitDefault	();
    m_ActionCount		= 0;
    m_ActionList		= 0;
    m_MaxParticles		= 0;
	m_CachedShader		= 0;
	m_fTimeLimit		= 0.f;
    // collision
    m_CollideOneMinusFriction 	= 1.f;
    m_CollideResilience			= 0.f;
    m_CollideSqrCutoff			= 0.f;
}

CPEDef::~CPEDef()
{
    xr_free				(m_ShaderName);
    xr_free				(m_TextureName);
    xr_free				(m_ActionList);
}
void CPEDef::SetName(LPCSTR name)
{
    strcpy				(m_Name,name);
}
void CPEDef::pAlignToPath()
{
	m_Flags.set			(dfAlignToPath,TRUE);
}
void CPEDef::pCollision(float friction, float resilience, float cutoff, BOOL destroy_on_contact)
{
    m_CollideOneMinusFriction 	= 1.f-friction;
    m_CollideResilience			= resilience;
    m_CollideSqrCutoff			= cutoff*cutoff;
	m_Flags.set					(dfCollision,TRUE);
	m_Flags.set					(dfCollisionDel,destroy_on_contact);
}

void CPEDef::pSprite(string128& sh_name, string128& tex_name)
{
    xr_free(m_ShaderName);	m_ShaderName	= xr_strdup(sh_name);
    xr_free(m_TextureName);	m_TextureName	= xr_strdup(tex_name);
	m_Flags.set	(dfSprite,TRUE);
}
void CPEDef::pFrame(BOOL random_frame, u32 frame_count, u32 tex_width, u32 tex_height, u32 frame_width, u32 frame_height)
{
	m_Flags.set			(dfFramed,TRUE);
	m_Flags.set			(dfRandomFrame,random_frame);
	m_Frame.Set			(frame_count, (float)tex_width, (float)tex_height, (float)frame_width, (float)frame_height);
}
void CPEDef::pAnimate(float speed, BOOL random_playback)
{
	m_Frame.m_fSpeed	= speed;
	m_Flags.set			(dfAnimated,TRUE);
	m_Flags.set			(dfRandomPlayback,random_playback);
}
void CPEDef::pTimeLimit(float time_limit)
{
	m_Flags.set			(dfTimeLimit,TRUE);
	m_fTimeLimit		= time_limit;
}
void CPEDef::pFrameInitExecute(ParticleEffect *effect)
{
	for(int i = 0; i < effect->p_count; i++){
		Particle &m = effect->list[i];
        if (m.flags.is(Particle::BIRTH)){
            if (m_Flags.is(dfRandomFrame))
                m.frame	= (float)Random.randI(m_Frame.m_iFrameCount);
            if (m_Flags.is(dfAnimated)&&m_Flags.is(dfRandomPlayback)&&Random.randI(2))
                m.flags.set(Particle::ANIMATE_CCW,TRUE);
        }
    }
}
void CPEDef::pAnimateExecute(ParticleEffect *effect, float dt)
{
	float speedFac = m_Frame.m_fSpeed * dt;
	for(int i = 0; i < effect->p_count; i++){
		Particle &m = effect->list[i];
		m.frame						+= ((m.flags.is(Particle::ANIMATE_CCW))?-1.f:1.f)*speedFac;
		if (m.frame>m_Frame.m_iFrameCount)	m.frame-=m_Frame.m_iFrameCount;
		if (m.frame<0.f)					m.frame+=m_Frame.m_iFrameCount;
	}
}

void CPEDef::pCollisionExecute(PAPI::ParticleEffect *effect, float dt, CParticleEffect* owner, CollisionCallback cb)
{
    pVector pt,n;
	// Must traverse list in reverse order so Remove will work
	for(int i = effect->p_count-1; i >= 0; i--){
		Particle &m = effect->list[i];

        bool pick_needed;
        int pick_cnt=0;
		do{		
        	pick_needed = false;
            Fvector 	dir;
            dir.sub		(m.pos,m.posB);
            float dist 	= dir.magnitude();
            if (dist>=EPS){
            	dir.div	(dist);
#ifdef _EDITOR                
                if (Tools.RayPick(m.posB,dir,dist,&pt,&n)){
#else
                Collide::ray_query	RQ;
                if (g_pGameLevel->ObjectSpace.RayPick(m.posB,dir,dist,RQ)){	
                    pt.mad	(m.posB,dir,RQ.range);
                    if (RQ.O){
                    	n.set(0.f,1.f,0.f);
                    }else{
                    	CDB::TRI* T =  	g_pGameLevel->ObjectSpace.GetStaticTris()+RQ.element;
						n.mknormal(*T->verts[0],*T->verts[1],*T->verts[2]);
                    }
#endif
		            pick_cnt++;
                    if (cb&&(pick_cnt==1)) if (!cb(owner,m,pt,n)) break;
                    if (m_Flags.is(dfCollisionDel)) effect->Remove(i);
                    else{
                        // Compute tangential and normal components of velocity
                        float nmag = m.vel * n;
                        pVector vn(n * nmag); 	// Normal Vn = (V.N)N
                        pVector vt(m.vel - vn);	// Tangent Vt = V - Vn

                        // Compute _new velocity heading out:
                        // Don't apply friction if tangential velocity < cutoff
                        if(vt.length2() <= m_CollideSqrCutoff){
                            m.vel = vt - vn * m_CollideResilience;
                        }else{
                            m.vel = vt * m_CollideOneMinusFriction - vn * m_CollideResilience;
                        }
                        m.pos	= m.posB + m.vel * dt; 
                        pick_needed = true;
                    }
                }
            }else{
                m.pos	= m.posB;
            }
   		}while(pick_needed&&(pick_cnt<2));
	}
}

//------------------------------------------------------------------------------
// I/O part
//------------------------------------------------------------------------------
BOOL CPEDef::Load(IReader& F)
{
	R_ASSERT		(F.find_chunk(PED_CHUNK_VERSION));
	u16 version		= F.r_u16();

    if (version!=PED_VERSION)
    	return FALSE;

	R_ASSERT		(F.find_chunk(PED_CHUNK_NAME));
	F.r_stringZ		(m_Name);

	R_ASSERT		(F.find_chunk(PED_CHUNK_EFFECTDATA));
    m_MaxParticles	= F.r_u32();

    R_ASSERT		(F.find_chunk(PED_CHUNK_ACTIONLIST));
    m_ActionCount	= F.r_u32();
    m_ActionList	= xr_alloc<PAPI::PAHeader>(m_ActionCount);
    F.r				(m_ActionList,m_ActionCount*sizeof(PAPI::PAHeader));

	F.r_chunk		(PED_CHUNK_FLAGS,&m_Flags);

    string256		buf;
    if (m_Flags.is(dfSprite)){
        R_ASSERT	(F.find_chunk(PED_CHUNK_SPRITE));
        F.r_stringZ	(buf); m_ShaderName = xr_strdup(buf);
        F.r_stringZ	(buf); m_TextureName= xr_strdup(buf);
    }

    if (m_Flags.is(dfFramed)){
        R_ASSERT	(F.find_chunk(PED_CHUNK_FRAME));
        F.r			(&m_Frame,sizeof(SFrame));
    }

    if (m_Flags.is(dfTimeLimit)){
        R_ASSERT(F.find_chunk(PED_CHUNK_TIMELIMIT));
        m_fTimeLimit= F.r_float();
    }

    if (m_Flags.is(dfCollision)){
    	R_ASSERT(F.find_chunk(PED_CHUNK_COLLISION));
        m_CollideOneMinusFriction 	= F.r_float();
        m_CollideResilience			= F.r_float();
        m_CollideSqrCutoff			= F.r_float();
    }
    
#ifdef _PARTICLE_EDITOR
	F.find_chunk	(PED_CHUNK_SOURCETEXT);
    F.r_stringZ		(m_SourceText);
#endif

    return TRUE;
}

void CPEDef::Save(IWriter& F)
{
	F.open_chunk	(PED_CHUNK_VERSION);
	F.w_u16			(PED_VERSION);
    F.close_chunk	();

	F.open_chunk	(PED_CHUNK_NAME);
    F.w_stringZ		(m_Name);
    F.close_chunk	();

	F.open_chunk	(PED_CHUNK_EFFECTDATA);
    F.w_u32			(m_MaxParticles);
    F.close_chunk	();

	F.open_chunk	(PED_CHUNK_ACTIONLIST);
    F.w_u32			(m_ActionCount);
    F.w				(m_ActionList,m_ActionCount*sizeof(PAPI::PAHeader));
    F.close_chunk	();

	F.w_chunk		(PED_CHUNK_FLAGS,&m_Flags,sizeof(m_Flags));

    if (m_Flags.is(dfSprite)){
        F.open_chunk	(PED_CHUNK_SPRITE);
        F.w_stringZ		(m_ShaderName);
        F.w_stringZ		(m_TextureName);
        F.close_chunk	();
    }

    if (m_Flags.is(dfFramed)){
        F.open_chunk	(PED_CHUNK_FRAME);
        F.w				(&m_Frame,sizeof(SFrame));
        F.close_chunk	();
    }

    if (m_Flags.is(dfTimeLimit)){
        F.open_chunk	(PED_CHUNK_TIMELIMIT);
        F.w_float		(m_fTimeLimit);
        F.close_chunk	();
    }

    if (m_Flags.is(dfCollision)){
        F.open_chunk	(PED_CHUNK_COLLISION);
        F.w_float		(m_CollideOneMinusFriction);
        F.w_float		(m_CollideResilience);
        F.w_float		(m_CollideSqrCutoff);
        F.close_chunk	();
    }
    
#ifdef _PARTICLE_EDITOR
	F.open_chunk	(PED_CHUNK_SOURCETEXT);
    F.w_stringZ		(m_SourceText.c_str());
	F.close_chunk	();
#endif
}
//------------------------------------------------------------------------------
// class CParticleEffect
//------------------------------------------------------------------------------
CParticleEffect::CParticleEffect()
{
	m_HandleEffect 			= pGenParticleEffects(1, 1);
    m_HandleActionList		= pGenActionLists();
    m_RT_Flags.zero			();
    m_Def					= 0;
    m_fElapsedLimit			= 0.f;
	m_MemDT					= 0;
	m_InitialPosition.set	(0,0,0);
	m_DestroyCallback		= 0;
	m_CollisionCallback		= 0;
}
CParticleEffect::~CParticleEffect()
{
	OnDeviceDestroy			();
	pDeleteParticleEffects	(m_HandleEffect);
	pDeleteActionLists		(m_HandleActionList);
}

void CParticleEffect::OnDeviceCreate()
{
	if (m_Def){
        if (m_Def->m_Flags.is(CPEDef::dfSprite)){
            hGeom.create	(FVF::F_TL, RCache.Vertex.Buffer(), RCache.QuadIB);
            if (m_Def) hShader = m_Def->m_CachedShader;
        }
    }
}

void CParticleEffect::OnDeviceDestroy()
{
	if (m_Def){
        if (m_Def->m_Flags.is(CPEDef::dfSprite)){
            hGeom.destroy		();
            hShader.destroy		();
        }    
    }
}

void CParticleEffect::Play()
{
	m_RT_Flags.zero		();
	m_RT_Flags.set		(flRT_Playing,TRUE);
   	pStartPlaying		(m_HandleActionList);
}
void CParticleEffect::Stop(BOOL bDefferedStop)
{
	if (bDefferedStop){
		m_RT_Flags.set	(flRT_DefferedStop,TRUE);
    	pStopPlaying	(m_HandleActionList);
    }else{
    	m_RT_Flags.set	(flRT_Playing,FALSE);
		ResetParticles	();
    }
}
void CParticleEffect::RefreshShader()
{
	OnDeviceDestroy();
    OnDeviceCreate();
}

void CParticleEffect::ResetParticles()
{
	if (m_Def){
        pSetMaxParticlesG	(m_HandleEffect,0);
        pSetMaxParticlesG	(m_HandleEffect,m_Def->m_MaxParticles);
    }
}
void CParticleEffect::UpdateParent(const Fmatrix& m, const Fvector& velocity)
{
	m_InitialPosition		= m.c;
	pSetActionListParenting	(m_HandleActionList,m,velocity);
}

static const u32	uDT_STEP = 33;
static const float	fDT_STEP = float(uDT_STEP)/1000.f;

static u32 mb_dt = 500;

void CParticleEffect::OnFrame(u32 frame_dt)
{
	if (m_Def && m_RT_Flags.is(flRT_Playing)){
		m_MemDT			+= frame_dt;
		for (;m_MemDT>=uDT_STEP; m_MemDT-=uDT_STEP){
            if (m_Def->m_Flags.is(CPEDef::dfTimeLimit)){ 
				if (!m_RT_Flags.is(flRT_DefferedStop)){
                    m_fElapsedLimit -= fDT_STEP;
                    if (m_fElapsedLimit<0.f){
                        m_fElapsedLimit = m_Def->m_fTimeLimit;
                        Stop		(true);
//                        Msg			("timelimit(%s)",GetDefinition()->m_Name);
//                        return;
                    }
                }
            }
			pTimeStep			(fDT_STEP);
			pCurrentEffect		(m_HandleEffect);

			// execute action list
			pCallActionList		(m_HandleActionList);

			if (action_list_handle>-1) 
            	pCallActionList	(action_list_handle);
            
			ParticleEffect *pg 	= _GetEffectPtr(m_HandleEffect);
			// our actions
			if (m_Def->m_Flags.is(CPEDef::dfFramed))    		  		m_Def->pFrameInitExecute(pg);
			if (m_Def->m_Flags.is(CPEDef::dfFramed|CPEDef::dfAnimated))	m_Def->pAnimateExecute	(pg,fDT_STEP);
            if (m_Def->m_Flags.is(CPEDef::dfCollision)) 				m_Def->pCollisionExecute(pg,fDT_STEP,this,m_CollisionCallback);

			//-move action
			if (pg->p_count)	
			{
				vis.box.invalidate	();
				float p_size = 0.f;
				for(int i = 0; i < pg->p_count; i++){
					Particle &m 	= pg->list[i]; 
					if (m.flags.is(Particle::DYING)){if (m_DestroyCallback) m_DestroyCallback(this,m);}
					if (m.flags.is(Particle::BIRTH))m.flags.set(Particle::BIRTH,FALSE);
					vis.box.modify((Fvector&)m.pos);
					if (m.size.x>p_size) p_size = m.size.x;
                    // ---
		            if (m_Def->m_Flags.is(CPEDef::dfAlignToPath)){
                    	u32 a0 = iFloor((m.age-fDT_STEP)*1000.f)/mb_dt;
                    	u32 a1 = iFloor(m.age*1000.f)/mb_dt;
                    	if (a0!=a1){
	                        Msg("%3.2f, %3.2f, %3.2f, %3.2f",m.pos.y,m.p[0].y,m.p[1].y,m.p[2].y);
                            m.p[3] = m.p[2];
                            m.p[2] = m.p[1];
                            m.p[1] = m.p[0];
                            m.p[0] = m.pos;
                        }
                    }
				}
				vis.box.grow		(p_size);
				vis.box.getsphere	(vis.sphere.P,vis.sphere.R);
			}
            if (m_RT_Flags.is(flRT_DefferedStop)&&(0==pg->p_count)){
				m_RT_Flags.set		(flRT_Playing|flRT_DefferedStop,FALSE);
                break;
            }
		}
	} else {
		vis.box.set			(m_InitialPosition,m_InitialPosition);
		vis.box.grow		(EPS_L);
		vis.box.getsphere	(vis.sphere.P,vis.sphere.R);
	}
}

BOOL CParticleEffect::Compile(CPEDef* def)
{
	m_Def 						= def;
    if (m_Def){
        // set current effect for action
        pCurrentEffect			(m_HandleEffect);
        // refresh shader
        RefreshShader			();
        // reset particles
        ResetParticles			();
        // load action list
        // get pointer to specified action list.
        PAPI::PAHeader* pa		= _GetListPtr(m_HandleActionList);
        if(pa == NULL)	
			return FALSE; // ERROR

        // start append actions
        pNewActionList			(m_HandleActionList);
        for (int k=0; k<m_Def->m_ActionCount; k++)
            pAddActionToList	(m_Def->m_ActionList+k);
        // end append action
        pEndActionList();
        // time limit
		if (m_Def->m_Flags.is(CPEDef::dfTimeLimit))
			m_fElapsedLimit 	= m_Def->m_fTimeLimit;
    }
	if (def)	hShader			= def->m_CachedShader;
	return TRUE;
}

//------------------------------------------------------------------------------
// Render
//------------------------------------------------------------------------------
void CParticleEffect::Copy(IRender_Visual* pFrom)
{
	Debug.fatal("Can't duplicate particle system - NOT IMPLEMENTED");
}

//----------------------------------------------------
IC void FillSprite	(FVF::TL*& pv, const Fmatrix& M, const Fvector& pos, const Fvector2& lt, const Fvector2& rb, float radius, u32 clr, float angle, float scale, float w_2, float h_2)
{
	FVF::TL			PT;

	PT.transform	(pos, M);
	float sz		= scale * radius / PT.p.w;
	// 'Cause D3D clipping have to clip Four points
	// We can help him :)

	if (sz<1.5f)	return;
	if (PT.p.x< -1)	return;
	if (PT.p.x>  1)	return;
	if (PT.p.y< -1)	return;
	if (PT.p.y>  1)	return;
	if (PT.p.z<  0) return;

	// Convert to screen coords
	Fvector2	c;
	c.x				= (PT.p.x+1)*w_2;
	c.y				= (PT.p.y+1)*h_2;

	// Rotation
	float	_sin1,_cos1,_sin2,_cos2,da;
	da = angle;		 _sin1=_sin(da); _cos1=_cos(da);
	da += PI_DIV_2;  _sin2=_sin(da); _cos2=_cos(da);

	pv->set	(c.x+sz*_sin1,	c.y+sz*_cos1,	PT.p.z, PT.p.w, clr, lt.x,rb.y);	pv++;
	pv->set	(c.x-sz*_sin2,	c.y-sz*_cos2,	PT.p.z, PT.p.w, clr, lt.x,lt.y);	pv++;
	pv->set	(c.x+sz*_sin2,	c.y+sz*_cos2,	PT.p.z, PT.p.w, clr, rb.x,rb.y);	pv++;
	pv->set	(c.x-sz*_sin1,	c.y-sz*_cos1,	PT.p.z, PT.p.w, clr, rb.x,lt.y);	pv++;
}

IC void FillSprite	(FVF::TL*& pv, const Fmatrix& M, const Fvector& pos, const Fvector2& lt, const Fvector2& rb, float radius, u32 clr, const Fvector& D, float scale, float factor, float w_2, float h_2)
{
	Fvector			P1,P2;

	P1.mad			(pos,D,-radius*factor);
	P2.mad			(pos,D,radius*factor);

	FVF::TL			s1,s2;
	s1.transform	(P1,M);
	s2.transform	(P2,M);

	if ((s1.p.w<=0)||(s2.p.w<=0)) return;

    float l1,l2		= 0.7071f*scale*radius; l1 = l2;
	l1				/= s1.p.w;
	l2				/= s2.p.w;

	Fvector2		dir,R;
	R.cross			(dir.set(s2.p.x-s1.p.x,s2.p.y-s1.p.y).norm());
	s1.p.x = (s1.p.x+1)*w_2; s1.p.y	= (s1.p.y+1)*h_2;
	s2.p.x = (s2.p.x+1)*w_2; s2.p.y	= (s2.p.y+1)*h_2;
	pv->set			(s1.p.x+l1*R.x,	s1.p.y+l1*R.y,	s2.p.z, s2.p.w, clr, lt.x,rb.y);	pv++;
	pv->set			(s2.p.x+l2*R.x,	s2.p.y+l2*R.y,	s2.p.z, s2.p.w, clr, lt.x,lt.y);	pv++;
	pv->set			(s1.p.x-l1*R.x,	s1.p.y-l1*R.y,	s2.p.z, s2.p.w, clr, rb.x,rb.y);	pv++;
	pv->set			(s2.p.x-l2*R.x,	s2.p.y-l2*R.y,	s2.p.z, s2.p.w, clr, rb.x,lt.y);	pv++;
}
/*
IC void FillSprite	(FVF::TL*& pv, const Fmatrix& M, const Fvector& p0, const Fvector& p1, const Fvector2& lt, const Fvector2& rb, float radius, u32 clr, const Fvector& D, float scale, float factor, float w_2, float h_2)
{
	Fvector			P1,P2;

	P1.mad			(p0,D,-radius*factor);
	P2.mad			(p1,D,radius*factor);

	FVF::TL			s1,s2;
	s1.transform	(P1,M);
	s2.transform	(P2,M);

	if ((s1.p.w<=0)||(s2.p.w<=0)) return;

    float l1,l2		= 0.7071f*scale*radius; l1 = l2;
	l1				/= s1.p.w;
	l2				/= s2.p.w;

	Fvector2		dir,R;
	R.cross			(dir.set(s2.p.x-s1.p.x,s2.p.y-s1.p.y).norm());
	s1.p.x = (s1.p.x+1)*w_2; s1.p.y	= (s1.p.y+1)*h_2;
	s2.p.x = (s2.p.x+1)*w_2; s2.p.y	= (s2.p.y+1)*h_2;
	pv->set			(s1.p.x+l1*R.x,	s1.p.y+l1*R.y,	s2.p.z, s2.p.w, clr, lt.x,rb.y);	pv++;
	pv->set			(s2.p.x+l2*R.x,	s2.p.y+l2*R.y,	s2.p.z, s2.p.w, clr, lt.x,lt.y);	pv++;
	pv->set			(s1.p.x-l1*R.x,	s1.p.y-l1*R.y,	s2.p.z, s2.p.w, clr, rb.x,rb.y);	pv++;
	pv->set			(s2.p.x-l2*R.x,	s2.p.y-l2*R.y,	s2.p.z, s2.p.w, clr, rb.x,lt.y);	pv++;
}
*/
void CParticleEffect::Render(float LOD)
{
	u32			dwOffset,dwCount;
    // Get a pointer to the particles in gp memory
	ParticleEffect *pe 		= _GetEffectPtr(m_HandleEffect);
    if(pe == NULL)		return;
    if(pe->p_count < 1)	return;

    if (m_Def->m_Flags.is(CPEDef::dfSprite)){
        // build transform matrix
        Fmatrix mSpriteTransform	= Device.mFullTransform;

        float	w_2			= float(::Render->getTarget()->get_width()) / 2;
        float	h_2			= float(::Render->getTarget()->get_height()) / 2;
        float	fov_rate	= (Device.fFOV/90.f);
        float	fov_scale_w	= float(::Render->getTarget()->get_width()) / fov_rate;
        float 	factor_r	= (0.2952f*fov_rate*fov_rate - 0.0972f*fov_rate + 0.8007f) * (1.41421356f/Device.fASPECT); // � ������ ������� ��������� �����������

        FVF::TL* pv_start	= (FVF::TL*)RCache.Vertex.Lock(pe->p_count*4*4,hGeom->vb_stride,dwOffset);
        FVF::TL* pv			= pv_start;

        for(int i = 0; i < pe->p_count; i++){
            PAPI::Particle &m = pe->list[i];

            Fcolor c; c.set(m.color.x,m.color.y,m.color.z,m.alpha);
            u32 C 			= c.get();
            Fvector2 lt,rb;
            lt.set			(0.f,0.f);
            rb.set			(1.f,1.f);
            if (m_Def->m_Flags.is(CPEDef::dfFramed)) m_Def->m_Frame.CalculateTC(iFloor(m.frame),lt,rb);
            if (m_Def->m_Flags.is(CPEDef::dfAlignToPath)){
                Fvector 	dir;
                dir.sub		(m.pos,m.posB);
                float dist 	= dir.magnitude();
                if (dist>=EPS_S)dir.div(dist);
                else			dir.set(0,1,0);
                FillSprite	(pv,mSpriteTransform,m.pos,lt,rb,m.size.x*.5f,C,dir,fov_scale_w,factor_r,w_2,h_2);
                dir.sub		(m.p[0],m.p[1]);
                dist 		= dir.magnitude();
                if (dist>=EPS_S)dir.div(dist);
                else			dir.set(0,1,0);
                FillSprite	(pv,mSpriteTransform,m.p[1],lt,rb,m.size.x*.5f,C,dir,fov_scale_w,factor_r,w_2,h_2);
				dir.sub		(m.p[1],m.p[2]);
                dist 		= dir.magnitude();
                if (dist>=EPS_S)dir.div(dist);
                else			dir.set(0,1,0);
                FillSprite	(pv,mSpriteTransform,m.p[2],lt,rb,m.size.x*.5f,C,dir,fov_scale_w,factor_r,w_2,h_2);
                dir.sub		(m.p[2],m.p[3]);
                dist 		= dir.magnitude();
                if (dist>=EPS_S)dir.div(dist);
                else			dir.set(0,1,0);
                FillSprite	(pv,mSpriteTransform,m.p[3],lt,rb,m.size.x*.5f,C,dir,fov_scale_w,factor_r,w_2,h_2);

            }else{
                FillSprite	(pv,mSpriteTransform,m.pos,lt,rb,m.size.x*.5f,C,m.rot.x,fov_scale_w,w_2,h_2);
            }
        }
        dwCount 			= pv-pv_start;
        RCache.Vertex.Unlock(dwCount,hGeom->vb_stride);
        if (dwCount)    {
            RCache.set_Geometry	(hGeom);
            RCache.Render	   	(D3DPT_TRIANGLELIST,dwOffset,0,dwCount,0,dwCount/2);
        }
    }
}

void CParticleEffect::ApplyExplosion()
{
/*
//    pExplosion			(0,0,0, 1, 1, 0.1f, EPS_L, 1);
	pExplosion			(0,0,0, 1, 8, 3, 0.1, 1.0f);
*/
    pCurrentEffect		(m_HandleEffect);

    action_list_handle	= pGenActionLists();
	pNewActionList		(action_list_handle);
	pExplosion			(0,0,0, 1, 8, 3, 0.1, 1.0f);
	pEndActionList		();
}



/*
::Load
    if (m_Flags.is(dfObject)){
        R_ASSERT	(F.find_chunk(PED_CHUNK_MODEL));
        F.r_stringZ	(buf); m_ObjectName = xr_strdup(buf.c_str());
    }
::Save
	if (m_Flags.is(dfObject)){
        F.open_chunk	(PED_CHUNK_MODEL);
        F.w_stringZ		(m_ObjectName);
        F.close_chunk	();
    }
void CPEDef::pObject(string128& obj_name)
{
	if (m_Flags.is(dfSprite)){
    	ELog.DlgMsg		(mtError,"Sprite and Object can't work jointly.");
    	return;
    }
    xr_free(m_ObjectName);	m_ObjectName	= xr_strdup(obj_name);
	m_Flags.set	(dfObject,TRUE);
}
::Render
        else if (m_Def->m_Flags.is(CPEDef::dfObject)){
            // Get a pointer to the particles in pe memory
            PAPI::ParticleEffect *pe = PAPI::_GetEffectPtr(m_HandleEffect);
            if((pe!=NULL)&&(pe->p_count>0)){
                for(int i = 0; i < pe->p_count; i++){
                    PAPI::Particle &m = pe->list[i];
                    Fmatrix M,S;
                    M.setXYZi	(m.rot.x,m.rot.y,m.rot.z);
					S.scale		(m.size);
					M.mulB		(S);
                    M.translate_over(m.pos);
					::Render->set_Transform(&M);
					::Render->add_Visual   (m_Object);
                }
            }
        }
*/
