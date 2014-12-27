#include "stdafx.h"
#include "resource.h"
#include "MainFrm.h"
#include "RenderPump.h"

IMPLEMENT_DYNAMIC(MainFrame, CBCGPMDIFrameWnd)

MainFrame::MainFrame()
{
}

MainFrame::~MainFrame()
{
}

CBCGPToolBar *MainFrame::getObjectEditToolBar()
{
	return &objectEditToolBar;
}

CBCGPRibbonStatusBar *MainFrame::getStatusBar()
{
	return &statusBar;
}

BEGIN_MESSAGE_MAP(MainFrame, CBCGPMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

int MainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBCGPMDITabParams mdiTabParams;
	mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_VS2005; // 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = TRUE; // 激活Tab上的关闭按钮
	mdiTabParams.m_bDocumentMenu = TRUE; // 在选项卡区域的右边缘启用文档菜单
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	//////////////////////////////////////////////////
	// 第三方
	//////////////////////////////////////////////////

	initScintilla();
	initOGRE();

	createToolBar();
	createStatusBar();
	createDockingWindows();

	CBCGPVisualManager::SetDefaultManager(RUNTIME_CLASS(CBCGPVisualManagerXP));
	CBCGPDockManager::SetDockMode(BCGP_DT_SMART);
	SetTimer(1, 30, NULL);

	return 0;
}

void MainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		renderPump.renderOneFrame();
	}
}

void MainFrame::createToolBar()
{
	objectEditToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	objectEditToolBar.LoadToolBar(IDR_OBJECT_EDIT);

	objectEditToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&objectEditToolBar);

	CBCGPToolBar::ResetAllImages();
	objectEditToolBar.LoadBitmap(IDB_OBJECT_EDIT);
}

void MainFrame::createStatusBar()
{
	statusBar.Create(this);
	statusBar.AddExtendedElement(new CBCGPRibbonLabel("Camera Speed(10units per second)"), "Camera Speed");
	CBCGPRibbonSlider *cameraSpeedSlider = new CBCGPRibbonSlider(ID_STATUSBAR_CAMERA_SPEED_SLIDER);
	cameraSpeedSlider->SetZoomButtons();
	cameraSpeedSlider->SetPageSize(1);
	cameraSpeedSlider->SetRange(0, 100);
	cameraSpeedSlider->SetPos(CameraManager::defaultCameraSpeed / 10);
	statusBar.AddExtendedElement(cameraSpeedSlider, "Adjust camera speed.");
}

void MainFrame::createDockingWindows()
{
	CBCGPToolBarImages imagesWorkspace;
	imagesWorkspace.SetImageSize(CSize(16, 16));
	imagesWorkspace.SetTransparentColor(RGB(192, 192, 192));
	imagesWorkspace.Load(IDB_DOCKING_WINDOW);

	// 创建资源管理器窗口
	resourceManager.Create("Resource Manager", this, CRect(0, 0, 200, 200), TRUE, ID_RESOURCE_MANAGER, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI);
	resourceManager.SetIcon(imagesWorkspace.ExtractIcon(0), FALSE);
	resourceManager.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&resourceManager);

	// 创建场景资源窗口
	sceneResource.Create("Scene Resource", this, CRect(0, 0, 200, 200), TRUE, ID_SCENE_RESOURCE, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI);
	sceneResource.SetIcon(imagesWorkspace.ExtractIcon(1), FALSE);
	sceneResource.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&sceneResource);

	// 创建混合编辑窗口
	miscEditWnd.Create("Miscellanies", this, CRect(0, 0, 200, 200), TRUE, ID_MISC_EDIT_WND,
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI);
	miscEditWnd.EnableDocking(CBRS_ALIGN_ANY);
	miscEditWnd.SetIcon(imagesWorkspace.ExtractIcon(2), FALSE);
	DockControlBar(&miscEditWnd);

	sceneResource.AttachToTabWnd(&resourceManager, BCGP_DM_STANDARD, FALSE, NULL);
	miscEditWnd.AttachToTabWnd(&resourceManager, BCGP_DM_STANDARD, FALSE, NULL);

	// 创建属性窗口
	propertyWnd.Create("Property", this, CRect(0, 0, 200, 200), TRUE, ID_PROPERTY_WND, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_RIGHT|CBRS_FLOAT_MULTI);
	propertyWnd.SetIcon(imagesWorkspace.ExtractIcon(4), FALSE);
	propertyWnd.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&propertyWnd);

	// 创建相机窗口
	cameraWnd.Create("Camera", this, CRect(0, 0, 200, 200), TRUE, ID_CAMERA_WND, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_RIGHT|CBRS_FLOAT_MULTI);
	cameraWnd.SetIcon(imagesWorkspace.ExtractIcon(3), FALSE);
	cameraWnd.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&cameraWnd);

	// 创建资源预览窗口
	resourcePreview.Create("Resource Preview", this, CRect(0, 0, 200, 200), TRUE, ID_RESOURCE_PREVIEW, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_LEFT|CBRS_FLOAT_MULTI);
	resourcePreview.SetIcon(imagesWorkspace.ExtractIcon(5), FALSE);
	resourcePreview.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&resourcePreview);

	resourcePreview.DockToWindow(&propertyWnd, CBRS_ALIGN_BOTTOM);
	cameraWnd.DockToWindow(&propertyWnd, CBRS_ALIGN_BOTTOM);

	// 创建输出窗口
	outputWnd.Create("Output", this, CRect(0, 0, 200, 200), TRUE, ID_RESOURCE_PREVIEW, 
		WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBRS_BOTTOM|CBRS_FLOAT_MULTI);
	outputWnd.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&outputWnd);
}

void MainFrame::initScintilla()
{
	LoadLibrary("SciLexer.dll");
}

void MainFrame::initOGRE()
{
	renderPump.initialize();
}
