#ifndef __SLANGER_DIALOGS_PASS__
#define __SLANGER_DIALOGS_PASS__


#ifdef __GNUG__
#pragma implementation
#pragma interface
#endif


// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/notebook.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/event.h>
#include <wx/grid.h>
#include <wx/colordlg.h>
#include <wx/frame.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/propgrid/manager.h>

class ImageGridCellRenderer;

#include <curitiba/render/pipeline.h>
#include <curitiba/render/pass.h>
#include <curitiba/material/materiallibmanager.h>
#include <curitiba.h>

#include <curitiba/event/ilistener.h>


using namespace curitiba::render;


class DlgPass : public wxDialog, IListener
{
	protected:
		DlgPass();
		DlgPass(const DlgPass&);
		DlgPass& operator= (const DlgPass&);

	public:

		static DlgPass* Instance () ;
		static void SetParent(wxWindow *parent);

		std::string &getName () {return m_Name;};

		void eventReceived(const std::string &sender, const std::string &eventType, curitiba::event_::IEventData *evt);

		void updateDlg();	

	private:
		static wxWindow *parent; 
		static DlgPass *inst;

		wxWindow *m_parent;
		std::string m_Name;

		// Data is presented in these
		wxComboBox *m_PipelineList, *m_PassList;
		wxPropertyGridManager *m_pg;
		wxPGProperty *pidScenes;

		// These vars must be updated whenever stuff (assigned to current pass) is updated
		wxPGChoices m_pgCamList, m_pgLightList, m_pgMaterialList,m_pgMaterialListPlus, 
			m_pgSceneList, m_pgRenderTargetList, m_pgViewportList;

		wxPGProperty *m_pgPropCam, *m_pgPropViewport, *m_pgPropRenderTarget;

		// Called by updateDlg
		void updateLists(Pass *p);
		void updateLights(Pass *p);
		void updateMaterialList();
		void updateScenes(Pass *p);
		void updateCameraList(Pass *p);
		void updateViewportList(Pass *p);
		void updateRenderTargetList(Pass *p);
		void updatePipelines();

		// Event Processing
		void OnClose(wxCloseEvent& events);
		void OnSelectPass(wxCommandEvent& events);
		void OnSelectPipeline(wxCommandEvent& events);
		void OnProcessPGChange( wxPropertyGridEvent& e);
		// Auxiliary function when REMAP_TO_ONE is selected
		void updateMats(Pass *p);

		// Called when selecting a pass, or after preparing the property grid
		void updateProperties(Pass *p);

		Pass *getPass();

		/* GENERAL */

		enum {
			DLG_MI_COMBO_PASS,
			DLG_MI_COMBO_PIPELINE,
			DLG_MI_PG,


			/* TOOLBAR */
			PIPELINE_NEW,
			PIPELINE_REMOVE,
			PASS_NEW,
			PASS_REMOVE,
			TOOLBAR_ID
		};

		// Toolbar stuff
		wxToolBar *m_toolbar;
		void toolbarPipelineNew(wxCommandEvent& WXUNUSED(event) );
		void toolbarPipelineRemove(wxCommandEvent& WXUNUSED(event) );

		void toolbarPassNew(wxCommandEvent& WXUNUSED(event) );
		void toolbarPassRemove(wxCommandEvent& WXUNUSED(event) );

		DECLARE_EVENT_TABLE()
};

#endif


