///////////////////////////////////////////////////////////////
// Silencer.cpp
// Silencer - ������� ������ ��������� 
///////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "silencer.h"
#include "PhysicsShell.h"

CSilencer::CSilencer()
{
	m_bForPistol = false;
}

CSilencer::~CSilencer() 
{
}

BOOL CSilencer::net_Spawn(CSE_Abstract* DC) 
{
	return		(inherited::net_Spawn(DC));
}

void CSilencer::Load(LPCSTR section) 
{
	inherited::Load(section);
}

void CSilencer::net_Destroy() 
{
	inherited::net_Destroy();
}

void CSilencer::UpdateCL() 
{
	inherited::UpdateCL();
}

void CSilencer::OnH_A_Chield() 
{
	inherited::OnH_A_Chield		();
}

void CSilencer::OnH_B_Independent() 
{
	inherited::OnH_B_Independent();
}

void CSilencer::renderable_Render() 
{
	inherited::renderable_Render();
}