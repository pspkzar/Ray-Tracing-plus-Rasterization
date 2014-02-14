#include "dialogs/dlgScenes.h"
#include "main.h"

#include <curitiba/scene/iscenepartitioned.h>
#include <curitiba/event/eventFactory.h>
#include <curitiba/math/utils.h>
#include <curitiba/geometry/iboundingvolume.h>

#ifdef CURITIBA_PLATFORM_WIN32
#include <curitiba/system/dirent.h>
#else
#include <dirent.h>
#include <sys/types.h>
#endif

#include <wx/dir.h>

using namespace curitiba::geometry;

BEGIN_EVENT_TABLE(DlgScenes, wxDialog)

	EVT_COMBOBOX(DLG_COMBO, DlgScenes::OnListSelect)

	EVT_MENU(NEW_SCENE,DlgScenes::OnNewScene)
	EVT_MENU(SAVE_SCENE, DlgScenes::OnSaveScene)
	EVT_MENU(ADD_FILE,DlgScenes::OnAddFile)
	EVT_MENU(ADD_DIR,DlgScenes::OnAddDir)
	EVT_MENU(COMPILE, DlgScenes::OnCompile)
	EVT_MENU(BUILD, DlgScenes::OnBuild)
	
END_EVENT_TABLE()

wxWindow *DlgScenes::parent = NULL;
DlgScenes *DlgScenes::inst = NULL;


void 
DlgScenes::SetParent(wxWindow *p) 
{
	parent = p;
}

DlgScenes* 
DlgScenes::Instance () 
{
	if (inst == NULL)
		inst = new DlgScenes();

	return inst;
}



DlgScenes::DlgScenes()
	: wxDialog(DlgScenes::parent, -1, wxT("Slanger - Scenes"),wxDefaultPosition,wxDefaultSize,wxRESIZE_BORDER|wxDEFAULT_DIALOG_STYLE)                
{
	m_Parent = DlgScenes::parent;
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	/* ----------------------------------------------------------------
	                             Toolbar
	-----------------------------------------------------------------*/

    // load the image wanted on the toolbar
    wxBitmap newImage(wxT("bitmaps/new.bmp"), wxBITMAP_TYPE_BMP);
    wxBitmap cutImage(wxT("bitmaps/cut.bmp"), wxBITMAP_TYPE_BMP);

//	wxBitmap aboutImage = wxBITMAP(help) ;

    // create the toolbar and add our 1 tool to it
    m_toolbar = new wxToolBar(this,TOOLBAR_ID);
	long tstyle = m_toolbar->GetWindowStyle();
	tstyle |= wxTB_TEXT;
	m_toolbar->SetWindowStyle(tstyle);

    m_toolbar->AddTool(NEW_SCENE, _("New"), newImage, _("New Scene"));
    m_toolbar->AddTool(SAVE_SCENE, _("Save"), newImage, _("Save Scene as CBO"));

 	m_toolbar->AddSeparator();
    m_toolbar->AddTool(ADD_FILE, _("Model"), cutImage, _("Add Model"));
    m_toolbar->AddTool(ADD_DIR, _("Folder"), cutImage, _("Add Folder with Models"));

 	m_toolbar->AddSeparator();
    m_toolbar->AddTool(BUILD, _("Build"), cutImage, _("Build Octree"));
    m_toolbar->AddTool(COMPILE, _("Compile"), newImage, _("Compile Scene"));

	m_toolbar->Realize();

	sizer->Add(m_toolbar,0,wxGROW |wxALL, 1);

	/* ----------------------------------------------------------------
	                             End Toolbar
	-----------------------------------------------------------------*/

	setupPanel(sizer,parent);

	SetAutoLayout(TRUE);
    sizer->SetSizeHints(this);
    sizer->Fit(this);
    SetSizer(sizer);

	this->SetTitle(wxT("Slanger - Scenes"));
	this->SetSize(350,400);
}


void DlgScenes::updateDlg() {
	
	updateList();
	list->SetSelection(0);
	update();
	m_parent->Refresh();	
}


void DlgScenes::setupPanel(wxSizer *siz, wxWindow *parent) {


	/* TOP: COMBO with scene names */
	wxBoxSizer *sizH1 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText *stg1 =  new wxStaticText(this,-1,wxT("Scene: "));
	list = new wxComboBox(this,DLG_COMBO,wxT(""),wxDefaultPosition,wxDefaultSize,0,NULL,wxCB_READONLY );

	updateList();
	list->SetSelection(0);

	sizH1->Add(stg1, 2, wxGROW|wxALL|wxHORIZONTAL,5);
	sizH1->Add(list, 5, wxGROW|wxALL|wxHORIZONTAL,5);

	wxStaticBox *sb = new wxStaticBox(this,-1,wxT(" Properties "));
	wxSizer *sizer = new wxStaticBoxSizer(sb,wxHORIZONTAL);

		wxFlexGridSizer *gs = new wxFlexGridSizer(3,2,0,0);

		wxStaticText *st1 = new wxStaticText(this,-1,wxT("Type:"));
		tType = new wxStaticText(this,-1,wxT("Octree"));

		wxStaticText *st3 = new wxStaticText(this,-1,wxT("BB Min:"));
		tBoundingBoxMin = new wxStaticText(this,-1,wxT("0.00000f, 0.00000f, 0.00000f"));

		wxStaticText *st5 = new wxStaticText(this,-1,wxT("BB Max :"));
		tBoundingBoxMax = new wxStaticText(this,-1,wxT("0.00000f, 0.00000f, 0.00000f"));

		gs ->Add(st1,0,wxALL,3);
		gs ->Add(tType,0,wxALL,3);
		gs ->Add(st3,0,wxALL,3);
		gs ->Add(tBoundingBoxMin,0,wxALL,3);
		gs ->Add(st5,0,wxALL,3);
		gs ->Add(tBoundingBoxMax,0,wxALL,3);

	sizer->Add(gs,0,wxGROW|wxALL,5);



	wxStaticBox *sb3 = new wxStaticBox(this,-1,wxT(" Scene Objects "));
	wxSizer *sizer3 = new wxStaticBoxSizer(sb3,wxVERTICAL);

	objects = new wxListBox(this,-1);//,wxDefaultPosition,wxDefaultSize,li);

	sizer3->Add(objects,1,wxGROW|wxEXPAND|wxALL,5);

	siz->Add(sizH1, 0, wxGROW|wxALL | wxHORIZONTAL, 5);
	siz->Add(sizer,	0,wxALL |wxGROW |wxHORIZONTAL,5);
	siz->Add(sizer3,1,wxALL |wxGROW |wxHORIZONTAL,5);

}


void DlgScenes::updateList() {

	std::vector<std::string> *names = RENDERMANAGER->getSceneNames();
	int num = names->size();

	list->Clear();

	for(int i = 0; i < num; i++)  {
		wxString s;
		s << i;
		list->Append(wxString(names->at(i).c_str()));
	}
	m_Active = names->at(0);
	delete names;
}


void DlgScenes::update() 
{
	IScene *scene = RENDERMANAGER->getScene(m_Active);

	/* Is scene a partitioned scene? */
	if (scene->getType() == "Octree" || scene->getType() == "OctreeByMatScene") {
	
		IScenePartitioned *part = (IScenePartitioned *)scene;

		if (part->isBuilt() || part->isCompiled())
			m_toolbar->EnableTool(BUILD, FALSE);
		else
			m_toolbar->EnableTool(BUILD, TRUE);
	}
	else
		m_toolbar->EnableTool(BUILD, FALSE);

	if (scene->isCompiled() || (scene->getType() == "ScenePoses") || (scene->getType() == "SceneSkeleton"))
		m_toolbar->EnableTool(COMPILE, FALSE);
	else
		m_toolbar->EnableTool(COMPILE, TRUE);

	tType->SetLabel(wxString(scene->getType().c_str()));
	IBoundingVolume &bv = scene->getBoundingVolume();
	wxString s;
	s.Printf("(%f,%f,%f)",bv.getMax().x, bv.getMax().y, bv.getMax().z);
	tBoundingBoxMax->SetLabel(s);
	s.Printf("(%f,%f,%f)",bv.getMin().x, bv.getMin().y, bv.getMin().z);
	tBoundingBoxMin->SetLabel(s);

	objects->Clear();
	std::vector<SceneObject*> objList = scene->getAllObjects();
	std::vector<SceneObject*>::iterator iter = objList.begin();

	for ( ; iter != objList.end(); ++iter )
		objects->AppendString(wxString((*iter)->getName().c_str()));

}


void DlgScenes::OnPropsChange( wxPropertyGridEvent& e) 
{

}

void 
DlgScenes::OnSaveScene( wxCommandEvent& event)
{
	wxFileDialog *saveOctDlg = 
		new wxFileDialog (this, _("Save As CBO"), _(""), _(""), _(""), wxFD_SAVE, wxDefaultPosition);

	if (wxID_OK == saveOctDlg->ShowModal ()) {
		curitiba::CURITIBA->writeAssets("CBO", (const char *)(saveOctDlg->GetPath()).c_str(), m_Active);
	}		

}


void 
DlgScenes::OnCompile( wxCommandEvent& event)
{
	IScene *scene = RENDERMANAGER->getScene(m_Active);
	((FrmMainFrame *)parent)->compile(scene);
	//scene->compile();

	m_toolbar->EnableTool(COMPILE, FALSE);
	m_toolbar->EnableTool(BUILD, FALSE);
}

	
void 
DlgScenes::OnBuild( wxCommandEvent& event)
{
	IScenePartitioned *part = (IScenePartitioned *) RENDERMANAGER->getScene(m_Active);
	part->build();

	m_toolbar->EnableTool(BUILD, FALSE);
	update();
}


void DlgScenes::updateInfo(std::string name) 
{
	if (name == m_Active) 
		update();
}


void DlgScenes::OnNewScene(wxCommandEvent& event) 
{
	int result;
	bool nameUnique,exit = false;
	std::string name;

	do {
		wxTextEntryDialog dialog(this,
								 _T("(the name must be unique)\n")
								 _T("Please Input a Scene Name"),
								 _T("Scene Name\n"),
								 _T(""),
								 wxOK | wxCANCEL);

		result = dialog.ShowModal();
		name = std::string(dialog.GetValue().mb_str());
		nameUnique =  !RENDERMANAGER->hasScene(name); 

		if (!nameUnique && (result == wxID_OK)){
			wxMessageBox(_T("Scene name must be unique") , _T("Scene Name Error"), wxOK | wxICON_INFORMATION, this);
		}
		if (name == "" && (result == wxID_OK)){
			wxMessageBox(_T("Scene name must not be void"), _T("Scene Name Error"), wxOK | wxICON_INFORMATION, this);
		}

		if (result == wxID_CANCEL) {
			exit = true;
		}
		else if (nameUnique && name != "") {
			exit = true;
		}

	} while (!exit);

	if (result == wxID_OK) {
		RENDERMANAGER->createScene(name);
		updateList();
		list->Select(list->FindString(wxString(name.c_str())));
		m_Active = name;
		update();
		EVENTMANAGER->notifyEvent("NEW_SCENE", m_Active,"", NULL);
	}
}



void DlgScenes::OnAddFile(wxCommandEvent& event) 
{
	static const wxChar *fileTypes = _T( "3D Files (*.cbo, *.3ds, *.dae, *.obj, *.xml)|*.cbo;*.3ds;*.dae;*.obj; *.xml|CBO files (*.cbo)|*.cbo|COLLADA files (*.dae)|*.dae|3DS files (*.3ds)|*.3ds|OBJ files (*.obj)|*.obj|Ogre XML Meshes (*.xml)|*.xml");
	wxFileDialog *openFileDlg = new wxFileDialog (this, _("Open File"), _(""), _(""), fileTypes, wxFD_OPEN, wxDefaultPosition);

	if (wxID_OK == openFileDlg->ShowModal ()) {
		wxStopWatch aTimer;
		aTimer.Start();
		wxString path = openFileDlg->GetPath ();

		try {
			CURITIBA->loadAsset (std::string(path.mb_str()), m_Active);
			EVENTMANAGER->notifyEvent("SCENE_CHANGED", m_Active,"", NULL);

			char buffer [1024];
			sprintf (buffer, "Elapsed time: %f", aTimer.Time()/1000.0);
			wxMessageBox (wxString(buffer));
		}
		catch (std::string &s) {
			wxMessageBox(wxString(s.c_str()));
		}
	}
	IScene *scene = RENDERMANAGER->getScene(m_Active);
	IBoundingVolume &bv = scene->getBoundingVolume();
	wxString s;
	s.Printf("(%f,%f,%f)",bv.getMax().x, bv.getMax().y, bv.getMax().z);
	tBoundingBoxMax->SetLabel(s);
	s.Printf("(%f,%f,%f)",bv.getMin().x, bv.getMin().y, bv.getMin().z);
	tBoundingBoxMin->SetLabel(s);

}


void DlgScenes::OnAddDir(wxCommandEvent& event) 
{
	DIR *dir;
	struct dirent *ent;
	bool result = true;
	char fileName [1024];

	wxDirDialog *openDirDlg = new wxDirDialog (this);

	wxStopWatch aTimer;

	if (wxID_OK == openDirDlg->ShowModal()) {
		aTimer.Start();
		wxDir directory (openDirDlg->GetPath());

		try {
			dir = opendir ((const char *)directory.GetName().c_str());

			if (0 == dir) {
				wxMessageBox(wxT("Can't open folder"));
			}
			while (0 != (ent = readdir (dir))) {

		#ifdef CURITIBA_PLATFORM_WIN32
				sprintf (fileName, "%s\\%s", directory.GetName().c_str(), ent->d_name);
		#else
				sprintf (fileName, "%s/%s", dirName, ent->d_name);						
		#endif
				try {
					CURITIBA->loadAsset (fileName, m_Active);
				}
				catch(std::string &s) {
					closedir(dir);
					wxMessageBox(wxString(s.c_str()));
				}
			}
			EVENTMANAGER->notifyEvent("SCENE_CHANGED", m_Active,"", NULL);
			closedir (dir);

#ifndef FINAL
			char buffer [1024];
			sprintf (buffer, "Elapsed time: %f", aTimer.Time()/1000.0);
				
			wxMessageBox (wxString(buffer));
#endif		
		}
		catch(std::string &s) {
			wxMessageBox(wxString(s.c_str()));
		}
	}

}



void DlgScenes::OnListSelect(wxCommandEvent& event)
{
	int sel;

	sel = event.GetSelection();
	m_Active = std::string(list->GetString(sel).mb_str());
	update();
}
