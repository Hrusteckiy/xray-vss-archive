#include "stdafx.h"
#pragma hdrstop

#include "ESceneAIMapControls.h"
#include "ESceneAIMapTools.h"
#include "ui_main.h"
#include "FrameAIMap.h"
#include "scene.h"
#include "ui_leveltools.h"

//------------------------------------------------------------------------------
// Node Add
//------------------------------------------------------------------------------
TUI_ControlAIMapNodeAdd::TUI_ControlAIMapNodeAdd(int st, int act, ESceneCustomMTools* parent):TUI_CustomControl(st,act,parent){
}

bool __fastcall TUI_ControlAIMapNodeAdd::Start(TShiftState Shift)
{
	append_nodes = 0;
	Fvector p;
    ESceneAIMapTools* S 	= (ESceneAIMapTools*)parent_tool;
    if (S->PickObjects(p,UI->m_CurrentRStart,UI->m_CurrentRNorm,UI->ZFar())){
    	S->SelectObjects	(false);
	    append_nodes		= S->AddNode(p,((TfraAIMap*)S->pFrame)->ebIgnoreConstraints->Down,((TfraAIMap*)S->pFrame)->ebAutoLink->Down,S->m_BrushSize);
		if (!Shift.Contains(ssAlt)){ 
		    if (append_nodes) Scene->UndoSave();
        	ResetActionToSelect();
            return false;
        }else return true;
    }
    return false;
}
void TUI_ControlAIMapNodeAdd::Move(TShiftState _Shift)
{
	Fvector p;
    ESceneAIMapTools* S 	= (ESceneAIMapTools*)parent_tool;
    if (S->PickObjects(p,UI->m_CurrentRStart,UI->m_CurrentRNorm,UI->ZFar())){
	    append_nodes+=S->AddNode(p,((TfraAIMap*)S->pFrame)->ebIgnoreConstraints->Down,((TfraAIMap*)S->pFrame)->ebAutoLink->Down,S->m_BrushSize);
    }
}
bool TUI_ControlAIMapNodeAdd::End(TShiftState _Shift)
{
	if (!_Shift.Contains(ssAlt)) ResetActionToSelect();
    if (append_nodes) Scene->UndoSave();
	return true;
}


//------------------------------------------------------------------------------------
// Node Move
//------------------------------------------------------------------------------------
TUI_ControlAIMapNodeMove::TUI_ControlAIMapNodeMove(int st, int act, ESceneCustomMTools* parent):TUI_CustomControl(st,act,parent)
{
}
bool TUI_ControlAIMapNodeMove::Start(TShiftState Shift)
{
    if(parent_tool->SelectionCount(true)==0) return false;

    if (etAxisY==Tools->GetAxis()){
        m_MovingXVector.set(0,0,0);
        m_MovingYVector.set(0,1,0);
    }else{
        m_MovingXVector.set(0,0,0);
        m_MovingYVector.set(0,0,0);
    }
    m_MovingReminder.set(0,0,0);
    return true;
}

void __fastcall TUI_ControlAIMapNodeMove::Move(TShiftState _Shift)
{
	Fvector amount;
	if (DefaultMovingProcess(_Shift,amount)){
       	AINodeVec& lst = ((ESceneAIMapTools*)parent_tool)->Nodes();
        for(AINodeIt _F = lst.begin();_F!=lst.end();_F++)
            if((*_F)->flags.is(SAINode::flSelected)){ 
            	(*_F)->Pos.add(amount);
            	(*_F)->Plane.build((*_F)->Pos,(*_F)->Plane.n);
            }
    }
}

bool __fastcall TUI_ControlAIMapNodeMove::End(TShiftState _Shift)
{
	return MovingEnd(_Shift);
}

//------------------------------------------------------------------------------------
// Rotate Node
//------------------------------------------------------------------------------------
TUI_ControlAIMapNodeRotate::TUI_ControlAIMapNodeRotate(int st, int act, ESceneCustomMTools* parent):TUI_CustomControl(st,act,parent)
{
}                                           
bool __fastcall TUI_ControlAIMapNodeRotate::Start(TShiftState Shift)
{
    if(parent_tool->SelectionCount(true)==0) return false;

    m_RotateVector.set(0,0,0);
    if (etAxisX==Tools->GetAxis()) m_RotateVector.set(1,0,0);
    else if (etAxisY==Tools->GetAxis()) m_RotateVector.set(0,0,0);
    else if (etAxisZ==Tools->GetAxis()) m_RotateVector.set(0,0,1);
	m_fRotateSnapAngle = 0;
    return true;
}

void __fastcall TUI_ControlAIMapNodeRotate::Move(TShiftState _Shift)
{
    if (_Shift.Contains(ssLeft)){
        float amount = -UI->m_DeltaCpH.x * UI->m_MouseSR;

        if( Tools->GetSettings(etfASnap) ) CHECK_SNAP(m_fRotateSnapAngle,amount,Tools->m_RotateSnapAngle);

        Fmatrix R;
        if 	(fis_zero(m_RotateVector.x)) 	R.rotateZ(amount);
        else								R.rotateX(amount);
        
       	AINodeVec& lst = ((ESceneAIMapTools*)parent_tool)->Nodes();
        for(AINodeIt _F = lst.begin();_F!=lst.end();_F++)
            if((*_F)->flags.is(SAINode::flSelected)){
            	R.transform_dir((*_F)->Plane.n);
            	(*_F)->Plane.build((*_F)->Pos,(*_F)->Plane.n);
            }
    }
}
bool __fastcall TUI_ControlAIMapNodeRotate::End(TShiftState _Shift)
{
	return RotateEnd(_Shift);
}

