#ifndef PH_CONTACT_BODY_EFFECTOR_H
#define PH_CONTACT_BODY_EFFECTOR_H
#include "PHBaseBodyEffector.h"
class CPHContactBodyEffector : public CPHBaseBodyEffector
{
dContact m_contact;
float	 m_recip_flotation;
public:
void	Init(dBodyID body, dContact& contact, float flotation)
		{
			CPHBaseBodyEffector::Init(body);
			m_contact=contact;
			m_recip_flotation=1.f/flotation;
		}
void	Merge(dContact& contact, float flotation);
void	Apply();
};
#endif	//PH_CONTACT_BODY_EFFECTOR_H