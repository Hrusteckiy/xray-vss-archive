//----------------------------------------------------
// file: EditMeshModify.cpp
//----------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

#include "EditMesh.h"
#include "EditObject.h"
//----------------------------------------------------
void CEditableMesh::Transform(const Fmatrix& parent)
{
	// transform position
	for(FvectorIt pt=m_Points.begin(); pt!=m_Points.end(); pt++)
		parent.transform_tiny(*pt);
    // RecomputeBBox
	RecomputeBBox();
    // update normals & cform
    UnloadFNormals();
    UnloadPNormals();
    UnloadCForm();
}
//----------------------------------------------------

int CEditableMesh::FindSimilarUV(st_VMap* vmap, Fvector2& _uv)
{
	int sz			= vmap->size();
	for (int k=0; k<sz; k++){
		Fvector2& uv = vmap->getUV(k);
		if (uv.similar(_uv)) 
			return k;
	}
	return -1;
}

int CEditableMesh::FindSimilarWeight(st_VMap* vmap, float _w)
{
	int sz			= vmap->size();
	for (int k=0; k<sz; k++){
		float w		= vmap->getW(k);
		if (fsimilar(w,_w)) return k;
	}
	return -1;
}

void CEditableMesh::RebuildVMaps()
{
	Log			("Rebuild VMaps...");
	IntVec		m_PointVMap;
	m_PointVMap.resize(m_Points.size(),-1);
	VMapVec		nVMaps;
	VMRefsVec	nVMRefs = m_VMRefs;
	for (FaceIt f_it=m_Faces.begin(); f_it!=m_Faces.end(); f_it++){
		st_Face& F=*f_it;
		for (int k=0; k<3; k++){
			VMapPtSVec& pts=m_VMRefs[F.pv[k].vmref];
			VMapPtIt n_pt_it=nVMRefs[F.pv[k].vmref].begin();
			for (VMapPtIt pt_it=pts.begin(); pt_it!=pts.end(); pt_it++,n_pt_it++){
				st_VMap* vmap=m_VMaps[pt_it->vmap_index];
				switch (vmap->type){
				case vmtUV:{
					int& pm=m_PointVMap[F.pv[k].pindex];
					if (-1==pm){ // point map
						pm=F.pv[k].vmref;
						int vm_idx=FindVMapByName(nVMaps,vmap->name.c_str(),vmap->type,false);
						if (-1==vm_idx){
							nVMaps.push_back(xr_new<st_VMap>(vmap->name.c_str(),vmap->type,false));
							vm_idx=nVMaps.size()-1;
						}
						st_VMap* nVMap=nVMaps[vm_idx];
/*
						int uv_idx = FindSimilarUV(nVMap,vmap->getUV(pt_it->index));
						if (uv_idx==-1){
							uv_idx	= nVMap->size();
							nVMap->appendUV(vmap->getUV(pt_it->index));
							nVMap->appendVI(F.pv[k].pindex);
						}
*/
						nVMap->appendUV(vmap->getUV(pt_it->index));
						nVMap->appendVI(F.pv[k].pindex);
						n_pt_it->index = nVMap->size()-1;
						n_pt_it->vmap_index=vm_idx;
					}
					else{ // poly map
						int vm_idx=FindVMapByName(nVMaps,vmap->name.c_str(),vmap->type,true);
						if (-1==vm_idx){
							nVMaps.push_back(xr_new<st_VMap>(vmap->name.c_str(),vmap->type,true));
							vm_idx=nVMaps.size()-1;
						}
						st_VMap* nVMapPM=nVMaps[vm_idx];
/*
						int uv_idx = FindSimilarUV(nVMapPM,vmap->getUV(pt_it->index));
						if (uv_idx==-1){
							uv_idx	= nVMapPM->size();
							nVMapPM->appendUV(vmap->getUV(pt_it->index));
							nVMapPM->appendVI(F.pv[k].pindex);
							nVMapPM->appendPI(f_it-m_Faces.begin());
						}
						n_pt_it->index = uv_idx;
*/
						nVMapPM->appendUV(vmap->getUV(pt_it->index));
						nVMapPM->appendVI(F.pv[k].pindex);
						nVMapPM->appendPI(f_it-m_Faces.begin());
						n_pt_it->index = nVMapPM->size()-1;
						n_pt_it->vmap_index=vm_idx;
					}
				}break;
				case vmtWeight:{
					int vm_idx=FindVMapByName(nVMaps,vmap->name.c_str(),vmap->type,false);
					if (-1==vm_idx){
						nVMaps.push_back(xr_new<st_VMap>(vmap->name.c_str(),vmap->type,false));
						vm_idx=nVMaps.size()-1;
					}
					st_VMap* nWMap=nVMaps[vm_idx];
					nWMap->appendW(vmap->getW(pt_it->index));
					nWMap->appendVI(F.pv[k].pindex);
					n_pt_it->index = nWMap->size()-1;
					n_pt_it->vmap_index=vm_idx;
				}break;
				}
			}
		}
	}
	for (VMapIt vm_it=m_VMaps.begin(); vm_it!=m_VMaps.end(); vm_it++)
		xr_delete(*vm_it);

	m_VMaps.clear();
	m_VMaps=nVMaps;
	m_VMRefs.clear();
	m_VMRefs = nVMRefs;
}

bool CEditableMesh::UpdateAdjacency()
{
	if (m_Faces.empty()) return false;
    Log				(".. Update adjacency");
    m_Adjs.clear	();
    m_Adjs.resize	(m_Points.size());
	for (FaceIt f_it=m_Faces.begin(); f_it!=m_Faces.end(); f_it++)
		for (int k=0; k<3; k++) m_Adjs[f_it->pv[k].pindex].push_back(f_it-m_Faces.begin());
	return true;
}

#define MX 25
#define MY 15
#define MZ 25
static Fvector		VMmin, VMscale;
static U32Vec		VM[MX+1][MY+1][MZ+1];
static Fvector		VMeps;

static FvectorVec	m_NewPoints;
bool CEditableMesh::OptimizeFace(st_Face& face){
	Fvector points[3];
	int mface[3];
	int k;

	for (k=0; k<3; k++){
    	points[k].set(m_Points[face.pv[k].pindex]);
		mface[k] = -1;
    }

	// get similar vert idx list
	for (k=0; k<3; k++){
		U32Vec* vl;
		int ix,iy,iz;
		ix = iFloor(float(points[k].x-VMmin.x)/VMscale.x*MX);
		iy = iFloor(float(points[k].y-VMmin.y)/VMscale.y*MY);
		iz = iFloor(float(points[k].z-VMmin.z)/VMscale.z*MZ);
		vl = &(VM[ix][iy][iz]);
		for(U32It it=vl->begin();it!=vl->end(); it++){
			FvectorIt v = m_NewPoints.begin()+(*it);
            if( v->similar(points[k],EPS) )
                mface[k] = *it;
		}
	}
	for(k=0; k<3; k++ ){
		if( mface[k] == -1 ){
			mface[k] = m_NewPoints.size();
			m_NewPoints.push_back( points[k] );
			int ix,iy,iz;
			ix = iFloor(float(points[k].x-VMmin.x)/VMscale.x*MX);
			iy = iFloor(float(points[k].y-VMmin.y)/VMscale.y*MY);
			iz = iFloor(float(points[k].z-VMmin.z)/VMscale.z*MZ);
			VM[ix][iy][iz].push_back(mface[k]);
			int ixE,iyE,izE;
			ixE = iFloor(float(points[k].x+VMeps.x-VMmin.x)/VMscale.x*MX);
			iyE = iFloor(float(points[k].y+VMeps.y-VMmin.y)/VMscale.y*MY);
			izE = iFloor(float(points[k].z+VMeps.z-VMmin.z)/VMscale.z*MZ);
			if (ixE!=ix)
				VM[ixE][iy][iz].push_back(mface[k]);
			if (iyE!=iy)
				VM[ix][iyE][iz].push_back(mface[k]);
			if (izE!=iz)
				VM[ix][iy][izE].push_back(mface[k]);
			if ((ixE!=ix)&&(iyE!=iy))
				VM[ixE][iyE][iz].push_back(mface[k]);
			if ((ixE!=ix)&&(izE!=iz))
				VM[ixE][iy][izE].push_back(mface[k]);
			if ((iyE!=iy)&&(izE!=iz))
				VM[ix][iyE][izE].push_back(mface[k]);
			if ((ixE!=ix)&&(iyE!=iy)&&(izE!=iz))
				VM[ixE][iyE][izE].push_back(mface[k]);
		}
	}

	if ((mface[0]==mface[1])||(mface[1]==mface[2])||(mface[0]==mface[2])){
		Msg("!Optimize: Invalid face found. Removed.");
        return false;
	}else{
    	face.pv[0].pindex = mface[0];
    	face.pv[1].pindex = mface[1];
    	face.pv[2].pindex = mface[2];
        return true;
	}
}

void CEditableMesh::Optimize(BOOL NoOpt)
{
	if (!NoOpt){
        UnloadCForm     	();
        UnloadFNormals   	();
        UnloadPNormals   	();
       	UnloadSVertices  	();
    	
		// clear static data
		for (int x=0; x<MX+1; x++)
			for (int y=0; y<MY+1; y++)
    			for (int z=0; z<MZ+1; z++)
            		VM[x][y][z].clear();
		VMscale.set(m_Box.max.x-m_Box.min.x+EPS_S, m_Box.max.y-m_Box.min.y+EPS_S, m_Box.max.z-m_Box.min.z+EPS_S);
		VMmin.set(m_Box.min.x, m_Box.min.y, m_Box.min.z);

		VMeps.set(VMscale.x/MX/2,VMscale.y/MY/2,VMscale.z/MZ/2);
		VMeps.x = (VMeps.x<EPS_L)?VMeps.x:EPS_L;
		VMeps.y = (VMeps.y<EPS_L)?VMeps.y:EPS_L;
		VMeps.z = (VMeps.z<EPS_L)?VMeps.z:EPS_L;

		m_NewPoints.clear();
		m_NewPoints.reserve(m_Points.size());

		Msg("Optimize...");
		Msg(".. Merge points");

		IntVec mark_for_del;
		mark_for_del.clear();
		for (u32 k=0; k<m_Faces.size(); k++){
    		if (!OptimizeFace(m_Faces[k]))
				mark_for_del.push_back(k);
		}

		m_Points.clear();
		m_Points = m_NewPoints;
		if (mark_for_del.size()>0){
			std::sort	(mark_for_del.begin(),mark_for_del.end());
			std::reverse(mark_for_del.begin(),mark_for_del.end());
			// delete degenerate faces
			for (IntIt i_it=mark_for_del.begin(); i_it!=mark_for_del.end(); i_it++){
				m_Faces.erase	(m_Faces.begin()+(*i_it));
                m_SGs.erase		(m_SGs.begin()+(*i_it));
            }
			// delete degenerate faces refs
			for (IntIt m_d=mark_for_del.begin(); m_d!=mark_for_del.end(); m_d++){
				for (SurfFacesPairIt plp_it=m_SurfFaces.begin(); plp_it!=m_SurfFaces.end(); plp_it++){
					IntVec& 	pol_lst = plp_it->second;
					for (int k=0; k<int(pol_lst.size()); k++){
						int& f = pol_lst[k];
						if (f>*m_d){ f--;
						}else if (f==*m_d){
							pol_lst.erase(pol_lst.begin()+k);
							k--;
						}
					}
				}
			}
		}
	}
    UpdateAdjacency();
    VERIFY(m_SGs.size()==m_Faces.size());
}

