#include "stdafx.h"
#include "ai_monster_motion.h"
#include "ai_monster_jump.h"
#include "biting\\ai_biting.h"

CMotionManager::CMotionManager()
{
}

void CMotionManager::Init (CAI_Biting	*pM)
{
	pMonster				= pM;
	pJumping				= dynamic_cast<CJumping*>(pM);
	pVisual					= 0;

	prev_anim				= cur_anim	= eAnimStandIdle; 
	m_tAction				= ACT_STAND_IDLE;
	m_tpCurAnim				= 0;
	spec_params				= 0;

	time_start_stand		= 0;
	prev_action				= ACT_STAND_IDLE;

	should_play_die_anim	= true;			//���� ���� �� NetSpawn ������ ��������������� � true

	Seq_Init				();
}

// �������� ���������� ��������. �������� ���������� �� Monster::Load
void CMotionManager::AddAnim(EMotionAnim ma, LPCTSTR tn, int s_id, float speed, float r_speed, EPState p_s)
{
	SAnimItem new_item;

	new_item.target_name	= tn;
	new_item.spec_id		= s_id;
	new_item.speed.linear	= speed;
	new_item.speed.angular	= r_speed;
	new_item.pos_state		= p_s;

	m_tAnims.insert			(mk_pair(ma, new_item));
}

// �������� ��������. ���������� �������� �� Monster::NetSpawn 
void CMotionManager::LoadVisualData()
{
	for (ANIM_ITEM_MAP_IT item_it = m_tAnims.begin(); item_it != m_tAnims.end(); item_it++) {
		// ������� ������ ��������
		if (!item_it->second.pMotionVect.empty()) item_it->second.pMotionVect.clear();
		// �������� �����
		Load(*item_it->second.target_name, &item_it->second.pMotionVect);
	}
}

void CMotionManager::AddTransition_A2A(EMotionAnim from, EMotionAnim to, EMotionAnim trans, bool chain)
{
	STransition new_item;

	new_item.from.state_used	= false;
	new_item.from.anim			= from;

	new_item.target.state_used	= false;
	new_item.target.anim		= to;

	new_item.anim_transition	= trans;
	new_item.chain				= chain;

	m_tTransitions.push_back(new_item);
}


void CMotionManager::AddTransition_A2S(EMotionAnim from, EPState to, EMotionAnim trans, bool chain)
{
	STransition new_item;

	new_item.from.state_used	= false;
	new_item.from.anim			= from;

	new_item.target.state_used	= true;
	new_item.target.state		= to;

	new_item.anim_transition	= trans;
	new_item.chain				= chain;

	m_tTransitions.push_back(new_item);
}

void CMotionManager::AddTransition_S2A(EPState from, EMotionAnim to, EMotionAnim trans, bool chain)
{
	STransition new_item;

	new_item.from.state_used	= true;
	new_item.from.state			= from;

	new_item.target.state_used	= false;
	new_item.target.anim		= to;


	new_item.anim_transition	= trans;
	new_item.chain				= chain;

	m_tTransitions.push_back(new_item);
}

void CMotionManager::AddTransition_S2S(EPState from, EPState to, EMotionAnim trans, bool chain)
{
	STransition new_item;

	new_item.from.state_used	= true;
	new_item.from.state			= from;

	new_item.target.state_used	= true;
	new_item.target.state		= to;

	new_item.anim_transition	= trans;
	new_item.chain				= chain;

	m_tTransitions.push_back(new_item);
}

void CMotionManager::LinkAction(EAction act, EMotionAnim pmt_motion, EMotionAnim pmt_left, EMotionAnim pmt_right, float pmt_angle)
{
	SMotionItem new_item;

	new_item.anim				= pmt_motion;
	new_item.is_turn_params		= true;
	new_item.turn.anim_left		= pmt_left;
	new_item.turn.anim_right	= pmt_right;
	new_item.turn.min_angle		= pmt_angle;

	m_tMotions.insert	(mk_pair(act, new_item));
}

void CMotionManager::LinkAction(EAction act, EMotionAnim pmt_motion)
{
	SMotionItem new_item;

	new_item.anim				= pmt_motion;
	new_item.is_turn_params		= false;

	m_tMotions.insert	(mk_pair(act, new_item));
}

// �������� �������� �� ������ ������������ � pmt_name � ������ pMotionVect
void CMotionManager::Load(LPCTSTR pmt_name, ANIM_VECTOR	*pMotionVect)
{
	string256	S1, S2; 
	CMotionDef	*tpMotionDef;
	for (int i=0; ; i++) {
		if (0 != (tpMotionDef = PSkeletonAnimated(pVisual)->ID_Cycle_Safe(strconcat(S1,pmt_name,itoa(i,S2,10)))))  pMotionVect->push_back(tpMotionDef);
		else if (0 != (tpMotionDef = PSkeletonAnimated(pVisual)->ID_FX_Safe(strconcat(S1,pmt_name,itoa(i,S2,10))))) pMotionVect->push_back(tpMotionDef);
		else break;
	}
}

// ������������� ������� ��������, ������� ���������� ���������.
// ���������� false, ���� � ����� �������� ��� �������������
bool CMotionManager::PrepareAnimation()
{
	if (pJumping && pJumping->IsActive())  return pJumping->PrepareAnimation(&m_tpCurAnim);
	if (m_tpCurAnim != 0) return false;

	// �������� �� ����������� �������� ������
	if (!pMonster->g_Alive()) 
		if (should_play_die_anim) {
			should_play_die_anim = false;  // �������� �������� ������ ������ ���
			if (m_tAnims.find(eAnimDie) != m_tAnims.end()) cur_anim = eAnimDie;
			else return false;
		} else return false;

	// �������� ������� SAnimItem ��������������� cur_anim
	ANIM_ITEM_MAP_IT anim_it = m_tAnims.find(cur_anim);
	R_ASSERT(anim_it != m_tAnims.end());

	// ���������� ����������� ������
	int index;
	if (anim_it->second.spec_id != -1) index = anim_it->second.spec_id;
	else {
		R_ASSERT(!anim_it->second.pMotionVect.empty());
		index = ::Random.randI(anim_it->second.pMotionVect.size());
	}

	// ���������� ��������	
	m_tpCurAnim = anim_it->second.pMotionVect[index];

	// ���������� ��������� �����
	AA_SwitchAnimation(cur_anim, index);

	// todo: find out the reason quick animation changes
	Msg("--------------------------------------------------------------------");
	Msg("ANIM_SELECT: anim = [%s] index= [%i] Time = [%i]",*anim_it->second.target_name, index, pMonster->m_dwCurrentTime);

	return true;
}

// ��������� ���������� �� ������� �� �������� from � to
void CMotionManager::CheckTransition(EMotionAnim from, EMotionAnim to)
{
	// ����� ���������������� ��������
	bool		bActivated	= false;
	EMotionAnim cur_from = from; 
	EPState		state_from	= GetState(cur_from);
	EPState		state_to	= GetState(to);

	TRANSITION_ANIM_VECTOR_IT I = m_tTransitions.begin();
	bool bVectEmpty = m_tTransitions.empty();

	while (!bVectEmpty) {		// ���� � ����, ���� ������ ��������� �� ������

		bool from_is_good	= ((I->from.state_used) ? (I->from.state == state_from) : (I->from.anim == cur_from));
		bool target_is_good = ((I->target.state_used) ? (I->target.state == state_to) : (I->target.anim == to));

		if (from_is_good && target_is_good) {

			// ������� �������
			Seq_Add(I->anim_transition);
			bActivated	= true;	

			if (I->chain) {
				cur_from	= I->anim_transition;
				state_from	= GetState(cur_from);
				I = m_tTransitions.begin();			// ������ �������
				continue;
			} else break;
		}
		if ((++I) == m_tTransitions.end()) break;
	}

	if (bActivated) Seq_Switch();
}

// � ������������ �������� �������� m_tAction, ��������� �������������� �������� � ��������� ��������
// ����������� �� ������ ���.�������, ����� ���������� ���������
void CMotionManager::ProcessAction()
{
	if (pJumping && pJumping->IsActive()) return;

	if (!seq_playing) {

		// ������������� Action � Motion � �������� ����� ��������
		SMotionItem MI = m_tMotions[m_tAction];
		cur_anim = MI.anim;

		// ���������� target.yaw
		if (!pMonster->AI_Path.TravelPath.empty()) pMonster->SetDirectionLook( ((spec_params & ASP_MOVE_BKWD) == ASP_MOVE_BKWD) );

		// ��������� ������������� ��������� �������� ��������
		float &cur_yaw		= pMonster->r_torso_current.yaw;
		float &target_yaw	= pMonster->r_torso_target.yaw;
		if (MI.is_turn_params) {
			if (!getAI().bfTooSmallAngle(cur_yaw, target_yaw, MI.turn.min_angle)) {
				// ��������� ������� ����� ��� ������
				if (angle_normalize_signed(target_yaw - cur_yaw) > 0) 	cur_anim = MI.turn.anim_right;	// ������
				else													cur_anim = MI.turn.anim_left; 	// �����

				Seq_Add(cur_anim);
				Seq_Switch();
			}
		}	

		pMonster->ProcessTurn();

		// ���� ����� �������� �� ��������� � ����������, ��������� �������
		if (prev_anim != cur_anim) CheckTransition(prev_anim, cur_anim);

		if (!seq_playing) {
			// ��������� ASP � ���. ��������� �������� (kinda damaged)
			pMonster->CheckSpecParams(spec_params); 

			// ���� ������ ����� �� ����� � ������ �������� ��������, ������� stand_idle
			FixBadState();
		}
	} 

	else if (seq_playing) cur_anim = *seq_it;

	ApplyParams();

	// ���� ������������� �������� ���������� �� ���������� - ���������� ����� ��������
	if (cur_anim != prev_anim) ForceAnimSelect();		

	prev_anim	= cur_anim;
	spec_params = 0;
}


// ��������� �������� � ������� ��������� ��� cur_anim
void CMotionManager::ApplyParams()
{
	ANIM_ITEM_MAP_IT	item_it = m_tAnims.find(cur_anim);
	R_ASSERT(item_it != m_tAnims.end());

	pMonster->m_fCurSpeed		= item_it->second.speed.linear;
	pMonster->r_torso_speed		= item_it->second.speed.angular;
}

// Callback �� ���������� ��������
void CMotionManager::OnAnimationEnd() 
{ 
	m_tpCurAnim = 0;
	if (seq_playing) Seq_Switch();

	if (pJumping && pJumping->IsActive()) pJumping->OnAnimationEnd();
}

// ���� ������ ����� �� ����� � ������ �������� �������� - force stand idle
void CMotionManager::FixBadState()
{	
	bool is_moving_action = IsMoving();
	bool is_action_changed = prev_action != m_tAction;
	TTime cur_time = Level().timeServer();

	// ���� ����� ���� � ������ ��� - ���������
	if (!pMonster->AI_Path.TravelPath.empty() && ((pMonster->AI_Path.TravelPath.size() - 1 ) <= pMonster->AI_Path.TravelStart) && is_moving_action) {
		cur_anim = eAnimStandIdle;
		return;
	}

	// ����������� �������� '���� �� �����'
	if (is_action_changed) {
		time_start_stand = 0;
	}

	// ���� ������ �������� ������, ���������� �������� ��������
	if (is_moving_action && (time_start_stand == 0)) {
		time_start_stand	= cur_time;
	}

	if (is_moving_action && !is_action_changed && (time_start_stand + CRITICAL_STAND_TIME < cur_time) && pMonster->IsStanding(CRITICAL_STAND_TIME)) {
		cur_anim = eAnimStandIdle;	

		if (time_start_stand + CRITICAL_STAND_TIME + TIME_STAND_RECHECK < cur_time) time_start_stand = 0;
	}

	prev_action = m_tAction;
}

bool CMotionManager::IsMoving()
{
	return ((m_tAction == ACT_WALK_FWD) || (m_tAction == ACT_WALK_BKWD) || (m_tAction == ACT_RUN) || 
		(m_tAction == ACT_DRAG) || (m_tAction == ACT_STEAL));
}

// FX plaing stuff
void CMotionManager::AddHitFX(LPCTSTR name)
{
	m_tHitFXs.push_back(PSkeletonAnimated(pVisual)->ID_FX_Safe(name));
}

void CMotionManager::PlayHitFX(float amount)
{
	// info: check if function can be called more than once at a time

	if (m_tHitFXs.empty()) return;

	float power_factor = amount/100.f; 
	clamp(power_factor,0.f,1.f);

	//tpKinematics->PlayFX(m_tHitFXs[::Random.randI(m_tHitFXs.size())],power_factor);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// ������ � ��������������������
void CMotionManager::Seq_Init()
{
	seq_states.clear	();
	seq_it				= seq_states.end();
	seq_playing			= false;	
}

void CMotionManager::Seq_Add(EMotionAnim a)
{
	seq_states.push_back(a);
}

void CMotionManager::Seq_Switch()
{
	if (!seq_playing) {
		// ��������� �������������������
		seq_it = seq_states.begin();
	} else {
		seq_it++; 
		if (seq_it == seq_states.end()) {
			Seq_Finish();
			return;
		}
	}

	seq_playing = true;

	// ���������� ���������
	cur_anim	= *seq_it;
	ApplyParams ();
}

void CMotionManager::Seq_Finish()
{
	Seq_Init(); 

	prev_anim = cur_anim = m_tMotions[m_tAction].anim;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Attack Animation
void CMotionManager::AA_Clear()
{ 
	aa_time_started		= 0;
	aa_time_last_attack	= 0;
	aa_all.clear		(); 
	aa_stack.clear		(); 
}

void CMotionManager::AA_PushAttackAnim(SAttackAnimation AttackAnim)
{
	aa_all.push_back(AttackAnim);
}

void CMotionManager::AA_PushAttackAnim(EMotionAnim a, u32 i3, TTime from, TTime to, Fvector &ray, float dist, float damage, float yaw, float pitch, u32 flags)
{
	SAttackAnimation anim;
	anim.anim			= a;
	anim.anim_i3		= i3;

	anim.time_from		= from;
	anim.time_to		= to;

	anim.trace_offset	= ray;
	anim.dist			= dist;

	anim.damage			= damage;
	anim.dir_yaw		= yaw;
	anim.dir_pitch		= pitch;
	anim.flags			= flags;

	AA_PushAttackAnim	(anim);
}

void CMotionManager::AA_SwitchAnimation(EMotionAnim anim, u32 i3)
{
	aa_time_started = Level().timeServer();
	aa_stack.clear();

	// ����� � m_all �������� � ����������� (anim,i3) � ��������� m_stack
	ATTACK_ANIM_IT I = aa_all.begin();
	ATTACK_ANIM_IT E = aa_all.end();

	for (;I!=E; I++) {
		if ((I->anim == anim) && (I->anim_i3 == i3)) {
			aa_stack.push_back(*I);
		}
	}
}

// ���������� ���. ����� � ��������� ����� � �����
bool CMotionManager::AA_CheckTime(TTime cur_time, SAttackAnimation &anim)
{
	// ������� ����� �� ����� ���� ������ 'time_delta'
	TTime time_delta = 1000;

	if (aa_stack.empty()) return false;
	if (aa_time_last_attack + time_delta > cur_time) return false;

	ATTACK_ANIM_IT I = aa_stack.begin();
	ATTACK_ANIM_IT E = aa_stack.end();

	for (;I!=E; I++) if ((aa_time_started + I->time_from <= cur_time) && (cur_time <= aa_time_started + I->time_to)) {
		anim = (*I);
		return true;
	}
	return false;
}

EPState	CMotionManager::GetState (EMotionAnim a)
{
	// ����� �������� 
	ANIM_ITEM_MAP_IT  item_it = m_tAnims.find(a);
	R_ASSERT(item_it != m_tAnims.end());

	return item_it->second.pos_state;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Other
/////////////////////////////////////////////////////////////////////////////////////////////////

void CMotionManager::ForceAnimSelect() 
{
	m_tpCurAnim = 0; 
	pMonster->SelectAnimation(pMonster->Direction(),pMonster->Direction(),0);
}

void CMotionManager::UpdateVisual()
{
	pVisual = pMonster->Visual();
	LoadVisualData();
}	

/////////////////////////////////////////////////////////////////////////////////////////////////
// Scripting
#define TEST_STRING(a,b,c) {if (strcmp(a,b) == 0) cur_anim = c;}


