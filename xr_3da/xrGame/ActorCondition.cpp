#include "stdafx.h"
#include "actorcondition.h"
#include "actor.h"
#include "actorEffector.h"
#include "inventory.h"
#include "level.h"
#include "sleepeffector.h"
#include "game_base_space.h"
#include "autosave_manager.h"
#include "xrserver.h"
#include "ai_space.h"
#include "script_callback_ex.h"
#include "script_game_object.h"
#include "game_object_space.h"
#include "ui\UIVideoPlayerWnd.h"
#include "script_callback_ex.h"
#include "object_broker.h"
#include "weapon.h"

#define ENEMIES_RADIUS				20.f

BOOL	GodMode	()	
{ 
	if (GameID() == GAME_SINGLE) 
		return psActorFlags.test(AF_GODMODE); 
	return FALSE;	
}

CActorCondition::CActorCondition(CActor *object) :
	inherited	(object)
{
	m_fJumpPower				= 0.f;
	m_fStandPower				= 0.f;
	m_fWalkPower				= 0.f;
	m_fJumpWeightPower			= 0.f;
	m_fWalkWeightPower			= 0.f;
	m_fOverweightWalkK			= 0.f;
	m_fOverweightJumpK			= 0.f;
	m_fAccelK					= 0.f;
	m_fSprintK					= 0.f;
	m_fAlcohol					= 0.f;

	VERIFY						(object);
	m_object					= object;
	m_actor_sleep_wnd			= NULL;
	m_can_sleep_callback		= NULL;
	m_get_sleep_video_name_callback	= NULL;
	m_condition_flags.zero		();
}

CActorCondition::~CActorCondition(void)
{
	xr_delete					(m_actor_sleep_wnd);
	xr_delete					(m_can_sleep_callback);
	xr_delete					(m_get_sleep_video_name_callback);
}

void CActorCondition::LoadCondition(LPCSTR entity_section)
{
	inherited::LoadCondition(entity_section);

	LPCSTR						section = READ_IF_EXISTS(pSettings,r_string,entity_section,"condition_sect",entity_section);
	m_fJumpPower				= pSettings->r_float(section,"jump_power");
	m_fStandPower				= pSettings->r_float(section,"stand_power");
	m_fWalkPower				= pSettings->r_float(section,"walk_power");
	m_fJumpWeightPower			= pSettings->r_float(section,"jump_weight_power");
	m_fWalkWeightPower			= pSettings->r_float(section,"walk_weight_power");
	m_fOverweightWalkK			= pSettings->r_float(section,"overweight_walk_k");
	m_fOverweightJumpK			= pSettings->r_float(section,"overweight_jump_k");
	m_fAccelK					= pSettings->r_float(section,"accel_k");
	m_fSprintK					= pSettings->r_float(section,"sprint_k");

	//����� ���� � �������� ������ �������� ����� �������� �������
	m_fLimpingHealthBegin		= pSettings->r_float(section,	"limping_health_begin");
	m_fLimpingHealthEnd			= pSettings->r_float(section,	"limping_health_end");
	R_ASSERT					(m_fLimpingHealthBegin<m_fLimpingHealthEnd);

	m_fLimpingPowerBegin		= pSettings->r_float(section,	"limping_power_begin");
	m_fLimpingPowerEnd			= pSettings->r_float(section,	"limping_power_end");
	R_ASSERT					(m_fLimpingPowerBegin<m_fLimpingPowerEnd);

	m_fCantWalkPowerBegin		= pSettings->r_float(section,	"cant_walk_power_begin");
	m_fCantWalkPowerEnd			= pSettings->r_float(section,	"cant_walk_power_end");
	R_ASSERT					(m_fCantWalkPowerBegin<m_fCantWalkPowerEnd);

	m_fCantSprintPowerBegin		= pSettings->r_float(section,	"cant_sprint_power_begin");
	m_fCantSprintPowerEnd		= pSettings->r_float(section,	"cant_sprint_power_end");
	R_ASSERT					(m_fCantSprintPowerBegin<m_fCantSprintPowerEnd);

	m_fPowerLeakSpeed			= READ_IF_EXISTS(pSettings,r_float,section,"max_power_leak_speed",0.0f);


	m_fK_SleepHealth		= pSettings->r_float(section,"sleep_health");
	m_fK_SleepPower			= pSettings->r_float(section,"sleep_power");
	m_fK_SleepMaxPower		= pSettings->r_float(section,"sleep_max_power");
	m_fK_SleepSatiety		= pSettings->r_float(section,"sleep_satiety");	
	m_fK_SleepRadiation		= pSettings->r_float(section,"sleep_radiation");
	m_fK_SleepPsyHealth		= pSettings->r_float(section,"sleep_psy_health");

	m_fV_Alcohol				= pSettings->r_float(section,"alcohol_v");

	LPCSTR cb_name			= READ_IF_EXISTS(pSettings,r_string,section,"can_sleep_callback","");

	if(cb_name && xr_strlen(cb_name)){
		m_can_sleep_callback		= xr_new<CScriptCallbackEx<LPCSTR> >();
		luabind::functor<LPCSTR>		f;
		R_ASSERT					(ai().script_engine().functor<LPCSTR>(cb_name,f));
		m_can_sleep_callback->set	(f);
	}
	cb_name					= READ_IF_EXISTS(pSettings,r_string,section,"sleep_video_name_callback","");

	if(cb_name && xr_strlen(cb_name)){
		m_get_sleep_video_name_callback		= xr_new<CScriptCallbackEx<LPCSTR> >();
		luabind::functor<LPCSTR>			fl;
		R_ASSERT							(ai().script_engine().functor<LPCSTR>(cb_name,fl));
		m_get_sleep_video_name_callback->set(fl);
	}

}


//���������� ���������� � ����� �������
#include "UI.h"
#include "HUDManager.h"

void CActorCondition::UpdateCondition()
{
	if (GodMode())				return;
	if (!object().g_Alive())	return;
	if (!object().Local())		return;	
	

	if ((object().mstate_real&mcAnyMove)) {
		ConditionWalk(object().inventory().TotalWeight()/object().inventory().GetMaxWeight(), isActorAccelerated(object().mstate_real,object().IsZoomAimingMode()), (object().mstate_real&mcSprint) != 0);
	}
	else {
		ConditionStand(object().inventory().TotalWeight()/object().inventory().GetMaxWeight());
	};
	
	float delta_time = float(m_iDeltaTime)/1000.f;

	if( !IsSleeping() && IsGameTypeSingle() ){

		float k_max_power = 1.0f;
		float weight = object().inventory().TotalWeight();
		float base_w = 40.0f;

		k_max_power = 1.0f + _min(weight,base_w)/base_w + _max(0.0f, (weight-base_w)/10.0f);

		SetMaxPower		(GetMaxPower() - m_fPowerLeakSpeed*delta_time*k_max_power);
	}

	m_fAlcohol		+= m_fV_Alcohol*delta_time;
	clamp			(m_fAlcohol,			0.0f,		1.0f);

	if ( IsGameTypeSingle() )
	{	
		CEffectorCam* ce = Actor()->Cameras().GetCamEffector((ECamEffectorType)effAlcohol);
		if	((m_fAlcohol>0.0001f) ){
			if(!ce){
				AddEffector(m_object,effAlcohol, "effector_alcohol", GET_KOEFF_FUNC(this, &CActorCondition::GetAlcohol));
			}
		}else{
			if(ce)
				RemoveEffector(m_object,effAlcohol);
		}

		
		CEffectorPP* ppe = object().Cameras().GetPPEffector((EEffectorPPType)effPsyHealth);
		if	((GetPsyHealth()>0.0001f) ){
			if(!ppe){
				AddEffector(m_object,effPsyHealth, "effector_psy_health", GET_KOEFF_FUNC(this, &CActorCondition::GetPsy));
			}
		}else{
			if(ppe)
				RemoveEffector(m_object,effPsyHealth);
		}
		if(fis_zero(GetPsyHealth()))
			health() =0.0f;
	};

	inherited::UpdateCondition();

	if( IsGameTypeSingle() )
		UpdateTutorialThresholds();
}

CWound* CActorCondition::ConditionHit(SHit* pHDS)
{
	if (GodMode()) return NULL;
	return inherited::ConditionHit(pHDS);
}

//weight - "��������" ��� �� 0..1
void CActorCondition::ConditionJump(float weight)
{
	float power			=	m_fJumpPower;
	power				+=	m_fJumpWeightPower*weight*(weight>1.f?m_fOverweightJumpK:1.f);
	m_fPower			-=	HitPowerEffect(power);
}
void CActorCondition::ConditionWalk(float weight, bool accel, bool sprint)
{	
	float delta_time	=	float(m_iDeltaTime)/1000.f;
	float power			=	m_fWalkPower;
	power				+=	m_fWalkWeightPower*weight*(weight>1.f?m_fOverweightWalkK:1.f);
	power				*=	delta_time*(accel?(sprint?m_fSprintK:m_fAccelK):1.f);
	m_fPower			-=	HitPowerEffect(power);
}

void CActorCondition::ConditionStand(float weight)
{	
	float delta_time	= float(m_iDeltaTime)/1000.f;
	float power			= m_fStandPower;
	power				*= delta_time;
	m_fPower			-= power;
}


bool CActorCondition::IsCantWalk() const
{
	if(m_fPower< m_fCantWalkPowerBegin)
		m_bCantWalk		= true;
	else if(m_fPower > m_fCantWalkPowerEnd)
		m_bCantWalk		= false;
	return				m_bCantWalk;
}

bool CActorCondition::IsCantSprint() const
{
	if(m_fPower< m_fCantSprintPowerBegin)
		m_bCantSprint	= true;
	else if(m_fPower > m_fCantSprintPowerEnd)
		m_bCantSprint	= false;
	return				m_bCantSprint;
}

bool CActorCondition::IsLimping() const
{
	if(m_fPower< m_fLimpingPowerBegin || GetHealth() < m_fLimpingHealthBegin)
		m_bLimping = true;
	else if(m_fPower > m_fLimpingPowerEnd && GetHealth() > m_fLimpingHealthEnd)
		m_bLimping = false;
	return m_bLimping;
}
extern bool g_bShowHudInfo;

bool CActorCondition::AllowSleep ()
{
	EActorSleep result		= CanSleepHere		();
	return( !stricmp(ACTOR_DEFS::easCanSleepResult, result)  );
}

EActorSleep CActorCondition::GoSleep(ALife::_TIME_ID sleep_time, bool without_check)
{
	if (IsSleeping()) return ACTOR_DEFS::easCanSleepResult;

	EActorSleep result = without_check?ACTOR_DEFS::easCanSleepResult : CanSleepHere();
	if( 0 != stricmp(ACTOR_DEFS::easCanSleepResult, result)  ) 
			return result;

	g_bShowHudInfo				= false;
	m_bIsSleeping				= true;

	ProcessSleep				(sleep_time);// change conditions

	//���������� ������, ���� �� ��������
	object().mstate_wishful	&=		~mcAnyMove;
	object().mstate_real	&=		~mcAnyMove;


	//��������� ���������
	object().m_dwWakeUpTime = Level().GetGameTime() + sleep_time;

	VERIFY	(m_object == smart_cast<CActor*>(Level().CurrentEntity()));

	m_object->Cameras().RemovePPEffector(EEffectorPPType(SLEEP_EFFECTOR_TYPE_ID));
	object().m_pSleepEffectorPP = xr_new<CSleepEffectorPP>(object().m_pSleepEffector->ppi,
													object().m_pSleepEffector->time,
													object().m_pSleepEffector->time_attack,
													object().m_pSleepEffector->time_release);

	m_object->Cameras().AddPPEffector(object().m_pSleepEffectorPP);

	m_object->callback(GameObject::eActorSleep)( m_object->lua_game_object() );


	m_actor_sleep_wnd			= xr_new<CUIActorSleepVideoPlayer>();
	m_actor_sleep_wnd->Init		( (*m_get_sleep_video_name_callback)() );

	return ACTOR_DEFS::easCanSleepResult;
}

void CActorCondition::Awoke()
{
	if(!IsSleeping())		return;

	m_bIsSleeping			= false;

	if ( ai().get_alife() ) {
		NET_Packet		P;
		P.w_begin		(M_SWITCH_DISTANCE);
		P.w_float		(object().m_fOldOnlineRadius);
		Level().Send	(P,net_flags(TRUE,TRUE));
	}

	m_actor_sleep_wnd->DeActivate	();
	xr_delete						(m_actor_sleep_wnd);

	VERIFY(m_object == smart_cast<CActor*>(Level().CurrentEntity()));
	VERIFY(object().m_pSleepEffectorPP);

	object().m_pSleepEffectorPP->m_eSleepState = CSleepEffectorPP::AWAKING;
	object().m_pSleepEffectorPP = NULL;

	g_bShowHudInfo			= true;
	
}

//�������� ����� �� �� ����� �� ���� �����
EActorSleep CActorCondition::CanSleepHere()
{
	if( m_can_sleep_callback && *m_can_sleep_callback)
		return (*m_can_sleep_callback)();
	
	R_ASSERT		(0);
	if(0 != object().mstate_real) return "cant_sleep_not_on_solid_ground";

	collide::rq_result RQ;

	Fvector pos, dir;
	pos.set(object().Position());
	pos.y += 0.1f;
	dir.set(0, -1.f, 0);
	BOOL				result = 
		Level().ObjectSpace.RayPick(
			pos,
			dir,
			0.3f, 
			collide::rqtBoth,
			RQ,
			&object()
		);
	
	//����� ����� �� ������������ ������� ��� ������ ������ - 
	//����� ������
	if(!result || RQ.O)	
		return "cant_sleep_not_on_solid_ground";

	xr_vector<CObject*> NearestList;	// = Level().ObjectSpace.q_nearest; 
	Level().ObjectSpace.GetNearest	(NearestList, pos, ENEMIES_RADIUS, &object()); 

	for(xr_vector<CObject*>::iterator it = NearestList.begin();
									NearestList.end() != it;
									it++)
	{
		CEntityAlive* entity = smart_cast<CEntityAlive*>(*it);
		if(entity && entity->g_Alive() && entity->is_relation_enemy(m_object))
			return "cant_sleep_near_enemies";
	}

	return easCanSleepResult;
}

void CActorCondition::save(NET_Packet &output_packet)
{
	inherited::save					(output_packet);
	save_data						(m_fAlcohol, output_packet);
	save_data						(m_condition_flags, output_packet);
}

void CActorCondition::load(IReader &input_packet)
{
	inherited::load		(input_packet);
	load_data			(m_fAlcohol, input_packet);
	load_data			(m_condition_flags, input_packet);
}

void CActorCondition::ProcessSleep(ALife::_TIME_ID sleep_time)
{
	if(GetHealth()<=0) return;

	//������ ������
	m_iDeltaTime = 1000*10;

	for(float time=0; time<sleep_time; time += m_iDeltaTime)
	{
		UpdateHealth			();
		UpdatePower				();
		UpdateSatiety			(m_fK_SleepSatiety);
		UpdateRadiation			(m_fK_SleepRadiation);
		UpdatePsyHealth			(m_fK_SleepPsyHealth);

		health()				+= m_fDeltaHealth;
		m_fPower				+= m_fDeltaPower;
		m_fSatiety				+= m_fDeltaSatiety;
		m_fRadiation			+=  m_fDeltaRadiation;
		m_fPsyHealth			+= m_fDeltaPsyHealth;

		SetMaxPower				(m_fPowerMax+m_fK_SleepMaxPower*float(m_iDeltaTime)/1000.0f );

		m_fDeltaHealth			= 0;
		m_fDeltaPower			= 0;
		m_fDeltaSatiety			= 0;
		m_fDeltaRadiation		= 0;
		m_fDeltaPsyHealth		= 0;
		

		clamp					(health(),		0.0f,		GetMaxHealth());
		clamp					(m_fPower,		0.0f,		m_fPowerMax);
		clamp					(m_fRadiation,	0.0f,		m_fRadiationMax);
		clamp					(m_fSatiety,	0.0f,		m_fSatietyMax);
		clamp					(m_fPsyHealth,	0.0f,		m_fPsyHealthMax);
	}

}

void CActorCondition::reinit	()
{
	inherited::reinit	();
	m_bLimping					= false;
	m_bIsSleeping				= false;
}

void CActorCondition::ChangeAlcohol	(float value)
{
	m_fAlcohol += value;
}

void CActorCondition::UpdateTutorialThresholds()
{
	string256 cb_name;
	static float _cPowerThr			= pSettings->r_float("tutorial_conditions_thresholds","power");
	static float _cPowerMaxThr		= pSettings->r_float("tutorial_conditions_thresholds","max_power");
	static float _cBleeding			= pSettings->r_float("tutorial_conditions_thresholds","bleeding");
	static float _cSatiety			= pSettings->r_float("tutorial_conditions_thresholds","satiety");
	static float _cRadiation		= pSettings->r_float("tutorial_conditions_thresholds","radiation");
	static float _cWpnCondition		= pSettings->r_float("tutorial_conditions_thresholds","weapon_jammed");

	bool b = true;
	if(b && !m_condition_flags.test(eCriticalPowerReached) && GetPower()<_cPowerThr){
		m_condition_flags.set			(eCriticalPowerReached, TRUE);b=false;
		strcpy(cb_name,"_G.on_actor_critical_power");
	}

	if(b && !m_condition_flags.test(eCriticalMaxPowerReached) && GetMaxPower()<_cPowerMaxThr){
		m_condition_flags.set			(eCriticalMaxPowerReached, TRUE);b=false;
		strcpy(cb_name,"_G.on_actor_critical_max_power");
	}

	if(b && !m_condition_flags.test(eCriticalBleedingSpeed) && BleedingSpeed()>_cBleeding){
		m_condition_flags.set			(eCriticalBleedingSpeed, TRUE);b=false;
		strcpy(cb_name,"_G.on_actor_bleeding");
	}

	if(b && !m_condition_flags.test(eCriticalSatietyReached) && GetSatiety()<_cSatiety){
		m_condition_flags.set			(eCriticalSatietyReached, TRUE);b=false;
		strcpy(cb_name,"_G.on_actor_satiety");
	}

	if(b && !m_condition_flags.test(eCriticalRadiationReached) && GetRadiation()>_cRadiation){
		m_condition_flags.set			(eCriticalRadiationReached, TRUE);b=false;
		strcpy(cb_name,"_G.on_actor_radiation");
	}

	if(b && !m_condition_flags.test(eWeaponJammedReached)&&m_object->inventory().GetActiveSlot()!=NO_ACTIVE_SLOT){
		PIItem item							= m_object->inventory().ItemFromSlot(m_object->inventory().GetActiveSlot());
		CWeapon* pWeapon					= smart_cast<CWeapon*>(item); 
		if(pWeapon&&pWeapon->GetCondition()<_cWpnCondition){
			m_condition_flags.set			(eWeaponJammedReached, TRUE);b=false;
			strcpy(cb_name,"_G.on_actor_weapon_jammed");
		}
	}
	
	if(!b){
		luabind::functor<LPCSTR>			fl;
		R_ASSERT							(ai().script_engine().functor<LPCSTR>(cb_name,fl));
		fl									();
	}
}
