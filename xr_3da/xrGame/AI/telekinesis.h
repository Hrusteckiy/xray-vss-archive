#pragma once

#include "telekinetic_object.h"
#include "../PHObject.h"

class CTelekinesis : public CPHUpdateObject {
	
	DEFINE_VECTOR(CTelekineticObject,TELE_OBJECTS,TELE_OBJECTS_IT);
	
	TELE_OBJECTS	objects;
	bool			active;

public:
					CTelekinesis		();
	virtual			~CTelekinesis		();

			
			// ������������ ������
			void	activate			(CPhysicsShellHolder *obj, float strength, float height, u32 max_time_keep);

			// �������������� ��� �������
			void	deactivate			();
			// �������������� ������
			void	deactivate			(CPhysicsShellHolder *obj);

			// ������� ������ 'obj' � ������� 'target'
			void	fire				(CPhysicsShellHolder *obj, const Fvector &target);
	
			// ������� ��� ������� � ������� 'target'
			void	fire				(const Fvector &target);
			
			// ������� ������ 'obj' � ������� 'target' � ������ ���� ���� 
			void	fire				(CPhysicsShellHolder *obj, const Fvector &target, float power);

			// ������� ���������� ����������
			bool	is_active			() {return active;}

			// ������� ���������� �������		
			bool	is_active_object	(CPhysicsShellHolder *obj);

			// ������� ���������� �������������� ��������
			u32		get_objects_count	();
			
			// ������� ������ �� ������� � �������
CTelekineticObject	get_object_by_index (u32 index) {VERIFY(objects.size() > index); return objects[index];}
					
			// �������� ��������� �� shedule_Update			
			void	schedule_update		();

private:

	// ���������� �� ����� ������
	virtual void 	PhDataUpdate		(dReal step);
	virtual void 	PhTune				(dReal step);
};

