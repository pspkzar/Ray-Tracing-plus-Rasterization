#include "dlgAtomics.h"
#include <curitiba/render/irenderer.h>
#include <curitiba.h>

///////////////////////////////////////////////////////////////////////////

wxWindow *DlgAtomics::parent = NULL;
DlgAtomics *DlgAtomics::inst = NULL;


void 
DlgAtomics::SetParent(wxWindow *p) {

	parent = p;
}


DlgAtomics* 
DlgAtomics::Instance () {

	if (inst == NULL)
		inst = new DlgAtomics();

	return inst;
}


DlgAtomics::DlgAtomics(): 
	wxDialog(DlgAtomics::parent, -1, wxT("Slanger - Atomics"),wxDefaultPosition,wxDefaultSize,wxRESIZE_BORDER|wxDEFAULT_DIALOG_STYLE)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	m_propertyGrid1 = new wxPropertyGridManager(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPGMAN_DEFAULT_STYLE);
	m_propertyGrid1->AddPage(wxT("Atomics"));
	m_propertyGrid1->Append(new wxFloatProperty( wxString("bla"), wxPG_LABEL ));

	bSizer1->Add( m_propertyGrid1, 1, wxEXPAND | wxALL, 5 );
	

	this->SetSizer( bSizer1 );
	this->Layout();
	
	this->Centre( wxBOTH );
}

DlgAtomics::~DlgAtomics()
{
}


void
DlgAtomics::updateDlg() {


	m_propertyGrid1->Clear();
	m_propertyGrid1->AddPage(wxT("Atomics"));

	std::map<int, std::string>::iterator iter;
	iter = IRenderer::AtomicLabels.begin();
	for (; iter != IRenderer::AtomicLabels.end(); ++iter) {
		m_propertyGrid1->Append(new wxFloatProperty( wxString(iter->second.c_str()), wxPG_LABEL ));

	}
}


void
DlgAtomics::update() {

	std::map<int, std::string>::iterator iter;
	iter = IRenderer::AtomicLabels.begin();
	for (; iter != IRenderer::AtomicLabels.end(); ++iter) {
	
		m_propertyGrid1->SetPropertyValue(wxString(iter->second.c_str()),(int)(RENDERER->getCounter(iter->first)));
	}
}