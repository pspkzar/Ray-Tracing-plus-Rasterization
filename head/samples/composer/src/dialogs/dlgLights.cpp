#include "dialogs/dlgLights.h"
#include <curitiba.h>
#include <curitiba/event/eventFactory.h> 


BEGIN_EVENT_TABLE(DlgLights, wxDialog)

	EVT_COMBOBOX(DLG_COMBO, DlgLights::OnListSelect)
	EVT_PG_CHANGED( DLG_PROPS, DlgLights::OnPropsChange )
	EVT_BUTTON(DLG_BUTTON_ADD, DlgLights::OnAdd)
	
END_EVENT_TABLE()

wxWindow *DlgLights::parent = NULL;
DlgLights *DlgLights::inst = NULL;


void DlgLights::SetParent(wxWindow *p) {

	parent = p;
}

DlgLights* DlgLights::Instance () {

	if (inst == NULL)
		inst = new DlgLights();

	return inst;
}


DlgLights::DlgLights()
	: wxDialog(DlgLights::parent, -1, wxT("Slanger - Lights"),wxDefaultPosition,wxDefaultSize,wxRESIZE_BORDER|wxDEFAULT_DIALOG_STYLE)
                
{
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	setupPanel(sizer,parent);

	SetAutoLayout(TRUE);
	sizer->SetSizeHints(this);
	sizer->Fit(this);
    SetSizer(sizer);

	this->SetTitle(wxT("Slanger - Lights"));
	this->SetSize(350,200);
}


void
DlgLights::notifyUpdate(Notification aNot, std::string lightName, std::string value) {

	// sends events on behalf of the light
	curitiba::event_::IEventData *e= curitiba::event_::EventFactory::create("String");
	if (aNot == NEW_LIGHT) {
		e->setData(&lightName);
		EVENTMANAGER->notifyEvent("NEW_LIGHT", lightName,"", e);
	}
	else  {
		e->setData(&value);
		EVENTMANAGER->notifyEvent("LIGHT_CHANGED", lightName ,"", e);
	}
	delete e;
}



void 
DlgLights::updateDlg() {

	updateList();
	list->SetSelection(0);
	update();
	m_parent->Refresh();	


}

void 
DlgLights::setupPanel(wxSizer *siz, wxWindow *parent) {

	/* TOP: COMBO with camera names */
	wxBoxSizer *sizH1 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *stg1 =  new wxStaticText(this,-1,wxT("Light: "));
	list = new wxComboBox(this,DLG_COMBO,wxT(""),wxDefaultPosition,wxDefaultSize,0,NULL,wxCB_READONLY );

	updateList();
	list->SetSelection(0);

	sizH1->Add(stg1, 0, wxGROW|wxHORIZONTAL,5);
	sizH1->Add(list, 1, wxGROW|wxHORIZONTAL,5);
	siz->Add(sizH1, 0, wxGROW|wxALL, 15);

	/* MIDDLE: Property grid */


	pg = new wxPropertyGridManager(this, DLG_PROPS,
				wxDefaultPosition, wxDefaultSize,
				// These and other similar styles are automatically
				// passed to the embedded wxPropertyGrid.
				wxPG_BOLD_MODIFIED|wxPG_SPLITTER_AUTO_CENTER|
				// Plus defaults.
				wxPGMAN_DEFAULT_STYLE
           );

	pg->AddPage(wxT("Lights"));
	//wxPropertyGridPage* pgPropsPage = pg->GetPage("Cameras");

	const wxChar* lightType[] = { wxT("DIRECTIONAL"), wxT("POINT"), wxT("SPOT"), NULL};
	const long lightTypeInd[] = { Light::DIRECTIONAL, Light::POSITIONAL, Light::SPOT_LIGHT};

	//pg->Append(new wxStringProperty(wxT("Name"),wxPG_LABEL,""));
 	//pg->DisableProperty(wxT("Name"));

	pg->Append(new wxEnumProperty(wxT("TYPE"),wxPG_LABEL,lightType,lightTypeInd,Light::DIRECTIONAL));
    pg->Append( new wxBoolProperty( wxT("ENABLED"), wxPG_LABEL ) );

	wxPGProperty* topId;
	
	topId= pg->Append( new wxStringProperty(wxT("POSITION"), wxPG_LABEL, wxT("<composed>")) );

    pg->AppendIn( topId, new wxFloatProperty( wxT("X"), wxPG_LABEL ) );
    pg->AppendIn( topId, new wxFloatProperty( wxT("Y"), wxPG_LABEL ) );
    pg->AppendIn( topId, new wxFloatProperty( wxT("Z"), wxPG_LABEL ) );

	topId= pg->Append( new wxStringProperty(wxT("DIRECTION"), wxPG_LABEL, wxT("<composed>")) );

    pg->AppendIn( topId, new wxFloatProperty( wxT("X"), wxPG_LABEL ) );
    pg->AppendIn( topId, new wxFloatProperty( wxT("Y"), wxPG_LABEL ) );
    pg->AppendIn( topId, new wxFloatProperty( wxT("Z"), wxPG_LABEL ) );

	topId= pg->Append( new wxStringProperty(wxT("DIFFUSE"), wxPG_LABEL, wxT("<composed>")) );
	pg->AppendIn(topId,new wxColourProperty(wxT("RGB"),wxPG_LABEL,
					wxColour(255,255,255)));

	topId= pg->Append( new wxStringProperty(wxT("AMBIENT"), wxPG_LABEL, wxT("<composed>")) );
	pg->AppendIn(topId,new wxColourProperty(wxT("RGB"),wxPG_LABEL,
					wxColour(255,255,255)));

	topId= pg->Append( new wxStringProperty(wxT("SPECULAR"), wxPG_LABEL, wxT("<composed>")) );
	pg->AppendIn(topId,new wxColourProperty(wxT("RGB"),wxPG_LABEL,
					wxColour(255,255,255)));

	pg->Append( new wxFloatProperty(wxT("SPOT_CUTOFF"), wxPG_LABEL ));
	pg->Append( new wxFloatProperty(wxT("SPOT_EXPONENT"), wxPG_LABEL ));

	pg->Append( new wxFloatProperty(wxT("CONSTANT_ATT"), wxPG_LABEL));
	pg->Append( new wxFloatProperty(wxT("LINEAR_ATT"), wxPG_LABEL));
	pg->Append( new wxFloatProperty(wxT("QUADRATIC_ATT"), wxPG_LABEL));

	update();

	pg->SetSplitterLeft(true);
//	pg->SetSplitterPosition(100);
	siz->Add(pg,1, wxEXPAND|wxALL,5);

	//wxBoxSizer *sizH2 = new wxBoxSizer(wxHORIZONTAL);
	//sizH2->Add(pg,1,wxALIGN_CENTER_HORIZONTAL | wxGROW | wxALL  ,5);
	//siz->Add(sizH2,1,wxGROW|wxALL|wxEXPAND,5);

	/* BOTTOM: Add Light Button */

	wxBoxSizer *sizH3 = new wxBoxSizer(wxHORIZONTAL);

	bAdd = new wxButton(this,DLG_BUTTON_ADD,wxT("Add Light"));
	sizH3-> Add(bAdd,0,wxALL |wxGROW |wxHORIZONTAL|wxALIGN_CENTER_HORIZONTAL,5);

	siz->Add(sizH3,0,wxALL |wxGROW |wxHORIZONTAL,15);
}


void DlgLights::updateList() {

	std::vector<std::string> *names = RENDERMANAGER->getLightNames();
	int num = names->size();

	list->Clear();

	for(int i = 0; i < num; i++)  {
		wxString s;
		s << i;
		list->Append(wxString(names->at(i).c_str()));
	}
	m_active = names->at(0);

	delete names;
}



void DlgLights::update() {


	curitiba::scene::Light *light;		
	light = RENDERMANAGER->getLight(m_active);
	unsigned int type = light->getPrope(Light::TYPE);

	pg->ClearSelection();

	pg->SetPropertyValue(wxT("ENABLED"),light->getPropb(Light::ENABLED));

	if (curitiba::scene::Light::DIRECTIONAL == type) {
		pg->SetPropertyValue(wxT("TYPE"), DLG_MI_DIRECTIONAL);
		pg->DisableProperty(wxT("POSITION"));
		pg->DisableProperty(wxT("SPOT_CUTOFF"));
		pg->DisableProperty(wxT("SPOT_EXPONENT"));
		//pg->DisableProperty(wxT("Attenuation"));
		pg->DisableProperty(wxT("CONSTANT_ATT"));
		pg->DisableProperty(wxT("LINEAR_ATT"));
		pg->DisableProperty(wxT("QUADRATIC_ATT"));
	}
	else if (curitiba::scene::Light::POSITIONAL == type) {
		pg->SetPropertyValue(wxT("TYPE"), DLG_MI_POINT);
		pg->EnableProperty(wxT("POSITION"));
		pg->DisableProperty(wxT("SPOT_CUTOFF"));
		pg->DisableProperty(wxT("SPOT_EXPONENT"));
		//pg->EnableProperty(wxT("Attenuation"));
		pg->EnableProperty(wxT("CONSTANT_ATT"));
		pg->EnableProperty(wxT("LINEAR_ATT"));
		pg->EnableProperty(wxT("QUADRATIC_ATT"));
	}
	else if (curitiba::scene::Light::SPOT_LIGHT == type) {
		pg->SetPropertyValue(wxT("TYPE"), DLG_MI_SPOT);
		pg->EnableProperty(wxT("POSITION"));
		pg->EnableProperty(wxT("SPOT_CUTOFF"));
		pg->EnableProperty(wxT("SPOT_EXPONENT"));
		//pg->EnableProperty(wxT("Attenuation"));
		pg->EnableProperty(wxT("CONSTANT_ATT"));
		pg->EnableProperty(wxT("LINEAR_ATT"));
		pg->EnableProperty(wxT("QUADRATIC_ATT"));
	}

	curitiba::math::vec4 v = light->getPropfv(Light::POSITION);
	pg->SetPropertyValue(wxT("POSITION.X"),v.x);
	pg->SetPropertyValue(wxT("POSITION.Y"),v.y);
	pg->SetPropertyValue(wxT("POSITION.Z"),v.z);

	v = light->getPropfv(Light::DIRECTION);
	pg->SetPropertyValue(wxT("DIRECTION.X"),v.x);
	pg->SetPropertyValue(wxT("DIRECTION.Y"),v.y);
	pg->SetPropertyValue(wxT("DIRECTION.Z"),v.z);

	v = light->getPropfv(Light::DIFFUSE);
	pg->SetPropertyValue(wxT("DIFFUSE.RGB"),
					wxColour(255*v.x,255*v.y,255*v.z));

	v = light->getPropfv(Light::AMBIENT);
	pg->SetPropertyValue(wxT("AMBIENT.RGB"),
					wxColour(255*v.x,255*v.y,255*v.z));

	v = light->getPropfv(Light::SPECULAR);
	pg->SetPropertyValue(wxT("SPECULAR.RGB"),
					wxColour(255*v.x,255*v.y,255*v.z));

	float ff;

	ff = light->getPropf(Light::SPOT_CUTOFF);
	pg->SetPropertyValue(wxT("SPOT_CUTOFF"),ff);

	ff = light->getPropf(Light::SPOT_EXPONENT);
	pg->SetPropertyValue(wxT("SPOT_EXPONENT"),ff);

	ff = light->getPropf(Light::CONSTANT_ATT);
	pg->SetPropertyValue(wxT("CONSTANT_ATT"),ff);

	ff = light->getPropf(Light::LINEAR_ATT);
	pg->SetPropertyValue(wxT("LINEAR_ATT"),ff);

	ff = light->getPropf(Light::QUADRATIC_ATT);
	pg->SetPropertyValue(wxT("QUADRATIC_ATT"),ff);

}



void DlgLights::OnPropsChange( wxPropertyGridEvent& e) {

	curitiba::scene::Light *light = RENDERMANAGER->getLight(m_active);
	const wxString& name = e.GetPropertyName();
	unsigned int dotLocation = name.find_first_of(wxT("."),0);
	std::string topProp = std::string(name.substr(0,dotLocation).mb_str());
	std::string prop = std::string(name.substr(dotLocation+1,name.size()-dotLocation-1).mb_str());
	wxVariant variant;
	wxColour col;

//	pg->ClearSelection();
	curitiba::math::vec4 v1,v2;
	if (topProp == "ENABLED")
		light->setProp(Light::ENABLED, e.GetPropertyValue().GetBool() != 0);


	if (topProp == "POSITION") {
		v1 = light->getPropfv(Light::POSITION);
		if      (prop == "X") {
			v2.set(e.GetPropertyValue().GetDouble(),v1.y,v1.z);
			light->setProp(Light::POSITION,v2);
		}
		else if (prop == "Y") {
			v2.set(v1.x,e.GetPropertyValue().GetDouble(),v1.z);
			light->setProp(Light::POSITION,v2);
		}
		else if (prop == "Z") {
			v2.set(v1.x,v1.y,e.GetPropertyValue().GetDouble());
			light->setProp(Light::POSITION,v2);
		}
	}
	else if (topProp == "DIRECTION") {
		v1 = light->getPropfv(Light::DIRECTION);
		if      (prop == "X") {
			v2.set(e.GetPropertyValue().GetDouble(),v1.y,v1.z, 0.0);
			light->setProp(Light::DIRECTION,v2);
		}
		else if (prop == "Y") {
			v2.set(v1.x,e.GetPropertyValue().GetDouble(),v1.z, 0.0);
			light->setProp(Light::DIRECTION,v2);
		}
		else if (prop == "Z") {
			v2.set(v1.x,v1.y,e.GetPropertyValue().GetDouble(), 0.0);
			light->setProp(Light::DIRECTION,v2);
		}
	}	
		
	else if (topProp == "TYPE") {

		int op = e.GetPropertyValue().GetInteger();
		light->setProp(Light::TYPE, (Light::EnumProperty)op);

		if (op == Light::DIRECTIONAL) {	
			pg->DisableProperty(wxT("POSITION"));
			pg->EnableProperty(wxT("DIRECTION"));
			pg->DisableProperty(wxT("SPOT_CUTOFF"));
			pg->DisableProperty(wxT("SPOT_EXPONENT"));
			pg->DisableProperty(wxT("CONSTANT_ATT"));
			pg->DisableProperty(wxT("LINEAR_ATT"));
			pg->DisableProperty(wxT("QUADRATIC_ATT"));
		}
		else if (op == Light::POSITIONAL){
			pg->EnableProperty(wxT("POSITION"));
			pg->DisableProperty(wxT("DIRECTION"));
			pg->DisableProperty(wxT("SPOT_CUTOFF"));
			pg->DisableProperty(wxT("SPOT_EXPONENT"));
			pg->EnableProperty(wxT("CONSTANT_ATT"));
			pg->EnableProperty(wxT("LINEAR_ATT"));
			pg->EnableProperty(wxT("QUADRATIC_ATT"));
		}
		else if (op == Light::SPOT_LIGHT){
			pg->EnableProperty(wxT("POSITION"));
			pg->EnableProperty(wxT("DIRECTION"));
			pg->EnableProperty(wxT("SPOT_CUTOFF"));
			pg->EnableProperty(wxT("SPOT_EXPONENT"));
			pg->EnableProperty(wxT("CONSTANT_ATT"));
			pg->EnableProperty(wxT("LINEAR_ATT"));
			pg->EnableProperty(wxT("QUADRATIC_ATT"));
		}
	}

	else if (topProp == "DIFFUSE") {
		variant = pg->GetPropertyValue(wxT("DIFFUSE.RGB"));
		col << variant;
		light->setProp(Light::DIFFUSE, vec4(col.Red()/255.0f, col.Green()/255.0f, col.Blue()/255.0f, 1.0));
	}
	else if (topProp == "AMBIENT") {
		variant = pg->GetPropertyValue(wxT("AMBIENT.RGB"));
		col << variant;
		light->setProp(Light::AMBIENT, vec4(col.Red()/255.0f, col.Green()/255.0f, col.Blue()/255.0f, 1.0));
	}
	else if (topProp == "SPECULAR") {
		variant = pg->GetPropertyValue(wxT("SPECULAR.RGB"));
		col << variant;
		light->setProp(Light::SPECULAR, vec4(col.Red()/255.0f, col.Green()/255.0f, col.Blue()/255.0f, 1.0));
	}
	else if (topProp == "SPOT_CUTOFF") {
		light->setProp(Light::SPOT_CUTOFF, e.GetPropertyValue().GetDouble());
	}
	else if (topProp == "SPOT_EXPONENT") {
		light->setProp(Light::SPOT_EXPONENT, e.GetPropertyValue().GetDouble());
	}
	else if (topProp == "CONSTANT_ATT") {
		light->setProp(Light::CONSTANT_ATT, e.GetPropertyValue().GetDouble());
	}
	else if (topProp == "LINEAR_ATT") {
		light->setProp(Light::LINEAR_ATT, e.GetPropertyValue().GetDouble());
	}
	else if (topProp == "QUADRATIC_ATT") {
		light->setProp(Light::QUADRATIC_ATT, e.GetPropertyValue().GetDouble());
	}
	notifyUpdate(PROPS_CHANGED,m_active,topProp);
}


void DlgLights::updateInfo(std::string name) {

	if (name == m_active) {
		update();
	}
}


void DlgLights::OnAdd(wxCommandEvent& event) {

	int result;
	bool nameUnique,exit;
	std::string name;

	do {
		wxTextEntryDialog dialog(this,
								 _T("(the name must be unique)\n")
								 _T("Please Input a Light Name"),
								 _T("Light Name\n"),
								 _T(""),
								 wxOK | wxCANCEL);

		result = dialog.ShowModal();
		name = std::string(dialog.GetValue().mb_str());
		nameUnique =  !RENDERMANAGER->hasLight(name); 

		if (!nameUnique && (result == wxID_OK)){
			wxMessageBox(_T("Light name must be unique") , _T("Light Name Error"), wxOK | wxICON_INFORMATION, this);
		}
		if (name == "" && (result == wxID_OK)){
			wxMessageBox(_T("Light name must not be void"), _T("Light Name Error"), wxOK | wxICON_INFORMATION, this);
		}

		if (result == wxID_CANCEL) {
			exit = true;
		}
		else if (nameUnique && name != "") {
			exit = true;
		}

	} while (!exit);

	if (result == wxID_OK) {
		RENDERMANAGER->getLight(name);
		updateList();
		list->Select(list->FindString((wxChar *)name.c_str()));
		m_active = name;
		update();
		notifyUpdate(NEW_LIGHT,name,"");
	}
}

void DlgLights::OnListSelect(wxCommandEvent& event){

	int sel;

	sel = event.GetSelection();
	m_active = std::string(list->GetString(sel).mb_str());
	update();
//	parent->Refresh();
}
