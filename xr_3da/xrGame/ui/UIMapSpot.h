// UIMapSpot.h:  ����� �� �����, 
// ��� ������� ���� �������� ��������� ��������
//////////////////////////////////////////////////////////////////////

#pragma once

#include "UIStatic.h"
#include "UIButton.h"


#define MAP_ICON_WIDTH  64
#define MAP_ICON_HEIGHT 64



class CUIMapSpot: public CUIButton
{
private:
	typedef CUIButton inherited;
public:
	CUIMapSpot();
	virtual ~CUIMapSpot();

	virtual void Draw();
	virtual void Update();

	//��������� �� ������, ������� ������������
	//�� �����,
	//���� ������ ��� �� NULL
	CObject* m_pObject;
	//������� ����� � ������� ������� ���������,
	//����� ���� m_pObject NULL
	Fvector m_vWorldPos;
	
	//������������� ������ (���/����)
	bool m_bCenter;
};
