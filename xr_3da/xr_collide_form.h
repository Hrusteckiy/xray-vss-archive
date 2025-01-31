#ifndef __XR_COLLIDE_FORM_H__
#define __XR_COLLIDE_FORM_H__

#include "xr_collide_defs.h"

// refs
class ENGINE_API	CObject;
class ENGINE_API	CInifile;

// t-defs
const u32	clGET_TRIS			= (1<<0);
const u32	clGET_BOXES			= (1<<1);
const u32	clGET_SPHERES		= (1<<2);
const u32	clQUERY_ONLYFIRST	= (1<<3);	// stop if was any collision
const u32	clQUERY_TOPLEVEL	= (1<<4);	// get only top level of model box/sphere
const u32	clQUERY_STATIC		= (1<<5);	// static
const u32	clQUERY_DYNAMIC		= (1<<6);	// dynamic
const u32	clCOARSE			= (1<<7);	// coarse test (triangles vs obb)

struct clQueryTri
{
	Fvector				p[3];
	const CDB::TRI		*T;
};

struct clQueryCollision
{
	xr_vector<CObject*>		objects;		// affected objects
	xr_vector<clQueryTri>	tris;			// triangles		(if queried)
	xr_vector<Fobb>			boxes;			// boxes/ellipsoids	(if queried)
	xr_vector<Fvector4>		spheres;		// spheres			(if queried)
	
	IC void				Clear	()
	{
		objects.clear	();
		tris.clear		();
		boxes.clear		();
		spheres.clear	();
	}
	IC void				AddTri( const Fmatrix& m, const CDB::TRI* one, const Fvector* verts ) 
	{
		clQueryTri	T;
		m.transform_tiny	(T.p[0],verts[one->verts[0]]);
		m.transform_tiny	(T.p[1],verts[one->verts[1]]);
		m.transform_tiny	(T.p[2],verts[one->verts[2]]);
		T.T					= one;
		tris.push_back		(T);
	}
	IC void				AddTri(const CDB::TRI* one, const Fvector* verts ) 
	{
		clQueryTri			T;
		T.p[0]				= verts[one->verts[0]];
		T.p[1]				= verts[one->verts[1]];
		T.p[2]				= verts[one->verts[2]];
		T.T					= one;
		tris.push_back		(T);
	}
	IC void				AddBox(const Fmatrix& M, const Fbox& B)
	{
		Fobb			box;
		Fvector			c;
		B.getcenter		(c);
		B.getradius		(box.m_halfsize);
		
		Fmatrix			T,R;
		T.translate		(c);
		R.mul_43		(M,T);

		box.xform_set	(R);
		boxes.push_back	(box);
	}
	IC void				AddBox(const Fobb& B)
	{
		boxes.push_back	(B);
	}
};

enum ENGINE_API	ECollisionFormType{
	cftObject,
	cftShape
};

class ENGINE_API	ICollisionForm
{
	friend class	CObjectSpace;
protected:
	CObject*		owner;			// ��������
	u32				dwQueryID;
protected:
	Fbox			bv_box;			// (Local) BBox �������
	Fsphere			bv_sphere;		// (Local) Sphere 
private:
	ECollisionFormType	m_type;
public:
					ICollisionForm	( CObject* _owner, ECollisionFormType tp );
	virtual			~ICollisionForm	( );

	virtual BOOL	_RayQuery		( const collide::ray_defs& Q, collide::rq_results& R) = 0;
	virtual void	_BoxQuery		( const Fbox& B, const Fmatrix& M, u32 flags)	= 0;

	IC CObject*		Owner			( )	const				{ return owner;			}
	const Fbox&		getBBox			( )	const				{ return bv_box;		}
	float			getRadius		( )	const				{ return bv_sphere.R;	}
	const Fsphere&	getSphere		( )	const				{ return bv_sphere;		}
	const ECollisionFormType Type	( ) const				{ return m_type;		}
};

class ENGINE_API	CCF_Polygonal : public ICollisionForm
{
private:
	CDB::MODEL		model;
public:
					CCF_Polygonal	( CObject* _owner );

	virtual BOOL	_RayQuery		( const collide::ray_defs& Q, collide::rq_results& R);
	virtual void	_BoxQuery		( const Fbox& B, const Fmatrix& M, u32 flags);

	BOOL			LoadModel		( CInifile* ini, const char *section );
	CDB::TRI*		GetTris			() { return model.get_tris();	}
	CDB::MODEL*		GetModel		() { return &model;				}
};

struct CCF_OBB
{
	// Ray test
	Fmatrix		IM;		// world 2 bone xform
	Fbox		B;		// local(bone) space
	Fobb		OBB;	// world space
	u16			elem_id;
public:
				CCF_OBB():elem_id(u16(-1)){;}
				CCF_OBB(u16 id):elem_id(id){;}
	BOOL		valid(){return elem_id!=u16(-1);}
};

class ENGINE_API	CCF_Skeleton : public ICollisionForm
{
private:
	Fbox				base_box;
	xr_vector<CCF_OBB>	models;

	u32					dwFrame;		// The model itself
	u32					dwFrameTL;		// Top level

	void				BuildState		();
	void				BuildTopLevel	();
public:
						CCF_Skeleton	( CObject* _owner );

	virtual BOOL		_RayQuery		( const collide::ray_defs& Q, collide::rq_results& R);
	virtual void		_BoxQuery		( const Fbox& B, const Fmatrix& M, u32 flags);
	xr_vector<CCF_OBB>&	_GetElements	() { return models;	}
};

class ENGINE_API	CCF_Rigid : public ICollisionForm
{
private:
	Fbox			base_box;
	xr_vector<CCF_OBB>	models;

	u32				dwFrame;		// The model itself
	u32				dwFrameTL;		// Top level

	void			BuildState		();
	void			BuildTopLevel	();
	void			UpdateModel		(CCF_OBB& m, Fbox& box);
public:
					CCF_Rigid		( CObject* _owner );

	virtual BOOL		_RayQuery		( const collide::ray_defs& Q, collide::rq_results& R);
	virtual void		_BoxQuery		( const Fbox& B, const Fmatrix& M, u32 flags);
	xr_vector<CCF_OBB>&	_GetElements	() { return models;	}
};

class ENGINE_API	CCF_EventBox : public ICollisionForm
{
private:
	Fplane			Planes[6];
public:
					CCF_EventBox	( CObject* _owner );

	virtual BOOL	_RayQuery		( const collide::ray_defs& Q, collide::rq_results& R);
	virtual void	_BoxQuery		( const Fbox& B, const Fmatrix& M, u32 flags);

	BOOL			Contact			( CObject* O );
};

class ENGINE_API	CCF_Shape	: public ICollisionForm
{
public:
	union shape_data
	{
		Fsphere		sphere;
		struct{
			Fmatrix	box;
			Fmatrix	ibox;
		};
	};
	struct shape_def
	{
		int			type;
		shape_data	data;
	};
	xr_vector<shape_def>	shapes;
public:
					CCF_Shape		( CObject* _owner );

	virtual BOOL	_RayQuery		( const collide::ray_defs& Q, collide::rq_results& R);
	virtual void	_BoxQuery		( const Fbox& B, const Fmatrix& M, u32 flags);

	void			add_sphere		( Fsphere& S	);
	void			add_box			( Fmatrix& B	);
	void			ComputeBounds	( );
	BOOL			Contact			( CObject* O	);
	xr_vector<shape_def>& Shapes	(){return shapes;}
};

#endif //__XR_COLLIDE_FORM_H__
