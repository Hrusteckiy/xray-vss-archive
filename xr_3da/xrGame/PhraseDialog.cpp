///////////////////////////////////////////////////////////////
// PhraseDialog.cpp
// ����� ������� - ������� ��� ������ ���� 2� ���������� � ����
///////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "phrasedialog.h"
#include "phrasedialogmanager.h"
#include "gameobject.h"
#include "ai_debug.h"

//////////////////////////////////////////////////////////////////////////

SPhraseDialogData::SPhraseDialogData ()
{
	m_PhraseGraph.clear();
	m_eDialogType.zero();
	m_iPriority = 0;
}

SPhraseDialogData::~SPhraseDialogData ()
{
}

//////////////////////////////////////////////////////////////////////////



CPhraseDialog::CPhraseDialog(void)
{
	m_iSaidPhraseID = NO_PHRASE;
	m_bFinished = false;

	m_pSpeakerFirst		= NULL;
	m_pSpeakerSecond	= NULL;

	m_DialogId = NULL;
}

CPhraseDialog::~CPhraseDialog(void)
{
}


void CPhraseDialog::Init(CPhraseDialogManager* speaker_first, 
						 CPhraseDialogManager* speaker_second)
{
	THROW(!IsInit());

	m_pSpeakerFirst = speaker_first;
	m_pSpeakerSecond = speaker_second;

	m_iSaidPhraseID = NO_PHRASE;
	m_PhraseVector.clear();

	CPhraseGraph::CVertex* phrase_vertex = data()->m_PhraseGraph.vertex(START_PHRASE);
	THROW(phrase_vertex);
	m_PhraseVector.push_back(phrase_vertex->data());

	m_bFinished = false;
	m_bFirstIsSpeaking = true;
}

//�������� ��� �����
void CPhraseDialog::Reset ()
{
}

CPhraseDialogManager* CPhraseDialog::OurPartner	(CPhraseDialogManager* dialog_manager) const
{
	if(FirstSpeaker() == dialog_manager)
		return SecondSpeaker();
	else
		return FirstSpeaker();
}


CPhraseDialogManager* CPhraseDialog::CurrentSpeaker()	const 
{
	return FirstIsSpeaking()?m_pSpeakerFirst:m_pSpeakerSecond;
}
CPhraseDialogManager* CPhraseDialog::OtherSpeaker	()	const 
{
	return (!FirstIsSpeaking())?m_pSpeakerFirst:m_pSpeakerSecond;
}


//�������� ��� ���������� ������� ����
static bool PhraseGoodwillPred(const CPhrase* phrase1, const CPhrase* phrase2)
{
	return phrase1->GoodwillLevel()>phrase2->GoodwillLevel();
}

bool CPhraseDialog::SayPhrase (DIALOG_SHARED_PTR& phrase_dialog, PHRASE_ID phrase_id)
{
	THROW(phrase_dialog->IsInit());

	phrase_dialog->m_iSaidPhraseID = phrase_id;

	bool first_is_speaking = phrase_dialog->FirstIsSpeaking();
	phrase_dialog->m_bFirstIsSpeaking = !phrase_dialog->m_bFirstIsSpeaking;

	const CGameObject*	pSpeakerGO1 = smart_cast<const CGameObject*>(phrase_dialog->FirstSpeaker());	VERIFY(pSpeakerGO1);
	const CGameObject*	pSpeakerGO2 = smart_cast<const CGameObject*>(phrase_dialog->SecondSpeaker());	VERIFY(pSpeakerGO2);
	if(!first_is_speaking) std::swap(pSpeakerGO1, pSpeakerGO2);

	CPhraseGraph::CVertex* phrase_vertex = phrase_dialog->data()->m_PhraseGraph.vertex(phrase_dialog->m_iSaidPhraseID);
	THROW(phrase_vertex);

	CPhrase* last_phrase = phrase_vertex->data();

	//������ ��� ����, ���� ��������
	phrase_dialog->m_PhraseVector.clear();
	if(phrase_vertex->edges().empty())
	{
		phrase_dialog->m_bFinished = true;
	}
	else
	{
		//�������� ������ ����, ������� ������ ������ �������� ����������
		for(xr_vector<CPhraseGraph::CEdge>::const_iterator it = phrase_vertex->edges().begin();
			it != phrase_vertex->edges().end();
			it++)
		{
			const CPhraseGraph::CEdge& edge = *it;
			CPhraseGraph::CVertex* next_phrase_vertex = phrase_dialog->data()->m_PhraseGraph.vertex(edge.vertex_id());
			THROW(next_phrase_vertex);

			if(next_phrase_vertex->data()->m_PhraseScript.Precondition(pSpeakerGO2, pSpeakerGO1, *phrase_dialog->m_DialogId, (int)phrase_id)){
				phrase_dialog->m_PhraseVector.push_back(next_phrase_vertex->data());
#ifdef DEBUG
				if(psAI_Flags.test(aiDialogs)){
					LPCSTR phrase_text = next_phrase_vertex->data()->GetText();
					int iii = next_phrase_vertex->data()->GetIndex();
					Msg("----added phrase text [%s]phrase_id=[%d] index=[%d] to dialog [%s]",phrase_text,phrase_id,iii,*phrase_dialog->m_DialogId);
				}
#endif
			}

		}

		THROW3(!phrase_dialog->m_PhraseVector.empty(), "No available phrase to say.", *phrase_dialog->m_DialogId);

		//����������� ������ �� �������� ���������������
		std::sort(phrase_dialog->m_PhraseVector.begin(),
				 phrase_dialog->m_PhraseVector.end(), PhraseGoodwillPred);
	}

	//������� ���������� �������������� ������� 
	//������������ ����� ��������� �����
	//������ �������� - ��� ��� ������� �����, ������ - ��� ��� �������
	last_phrase->m_PhraseScript.Action(pSpeakerGO1, pSpeakerGO2, *phrase_dialog->m_DialogId, (int)phrase_id);

	//�������� CDialogManager, ��� ������� �����
	//� ��������� �����
	if(first_is_speaking)
		phrase_dialog->SecondSpeaker()->ReceivePhrase(phrase_dialog);
	else
		phrase_dialog->FirstSpeaker()->ReceivePhrase(phrase_dialog);


	return phrase_dialog?!phrase_dialog->m_bFinished:true;
}

LPCSTR CPhraseDialog::GetPhraseText	(PHRASE_ID phrase_id, bool current_speaking)
{
	
	CPhraseGraph::CVertex* phrase_vertex = data()->m_PhraseGraph.vertex(phrase_id);
	THROW(phrase_vertex);
	
	//���� ���� ���������� �����, �� �� � ����� �����
	const CGameObject*	pSpeakerGO1 = smart_cast<const CGameObject*>(CurrentSpeaker());
	const CGameObject*	pSpeakerGO2 = smart_cast<const CGameObject*>(OtherSpeaker());	

	if(!current_speaking) 
		std::swap(pSpeakerGO1, pSpeakerGO2);
	
	LPCSTR script_text = NULL;
	//���� ����������� ��� �� ������, �� ����� ����� ������������� ������ ��� 
	//������� �� �� ����������
	if(pSpeakerGO1 && pSpeakerGO2)
		script_text = phrase_vertex->data()->GetScriptText(pSpeakerGO1, pSpeakerGO2, *m_DialogId, (int)phrase_id);
	return script_text?script_text:phrase_vertex->data()->GetText();
}

LPCSTR CPhraseDialog::DialogCaption()
{
	return *data()->m_sCaption?*data()->m_sCaption:GetPhraseText(START_PHRASE);
}


int	 CPhraseDialog::Priority()
{
	return data()->m_iPriority;
}


//������������� �������
//���� ������ � ����� id ������ �� �������������
//�� ����� �������� �� �����
/*
void CPhraseDialog::Load(PHRASE_DIALOG_ID dialog_id)
{
	Load(id_to_index::IdToIndex(dialog_id));
}
*/
void CPhraseDialog::Load(PHRASE_DIALOG_ID dialog_id)
{
	m_DialogId = dialog_id;
	inherited_shared::load_shared(m_DialogId, NULL);
}


void CPhraseDialog::load_shared	(LPCSTR)
{
//	const id_to_index::ITEM_DATA& item_data = *id_to_index::GetByIndex(m_DialogIndex);
	const id_to_index::ITEM_DATA& item_data = *id_to_index::GetById(m_DialogId);

	string128 xml_file_full;
	strconcat(xml_file_full, *shared_str(item_data.file_name), ".xml");

	bool xml_result = uiXml.Init(CONFIG_PATH, GAME_PATH, xml_file_full);
	THROW3(xml_result, "xml file not found", xml_file_full);

	//loading from XML
	XML_NODE* dialog_node = uiXml.NavigateToNode(id_to_index::tag_name, item_data.pos_in_file);
	THROW3(dialog_node, "dialog id=", *item_data.id);

	uiXml.SetLocalRoot(dialog_node);

	int actor_dialog = uiXml.ReadAttribInt(dialog_node, "actor", 1);
	if (1 == actor_dialog) 
		data()->m_eDialogType.set(eDialogTypeActor, TRUE);

	int pda_dialog = uiXml.ReadAttribInt(dialog_node, "pda", 0);
	if (1 == pda_dialog) 
		data()->m_eDialogType.set(eDialogTypePDA, TRUE);

	int ai_dialog = uiXml.ReadAttribInt(dialog_node, "ai", 0);
	if (1 == ai_dialog) 
		data()->m_eDialogType.set(eDialogTypeAI, TRUE);


	data()->m_iPriority = uiXml.ReadAttribInt(dialog_node, "priority", 0);

	//��������� 
	data()->m_sCaption = uiXml.Read(dialog_node, "caption", 0, NULL);

	//��������� ������ �������
	data()->m_PhraseScript.Load(uiXml, dialog_node);

	//��������� ���� ������� �������
	data()->m_PhraseGraph.clear();

	phrase_list_node = uiXml.NavigateToNode(dialog_node, "phrase_list", 0);
	int phrase_num = uiXml.GetNodesNum(phrase_list_node, "phrase");
	THROW3(phrase_num, "dialog %s has no phrases at all", *item_data.id);

	uiXml.SetLocalRoot(phrase_list_node);

	LPCSTR wrong_phrase_id = uiXml.CheckUniqueAttrib(phrase_list_node, "phrase", "id");
	THROW3(wrong_phrase_id == NULL, *item_data.id, wrong_phrase_id);
	

	//���� ��������� �����
	XML_NODE* phrase_node = uiXml.NavigateToNodeWithAttribute("phrase", "id", START_PHRASE_STR);
	THROW(phrase_node);
	AddPhrase(phrase_node, START_PHRASE);
}


void CPhraseDialog::AddPhrase	(XML_NODE* phrase_node, PHRASE_ID phrase_id)
{
	//��������� �� ��������� �� �������
	if(data()->m_PhraseGraph.vertex(phrase_id)) 
		return;

	CPhrase* phrase = xr_new<CPhrase>(); VERIFY(phrase);
	phrase->SetIndex(phrase_id);
	
	//��������� ������������� �����
	phrase->SetText(uiXml.Read(phrase_node, "text", 0, ""));
	//������� ���������������
	phrase->m_iGoodwillLevel = uiXml.ReadInt(phrase_node, "goodwill", 0, -10000);


	phrase->m_sound = uiXml.Read	(phrase_node,	"sound", NULL);
	phrase->m_anim = uiXml.Read		(phrase_node,	"anim", NULL);

	
	//��������� �������� � ���������
	phrase->m_PhraseScript.Load(uiXml, phrase_node);
	
	data()->m_PhraseGraph.add_vertex(phrase, phrase_id);

	//����� ������� ���������� ����� �������� ����� ����
	int next_num = uiXml.GetNodesNum(phrase_node, "next");
	for(int i=0; i<next_num; i++)
	{
		LPCSTR next_phrase_id_str = uiXml.Read(phrase_node, "next", i, "");
		XML_NODE* next_phrase_node = uiXml.NavigateToNodeWithAttribute("phrase", "id", next_phrase_id_str);
		THROW(next_phrase_node);
		int next_phrase_id = atoi(next_phrase_id_str);

		AddPhrase (next_phrase_node, next_phrase_id);
		data()->m_PhraseGraph.add_edge(phrase_id, next_phrase_id, 0.f);
	}
}

bool  CPhraseDialog::Precondition(const CGameObject* pSpeaker1, const CGameObject* pSpeaker2)
{
	return data()->m_PhraseScript.Precondition(pSpeaker1, pSpeaker2, *m_DialogId, -1);
}

void   CPhraseDialog::InitXmlIdToIndex()
{
	if(!id_to_index::tag_name)
		id_to_index::tag_name = "dialog";
	if(!id_to_index::file_str)
		id_to_index::file_str = pSettings->r_string("dialogs", "files");
}

bool CPhraseDialog::allIsDummy	()
{
	PHRASE_VECTOR_IT it = m_PhraseVector.begin();
	bool bAllIsDummy = true;
	for(;it!=m_PhraseVector.end();++it){
		if( !(*it)->IsDummy() )
			bAllIsDummy=false;
	}

	return bAllIsDummy;
}
