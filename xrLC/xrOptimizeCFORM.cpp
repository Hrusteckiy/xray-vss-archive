#include "stdafx.h"
#include "build.h"
#include "cl_collector.h"

#define FLT_MIN	flt_min
#define FLT_MAX	flt_max

// OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/Types/TriMesh_ArrayKernelT.hh>
//#include <OpenMesh/Core/Utils/vector_cast.hh>
//#include <OpenMesh/Tools/Utils/getopt.h>
//#include <OpenMesh/Tools/Utils/Timer.hh>
#include <OpenMesh/Tools/Decimater/DecimaterT.hh>
#include <OpenMesh/Tools/Decimater/ModNormalFlippingT.hh>
#include <OpenMesh/Tools/Decimater/ModQuadricT.hh>
#include <OpenMesh/Tools/Decimater/ModProgMeshT.hh>
#include <OpenMesh/Tools/Decimater/ModIndependentSetsT.hh>
#include <OpenMesh/Tools/Decimater/ModRoundnessT.hh>

using namespace		CDB;
using namespace		OpenMesh;

//t-defs
struct		MyTraits : public OpenMesh::DefaultTraits	
{
	FaceTraits	{
	private:
		u32		props_;
	public:
		FaceT() : props_(0)					{ }

		const	u32 props() const			{ return props_;	}
		void	set_props(const u32 _p)		{ props_ = _p;		}
	};
};
typedef		TriMesh_ArrayKernelT	< MyTraits >			_mesh;			// Mesh type
typedef		Decimater::DecimaterT	< _mesh >				_decimater;		// Decimater type
typedef		Decimater::ModQuadricT	< _decimater >::Handle	_HModQuadric;	// Decimation Module Handle type

//main
void SimplifyCFORM		(CDB::CollectorPacked& CL)
{
	Phase		("CFORM: simplification...");
	//Status		("Building base mesh : vertices...");

	_mesh        mesh;						// a mesh object
	_decimater   decimater(mesh);			// a decimater object, connected to a mesh
	_HModQuadric hModQuadric;				// use a quadric module
	decimater.add( hModQuadric );			// register module at the decimater
	decimater.module(hModQuadric).set_max_err(0.00001,false);	// error-limit

	// Initializing mesh
	Status		("Building base mesh : vertices[%d]...",CL.getVS());
	xr_vector<_mesh::VertexHandle>	vhandles;
	vhandles.resize	(CL.getVS());
	for (u32 v_it=0; v_it<CL.getVS(); v_it++)		{
		Fvector3&	v	= CL.getV()[v_it];
		vhandles[v_it]	= mesh.add_vertex	(_mesh::Point(v.x,v.y,v.z));
	}
	Status		("Building base mesh : faces[%d]...",CL.getTS());
	std::vector <_mesh::VertexHandle>	fhandles;
	for (u32 f_it=0; f_it<CL.getTS(); f_it++)		{
		CDB::TRI&	f		= CL.getT()[f_it];
		fhandles.clear		();
		fhandles.push_back	(vhandles[f.IDvert(0)]);
		fhandles.push_back	(vhandles[f.IDvert(1)]);
		fhandles.push_back	(vhandles[f.IDvert(2)]);
		mesh.face			(mesh.add_face(fhandles)).set_props	(f.dummy);
	}
	vhandles.clear_and_free		();
	Status		("Building base mesh : normals...");
	mesh.request_vertex_normals	();
	mesh.update_vertex_normals	();
	CL.

	// Decimate
	Status		("Reconstructing mesh-topology...");
	decimater.initialize	();      // let the decimater initialize the mesh and the modules

	int		nv_before		= int	(mesh.n_vertices());
	int		nc				= decimater.decimate	(nv_before);	// do decimation, as large, as possible
							mesh.garbage_collection	();
	int		nv_after		= int	(mesh.n_vertices());
	clMsg					("was[%d], now[%d] => %f %% left",nv_before,nv_after, 100.f*float(nv_after)/float(nv_before) );

	// Decimate
	Status		("Refactoring CFORM...");
}
