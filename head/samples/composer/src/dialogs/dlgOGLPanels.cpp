#include "dlgOGLPanels.h"
#include <curitiba/render/istate.h>
#include <GL/glew.h>





DlgOGLPanels::DlgOGLPanels() {
	
}


DlgOGLPanels::~DlgOGLPanels(){

}


void DlgOGLPanels::setState(IState *aState) {

	m_glState = aState;
}


//void DlgOGLPanels::setCanvas(OGLCanvas *glCanvas) {
//
//	m_glCanvas = glCanvas;
//}



// FOG //
//
void DlgOGLPanels::setPanel(wxSizer *siz, wxWindow *parent){

	wxStaticBox *sf = new wxStaticBox(parent,-1,wxT("State"));
	wxSizer *sizerf = new wxStaticBoxSizer(sf,wxVERTICAL);

	pg = new wxPropertyGridManager(parent, PG,
				wxDefaultPosition, wxDefaultSize,
				// These and other similar styles are automatically
				// passed to the embedded wxPropertyGrid.
				wxPG_BOLD_MODIFIED|
				// Plus defaults.
				wxPGMAN_DEFAULT_STYLE
           );
	pg->AddPage(wxT("Standard Items"));

	pg->Append( new wxPropertyCategory(wxT("Fog"),wxPG_LABEL) );
	//pg->AppendCategory("Fog");
	
	const wxChar* fogF[] =  {wxT("LINEAR"),wxT("EXP"),wxT("EXP2"),NULL};
	const long fogFInd[] =  {IState::LINEAR,IState::EXP,IState::EXP2,NULL};

	const wxChar* enable[] = { wxT("Disabled"), wxT("Enabled"), NULL };
	const long enableInd[] = { 0, 1, NULL };

	pg->Append(new  wxEnumProperty(wxT("FOG"),wxPG_LABEL,enable,enableInd,m_glState->getSetting(IState::FOG)));

	vec4 fColor;
	fColor = m_glState->getProp4f(IState::FOG_COLOR);
	pg->Append(new  wxColourProperty(wxT("FOG_COLOR"),wxPG_LABEL,
				wxColour((unsigned char)fColor.x*255,(unsigned char)fColor.y*255,(unsigned char)fColor.z*255)) );
	
	pg->Append( new wxEnumProperty(wxT("FOG_MODE"),wxPG_LABEL,fogF,fogFInd,m_glState->getPrope(IState::FOG_MODE)));

	pg->Append(new  wxFloatProperty(wxT("FOG_DENSITY"),wxPG_LABEL,m_glState->getPropf(IState::FOG_DENSITY)));

	pg->Append(new  wxFloatProperty(wxT("FOG_START"),wxPG_LABEL,m_glState->getPropf(IState::FOG_START)));

	pg->Append(new  wxFloatProperty(wxT("FOG_END"),wxPG_LABEL,m_glState->getPropf(IState::FOG_END)));

	if (m_glState->getPrope(IState::FOG_MODE) == IState::LINEAR) {
		pg->EnableProperty(wxT("FOG_END"));
		pg->EnableProperty(wxT("FOG_START"));
		pg->DisableProperty(wxT("FOG_DENSITY"));
	}
	else {
		pg->EnableProperty(wxT("FOG_DENSITY"));
		pg->DisableProperty(wxT("FOG_START"));
		pg->DisableProperty(wxT("FOG_END"));
	}


	// BLEND
//	pg->AppendCategory("Blend");
	pg->Append( new wxPropertyCategory(wxT("Blend"),wxPG_LABEL) );

	const wxChar* blendF[] =  {wxT("ZERO"),wxT("ONE"),wxT("SRC_COLOR"),wxT("ONE_MINUS_SRC_COLOR"),wxT("DST_COLOR"),
							wxT("ONE_MINUS_DST_COLOR"),wxT("SRC_ALPHA"),wxT("ONE_MINUS_SRC_ALPHA"),wxT("DST_ALPHA"),
							wxT("ONE_MINUS_DST_ALPHA"),wxT("SRC_ALPHA_SATURATE"),wxT("CONSTANT_COLOR"),wxT("ONE_MINUS_CONSTANT_COLOR"),
							wxT("CONSTANT_ALPHA"),wxT("ONE_MINUS_CONSTANT_ALPHA"),NULL};
	const long blendFInd[] =  {IState::ZERO,IState::ONE,IState::SRC_COLOR,IState::ONE_MINUS_SRC_COLOR,IState::DST_COLOR,
							IState::ONE_MINUS_DST_COLOR,IState::SRC_ALPHA,IState::ONE_MINUS_SRC_ALPHA,IState::DST_ALPHA,
							IState::ONE_MINUS_DST_ALPHA,IState::SRC_ALPHA_SATURATE,IState::CONSTANT_COLOR,IState::ONE_MINUS_CONSTANT_COLOR,
							IState::CONSTANT_ALPHA,IState::ONE_MINUS_CONSTANT_ALPHA,NULL};



	const wxChar* blendEq[] =  {wxT("ADD"),wxT("SUBTRACT"),wxT("REVERSE_SUBTRACT"),wxT("MIN"),wxT("MAX"),NULL};
	const long blendEqInd[] = {IState::ADD,IState::SUBTRACT,IState::REVERSE_SUBTRACT,IState::MIN,IState::MAX,0};

	pg->Append( new wxEnumProperty(wxT("BLEND"),wxPG_LABEL,enable,enableInd,m_glState->getSetting(IState::BLEND)));

	fColor = m_glState->getProp4f(IState::BLEND_COLOR);
	pg->Append( new wxColourProperty(wxT("BLEND_COLOR"),wxPG_LABEL,
				wxColour((unsigned char)fColor.x*255,(unsigned char)fColor.y*255,(unsigned char)fColor.z*255)) );

	pg->Append( new wxEnumProperty(wxT("BLEND_EQUATION"),wxPG_LABEL,blendEq,blendEqInd,m_glState->getPrope(IState::BLEND_EQUATION)));

	pg->Append( new wxEnumProperty(wxT("BLEND_SRC"),wxPG_LABEL,blendF,blendFInd,m_glState->getPrope(IState::BLEND_SRC)));

	pg->Append(new wxEnumProperty(wxT("BLEND_DST"),wxPG_LABEL,blendF,blendFInd,m_glState->getPrope(IState::BLEND_DST)));


	pg->Append( new wxPropertyCategory(wxT("Alpha"),wxPG_LABEL) );	

	pg->Append( new wxEnumProperty(wxT("ALPHA_TEST"),wxPG_LABEL,enable,enableInd,m_glState->getSetting(IState::ALPHA_TEST)));

	const wxChar* alphaF[] =  {wxT("ALWAYS"),wxT("NEVER"),  wxT("LESS"), wxT("LEQUAL"),
				wxT("EQUAL"), wxT("GEQUAL"),wxT("GREATER"), wxT("NOT_EQUAL"), NULL};
	const long alphaFInd[] =  {IState::ALWAYS, IState::NEVER, IState::LESS, IState::LEQUAL,
				IState::EQUAL, IState::GEQUAL, IState::GREATER, IState::NOT_EQUAL,NULL};

	pg->Append( new wxEnumProperty(wxT("ALPHA_FUNC"),wxPG_LABEL,alphaF,alphaFInd,m_glState->getPrope(IState::ALPHA_FUNC)));

	pg->Append(new  wxFloatProperty(wxT("ALPHA_REF"),wxPG_LABEL,m_glState->getPropf(IState::ALPHA_VALUE)));

	pg->Append( new wxPropertyCategory(wxT("Cull"), wxPG_LABEL));
	pg->Append( new wxEnumProperty(wxT("CULL"), wxPG_LABEL,enable,enableInd,m_glState->getSetting(IState::CULL_FACE)));
	const wxChar* cullFace[] =  {wxT("FRONT"),wxT("BACK"),NULL};
	const long cullFaceInd[] =  {IState::FRONT_FACE, IState::BACK_FACE,NULL};
	pg->Append( new wxEnumProperty(wxT("FACE"),wxPG_LABEL,cullFace,cullFaceInd,m_glState->getPrope(IState::CULL_TYPE)));

	pg->Append( new wxPropertyCategory(wxT("Depth Test"), wxPG_LABEL));
		pg->Append( new wxEnumProperty(wxT("DEPTH TEST"), wxPG_LABEL,enable,enableInd,m_glState->getSetting(IState::DEPTH_TEST)));
	const wxChar* depthFunc[] =  {wxT("ALWAYS"),wxT("NEVER"),  wxT("LESS"), wxT("LEQUAL"),
				wxT("EQUAL"), wxT("GEQUAL"),wxT("GREATER"), wxT("NOT_EQUAL"), NULL};
	const long depthFuncInd[] =  {IState::ALWAYS, IState::NEVER, IState::LESS, IState::LEQUAL,
				IState::EQUAL, IState::GEQUAL, IState::GREATER, IState::NOT_EQUAL,NULL};
	pg->Append( new wxEnumProperty(wxT("FUNC"),wxPG_LABEL,depthFunc,depthFuncInd,m_glState->getPrope(IState::DEPTH_FUNC)));

	sizerf->Add(pg,1,wxEXPAND);
	siz->Add(sizerf,1,wxGROW|wxEXPAND|wxALL,5);
}


void DlgOGLPanels::OnProcessPanelChange(wxPropertyGridEvent& e){


	const wxString& name = e.GetPropertyName();
	wxString s;
	wxColour colour;
	wxVariant variant;
	float f[4];

	// FOG
	if (name== wxT("FOG")) {
		m_glState->enable(IState::FOG,pg->GetPropertyValueAsInt(name) != 0);
	}
	else if (name == wxT("FOG_COLOR")) {
		variant = pg->GetPropertyValue(wxT("FOG_COLOR"));
		colour << variant;
		//colour = (wxColour *)pg->GetPropertyValueAsWxObjectPtr("FOG_COLOR");
		f[0] = colour.Red()/255.0; f[1] = colour.Green()/255.0; f[2] = colour.Blue()/255.0; f[3] = 1.0;
		m_glState->setProp(IState::FOG_COLOR,f[0],f[1],f[2],1.0f);
	} 
	else if (name == wxT("FOG_MODE")) {

		int p = pg->GetPropertyValueAsInt(name);
		m_glState->setProp(IState::FOG_MODE,p);
		if ((IState::FogMode)p == IState::LINEAR) {
			pg->EnableProperty(wxT("FOG_END"));
			pg->EnableProperty(wxT("FOG_START"));
			pg->DisableProperty(wxT("FOG_DENSITY"));
		}
		else {
			pg->EnableProperty(wxT("FOG_DENSITY"));
			pg->DisableProperty(wxT("FOG_START"));
			pg->DisableProperty(wxT("FOG_END"));
		}
	}
	else if (name == wxT("FOG_DENSITY")) {
		m_glState->setProp(IState::FOG_DENSITY, (float)pg->GetPropertyValueAsDouble(name));
	}
	else if (name == wxT("FOG_START")) {
		m_glState->setProp(IState::FOG_START, (float)pg->GetPropertyValueAsDouble(name));
	}
	else if (name == wxT("FOG_END")) {
		m_glState->setProp(IState::FOG_END, (float)pg->GetPropertyValueAsDouble(name));
	}

	// BLEND
	else if (name == wxT("BLEND")) {
		m_glState->enable(IState::BLEND, 0 != pg->GetPropertyValueAsInt(name));
	}
	else if (name == wxT("BLEND_COLOR")) {
		variant = pg->GetPropertyValue(wxT("BLEND_COLOR"));
		colour << variant;
		//colour = (wxColour *)pg->GetPropertyValueAsWxObjectPtr("BLEND_COLOR");
		f[0] = colour.Red()/255.0; f[1] = colour.Green()/255.0; f[2] = colour.Blue()/255.0; f[3] = 1.0;
		m_glState->setProp(IState::BLEND_COLOR,f[0],f[1],f[2],1.0f);
	}
	else if (name == wxT("BLEND_EQUATION")) {
		m_glState->setProp(IState::BLEND_EQUATION, pg->GetPropertyValueAsInt(name));
	}
	else if (name == wxT("BLEND_SRC")) {
		m_glState->setProp(IState::BLEND_SRC, pg->GetPropertyValueAsInt(name));
	}
	else if (name == wxT("BLEND_DST")) {
		m_glState->setProp(IState::BLEND_DST, pg->GetPropertyValueAsInt(name));
	}	


	// ALPHA TEST
	else if (name == wxT("ALPHA_TEST")) {
		m_glState->enable(IState::ALPHA_TEST, 0 != pg->GetPropertyValueAsInt(name));
	}
	else if (name == wxT("ALPHA_FUNC")) {
		m_glState->setProp(IState::ALPHA_FUNC, pg->GetPropertyValueAsInt(name));	
	}
	else if (name == wxT("ALPHA_REF")) {
		m_glState->setProp(IState::ALPHA_VALUE, (float)pg->GetPropertyValueAsDouble(name));
	}

	else if (name == wxT("CULL")) {
		m_glState->enable(IState::CULL_FACE,  0 != pg->GetPropertyValueAsInt(name));
	}
	else if (name == wxT("FACE")) {
		m_glState->setProp(IState::CULL_TYPE, pg->GetPropertyValueAsInt(name));	
	}
	else if (name == wxT("DEPTH TEST")) {
		m_glState->enable(IState::DEPTH_TEST,  0 != pg->GetPropertyValueAsInt(name));
	}
	else if (name == wxT("FUNC")) {
		m_glState->setProp(IState::DEPTH_FUNC, pg->GetPropertyValueAsInt(name));	
	}
}


void DlgOGLPanels::updatePanel(){

	//FOG

	vec4 v;
	pg->SetPropertyValue(wxT("FOG"),(int)m_glState->getSetting(IState::FOG));
	pg->SetPropertyValue(wxT("FOG_MODE"),m_glState->getPrope(IState::FOG_MODE));
	v = m_glState->getProp4f(IState::FOG_COLOR);
	pg->SetPropertyValue(wxT("FOG_COLOR"),
				wxColour(255*v.x,
						 255*v.y,
						 255*v.z));

	pg->SetPropertyValue(wxT("FOG_DENSITY"),m_glState->getPrope(IState::FOG_MODE));
	pg->SetPropertyValue(wxT("FOG_START"),m_glState->getPropf(IState::FOG_START));
	pg->SetPropertyValue(wxT("FOG_END"),m_glState->getPropf(IState::FOG_END));

	if (m_glState->getPrope(IState::FOG_MODE) == IState::LINEAR) {
		pg->EnableProperty(wxT("FOG_END"));
		pg->EnableProperty(wxT("FOG_START"));
		pg->DisableProperty(wxT("FOG_DENSITY"));
	}
	else {
		pg->EnableProperty(wxT("FOG_DENSITY"));
		pg->DisableProperty(wxT("FOG_START"));
		pg->DisableProperty(wxT("FOG_END"));
	}

	//BLEND
	pg->SetPropertyValue(wxT("BLEND"),(int)m_glState->getSetting(IState::BLEND));
	pg->SetPropertyValue(wxT("BLEND_EQUATION"),m_glState->getPrope(IState::BLEND_EQUATION));
	v = m_glState->getProp4f(IState::BLEND_COLOR);
	pg->SetPropertyValue(wxT("BLEND_COLOR"),
				wxColour(255*v.x,
						 255*v.y,
						 255*v.z));

	pg->SetPropertyValue(wxT("BLEND_SRC"),m_glState->getPrope(IState::BLEND_SRC));
	pg->SetPropertyValue(wxT("BLEND_DST"),m_glState->getPrope(IState::BLEND_DST));

	//ALPHA
	pg->SetPropertyValue(wxT("ALPHA_TEST"),(int)m_glState->getSetting(IState::ALPHA_TEST));	
	pg->SetPropertyValue(wxT("ALPHA_FUNC"),m_glState->getPrope(IState::ALPHA_FUNC));
	pg->SetPropertyValue(wxT("ALPHA_REF"),m_glState->getPropf(IState::ALPHA_VALUE));

	//CULL
	pg->SetPropertyValue(wxT("CULL"),(int)m_glState->getSetting(IState::CULL_FACE));	
	pg->SetPropertyValue(wxT("FACE"),m_glState->getPrope(IState::CULL_TYPE));

	//DEPTH
	pg->SetPropertyValue(wxT("DEPTH TEST"),(int)m_glState->getSetting(IState::DEPTH_TEST));	
	pg->SetPropertyValue(wxT("FUNC"),m_glState->getPrope(IState::DEPTH_FUNC));
}


