#include "stdafx.h"
#include "xrMU_Model.h"

const u32	max_tile	= 16;
const s32	quant		= 32768/max_tile;

s16 QC	(float v)
{
	int t		=	iFloor(v*float(quant)); clamp(t,-32768,32767);
	return	s16	(t);
}

D3DVERTEXELEMENT9	decl[] = 
{
	{0, 0,  D3DDECLTYPE_FLOAT3,		D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_POSITION,	0 },
	{0, 12, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_NORMAL,	0 },
	{0, 16, D3DDECLTYPE_D3DCOLOR,	D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_COLOR,		0 },
	{0, 20, D3DDECLTYPE_SHORT4,		D3DDECLMETHOD_DEFAULT, 	D3DDECLUSAGE_TEXCOORD,	0 },
	D3DDECL_END()
};

void	xrMU_Model::export_geometry		()
{
	// Declarator
	VDeclarator			D;
	D.set				(decl);

	// RT-check, BOX, low-point, frac-size
	R_ASSERT		(m_vertices.size()==color.size());
	Fbox			BB; 
	BB.invalidate	();
	for (v_vertices_it vit=m_vertices.begin(); vit!=m_vertices.end(); vit++)
		BB.modify	((*vit)->P);

	Fvector			frac_low;
	float			frac_Ysize;
	BB.getcenter	(frac_low);		frac_low.y	= BB.min.y;
	frac_Ysize		= BB.max.y - BB.min.y;

	// Begin building
	for (v_subdivs_it it=m_subdivs.begin(); it!=m_subdivs.end(); it++)
	{
		// Vertices
		{
			g_VB.Begin		(D);

			vecOGF_V&	verts	= it->ogf->vertices;
			for (u32 v_it=0; v_it<verts.size(); v_it++)
			{
				OGF_Vertex&		oV	= verts[v_it];

				// Position
				g_VB.Add	(&oV.P,3*sizeof(float));

				// Normal
				Fvector N	= oV.N;
				N.add		(1.f);
				N.mul		(.5f*255.f);
				s32 nx		= iFloor(N.x);	clamp(nx,0,255);
				s32 ny		= iFloor(N.y);	clamp(ny,0,255);
				s32 nz		= iFloor(N.z);	clamp(nz,0,255);
				u32	uN		= color_rgba(nx,ny,nz,0);
				g_VB.Add	(&uN,4);

				// Color
				g_VB.Add	(&oV.Color,4);

				// TC
				s16	tu,tv,frac,dummy;
				tu			= QC(oV.UV.begin()->x);
				tv			= QC(oV.UV.begin()->y);
				g_VB.Add	(&tu,2);
				g_VB.Add	(&tv,2);

				// frac
				float	f1	= (oV.P.y - frac_low.y)		/frac_Ysize;
				float	f2	= oV.P.distance_to(frac_low)/frac_Ysize;
				frac		= QC((f1+f2)/2.f);
				dummy		= 0;
				g_VB.Add	(&frac,	2);
				g_VB.Add	(&dummy,2);
			}

			g_VB.End		(&it->vb_id,&it->vb_start);
		}

		// Indices
		g_IB.Register	(LPWORD(it->ogf->faces.begin()),LPWORD(it->ogf->faces.end()),&it->ib_id,&it->ib_start);
	}
}
