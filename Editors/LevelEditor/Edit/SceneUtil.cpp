//----------------------------------------------------
// file: SceneUtil.cpp
//----------------------------------------------------

#include "stdafx.h"
#pragma hdrstop

#include "Log.h"
#include "Scene.h"
#include "ELight.h"
#include "SceneObject.h"
#include "ui_leveltools.h"
#include "PropertiesListTypes.h"
#include "Render.h"

//----------------------------------------------------
CCustomObject* EScene::FindObjectByName( LPCSTR name, EObjClass classfilter )
{
	CCustomObject* object = 0;
    if (classfilter==OBJCLASS_DUMMY){
        SceneToolsMapPairIt _I = m_SceneTools.begin();
        SceneToolsMapPairIt _E = m_SceneTools.end();
        for (; _I!=_E; _I++){
            ESceneCustomOTools* mt = dynamic_cast<ESceneCustomOTools*>(_I->second);
            if (mt&&(0!=(object=mt->FindObjectByName(name)))) return object;
        }
    }else{
        ESceneCustomOTools* mt = GetOTools(classfilter); VERIFY(mt);
        if (mt&&(0!=(object=mt->FindObjectByName(name)))) return object;
    }
    return object;
}

CCustomObject* EScene::FindObjectByName( LPCSTR name, CCustomObject* pass_object )
{
	CCustomObject* object = 0;
    SceneToolsMapPairIt _I = m_SceneTools.begin();
    SceneToolsMapPairIt _E = m_SceneTools.end();
    for (; _I!=_E; _I++){
        ESceneCustomOTools* mt = dynamic_cast<ESceneCustomOTools*>(_I->second);
        if (mt&&(0!=(object=mt->FindObjectByName(name,pass_object)))) return object;
    }
    return 0;
}

bool EScene::FindDuplicateName()
{
// find duplicate name
    SceneToolsMapPairIt _I = m_SceneTools.begin();
    SceneToolsMapPairIt _E = m_SceneTools.end();
    for (; _I!=_E; _I++){
        ESceneCustomOTools* mt = dynamic_cast<ESceneCustomOTools*>(_I->second);
        if (mt){
        	ObjectList& lst = mt->GetObjects(); 
            for(ObjectIt _F = lst.begin();_F!=lst.end();_F++)
                if (FindObjectByName((*_F)->Name, *_F)){
                    ELog.DlgMsg(mtError,"Duplicate object name already exists: '%s'",(*_F)->Name);
                    return true;
                }
        }
    }
    return false;
}

void EScene::GenObjectName( EObjClass cls_id, char *buffer, const char* pref )
{
	ESceneCustomOTools* ot = GetOTools(cls_id); VERIFY(ot);
/*
    // test exist name
    if (pref&&pref[0]){
	    strcpy			(buffer,pref);
    	if (!FindObjectByName(buffer,0)) return;
    }
    // genarate new name
    m_LastAvailObject 	= 0;
    string128 prefix; prefix[0]=0;
    if (pref&&pref[0]){
    	strcpy( prefix, pref );
        for (int i=strlen(prefix)-1; i>=0; i--) if (isdigit(prefix[i])) prefix[i]=0; else break;
		sprintf( buffer, "%s%04d", prefix, m_LastAvailObject++);
    }else        sprintf( buffer, "%s%04d", ot->ClassName(), m_LastAvailObject++ );
    while (FindObjectByName( buffer, 0 )){
        if (pref&&pref[0])	sprintf( buffer, "%s%04d", prefix, m_LastAvailObject++ );
        else   	  			sprintf( buffer, "%s%04d", ot->ClassName(), m_LastAvailObject++ );
	}
*/
    AnsiString result	= FHelper.GenerateName(pref&&pref[0]?pref:ot->ClassName(),4,FindObjectByNameCB);
    strcpy				(buffer,result.c_str());
}
//------------------------------------------------------------------------------


