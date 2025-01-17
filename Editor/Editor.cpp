#include "stdafx.h"
#include "Editor.h"

#include "MainFrm.h"
#include "ChildFrm.h"

#include "SceneDoc.h"
#include "SceneView.h"
#include "GameDoc.h"
#include "GameView.h"
#include "MaterialDoc.h"
#include "MaterialView.h"
#include "ParticleDoc.h"
#include "ParticleView.h"

#include "NewSceneDlg.h"
#include "DDSConverter.h"
#include "ImageCombinator.h"
#include "OgreXMLConverter.h"
#include "ScriptEditorDlg.h"

#include "OutputWnd.h"
#include "OgreLogManager.h"

CEditorApp theApp;

CEditorApp::CEditorApp()
{
	SetAppID(_T("Editor.AppID.NoVersion"));
	m_bSaveState = FALSE;
}

BOOL CEditorApp::InitInstance()
{
	CBCGPWinApp::InitInstance();
	EnableTaskbarInteraction(FALSE);

	docTemplates["Scene"] = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(SceneDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(SceneView));
	if (!docTemplates["Scene"])
		return FALSE;
	AddDocTemplate(docTemplates["Scene"]);

	docTemplates["Game"] = new CMultiDocTemplate(IDR_MAINFRAME,
		RUNTIME_CLASS(GameDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(GameView));
	if (!docTemplates["Game"])
		return FALSE;
	AddDocTemplate(docTemplates["Game"]);

	docTemplates["Material"] = new CMultiDocTemplate(IDR_MATERIAL,
		RUNTIME_CLASS(MaterialDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(MaterialView));
	if (!docTemplates["Material"])
		return FALSE;
	AddDocTemplate(docTemplates["Material"]);

	docTemplates["Particle"] = new CMultiDocTemplate(IDR_MATERIAL,
		RUNTIME_CLASS(ParticleDoc),
		RUNTIME_CLASS(CChildFrame),
		RUNTIME_CLASS(ParticleView));
	AddDocTemplate(docTemplates["Particle"]);

	//////////////////////////////////////////////////
	// MFC
	//////////////////////////////////////////////////

	MainFrame* pMainFrame = new MainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	if(!ProcessShellCommand(cmdInfo))
		return FALSE;

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CEditorApp::ExitInstance()
{
	return CBCGPWinApp::ExitInstance();
}

CMultiDocTemplate *CEditorApp::getDocTemplate(std::string name)
{
	return docTemplates[name];
}

void CEditorApp::addDocTemplate(std::string name, CMultiDocTemplate *docTemplate)
{
	docTemplates[name] = docTemplate;
}

std::string CEditorApp::getPath(std::string name)
{
	return paths[name];
}

void CEditorApp::addPath(std::string name, std::string path)
{
	paths[name] = path;
}

BEGIN_MESSAGE_MAP(CEditorApp, CBCGPWinApp)
	ON_COMMAND(ID_NEW_SCENE, &CEditorApp::OnNewScene)
	ON_COMMAND(ID_OPEN_SCENE, &CEditorApp::OnOpenScene)
	ON_COMMAND(ID_NEW_GAME, &CEditorApp::OnNewGame)
	ON_COMMAND(ID_XML_CONVERTER, &CEditorApp::OnOgreXmlConverter)
	ON_COMMAND(ID_SCRIPT_EDITOR, &CEditorApp::OnScriptEditor)
	ON_COMMAND(ID_CLEAR_LOG, &CEditorApp::OnClearLog)
	ON_COMMAND(ID_APP_ABOUT, &CEditorApp::OnAppAbout)
	ON_UPDATE_COMMAND_UI_RANGE(ID_LOAD_TEXTURE, ID_RESIZE_TEXTURE, OnUpdateTextureMenu)
	ON_COMMAND(ID_IMAGE_COMBINATOR, &CEditorApp::OnImageCombinator)
	ON_COMMAND(ID_DDSCONVERTER, &CEditorApp::OnDDSConverter)
	ON_COMMAND(ID_DETAIL_LOW, &CEditorApp::OnDetailLow)
	ON_COMMAND(ID_DETAIL_NORMAL, &CEditorApp::OnDetailNormal)
	ON_COMMAND(ID_DETAIL_BOREME, &CEditorApp::OnDetailBoreme)
	ON_UPDATE_COMMAND_UI(ID_DETAIL_LOW, &CEditorApp::OnUpdateDetailLow)
	ON_UPDATE_COMMAND_UI(ID_DETAIL_NORMAL, &CEditorApp::OnUpdateDetailNormal)
	ON_UPDATE_COMMAND_UI(ID_DETAIL_BOREME, &CEditorApp::OnUpdateDetailBoreme)
END_MESSAGE_MAP()

void CEditorApp::OnNewScene()
{
	NewSceneDlg Dlg;
	if(Dlg.DoModal() == IDOK)
	{
		docTemplates["Scene"]->OpenDocumentFile(NULL);
		SceneDoc::current->initialize(&Dlg);
	}
}

void CEditorApp::OnOpenScene()
{
	CFileDialog Dlg(TRUE, NULL, NULL, OFN_NOCHANGEDIR, 
		"Scene config (*.scene)|*.scene|\
		All Files (*.*)|*.*||", NULL);
	if(Dlg.DoModal() == IDOK)
	{
		docTemplates["Scene"]->OpenDocumentFile(NULL);
		SceneDoc::current->initialize(Dlg.GetPathName());
	}
}

void CEditorApp::OnNewGame()
{
	docTemplates["Game"]->OpenDocumentFile(NULL);
	GameDoc::current->Initialize();
}

void CEditorApp::OnOgreXmlConverter()
{
	OgreXMLConverter ogreXMLConverter;
	ogreXMLConverter.DoModal();
}

void CEditorApp::OnScriptEditor()
{
	CScriptEditorDlg Dlg;
	Dlg.DoModal();
}

void CEditorApp::OnClearLog()
{
	OutputWnd::Current->Clear();
}

void CEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CEditorApp::OnUpdateTextureMenu(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

CAboutDlg::CAboutDlg() : CBCGPDialog(CAboutDlg::IDD)
{
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBCGPDialog)
END_MESSAGE_MAP()

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	EnableVisualManagerStyle();
	return TRUE;
}

void CEditorApp::OnImageCombinator()
{
	ImageCombinator imageCombinator;
	imageCombinator.DoModal();
}

IDirect3DDevice9 *CEditorApp::getDevice()
{
	return device;
}

void CEditorApp::setDevice(IDirect3DDevice9 *device)
{
	this->device = device;
}


void CEditorApp::OnDDSConverter()
{
	DDSConverter ddsConverter;
	ddsConverter.DoModal();
}


void CEditorApp::OnDetailLow()
{
	Ogre::LogManager::getSingleton().getLog("tmp.log")->setLogDetail(Ogre::LL_LOW);
}


void CEditorApp::OnDetailNormal()
{
	Ogre::LogManager::getSingleton().getLog("tmp.log")->setLogDetail(Ogre::LL_NORMAL);
}


void CEditorApp::OnDetailBoreme()
{
	Ogre::LogManager::getSingleton().getLog("tmp.log")->setLogDetail(Ogre::LL_BOREME);
}


void CEditorApp::OnUpdateDetailLow(CCmdUI *pCmdUI)
{
	Ogre::LoggingLevel logDetail = Ogre::LogManager::getSingleton().getLog("tmp.log")->getLogDetail();
	pCmdUI->SetCheck(logDetail == Ogre::LL_LOW);
}


void CEditorApp::OnUpdateDetailNormal(CCmdUI *pCmdUI)
{
	Ogre::LoggingLevel logDetail = Ogre::LogManager::getSingleton().getLog("tmp.log")->getLogDetail();
	pCmdUI->SetCheck(logDetail == Ogre::LL_NORMAL);
}


void CEditorApp::OnUpdateDetailBoreme(CCmdUI *pCmdUI)
{
	Ogre::LoggingLevel logDetail = Ogre::LogManager::getSingleton().getLog("tmp.log")->getLogDetail();
	pCmdUI->SetCheck(logDetail == Ogre::LL_BOREME);
}
