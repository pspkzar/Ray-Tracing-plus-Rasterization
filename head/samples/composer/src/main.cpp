#pragma warning( disable: 4049)
#pragma warning( disable: 4005)
#pragma warning( disable: 4290)
#pragma warning( disable: 4299)
#pragma warning( disable: 4099)


#include <wx/filesys.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/splash.h>
#include <wx/bitmap.h>
#include <wx/image.h>

#include <fstream>

#include <main.h>
#include <glcanvas.h>
#include <curitiba/errors.h>
#include <curitiba/clogger.h>
#include <curitiba/slogger.h>

              
using namespace curitiba::math;

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//wxApplication
IMPLEMENT_APP(WndComposer);

bool WndComposer::OnInit()
{
    FrmMainFrame* frame = new FrmMainFrame (0L, wxT("curitiba engine (shaders edition)"));
    frame->Show();
    return true;
}

// Menu Options 
int idMenuProject = wxNewId();
int idMenuDir = wxNewId();
int idMenuModel = wxNewId();
int idMenuModelAppend = wxNewId();
int idMenuProcess = wxNewId();
int idMenuOctreeBuild = wxNewId();
int idMenuOctreeCompile = wxNewId();
int idMenuOctreeWrite = wxNewId();
int idMenuPhysicsBuild = wxNewId();
int idMenuQuit = wxNewId();
int idMenuAbout = wxNewId();

int idMenuWireframe = wxNewId();
int idMenuPoint = wxNewId();
int idMenuSolid = wxNewId();
int idMenuMaterial = wxNewId();

int idMenuRenderFlagBoundingBox = wxNewId();
int idMenuRenderFlagProfile = wxNewId();

int idMenuProfMatWhite = wxNewId();
int idMenuProfMatRed = wxNewId();
int idMenuProfMatGreen = wxNewId();
int idMenuProfMatBlue = wxNewId();
int idMenuProfMatBlack = wxNewId();

int idMenuPhysicsOn = wxNewId();
int idMenuPhysicsOff = wxNewId();
int idMenuMaterialsAll = wxNewId();

int idMenuReload = wxNewId();

// dialogs //

int idMenu_DLG_OGL = wxNewId();
int idMenu_DLG_TEXTURES = wxNewId();
int idMenu_DLG_CAMERAS = wxNewId();
int idMenu_DLG_MATERIALS = wxNewId();
int idMenu_DLG_LIGHTS = wxNewId();
int idMenu_DLG_SHADERS = wxNewId();
int idMenu_DLG_LOG = wxNewId();
int idMenu_DLG_SCENES = wxNewId();
int idMenu_DLG_PASS = wxNewId();
int idMenu_DLG_ATOMICS = wxNewId();



BEGIN_EVENT_TABLE(FrmMainFrame, wxFrame)
  EVT_MENU(idMenuProject, FrmMainFrame::OnProjectLoad)
  EVT_MENU(idMenuDir, FrmMainFrame::OnDirectoryLoad)
  EVT_MENU(idMenuModel, FrmMainFrame::OnModelLoad)
  EVT_MENU(idMenuModelAppend, FrmMainFrame::OnModelAppend)
  EVT_MENU(idMenuProcess, FrmMainFrame::OnProcess)
  EVT_MENU(idMenuOctreeBuild, FrmMainFrame::OnOctreeBuild)
  EVT_MENU(idMenuOctreeCompile, FrmMainFrame::OnOctreeCompile)
  EVT_MENU(idMenuOctreeWrite, FrmMainFrame::OnOctreeWrite)
  EVT_MENU(idMenuQuit, FrmMainFrame::OnQuit)
  EVT_MENU(idMenuAbout, FrmMainFrame::OnAbout)
  EVT_MENU_RANGE(idMenuWireframe, idMenuMaterial, FrmMainFrame::OnRenderMode)
  EVT_MENU(idMenuPhysicsBuild, FrmMainFrame::OnPhysicsBuild)
  EVT_MENU_RANGE(idMenuPhysicsOn, idMenuPhysicsOff, FrmMainFrame::OnPhysicsMode)
  EVT_MENU(idMenuRenderFlagBoundingBox, FrmMainFrame::OnSetRenderFlags)
  EVT_MENU(idMenuRenderFlagProfile, FrmMainFrame::OnSetRenderFlags)
  EVT_MENU(idMenuProfMatWhite, FrmMainFrame::OnSetProfileMaterial)
  EVT_MENU(idMenuProfMatRed, FrmMainFrame::OnSetProfileMaterial)
  EVT_MENU(idMenuProfMatGreen, FrmMainFrame::OnSetProfileMaterial)
  EVT_MENU(idMenuProfMatBlue, FrmMainFrame::OnSetProfileMaterial)
  EVT_MENU(idMenuProfMatBlack, FrmMainFrame::OnSetProfileMaterial)
  EVT_KEY_DOWN(FrmMainFrame::OnKeyDown)
  //EVT_IDLE(FrmMainFrame::OnIdle)

  EVT_MENU(idMenu_DLG_OGL, FrmMainFrame::OnDlgOGL)
  EVT_MENU(idMenu_DLG_TEXTURES, FrmMainFrame::OnDlgTextures)
  EVT_MENU(idMenu_DLG_CAMERAS, FrmMainFrame::OnDlgCameras)
  EVT_MENU(idMenu_DLG_MATERIALS, FrmMainFrame::OnDlgMaterials)
  EVT_MENU(idMenu_DLG_ATOMICS, FrmMainFrame::OnDlgAtomics)
  EVT_MENU(idMenu_DLG_LIGHTS, FrmMainFrame::OnDlgLights)
  EVT_MENU(idMenu_DLG_SHADERS, FrmMainFrame::OnDlgShaders)
  EVT_MENU(idMenu_DLG_LOG, FrmMainFrame::OnDlgLog)
  EVT_MENU(idMenu_DLG_SCENES, FrmMainFrame::OnDlgScenes)
  EVT_MENU(idMenu_DLG_PASS, FrmMainFrame::OnDlgPass)

 END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GlCanvas, wxGLCanvas)
  EVT_SIZE(GlCanvas::OnSize)
  EVT_PAINT(GlCanvas::OnPaint)
  EVT_ERASE_BACKGROUND(GlCanvas::OnEraseBackground)
  EVT_ENTER_WINDOW(GlCanvas::OnEnterWindow)
  EVT_KEY_UP(GlCanvas::OnKeyUp)
  EVT_KEY_DOWN(GlCanvas::OnKeyDown)
  EVT_MOTION(GlCanvas::OnMouseMove)
  EVT_LEFT_DOWN (GlCanvas::OnLeftDown)
  EVT_LEFT_UP (GlCanvas::OnLeftUp)
  EVT_IDLE(GlCanvas::OnIdle)
END_EVENT_TABLE()


//#undef wxUSE_MENUS
//#undef wxUSE_STATUSBAR
//#define FINAL

FrmMainFrame::FrmMainFrame (wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title), m_Canvas (0), materialsMenu (0), /*m_CurrentProject (0),*/ m_Width (0.0f), m_Height (0.0f), m_Inited (false)
{




#if wxUSE_MENUS
   // create a menu bar
	wxMenuBar* mbar = new wxMenuBar();
	wxMenu* fileMenu = new wxMenu(_T(""));
	fileMenu->Append(idMenuProject, _("&Open Project\tAlt-P"), _("Open a project file"));
	fileMenu->Append(idMenuDir, _("&Open Folder\tAlt-F"), _("Loads all files in a folder"));
	fileMenu->Append(idMenuModel, _("&Open Model\tAlt-M"), _("Reset and Loads a 3D Model"));
	fileMenu->Append(idMenuModelAppend, _("&Append Model\tAlt-M"), _("Appends a 3D Model to the scene"));
	fileMenu->Append(idMenuProcess, _("&Process Queue\tAlt-A"), _("Process queue"));
	fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
	mbar->Append(fileMenu, _("&File"));

	wxMenu* octreeMenu = new wxMenu(_T(""));
	octreeMenu->Append (idMenuOctreeBuild, _("&Build Octree"), _("Builds the Octree"));
	octreeMenu->Append (idMenuOctreeCompile, _("&Compile Octree"), _("Compiles the octree"));
	octreeMenu->Append (idMenuOctreeWrite, _("&Write Octree"), _("Write the octree to a file"));
	mbar->Append (octreeMenu, _("Octree"));

	wxMenu* renderMenu = new wxMenu(_T(""));
	renderMenu->AppendRadioItem (idMenuPoint, _("&Point"),_("Render in point mode"));
	renderMenu->AppendRadioItem (idMenuWireframe, _("&Wireframe"), _("Render in wireframe mode"));
	renderMenu->AppendRadioItem (idMenuSolid, _("&Solid"), _("Render in solid mode"));
	renderMenu->AppendRadioItem (idMenuMaterial, _("&Material"), _("Render with materials"));
	renderMenu->Check(idMenuMaterial, true);
	renderMenu->AppendSeparator ();
	renderMenu->AppendCheckItem(idMenuRenderFlagBoundingBox, _("Show &Bounding Boxes"));
	renderMenu->AppendSeparator();
	renderMenu->AppendCheckItem(idMenuRenderFlagProfile, _("Show &Profiler"));
	renderMenu->AppendRadioItem( idMenuProfMatWhite, _("White"));
	renderMenu->AppendRadioItem( idMenuProfMatRed, _("Red"));
	renderMenu->AppendRadioItem( idMenuProfMatGreen, _("Green"));
	renderMenu->AppendRadioItem( idMenuProfMatBlue, _("Blue"));
	renderMenu->AppendRadioItem( idMenuProfMatBlack, _("Black"));

	mbar->Append (renderMenu, _("&Render"));

	wxMenu* physicsMenu = new wxMenu(_T(""));
	physicsMenu->Append (idMenuPhysicsBuild, _("&Build physics"), _("Builds physics"));
	physicsMenu->AppendRadioItem (idMenuPhysicsOn, _("Physics On"), _("Physics On"));
	physicsMenu->AppendRadioItem (idMenuPhysicsOff, _("Physics Off"), _("Physics Off"));
	physicsMenu->Check (idMenuPhysicsOff, true);
	mbar->Append (physicsMenu, _("Physics"));

	helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    helpMenu->Append(idMenu_DLG_LOG, _("&Log\tF2"), _("Show Log"));
    helpMenu->Append(idMenu_DLG_OGL, _("&OpenGL Properties\tF3"), _("Show info about OpenGL Context"));
    helpMenu->Append(idMenu_DLG_TEXTURES, _("&Texture Library\tF4"), _("Show Texture Library"));
    helpMenu->Append(idMenu_DLG_CAMERAS, _("&Camera Library\tF5"), _("Show Camera Library"));
    helpMenu->Append(idMenu_DLG_MATERIALS, _("&Material Library Manager\tF6"), _("Show Material Libraries"));
    helpMenu->Append(idMenu_DLG_LIGHTS, _("&Light Library\tF7"), _("Show Light Library"));
    helpMenu->Append(idMenu_DLG_SHADERS, _("&Shader Library\tF8"), _("Show Shader Library"));
    helpMenu->Append(idMenu_DLG_SCENES, _("&Scene Library\tF9"), _("Show Scene Library"));
    helpMenu->Append(idMenu_DLG_PASS, _("&Pass Library\tF9"), _("Show Pass Library"));
	helpMenu->Append(idMenu_DLG_ATOMICS, _("&Atomics\tF10"), _("Show Atomics Info"));
    mbar->Append(helpMenu, _("&Help"));

	helpMenu->Enable(idMenu_DLG_TEXTURES,false);
	helpMenu->Enable(idMenu_DLG_CAMERAS,false);
	helpMenu->Enable(idMenu_DLG_LIGHTS,false);
	helpMenu->Enable(idMenu_DLG_SHADERS,false);
	helpMenu->Enable(idMenu_DLG_PASS,false);
	helpMenu->Enable(idMenu_DLG_ATOMICS, false);

   SetMenuBar(mbar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar with some information about the used wxWidgets version
   CreateStatusBar(2);
#endif // wxUSE_STATUSBAR

#ifdef FINAL
	//wxBitmap loaderBitmap;

	//wxInitAllImageHandlers();

	//wxSplashScreen *splash = 0;

	//if (loaderBitmap.LoadFile (wxT("loader.jpg"), wxBITMAP_TYPE_JPEG)) {
	//	splash = new wxSplashScreen (loaderBitmap, wxSPLASH_CENTRE_ON_SCREEN , 0, NULL, 
	//		-1, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER|wxSTAY_ON_TOP);
	//}
#endif

	m_pRoot = curitiba::Curitiba::create();


	bool curitibaInit (false);
        
    this->Show();
	int attribList[] = {
		WX_GL_RGBA,
		WX_GL_DOUBLEBUFFER,
		WX_GL_DEPTH_SIZE, 24,
		WX_GL_STENCIL_SIZE, 8, 
		WX_GL_SAMPLE_BUFFERS,1,
		WX_GL_SAMPLES,16,
		0};


#define WGL_CONTEXT_MAJOR_VERSION_ARB		0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB		0x2092
#define	WGL_CONTEXT_LAYER_PLANE_ARB			0x2093
#define WGL_CONTEXT_FLAGS_ARB				0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB		0x9126

#define WGL_CONTEXT_DEBUG_BIT_ARB				0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB	0x0002

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB			0x00000001
#define	WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB	0x00000002
	int major = CURITIBA_OPENGL_VERSION / 100;
	int minor = (CURITIBA_OPENGL_VERSION - major * 100 ) / 10;
	int contextAttribList[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, major,
            WGL_CONTEXT_MINOR_VERSION_ARB, minor, 
            WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#if (CURITIBA_CORE_OPENGL == 1)
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#else
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
#endif 
			0};

	m_Canvas = new GlCanvas (this , -1, attribList, contextAttribList);
                
//	m_Canvas->SetCurrent();
	try {
		curitibaInit = m_pRoot->init(true);
	}
	catch(std::string &s) {
		wxMessageBox((wxString)s.c_str());
	}

	if (true != curitibaInit){
		wxMessageBox (_("Curitiba error!"), _("Kaput!"));
		exit (1);
	}
	 
	m_Canvas->setEngine (m_pRoot);
	m_Canvas->setCamera();

	//SetClientSize (1024, 1024);

// Dialogs //
	DlgLog::SetParent(this);
	DlgLog::Instance()->updateDlg();
	DlgOGL::SetParent(this);
	DlgTextureLib::SetParent(this);
	DlgCameras::SetParent(this);
	DlgMaterials::SetParent(this);
	DlgLights::SetParent(this);
	DlgShaders::SetParent(this);
	DlgScenes::SetParent(this);
	DlgPass::SetParent(this);
	DlgAtomics::SetParent(this);

#ifdef FINAL
	startStandAlone();
	//delete splash;
#endif
}


FrmMainFrame::~FrmMainFrame()
{
}


void
FrmMainFrame::OnDlgScenes(wxCommandEvent& event) {

	DlgScenes::Instance()->Show(TRUE);
}


void
FrmMainFrame::OnDlgAtomics(wxCommandEvent& event) {

	DlgAtomics::Instance()->Show(TRUE);
}


void 
FrmMainFrame::OnDlgOGL(wxCommandEvent& event) {

	DlgOGL::Instance()->Show(TRUE);
}


void 
FrmMainFrame::OnDlgLog(wxCommandEvent& event) {

	DlgLog::Instance()->Show(TRUE);
}


void 
FrmMainFrame::OnDlgTextures(wxCommandEvent& event) {

	DlgTextureLib::Instance()->Show();
}


void 
FrmMainFrame::OnDlgShaders(wxCommandEvent& event) {

	DlgShaders::Instance()->Show();
}


void 
FrmMainFrame::OnDlgCameras(wxCommandEvent& event) {

	DlgCameras::Instance()->Show();
}


void 
FrmMainFrame::OnDlgLights(wxCommandEvent& event) {

	DlgLights::Instance()->Show();
}


void 
FrmMainFrame::OnDlgMaterials(wxCommandEvent& event) {

	DlgMaterials::Instance()->Show();
}


void 
FrmMainFrame::OnDlgPass(wxCommandEvent& event) {

	DlgPass::Instance()->Show();
}


void 
FrmMainFrame::updateDlgs()
{
	DlgAtomics::Instance()->updateDlg();
	DlgCameras::Instance()->updateDlg();
	DlgTextureLib::Instance()->updateDlg();
	DlgMaterials::Instance()->updateDlg();
	DlgLog::Instance()->updateDlg();
	DlgLights::Instance()->updateDlg();
	DlgShaders::Instance()->updateDlg();
	DlgScenes::Instance()->updateDlg();
	DlgPass::Instance()->updateDlg();
	helpMenu->Enable(idMenu_DLG_TEXTURES,true);
	helpMenu->Enable(idMenu_DLG_CAMERAS,true);
	helpMenu->Enable(idMenu_DLG_LIGHTS,true);
	helpMenu->Enable(idMenu_DLG_SHADERS,true);
	helpMenu->Enable(idMenu_DLG_PASS,true);
	helpMenu->Enable(idMenu_DLG_ATOMICS, true);
}


void
FrmMainFrame::OnDirectoryLoad (wxCommandEvent& event)
{
	wxDirDialog *openDirDlg = new wxDirDialog (this);

	wxStopWatch aTimer;

	if (wxID_OK == openDirDlg->ShowModal()) {
		aTimer.Start();
		wxDir dir (openDirDlg->GetPath());

		try {
			DlgLog::Instance()->updateDlg();
			DlgLog::Instance()->clear();
			m_pRoot->readDirectory (std::string(dir.GetName().mb_str()));
			m_Canvas->setCamera();
			updateDlgs();
			SLOG("done reading");
#ifndef FINAL
			long t =  aTimer.Time()/1000.0;
			SLOG("Elapsed time: %f", t);
#endif		
		}
		catch(std::string &s) {
			wxMessageBox(wxString(s.c_str()));
		}
	}
}


void
FrmMainFrame::OnModelLoad (wxCommandEvent& event)
{
	static const wxChar *fileTypes = _T( "3D Files (*.cbo, *.3ds, *.dae, *.obj, *.xml)|*.cbo;*.3ds;*.dae;*.obj; *.xml|CBO files (*.cbo)|*.cbo|COLLADA files (*.dae)|*.dae|3DS files (*.3ds)|*.3ds|OBJ files (*.obj)|*.obj|Ogre XML Meshes (*.xml)|*.xml");
	wxFileDialog *openFileDlg = new wxFileDialog (this, _("Open File"), _(""), _(""), fileTypes, wxFD_OPEN, wxDefaultPosition);

	if (wxID_OK == openFileDlg->ShowModal ()) {
		wxStopWatch aTimer;
		aTimer.Start();
		wxString path = openFileDlg->GetPath ();

		try {
			DlgLog::Instance()->updateDlg();
			DlgLog::Instance()->clear();
			m_pRoot->readModel (std::string(path.mb_str()));	
			m_Canvas->setCamera();
			updateDlgs();
#ifndef FINAL
			float t =  aTimer.Time()/1000.0;
			SLOG("Elapsed time: %f", t);
#endif		
		}
		catch(std::string &s) {
			wxMessageBox(wxString(s.c_str()));
		}
	}
}


void
FrmMainFrame::OnModelAppend (wxCommandEvent& event)
{
	static const wxChar *fileTypes = _T( "3D Files (*.cbo, *.3ds, *.dae, *.obj, *.xml)|*.cbo;*.3ds;*.dae;*.obj; *.xml|CBO files (*.cbo)|*.cbo|COLLADA files (*.dae)|*.dae|3DS files (*.3ds)|*.3ds|OBJ files (*.obj)|*.obj|Ogre XML Meshes (*.xml)|*.xml");
	wxFileDialog *openFileDlg = new wxFileDialog (this, _("Open File"), _(""), _(""), fileTypes, wxFD_OPEN, wxDefaultPosition);

	if (wxID_OK == openFileDlg->ShowModal ()) {
		wxStopWatch aTimer;
		aTimer.Start();
		wxString path = openFileDlg->GetPath ();

		try {
			DlgLog::Instance()->updateDlg();
			DlgLog::Instance()->clear();
			m_pRoot->appendModel (std::string(path.mb_str()));
			m_Canvas->setCamera();
			updateDlgs();
#ifndef FINAL
			float t =  aTimer.Time()/1000.0;
			SLOG("Elapsed time: %f", t);
#endif		
		}
		catch (std::string &s) {
			wxMessageBox((wxChar *)s.c_str());
		}
	}
}


void 
FrmMainFrame::OnProjectLoad(wxCommandEvent& event)
{
	static const wxChar *fileTypes = _T( "XML files|*.xml|All files|*.*");

	wxFileDialog *openFileDlg = new wxFileDialog (this, _("Open File"), _(""), _(""), fileTypes, wxFD_OPEN, wxDefaultPosition);

	if (wxID_OK == openFileDlg->ShowModal ()) {
		wxString path = openFileDlg->GetPath ();
		wxStopWatch aTimer;
		aTimer.Start();

		try {
			m_pRoot->clear();
			DlgLog::Instance()->updateDlg();
			DlgLog::Instance()->clear();
			int width, height;
			std::string ProjectFile ((const char *) path.c_str());
			m_pRoot->readProjectFile (ProjectFile, &width,&height);
			if (width)
				SetClientSize(width,height);
			m_Canvas->setCamera();
			updateDlgs();
#ifndef FINAL
			float t =  aTimer.Time()/1000.0;
			SLOG("Elapsed time: %f", t);
#endif

		} catch (curitiba::ProjectLoaderError &e) {
		  wxMessageBox (wxString (e.getException().c_str()));
		} 	
		catch (std::string s) {
			wxMessageBox(wxString (s.c_str()));
		}

	}
}


void 
FrmMainFrame::OnProcess (wxCommandEvent& event)
{
	wxDirDialog *openDirDlg = new wxDirDialog (this);

	if (wxID_OK == openDirDlg->ShowModal()) {
		wxDir dir (openDirDlg->GetPath());

		wxDirDialog *openDirDlgDst = new wxDirDialog (this);

		if (wxID_OK == openDirDlgDst->ShowModal()) {

			wxString aFile;

			bool cont = dir.GetFirst (&aFile);
			while (cont){

				wxFileName fn (openDirDlg->GetPath() + wxT("/") + aFile);
				LOG_INFO ("Loading: %s", (fn.GetFullPath()).c_str());
                m_pRoot->loadAsset (std::string(fn.GetFullPath().mb_str()), "MainScene");
				fn.SetPath (openDirDlgDst->GetPath());
				fn.SetExt(wxT("cbo"));
				m_pRoot->writeAssets ("CBO", (const char *)(fn.GetFullPath()).c_str(), "MainScene");
				cont = dir.GetNext (&aFile);
			}
			wxMessageBox (_("Process done..."), _("Composer"));
		}
	}
}


void FrmMainFrame::OnQuit(wxCommandEvent& event)
{
	Close();
}


void FrmMainFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to Composer"));
}


void
FrmMainFrame::OnRenderMode(wxCommandEvent& event)
{
	if (event.GetId() == idMenuWireframe){
		RENDERMANAGER->setRenderMode (curitiba::render::IRenderer::WIREFRAME_MODE);
	}
	if (event.GetId() == idMenuPoint) {
		RENDERMANAGER->setRenderMode (curitiba::render::IRenderer::POINT_MODE);
	}
	if (event.GetId() == idMenuSolid){
		RENDERMANAGER->setRenderMode (curitiba::render::IRenderer::SOLID_MODE);
	}
	if (event.GetId() == idMenuMaterial){
		RENDERMANAGER->setRenderMode (curitiba::render::IRenderer::MATERIAL_MODE);
	}
}

	
void
FrmMainFrame::OnSetProfileMaterial(wxCommandEvent& event)
{
	if (event.GetId() == idMenuProfMatWhite){
		//CURITIBA->setProfileMaterial ("__Emission White");
	}
	if (event.GetId() == idMenuProfMatRed) {
		//CURITIBA->setProfileMaterial ("__Emission Red");
	}
	if (event.GetId() == idMenuProfMatGreen){
		//CURITIBA->setProfileMaterial ("__Emission Green");
	}
	if (event.GetId() == idMenuProfMatBlue){
		//CURITIBA->setProfileMaterial ("__Emission Blue");
	}
	if (event.GetId() == idMenuProfMatBlack){
		//CURITIBA->setProfileMaterial ("Black");
	}
}


void 
FrmMainFrame::OnSetRenderFlags(wxCommandEvent& event)
{
	if (event.GetId() == idMenuRenderFlagBoundingBox) {
		CURITIBA->setRenderFlag(curitiba::Curitiba::BOUNDING_BOX_RENDER_FLAG,event.IsChecked());
	}
	else if (event.GetId() == idMenuRenderFlagProfile) {
		CURITIBA->setRenderFlag(curitiba::Curitiba::PROFILE_RENDER_FLAG,event.IsChecked());
	}
}


void
FrmMainFrame::OnOctreeBuild (wxCommandEvent& event)
{
	if (0 != m_pRoot) 
		RENDERMANAGER->buildOctrees();
}


void
FrmMainFrame::OnOctreeCompile (wxCommandEvent& event)
{
	if (0 != m_pRoot) 
		RENDERMANAGER->compile();
}


void FrmMainFrame::compile(IScene *scene) 
{
	scene->compile();
}


void
FrmMainFrame::OnOctreeWrite (wxCommandEvent& event) 
{
	if (0 != m_pRoot) {
		wxFileDialog *saveOctDlg = 
			new wxFileDialog (this, _("Save octree"), _(""), _(""), _(""), wxFD_SAVE, wxDefaultPosition);

		if (wxID_OK == saveOctDlg->ShowModal ()) {
			m_pRoot->writeAssets("CBO", (const char *)(saveOctDlg->GetPath()).c_str(), "MainScene");
		}		
	}
}


void
FrmMainFrame::OnPhysicsBuild (wxCommandEvent &event)
{
	buildPhysics();
}


void
FrmMainFrame::OnPhysicsMode (wxCommandEvent &event)
{
	curitiba::scene::Camera *cam = CURITIBA->getActiveCamera ();

	if (idMenuPhysicsOn == event.GetId()) {
		m_pRoot->enablePhysics();		
		cam->setDynamic(true);
	}
	
	if (idMenuPhysicsOff == event.GetId()) {
		m_pRoot->disablePhysics();	
		cam->setDynamic(false);
	}
}


void
FrmMainFrame::startStandAlone (void)
{
	char *cwd;

	cwd = _getcwd (0, 0);

	wxString path (_("C:\\pl3d\\nxtg"));

	wxString filename;
	wxArrayString files;

	wxDir::GetAllFiles (path, &files);
	
	files.Sort();

	wxArrayString::iterator filesIter;

	filesIter = files.begin();

	for ( ; filesIter != files.end(); filesIter++) {
	  wxFileName fn (path + _("/") + (*filesIter));

	  std::string AssetPath ((const char *) fn.GetFullPath().c_str());
	  m_pRoot->loadAsset (AssetPath, "MainScene");
	}

	//initScene();
	buildPhysics();

	RENDERMANAGER->getScene ("MainScene")->compile();

	m_pRoot->enablePhysics();

	curitiba::scene::Camera *cam = CURITIBA->getActiveCamera ();
	cam->setDynamic(true);			
	m_Canvas->setCamera();
}


void
FrmMainFrame::buildPhysics (void)
{
	curitiba::scene::Camera *cam = RENDERMANAGER->getCamera ("testCamera");

	if (0 != m_pRoot) {
		m_pRoot->getWorld().build();
		EVENTMANAGER->addListener("DYNAMIC_CAMERA",cam);
		m_pRoot->getWorld()._add (60.1f, cam, cam->getName(), vec3 (0.3f, 0.3f, 0.5f));
	}
}


void 
FrmMainFrame::OnKeyDown(wxKeyEvent & event) 
{
	event.Skip();
}


void 
FrmMainFrame::OnKeyUp(wxKeyEvent & event) 
{
	event.Skip();
}
/*
---------------------------------------------- CUT HERE (OLD CODE) ----------------------------------------------------------
*/


//void
//FrmMainFrame::initScene (void)
//{
	//m_CurrentProject->addPass(); /***MARK***/
	//m_CurrentProject->setModel (0, static_cast<CEditorModel*>(&m_pRoot->getScene())); /***MARK***/

	/****Scene Initialization****/

	//RENDERMANAGER->createScene ("MainScene");

	//curitiba::scene::Camera *aCamera = RENDERMANAGER->getCamera ("MainCamera"); //new curitiba::scene::Camera ("MainCamera");

	//curitiba::render::Viewport* MainViewport = m_pRoot->createViewport();
	//MainViewport->setBackgroundColor (curitiba::math::vec4 (0.0f, 0.0f, 0.0f, 1.0f));

	//aCamera->setViewport (MainViewport);
	//aCamera->setPerspective (60.0f, 0.5f, 10000.0f);

	//aCamera->setCamera (vec3 (-2.0f, 18.0f, 1.0f), vec3 (0.0f, 0.0f, -1.0f), vec3 (0.0f, 1.0f, 0.0f));

	//curitiba::scene::Light* sunLight = new curitiba::scene::Light ("Sun", vec3 (150.0f, 150.0f, 0.0f),
	//																		vec3 (-1.0f, 1.0f, 0.0f),
	//																		vec4 (0.9f, 0.9f, 0.9f, 0.0f), 
	//																		curitiba::scene::Light::DIRECTIONAL);

	//m_pRoot->getScene().addLight (sunLight);

	//curitiba::scene::Light* sunLight = RENDERMANAGER->getLight ("Sun");
	//sunLight->init (vec3 (150.0f, 150.0f, 0.0f),
	//				vec3 (-1.0f, 1.0f, 0.0f),
	//				vec4 (0.9f, 0.9f, 0.9f, 0.0f),
	//				curitiba::scene::Light::DIRECTIONAL);

	/***Material Lib Setup***/
	//MATERIALLIBMANAGER->getLib ("model");
	//MATERIALLIBMANAGER->setActiveLib ("model");

	/*** Pipeline Initialization ***/

	//curitiba::render::Pipeline* pip = RENDERMANAGER->getPipeline ("FixedFunction");
	//curitiba::render::Pass* p = pip->createPass (/*"fogwithcaustics"*/); //Adds default pass to pipeline	
	//p->setDoColorClear (true);
	//p->setDoDepthClear (true); 
	//p->addScene ("MainScene");
	//p->setCamera ("MainCamera");


	/*** Second pipeline ***/
	//pip = RENDERMANAGER->getPipeline ("Shadow32F");
	//p = pip->createPass ("depthmap");
	//p->setDoColorClear (true);
	//p->setDoDepthClear (true);
	//p->setRTMode (curitiba::render::Texture::RGBA32F);
	//p->setFBOs (1);


	//p->setFBOs (1);

	//p = pip->createPass ("quad");
	//p->setClearColor (true);
	//p->setClearDepth (true);
	//p->setMaterialLib ("model");

	//curitiba::material::Material *mat = new curitiba::material::Material;
	//mat->setName (p->getName() + "#quad");
	//mat->attachTexture (0, "FixedFunction#Pass1#rt#color0");
	//MATERIALLIBMANAGER->addMaterial (mat);

	/*RENDERMANAGER->setActivePipeline ("FixedFunction");*/



/***MARK***/ //This needs refactoring! The way camera lights are handled must change
	//curitiba::scene::Camera *sunCamera = RENDERMANAGER->getCamera ("SunCamera0"); 
	//sunCamera->deactivate();
	////m_pRoot->getScene().addCamera (sunCamera);
	//
	//curitiba::render::Viewport* sunViewport = m_pRoot->createViewport();
	//sunViewport->setBackgroundColor (curitiba::math::vec4 (0.0f, 0.0f, 0.0f, 1.0f));

	//sunCamera->setViewport (sunViewport);

	//sunCamera = RENDERMANAGER->getCamera ("SunCamera1"); 
	//sunCamera->deactivate();
	//sunCamera->setViewport (sunViewport);

	//sunCamera = RENDERMANAGER->getCamera ("SunCamera2"); 
	//sunCamera->deactivate();
	//sunCamera->setViewport (sunViewport);

	//sunCamera = RENDERMANAGER->getCamera ("SunCamera3"); 
	//sunCamera->deactivate();
	//sunCamera->setViewport (sunViewport);


	//curitiba::scene::Camera* quadCamera = RENDERMANAGER->getCamera ("QuadCamera");// new curitiba::scene::Camera ("QuadCamera");
	//quadCamera->deactivate();
	////m_pRoot->getScene().addCamera (quadCamera);


	//curitiba::render::Viewport* quadViewport = m_pRoot->createViewport();
	//quadViewport->setBackgroundColor (curitiba::math::vec4 (0.0f, 0.0f, 0.0f, 1.0f));
	//quadViewport->setSize (512, 512);
	//quadViewport->setFixed (true);

	//quadCamera->setViewport (MainViewport);
/***MARK***/

	//m_Canvas->setCamera (aCamera); /***MARK***/
//}

/*
void
FrmMainFrame::OnIdle (wxIdleEvent& event)
{
	if (0 != m_Canvas) {
		m_Canvas->Render();
		event.Skip();
	}
}
*/



/*
void 
FrmMainFrame::OnFileLoad(wxCommandEvent& event)
{


	static const wxChar *fileTypes = _T( "XML files|*.dae|"
									"All files|*.*|"
									);

	wxFileDialog *openFileDlg = new wxFileDialog (this, _("Open File"), _(""), _(""), fileTypes, wxOPEN, wxDefaultPosition);

	if (wxID_OK == openFileDlg->ShowModal ()) {
	  SetStatusText (openFileDlg->GetPath ());
	  m_CurrentProject = CProject::Instance();

	  m_CurrentProject->setCamera(new CCamera(), new CProjection(), new CViewport());
	  
	  if (0 != m_canvas){
			delete m_canvas;
			m_canvas = 0;
		}

		m_canvas = new CGlCanvas (this);
		SetClientSize (1024, 1024);
	}

	CResourceManager &manager = CResourceManager::getInstance ();
	wxString path = openFileDlg->GetPath ();

	wxFileName file (path);

	wxString url(wxFileSystem::FileNameToURL (file));

	bool result = false;
	
	CTextureManager *tm = CTextureManager::Instance();
	tm->setPath((openFileDlg->GetDirectory()).fn_str());
	
	try{
		if (file.GetExt() == "dae"){
			result = manager.loadWorld ((const char *) url.fn_str ()); 
		} else if (file.GetExt() == "oct") {
			result = manager.loadWorld ((const char*) file.GetFullPath().fn_str());
		}
	} catch(curitiba::CWorldFactoryError exception) {
		LOG_CRITICAL("Exception: %s", (exception.getException()).c_str());
	} 
	
	
	if (false == result){
		wxMessageBox(_("Unable to load: " + openFileDlg->GetFilename ()), _("Error"));
	} else {


		IWorld& world = CResourceManager::getInstance().getWorld();
		IRenderer &render = CRenderManager::getInstance().getRenderer();

		world.prepare();
		
		//Create the project to render materials

		m_CurrentProject->addPass();
		m_CurrentProject->setModel (0, static_cast<CEditorModel*>(&world));

		//build materials menu
		std::vector<std::string>& materialsNames = render.getMaterialsNamesList ();
		std::vector<std::string>::iterator materialsNamesIter = materialsNames.begin ();
	
		if (materialsNamesIter != materialsNames.end()){
			long int initId = wxNewId();
			long int finalId = initId;
			
			materialsMenu->AppendCheckItem(finalId, wxString((*materialsNamesIter).c_str(), wxConvUTF8));
			materialsMenu->Check(finalId, true);

			while (++materialsNamesIter != materialsNames.end()){
				finalId = wxNewId();
				materialsMenu->AppendCheckItem(finalId, wxString((*materialsNamesIter).c_str(), wxConvUTF8));
				materialsMenu->Check(finalId, true);
			}
			
			this->Connect (initId, finalId, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(FrmMainFrame::OnMaterialSelect));
		}
	}
}
*/

//void
//FrmMainFrame::setupMaterials (void)
//{

	//curitiba::material::Material* aMat = 0;

	///*** Vidro ***/
	//aMat = MATERIALLIBMANAGER->getMaterial ("Vidro");

	//aMat->setPriority (1);
	//aMat->setTransparent (true);
	//
	//aMat->OGLstate.m_glBlend = 1;
	//aMat->OGLstate.m_glBlendSrc = GL_SRC_ALPHA;
	//aMat->OGLstate.m_glBlendDst = GL_ONE_MINUS_SRC_ALPHA;

	//const float* diffuse = aMat->getColor().getDiffuse();
	//
	//aMat->getColor().setDiffuse (diffuse[0], diffuse[1], diffuse[2], 0.75f);		


	///*** Agua Compose ***/

	//aMat = MATERIALLIBMANAGER->getMaterial ("AguaComposer");
	//aMat->setTransparent (true);
	//aMat->setPriority (1);
	//aMat->OGLstate.m_glBlend = 1;
	//aMat->OGLstate.m_glBlendSrc = GL_SRC_ALPHA;
	//aMat->OGLstate.m_glBlendDst = GL_ONE_MINUS_SRC_ALPHA;

	//diffuse = aMat->getColor().getDiffuse();

	//aMat->getColor().setDiffuse (diffuse[0], diffuse[1], diffuse[2], 0.75f);				  

	///*** Grade ***/
	//std::vector<std::string> *materials = MATERIALLIBMANAGER->getMaterialNames ("model");

	//for (int i = 0; i < materials->size(); i++) {
	//	/*** All type of 'Grade' ***/
	//	int pos = materials->at (i).find ("Grade");
	//	if (std::string::npos != pos) {
	//		aMat = MATERIALLIBMANAGER->getMaterial (materials->at (i));
	//		aMat->setTransparent (true);
	//		aMat->setPriority (2);
	//		aMat->OGLstate.m_glBlend = 1;
	//		aMat->OGLstate.m_glBlendSrc = GL_SRC_ALPHA;
	//		aMat->OGLstate.m_glBlendDst = GL_ONE_MINUS_SRC_ALPHA;
	//	}
	//}

	//delete materials;
//}
