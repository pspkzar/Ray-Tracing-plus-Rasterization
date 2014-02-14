#include "dialogs/dlgShaders.h"
#include <curitiba/event/eventFactory.h>

BEGIN_EVENT_TABLE(DlgShaders, wxDialog)

	EVT_COMBOBOX(DLG_COMBO, DlgShaders::OnListSelect)
	EVT_PG_CHANGED( DLG_PROPS, DlgShaders::OnPropsChange )
	EVT_BUTTON(DLG_BUTTON_ADD, DlgShaders::OnAdd)

	EVT_BUTTON(DLG_SHADER_COMPILE, DlgShaders::OnProcessCompileShaders)
	EVT_BUTTON(DLG_SHADER_VALIDATE, DlgShaders::OnProcessValidateShaders)
	EVT_BUTTON(DLG_SHADER_LINK, DlgShaders::OnProcessLinkShaders)

END_EVENT_TABLE()

wxWindow *DlgShaders::parent = NULL;
DlgShaders *DlgShaders::inst = NULL;


void DlgShaders::SetParent(wxWindow *p) {

	parent = p;
}

DlgShaders* DlgShaders::Instance () {

	if (inst == NULL)
		inst = new DlgShaders();

	return inst;
}



DlgShaders::DlgShaders()
	: wxDialog(DlgShaders::parent, -1, wxT("Slanger - Programs"),wxDefaultPosition,wxDefaultSize,wxRESIZE_BORDER|wxDEFAULT_DIALOG_STYLE)
                
{
	SetSizeHints( wxDefaultSize, wxDefaultSize );
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	setupPanel(sizer,parent);

	//SetAutoLayout(TRUE);
    SetSizer(sizer);
	Layout();
	Centre(wxBOTH);

    //sizer->SetSizeHints(this);
    //sizer->Fit(this);

	this->SetTitle(wxT("Slanger - Programs"));
}

void
DlgShaders::notifyUpdate(Notification aNot, std::string shaderName, std::string value) {

	// sends events on behalf of the shader
	curitiba::event_::IEventData *e= curitiba::event_::EventFactory::create("String");
	if (aNot == NEW_SHADER) {
		e->setData(&shaderName);
		EVENTMANAGER->notifyEvent("NEW_SHADER", shaderName,"", e);
	}
	else  {
		e->setData(&value);
		EVENTMANAGER->notifyEvent("SHADER_CHANGED", shaderName ,"", e);
	}
	delete e;

}

void DlgShaders::updateDlg() {

	
	updateList();
	list->SetSelection(0);
	update();
	//m_parent->Refresh();	


}

void DlgShaders::setupPanel(wxSizer *siz, wxWindow *parent) {

	// Program identification
	wxBoxSizer *sizH1 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *stg1 =  new wxStaticText(this,-1,wxT("Program: "));
	list = new wxComboBox(this,DLG_COMBO,wxT(""),wxDefaultPosition,wxDefaultSize,0,NULL,wxCB_READONLY );

	updateList();
	list->SetSelection(0);

	sizH1->Add(stg1, 0, wxALL,5);
	sizH1->Add(list, 1, wxALIGN_RIGHT|wxALL,5);
	siz->Add(sizH1, 0, wxEXPAND, 5);

	/* MIDDLE: Property grid */


	pg = new wxPropertyGridManager(this, DLG_PROPS,
				wxDefaultPosition, wxDefaultSize,
				// These and other similar styles are automatically
				// passed to the embedded wxPropertyGrid.
				wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|
				// Plus defaults.
				wxPGMAN_DEFAULT_STYLE
           );

	pg->AddPage(wxT("Programs"));

	pg->Append( new wxPropertyCategory(wxT("Files"),wxPG_LABEL) );	

    m_vf = new wxFileProperty( wxT("Vertex Shader"), wxT("VertexFile") );
    m_vf->SetAttribute(wxPG_FILE_WILDCARD,wxT("Vertex Shader Files (*.vert)|*.vert"));
	//m_vf->SetAttribute(wxPG_FILE_SHOW_RELATIVE_PATH,wxT(m_project->m_path.c_str()));
	pg->Append(m_vf);

    m_gf = new wxFileProperty( wxT("Geometry Shader"), wxT("GeometryFile") );
    m_gf->SetAttribute(wxPG_FILE_WILDCARD,wxT("Geometry Shader Files (*.geom)|*.geom"));
	//m_vf->SetAttribute(wxPG_FILE_SHOW_RELATIVE_PATH,wxT(m_project->m_path.c_str()));
	pg->Append(m_gf);


    m_ff = new wxFileProperty( wxT("Fragment Shader"), wxT("FragmentFile") );
    m_ff->SetAttribute(wxPG_FILE_WILDCARD,wxT("Fragment Shader Files (*.frag)|*.frag"));
	//m_ff->SetAttribute(wxPG_FILE_SHOW_RELATIVE_PATH,wxT(m_project->m_path.c_str()));
    pg->Append(m_ff);

	pg->Append( new wxPropertyCategory(wxT("Program properties"),wxPG_LABEL) );	

    m_LinkStatus = new wxBoolProperty( wxT("Link Status"), wxPG_LABEL );
	pg->Append(m_LinkStatus);

	m_ValidateStatus = new wxBoolProperty( wxT("Validate Status"), wxPG_LABEL );
	pg->Append(m_ValidateStatus);

	m_ActiveAtomicBuffers = new wxIntProperty( wxT("Active Atomic Counter Buffers"), wxPG_LABEL );
	pg->Append(m_ActiveAtomicBuffers);
		
	m_ActiveAttributes = new wxIntProperty( wxT("Active Attributes"), wxPG_LABEL );
	pg->Append(m_ActiveAttributes);
		
	m_ActiveUniforms = new wxIntProperty( wxT("Active Uniforms"), wxPG_LABEL );
	pg->Append(m_ActiveUniforms);
		
	siz->Add(pg,1, wxEXPAND|wxALL,5);

	// Buttons

	wxSizer *sizerS_SB = new wxBoxSizer(wxHORIZONTAL);
		
	m_bValidate = new wxButton(this,DLG_SHADER_VALIDATE,wxT("Validate"));
	m_bCompile = new wxButton(this,DLG_SHADER_COMPILE,wxT("Compile"));
	m_bLink = new wxButton(this,DLG_SHADER_LINK,wxT("Link"));

	sizerS_SB->Add(m_bValidate,0,wxALL,5);
	sizerS_SB->Add(m_bCompile,0,wxALL,5);
	sizerS_SB->Add(m_bLink,0,wxALL,5);

	siz->Add(sizerS_SB,0,wxALIGN_CENTER_HORIZONTAL,5);

	/* Logs */

	wxBoxSizer *sizerL = new wxBoxSizer(wxHORIZONTAL);

	m_log = new wxListBox(this,DLG_SHADER_LOG,wxDefaultPosition,wxDefaultSize,0,NULL,0);
	m_log->SetMinSize( wxSize( -1,25 ) );

	sizerL->Add(m_log,1, wxALL|wxEXPAND, 5);
	siz->Add(sizerL,1,wxALL|wxEXPAND,5);


//	wxBoxSizer *sizH2 = new wxBoxSizer(wxHORIZONTAL);
//	sizH2->Add(siz,1,wxALIGN_CENTER_HORIZONTAL | wxGROW | wxALL  ,5);
//	siz->Add(sizH2,1,wxGROW|wxALL|wxEXPAND,5);

	/* BOTTOM: Add Shader Button */

	wxBoxSizer *sizH3 = new wxBoxSizer(wxVERTICAL);

	bAdd = new wxButton(this,DLG_BUTTON_ADD,wxT("Add Program"));
	sizH3-> Add(bAdd,0,wxALL ,5);

	siz->Add(sizH3,0,wxEXPAND,5);
}


void DlgShaders::updateList() {

	std::vector<std::string> *names = RESOURCEMANAGER->getProgramNames();
	int num = names->size();

	list->Clear();

	for(int i = 0; i < num; i++)  {
		wxString s;
		s << i;
		list->Append(wxString(names->at(i).c_str()));
	}
	if (num > 0)
		m_active = names->at(0);
	else
		m_active = "";
	delete names;
}


void DlgShaders::OnPropsChange( wxPropertyGridEvent& e) {

	std::string fn;
	const wxString& name = e.GetPropertyName();
	wxString filename = pg->GetPropertyValueAsString(name);
	fn = std::string(filename.mb_str());
	
	IProgram *p = RESOURCEMANAGER->getProgram(m_active);
	if (name == wxT("VertexFile"))
		p->setShaderFile(IProgram::VERTEX_SHADER,fn);
	else if (name == wxT("GeometryFile"))
		p->setShaderFile(IProgram::GEOMETRY_SHADER, fn);
	else if (name == wxT("FragmentFile"))
		p->setShaderFile(IProgram::FRAGMENT_SHADER,fn);

	updateShaderAux();
	notifyUpdate(PROPS_CHANGED, m_active,std::string(name.mb_str()));

}




void
DlgShaders::update(){

	if (list->GetCount() == 0) {
		list->Disable();
		pg->GetPageByName(wxT("Programs"));
		pg->DisableProperty(m_vf);
		pg->DisableProperty(m_ff);
		pg->DisableProperty(m_gf);
		pg->SetPropertyValue(m_vf,"");
		pg->SetPropertyValue(m_ff,"");
		pg->SetPropertyValue(m_gf,"");

		wxPGProperty *prop = pg->GetPropertyByName(wxT("Uniform Variables"));
		if (prop)
			pg->DeleteProperty(wxT("Uniform Variables"));

		m_bCompile->Disable();
		m_bLink->Disable();
		m_bValidate->Disable();
		m_log->Disable();
		m_log->Clear();
	}

	else {

		list->Enable();
		m_log->Enable();
		m_log->Clear();

		GlProgram *p = (GlProgram *)RESOURCEMANAGER->getProgram(m_active);
		std::string vfn = p->getShaderFile(IProgram::VERTEX_SHADER),
					ffn = p->getShaderFile(IProgram::FRAGMENT_SHADER),
					gfn = p->getShaderFile(IProgram::GEOMETRY_SHADER);

		pg->ClearSelection();
		pg->SetPropertyValue(m_vf,(char *)vfn.c_str());
		pg->SetPropertyValue(m_ff,(char *)ffn.c_str());
		pg->SetPropertyValue(m_gf,(char *)gfn.c_str());

		updateProgramProperties(p);
			
		pg->EnableProperty(m_vf);
		pg->EnableProperty(m_ff);
		pg->EnableProperty(m_gf);

		updateShaderAux();
	}
}


void 
DlgShaders::updateShaderAux() {

	GlProgram *p = (GlProgram *)RESOURCEMANAGER->getProgram(m_active);
	std::string vfn = p->getShaderFile(IProgram::VERTEX_SHADER),
				ffn = p->getShaderFile(IProgram::FRAGMENT_SHADER),
					gfn = p->getShaderFile(IProgram::GEOMETRY_SHADER);

	if (vfn != "" || ffn != "" || gfn != "") {
		m_bCompile->Enable();
	}
	else {
		m_bCompile->Disable();
	}

	if (p->areCompiled())
		m_bLink->Enable();
	else
		m_bLink->Disable();

	if (p->isLinked())
		m_bValidate->Enable();
	else
		m_bValidate->Disable();


	std::map<std::string, curitiba::material::ProgramValue> progValues;
	std::map<std::string, curitiba::material::ProgramValue>::iterator progValuesIter;

	GlUniform u;
	std::string s;
	pg->GetPage(0);
	updateProgramProperties(p);
	wxPGProperty *prop = pg->GetPropertyByName(wxT("Uniform Variables"));
	if (prop)
		pg->DeleteProperty(wxT("Uniform Variables"));

	if (p->isLinked()) {
		int uni = p->getNumberOfUniforms();
		if (uni){
			pg->Append( new wxPropertyCategory(wxT("Uniform Variables"),wxPG_LABEL));
		}
		p->updateUniforms();
		for (int i = 0; i < uni; i++) {
			u = p->getUniform(i);
			addUniform(wxString(u.getName().c_str()),wxString(u.getProgramValueType().c_str()));
		}
	}
	pg->Refresh();
	m_log->Clear();
	//DlgMaterials::Instance()->updateDlg();
}


void 
DlgShaders::addUniform(wxString name, wxString type) {

	wxPGProperty *pid;

	pid = pg->Append(new wxStringProperty(name,wxPG_LABEL,type));
	pg->DisableProperty(pid);

}


wxString 
DlgShaders::getUniformType(int type) {

	std::string s;
	switch (type) {
		case Enums::FLOAT:s = "float";break;
		case Enums::VEC2:s = "vec2";break;
		case Enums::VEC3:s = "vec3";break;
		case Enums::VEC4:s = "vec4";break;

		case Enums::INT: s = "int";break;
		case Enums::IVEC2: s = "ivec2";break;
		case Enums::IVEC3: s = "ivec3";break;
		case Enums::IVEC4: s = "ivec4";break;

		case Enums::BOOL: s = "bool";break;
		case Enums::BVEC2: s = "bvec2";break;
		case Enums::BVEC3: s = "bvec3";break;
		case Enums::BVEC4: s = "bvec4";break;

		case Enums::SAMPLER: s = "sampler";break;

		case Enums::MAT2: s = "mat2";break;
		case Enums::MAT3: s = "mat3";break;
		case Enums::MAT4: s = "mat4";break;
	}
	return wxString(s.c_str());
}



void 
DlgShaders::updateProgramProperties(GlProgram *p) {

	pg->SetPropertyValue(m_LinkStatus, p->getPropertyb(GL_LINK_STATUS));
	pg->DisableProperty(m_LinkStatus);
	pg->SetPropertyValue(m_ValidateStatus, p->getPropertyb(GL_VALIDATE_STATUS));
	pg->DisableProperty(m_ValidateStatus);
	pg->SetPropertyValue(m_ActiveAtomicBuffers, p->getPropertyi(GL_ACTIVE_ATOMIC_COUNTER_BUFFERS));
	pg->DisableProperty(m_ActiveAtomicBuffers);
	pg->SetPropertyValue(m_ActiveAttributes, p->getPropertyi(GL_ACTIVE_ATTRIBUTES));
	pg->DisableProperty(m_ActiveAttributes);
	pg->SetPropertyValue(m_ActiveUniforms, p->getPropertyi(GL_ACTIVE_UNIFORMS));
	pg->DisableProperty(m_ActiveUniforms);


}


void 
DlgShaders::updateLogAux(std::string s) {


	std::string aux = s;
	int loc0 = 0;
	int loc = aux.find(10);
	while (loc != aux.npos) {
		m_log->AppendAndEnsureVisible(wxString(aux.substr(0,loc).c_str()));
		aux = aux.substr(loc+1);
		loc = aux.find(10);
	}
	//aux = s.su;
	m_log->AppendAndEnsureVisible(wxString(aux.c_str()));

}


void 
DlgShaders::OnProcessCompileShaders(wxCommandEvent& event){

	std::string infoLog;
	IProgram *p = RESOURCEMANAGER->getProgram(m_active);

	if (p->getShaderFile(IProgram::VERTEX_SHADER) != "" &&p->reloadShaderFile(IProgram::VERTEX_SHADER)) {
		p->compileShader(IProgram::VERTEX_SHADER);
	}
	
	if (p->getShaderFile(IProgram::GEOMETRY_SHADER) != "" &&p->reloadShaderFile(IProgram::GEOMETRY_SHADER)) {
		p->compileShader(IProgram::GEOMETRY_SHADER);
	}

	if (p->getShaderFile(IProgram::FRAGMENT_SHADER) != "" &&p->reloadShaderFile(IProgram::FRAGMENT_SHADER)) {
		p->compileShader(IProgram::FRAGMENT_SHADER);
	}

	updateShaderAux();

	m_log->Clear();

	infoLog = p->getShaderInfoLog(IProgram::VERTEX_SHADER);
	updateLogAux(infoLog);

	infoLog = p->getShaderInfoLog(IProgram::GEOMETRY_SHADER);
	updateLogAux(infoLog);

	infoLog = p->getShaderInfoLog(IProgram::FRAGMENT_SHADER);
	updateLogAux(infoLog);

	notifyUpdate(PROPS_CHANGED,m_active,"Compiled");

}



void DlgShaders::OnProcessLinkShaders(wxCommandEvent& event){

	char *infoLog;
	IProgram *p = RESOURCEMANAGER->getProgram(m_active);

	p->linkProgram();

	updateShaderAux();

	m_log->Clear();
	infoLog = p->getProgramInfoLog();
	updateLogAux(infoLog);
//	free(infoLog);

	notifyUpdate(PROPS_CHANGED,m_active,"Linked");

}



void DlgShaders::OnProcessValidateShaders(wxCommandEvent& event){

	char *infoLog;
	IProgram *p = RESOURCEMANAGER->getProgram(m_active);
	wxString s;

	if (p->programValidate())
		s = wxT("Validate: OK");
	else
		s = wxT("Not a Valid Program");
	m_log->Clear();
	m_log->Append(s);
	infoLog = p->getProgramInfoLog();
	updateLogAux(infoLog);
//	free(infoLog);

	//updateShaderAux();

}


void DlgShaders::updateInfo(std::string name) {

	if (name == m_active) {
		update();
	}
}


void DlgShaders::OnAdd(wxCommandEvent& event) {

	int result;
	bool nameUnique,exit;
	std::string name;

	do {
		wxTextEntryDialog dialog(this,
								 _T("(the name must be unique)\n")
								 _T("Please Input a Program Name"),
								 _T("Program Name\n"),
								 _T(""),
								 wxOK | wxCANCEL);

		result = dialog.ShowModal();
		name = std::string(dialog.GetValue().mb_str());
		nameUnique =  !RESOURCEMANAGER->hasProgram(name); 

		if (!nameUnique && (result == wxID_OK)){
			wxMessageBox(_T("Program name must be unique") , _T("Program Name Error"), wxOK | wxICON_INFORMATION, this);
		}
		if (name == "" && (result == wxID_OK)){
			wxMessageBox(_T("Program name must not be void"), _T("Program Name Error"), wxOK | wxICON_INFORMATION, this);
		}

		if (result == wxID_CANCEL) {
			exit = true;
		}
		else if (nameUnique && name != "") {
			exit = true;
		}

	} while (!exit);

	if (result == wxID_OK) {
		RESOURCEMANAGER->getProgram(name);
		updateList();
		list->Select(list->FindString(wxString(name.c_str())));
		m_active = name;
		update();
		notifyUpdate(NEW_SHADER,name,"");

	}
}

void DlgShaders::OnListSelect(wxCommandEvent& event){

	int sel;

	sel = event.GetSelection();
	m_active = std::string(list->GetString(sel).mb_str());
	update();
//	parent->Refresh();
}
