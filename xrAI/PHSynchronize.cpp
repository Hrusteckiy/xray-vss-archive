#include "stdafx.h"
#include "PHSynchronize.h"
void	SPHNetState::net_Export(NET_Packet& P)
{
	P.w_vec3(linear_vel);
	//P.w_vec3(angular_vel);
	//P.w_vec3(force);
	//P.w_vec3(torque);
	P.w_vec3(position);
	P.w_vec4(*((Fvector4*)&quaternion));
	//P.w_vec4(*((Fvector4*)&previous_quaternion));
	P.w_u8	((u8)enabled);
}


void	SPHNetState::net_Import(NET_Packet&	P)
{
	P.r_vec3(linear_vel);
	angular_vel.set(0.f,0.f,0.f);		//P.r_vec3(angular_vel);
	force.set(0.f,0.f,0.f);				//P.r_vec3(force);
	torque.set(0.f,0.f,0.f);			//P.r_vec3(torque);
	P.r_vec3(position);
	P.r_vec4(*((Fvector4*)&quaternion));
	previous_quaternion.set(quaternion);//P.r_vec4(*((Fvector4*)&previous_quaternion));
	enabled=!!P.r_u8	();
}

void SPHNetState::net_Save(NET_Packet &P)
{
	net_Export(P);
}

void SPHNetState::net_Load(NET_Packet &P)
{
	net_Import(P);
	previous_position.set(position);
}