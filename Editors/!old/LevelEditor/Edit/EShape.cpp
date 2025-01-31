//---------------------------------------------------------------------------
#include "stdafx.h"
#pragma hdrstop

#include "EShape.h"
#include "D3DUtils.h"
#include "render.h"
#include "du_box.h"
#include "Scene.h"

#define SHAPE_COLOR_TRANSP		0x1800FF00
#define SHAPE_COLOR_EDGE		0xFF202020
//---------------------------------------------------------------------------

#define SHAPE_CURRENT_VERSION 	0x0001
//---------------------------------------------------------------------------
#define SHAPE_CHUNK_VERSION 	0x0000
#define SHAPE_CHUNK_SHAPES 		0x0001
//---------------------------------------------------------------------------

CEditShape::CEditShape(LPVOID data, LPCSTR name):CCustomObject(data,name)
{
	Construct(data);
}

CEditShape::~CEditShape()
{
}

void CEditShape::Construct(LPVOID data)
{
	ClassID		= OBJCLASS_SHAPE;
    m_DrawTranspColor	= SHAPE_COLOR_TRANSP;
    m_DrawEdgeColor		= SHAPE_COLOR_EDGE;
	m_Box.invalidate();
}

void CEditShape::OnUpdateTransform()
{
	inherited::OnUpdateTransform();
}

void CEditShape::ComputeBounds()
{
	m_Box.invalidate	();

	for (ShapeIt it=shapes.begin(); it!=shapes.end(); it++){
		switch (it->type){
		case cfSphere:{
            Fsphere&	T		= it->data.sphere;
            Fvector		P;
            P.set		(T.P);	P.sub(T.R);	m_Box.modify(P);
            P.set		(T.P);	P.add(T.R);	m_Box.modify(P);
		}break;
		case cfBox:{
            Fvector		P;
            Fmatrix&	T		= it->data.box;

            // Build points
            Fvector p;
            for (int i=0; i<DU_BOX_NUMVERTEX; i++){
                T.transform_tiny	(P,du_box_vertices[i]);
                m_Box.modify		(P);
            }
		}break;
		}
	}
	m_Box.getsphere(m_Sphere.P,m_Sphere.R);
}

void CEditShape::SetScale(const Fvector& val)
{
	if (shapes.size()==1){
		switch (shapes[0].type){
		case cfSphere:{
        	FScale.set(val.x,val.x,val.x);
        }break;
		case cfBox:		FScale.set(val.x,val.y,val.z);	break;
        default: THROW;
		}
    }else{
		FScale.set(val.x,val.x,val.x);
    }
	ComputeBounds	();
    UpdateTransform	();
}

void CEditShape::ApplyScale()
{
	for (ShapeIt it=shapes.begin(); it!=shapes.end(); it++){
		switch (it->type){
		case cfSphere:{
            Fsphere&	T	= it->data.sphere;
            FTransformS.transform_tiny(T.P);
            T.R				*= PScale.x;
		}break;
		case cfBox:{
            Fmatrix& B		= it->data.box;
            B.mulA			(FTransformS);
		}break;
        }
    }
    FScale.set		(1.f,1.f,1.f);
    UpdateTransform	(true);

    ComputeBounds	();
}

void CEditShape::add_sphere(const Fsphere& S)
{
	shapes.push_back(shape_def());
	shapes.back().type	= cfSphere;
	shapes.back().data.sphere.set(S);

	ComputeBounds();
}

void CEditShape::add_box(const Fmatrix& B)
{
	shapes.push_back(shape_def());
	shapes.back().type	= cfBox;
	shapes.back().data.box.set(B);

	ComputeBounds();
}

void CEditShape::Attach(CEditShape* from)
{
	ApplyScale				();
	// transfer data
    from->ApplyScale		();
	Fmatrix M 				= from->_Transform();
    M.mulA					(_ITransform());
	for (ShapeIt it=from->shapes.begin(); it!=from->shapes.end(); it++){
		switch (it->type){
		case cfSphere:{
            Fsphere& T		= it->data.sphere;
            M.transform_tiny(T.P);
            add_sphere		(T);
		}break;
		case cfBox:{
            Fmatrix B		= it->data.box;
            B.mulA			(M);
            add_box			(B);
		}break;
        default: THROW;
		}
    }
    // common
    Scene->RemoveObject		(from,true);
    xr_delete				(from);

	ComputeBounds			();
}

void CEditShape::Detach()
{
	if (shapes.size()>1){
    	Select			(true);
        ApplyScale		();
        // create scene shapes
        const Fmatrix& M = _Transform();
        ShapeIt it=shapes.begin(); it++;
        for (; it!=shapes.end(); it++){
            string256 namebuffer;
            Scene->GenObjectName	(OBJCLASS_SHAPE, namebuffer, Name);
            CEditShape* shape 	= (CEditShape*)Scene->GetOTools(ClassID)->CreateObject(0, namebuffer);
            switch (it->type){
            case cfSphere:{
                Fsphere	T		= it->data.sphere;
                M.transform_tiny(T.P);
                shape->PPosition= T.P;
                T.P.set			(0,0,0);
                shape->add_sphere(T);
            }break;
            case cfBox:{
                Fmatrix B		= it->data.box;
                B.mulA			(M);
                shape->PPosition= B.c;
                B.c.set			(0,0,0);
                shape->add_box	(B);
            }break;
            default: THROW;
            }
            Scene->AppendObject	(shape,false);
	    	shape->Select		(true);
        }
        // erase shapes in base object
        it=shapes.begin(); it++;
        shapes.erase(it,shapes.end());

        ComputeBounds();

        Scene->UndoSave();
    }
}

bool CEditShape::RayPick(float& distance, const Fvector& start, const Fvector& direction, SRayPickInfo* pinf)
{
    float dist					= distance;

	for (ShapeIt it=shapes.begin(); it!=shapes.end(); it++){
		switch (it->type){
		case cfSphere:{
            Fvector S,D;
            Fmatrix M;
            M.invert			(FTransformR);
            M.transform_dir		(D,direction);
            FITransform.transform_tiny(S,start);
            Fsphere&	T		= it->data.sphere;
            T.intersect			(S,D,dist);
            if (dist<=0.f)		dist = distance;
		}break;
		case cfBox:{
        	Fbox box;
            box.identity		();
            Fmatrix BI;
            BI.invert			(it->data.box);
		    Fvector S,D,S1,D1,P;
		    FITransform.transform_tiny	(S,start);
		    FITransform.transform_dir	(D,direction);
		    BI.transform_tiny			(S1,S);
		    BI.transform_dir			(D1,D);
            Fbox::ERP_Result	rp_res 	= box.Pick2(S1,D1,P);
            if (rp_res==Fbox::rpOriginOutside){
            	it->data.box.transform_tiny	(P);
                FTransform.transform_tiny	(P);
                P.sub			(start);
                dist			= P.magnitude();
            }
		}break;
		}
    }
    if (dist<distance){
        distance	= dist;
        return 		true;
    }
	return false;
}

bool CEditShape::FrustumPick(const CFrustum& frustum)
{
	const Fmatrix& M	= _Transform();
	for (ShapeIt it=shapes.begin(); it!=shapes.end(); it++){
		switch (it->type){
		case cfSphere:{
		    Fvector 	C;
            Fsphere&	T	= it->data.sphere;
		    M.transform_tiny(C,T.P);
        	if (frustum.testSphere_dirty(C,T.R*FScale.x)) return true;
		}break;
		case cfBox:{
        	Fbox 			box;
            box.identity	();
            Fmatrix B		= it->data.box;
            B.mulA	 		(_Transform());
            box.xform		(B);
			u32 mask		= 0xff;
            if (frustum.testAABB(box.data(),mask)) return true;
		}break;
		}
    }
	return false;
}

bool CEditShape::GetBox(Fbox& box)
{
	if (m_Box.is_valid()){
    	box.xform(m_Box,FTransform);
    	return true;
    }
	return false;
}

bool CEditShape::Load(IReader& F)
{
	R_ASSERT(F.find_chunk(SHAPE_CHUNK_VERSION));
    u16 vers		= F.r_u16();
	if (SHAPE_CURRENT_VERSION!=vers){
		ELog.DlgMsg( mtError, "CEditShape: unsupported version. Object can't load.");
    	return false;
    }
	inherited::Load	(F);

	R_ASSERT(F.find_chunk(SHAPE_CHUNK_SHAPES));
    shapes.resize	(F.r_u32());
    F.r				(shapes.begin(),shapes.size()*sizeof(shape_def));

	ComputeBounds();
	return true;
}

void CEditShape::Save(IWriter& F)
{
	inherited::Save	(F);

	F.open_chunk	(SHAPE_CHUNK_VERSION);
	F.w_u16			(SHAPE_CURRENT_VERSION);
	F.close_chunk	();

	F.open_chunk	(SHAPE_CHUNK_SHAPES);
    F.w_u32			(shapes.size());
    F.w				(shapes.begin(),shapes.size()*sizeof(shape_def));
	F.close_chunk	();
}

void CEditShape::FillProp(LPCSTR pref, PropItemVec& values)
{
	inherited::FillProp(pref,values);
}

void CEditShape::Render(int priority, bool strictB2F)
{
	inherited::Render(priority, strictB2F);
    if (1==priority){
        if (strictB2F){
	        Device.SetShader			(Device.m_WireShader);
            Device.SetRS				(D3DRS_CULLMODE,D3DCULL_NONE);
            u32 clr 					= Selected()?subst_alpha(m_DrawTranspColor, color_get_A(m_DrawTranspColor)*2):m_DrawTranspColor;
            Fvector zero				={0.f,0.f,0.f};
            for (ShapeIt it=shapes.begin(); it!=shapes.end(); it++){
                switch(it->type){
                case cfSphere:{
                    Fsphere& S			= it->data.sphere;
                    Fmatrix B;
                    B.scale				(S.R,S.R,S.R);
                    B.translate_over	(S.P);
                    B.mulA				(_Transform());
                    RCache.set_xform_world(B);
                    Device.SetShader	(Device.m_WireShader);
                    DU.DrawCross		(zero,1.f,m_DrawEdgeColor,false);
                    DU.DrawIdentSphere	(true,true,clr,m_DrawEdgeColor);
                }break;
                case cfBox:
                    Fmatrix B			= it->data.box;
                    B.mulA				(_Transform()); 
                    RCache.set_xform_world(B);
                    DU.DrawIdentBox		(true,true,clr,m_DrawEdgeColor);
                break;
                }
            }
            Device.SetRS(D3DRS_CULLMODE,D3DCULL_CCW);
        }else{
            if( Selected()&&m_Box.is_valid() ){
		        Device.SetShader		(Device.m_SelectionShader);
                RCache.set_xform_world	(_Transform());
                u32 clr 				= Locked()?0xFFFF0000:0xFFFFFFFF;
                Device.SetShader		(Device.m_WireShader);
                DU.DrawSelectionBox		(m_Box,&clr);
            }
        }
    }
}

void CEditShape::OnFrame()
{
	inherited::OnFrame();
}

void CEditShape::OnShowHint(AStringVec& dest)
{
}

