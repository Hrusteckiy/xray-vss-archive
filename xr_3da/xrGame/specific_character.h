//////////////////////////////////////////////////////////////////////////
// specific_character.h:	������� ���������� ��� � ���������� 
//							���������� � ����
//////////////////////////////////////////////////////////////////////////

#pragma		once


#include "character_info_defs.h"
#include "PhraseDialogDefs.h"

#include "shared_data.h"
#include "xml_str_id_loader.h"


//////////////////////////////////////////////////////////////////////////
// SSpecificCharacterData: ������ � ���������� ���������
//////////////////////////////////////////////////////////////////////////
struct SSpecificCharacterData : CSharedResource
{
	SSpecificCharacterData ();
	virtual ~SSpecificCharacterData ();

	//������� ��� ���������
	ref_str m_sGameName;

	//��������� ������
	PHRASE_DIALOG_INDEX m_iStartDialog;
	//������ ������, ������� ����� �������� ������ ��� ������� � ������ ����������
	PHRASE_DIALOG_INDEX m_iActorDialog;

	//��������� ������� ������ (��� �������� � �������) � ����� � �������� 
	int	m_iIconX, m_iIconY;
	//��������� ��������� ������ (��� �����)
	int	m_iMapIconX, m_iMapIconY;

	//������� 
	CHARACTER_COMMUNITY		m_Community;
	//����
	CHARACTER_RANK			m_Rank;
	//���������
	CHARACTER_REPUTATION	m_Reputation;
};


class CCharacterInfo;


class CSpecificCharacter: public CSharedClass<SSpecificCharacterData, SPECIFIC_CHARACTER_INDEX>,
						  public CXML_IdToIndex<SPECIFIC_CHARACTER_ID, SPECIFIC_CHARACTER_INDEX, CSpecificCharacter>
{
private:
	typedef CSharedClass	<SSpecificCharacterData, SPECIFIC_CHARACTER_INDEX>							inherited_shared;
	typedef CXML_IdToIndex	<SPECIFIC_CHARACTER_ID, SPECIFIC_CHARACTER_INDEX, CSpecificCharacter>	id_to_index;

	friend id_to_index;
	friend CCharacterInfo;
public:

	CSpecificCharacter();
	~CSpecificCharacter();

	//������������� �������
	//���� ����� id ������ �� ��������������
	//����� ��������� �� �����
	virtual void Load	(SPECIFIC_CHARACTER_ID		id);
	virtual void Load	(SPECIFIC_CHARACTER_INDEX	index);

protected:
	const SSpecificCharacterData* data() const { VERIFY(inherited_shared::get_sd()); return inherited_shared::get_sd();}
	SSpecificCharacterData*		  data()	   { VERIFY(inherited_shared::get_sd()); return inherited_shared::get_sd();}

	//�������� �� XML �����
	virtual void load_shared		(LPCSTR);
	static void	 InitXmlIdToIndex	();

	SPECIFIC_CHARACTER_INDEX m_iOwnIndex;
public:

	LPCSTR					Name		() const ;
	LPCSTR					Community	() const ;
	CHARACTER_RANK			Rank		() const ;
	CHARACTER_REPUTATION	Reputation	() const ;
	

	int		TradeIconX	() const	 {return data()->m_iIconX;}
	int		TradeIconY	() const	 {return data()->m_iIconY;}
	int		MapIconX	() const 	 {return data()->m_iMapIconX;}
	int		MapIconY	() const	 {return data()->m_iMapIconY;}
};


//////////////////////////////////////////////////////////////////////////
