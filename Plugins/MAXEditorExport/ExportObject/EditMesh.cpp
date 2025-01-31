//----------------------------------------------------
// file: StaticMesh.cpp
//----------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

#include "EditMesh.h"
#include "EditObject.h"

CEditableMesh::~CEditableMesh(){
	Clear();
}

void CEditableMesh::Construct(){
	m_Box.set	(0,0,0,0,0,0);
    m_Visible	= 1;
    m_Locked	= 0;
    m_Name[0]	= 0;
    m_LoadState	= 0;
#ifdef _EDITOR
    m_CFModel	= 0;
#endif
}

void CEditableMesh::Clear(){
#ifdef _EDITOR
	ClearRenderBuffers	();
#endif
	m_Points.clear 		();
    m_Adjs.clear		();
	m_Faces.clear		();
    m_VMaps.clear		();
    m_SurfFaces.clear	();
    m_VMRefs.clear		();
    UnloadCForm			();
    UnloadFNormals		();
    UnloadPNormals		();
}

void CEditableMesh::UnloadCForm     (){
#ifdef _EDITOR
	_DELETE(m_CFModel);
#endif
    m_LoadState &=~ EMESH_LS_CF_MODEL;
}

void CEditableMesh::UnloadFNormals   (){
	m_FNormals.clear	();
    m_LoadState &=~ EMESH_LS_FNORMALS;
}

void CEditableMesh::UnloadPNormals   (){
	m_PNormals.clear	();
    m_LoadState &=~ EMESH_LS_PNORMALS;
}

void CEditableMesh::UnloadSVertices	 (){
	m_SVertices.clear	();
    m_LoadState &=~ EMESH_LS_SVERTICES;
}

void CEditableMesh::RecomputeBBox(){
	if( m_Points.empty() ){
		m_Box.set(0,0,0, 0,0,0);
		return;
    }
	m_Box.set( m_Points[0], m_Points[0] );
	for(FvectorIt pt=m_Points.begin()+1; pt!=m_Points.end(); pt++)
		m_Box.modify(*pt);
}

void CEditableMesh::GenerateFNormals(){
    m_FNormals.resize	(m_Faces.size());
    // face normals
	FaceIt 		f_it	= m_Faces.begin();
	FvectorIt 	fn_it	= m_FNormals.begin();
    for (f_it; f_it!=m_Faces.end(); f_it++, fn_it++ ){
        fn_it->mknormal(	m_Points[f_it->pv[0].pindex],
                			m_Points[f_it->pv[1].pindex],
                            m_Points[f_it->pv[2].pindex]);
    }
/*
    for (int i=0; i<m_Faces.size(); i++){
    	st_Face& F = m_Faces[i];
        Fvector& N = m_FNormals[i];
        N.mknormal(	m_Points[F.pv[0].pindex],
        			m_Points[F.pv[1].pindex],
                    m_Points[F.pv[2].pindex]);
    }
*/
    m_LoadState |= EMESH_LS_FNORMALS;
}

void CEditableMesh::GeneratePNormals(){
	if (!(m_LoadState&EMESH_LS_FNORMALS)) GenerateFNormals();
    m_PNormals.resize	(m_Faces.size()*3);
	// vertex normals
    float m_fSoftAngle = cosf(deg2rad(60));
    for (DWORD f_i=0; f_i<m_Faces.size(); f_i++ ){
		Fvector& FN = m_FNormals[f_i];
        for (int k=0; k<3; k++){
            Fvector& N = m_PNormals[f_i*3+k];
            N.set(0,0,0);
            INTVec& a_lst=m_Adjs[m_Faces[f_i].pv[k].pindex];
            VERIFY(a_lst.size());
            for (INTIt i_it=a_lst.begin(); i_it!=a_lst.end(); i_it++){
            	Fvector& TN = m_FNormals[*i_it];
	            float cosa = TN.dotproduct(FN);
    	        if (cosa<m_fSoftAngle) continue;
                N.add(TN);
            }
            N.normalize_safe();
        }
    }
    m_LoadState |= EMESH_LS_PNORMALS;
}

#ifdef _EDITOR
#include "Bone.h"
void CEditableMesh::GenerateSVertices(){
	if (!m_Parent->IsSkeleton()) return;

    m_Parent->ResetAnimation();
    m_Parent->CalculateAnimation(true);
	m_SVertices.resize(m_Points.size());

    int kk=0;
    for (DWORD u=0; u<m_VMaps.size(); u++){
    	st_VMap& M = m_VMaps[u];
        if (M.type==vmtWeight) kk++;
    }


    for (DWORD i=0; i<m_Points.size(); i++){
    	Fvector&  P =m_Points[i];
    	st_SVert& SV=m_SVertices[i];
    	INTVec& a_lst = m_Adjs[i];
        VERIFY2(a_lst.size(),"Karma pidaras!!! Uberi degenerats.");
		bool bRes = false;
        for (DWORD a=0; a<m_Adjs[i].size(); a++){
            st_Face& F = m_Faces[m_Adjs[i][a]];
            for (int k=0; k<3; k++){
                st_FaceVert& fv = F.pv[k];
                if (fv.pindex==int(i)){
                    VMapPtSVec& vmpt_lst = m_VMRefs[fv.vmref];
                    for (VMapPtIt vmpt_it=vmpt_lst.begin(); vmpt_it!=vmpt_lst.end(); vmpt_it++){
                        st_VMap& VM = m_VMaps[vmpt_it->vmap_index];
                        if (VM.type==vmtWeight){
							//float& w = VM.getW(vmpt_it->index);
                            SV.bone 	= m_Parent->GetBoneIndexByWMap(VM.name);
                            VERIFY2(SV.bone>-1,"Can't find weight map");
                            CBone* B 	= m_Parent->m_Bones[SV.bone];
                            B->LITransform().transform_tiny(SV.offs,P);
                            bRes=true;
                            break;
                        }
                    }
                    break;
                }
            }
            if (bRes) break;
    	}
		if (!bRes) THROW2("Can't find weight for skel vertex.");
    }

    m_LoadState |= EMESH_LS_SVERTICES;
}
#endif

CSurface*	CEditableMesh::GetSurfaceByFaceID(int fid){
    for (SurfFacesPairIt sp_it=m_SurfFaces.begin(); sp_it!=m_SurfFaces.end(); sp_it++){
		INTVec& face_lst = sp_it->second;
        if (std::find(face_lst.begin(),face_lst.end(),fid)!=face_lst.end()) return sp_it->first;
	}
    return 0;
}

CSurface* CEditableMesh::GetFaceTC(int fid, const Fvector2* tc[3]){
    CSurface* surf = GetSurfaceByFaceID(fid);
    VERIFY(surf);

	st_Face& F = m_Faces[fid];
    for (int k=0; k<3; k++){
	    st_VMapPt& vmr = m_VMRefs[F.pv[k].vmref][0];
    	tc[k] = &(m_VMaps[vmr.vmap_index].getUV(vmr.index));
    }
    return surf;
}

CSurface* CEditableMesh::GetFacePT(int fid, const Fvector* pt[3]){
    CSurface* surf = GetSurfaceByFaceID(fid);
    VERIFY(surf);

	st_Face& F = m_Faces[fid];
    for (int k=0; k<3; k++)
    	pt[k] = &m_Points[F.pv[k].pindex];
    return surf;
}

int CEditableMesh::GetFaceCount(bool bMatch2Sided){
	int f_cnt = 0;
    for (SurfFacesPairIt sp_it=m_SurfFaces.begin(); sp_it!=m_SurfFaces.end(); sp_it++){
    	if (bMatch2Sided){
	    	if (sp_it->first->_2Sided())f_cnt+=sp_it->second.size()*2;
    	    else						f_cnt+=sp_it->second.size();
        }else{
        	f_cnt+=sp_it->second.size();
        }
	}
    return f_cnt;
}

int CEditableMesh::GetSurfFaceCount(CSurface* surf, bool bMatch2Sided){
	SurfFacesPairIt sp_it = m_SurfFaces.find(surf);
    if (sp_it==m_SurfFaces.end()) return 0;
	int f_cnt = sp_it->second.size();
    if (bMatch2Sided&&sp_it->first->_2Sided()) f_cnt*=2;
    return f_cnt;
}

void CEditableMesh::DumpAdjacency(){
	ELog.Msg(mtInformation,"Adjacency dump.");
	ELog.Msg(mtInformation,"------------------------------------------------------------------------");
    for (DWORD i=0; i<m_Adjs.size(); i++){
        INTVec& a_lst	= m_Adjs[i];
        AnsiString s; s = "Point "; s+=i; s+=":";
        AnsiString s1; 
        for (DWORD j=0; j<a_lst.size(); j++){ s1=a_lst[j]; s+=" "+s1; }
		ELog.Msg(mtInformation,s.c_str());
    }
}

