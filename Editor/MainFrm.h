#pragma once

#include "ResourceManager.h"
#include "SceneResource.h"
#include "MiscEditWnd.h"
#include "ResourcePreview.h"
#include "PropertyWnd.h"
#include "CameraWnd.h"
#include "OutputWnd.h"
#include "RenderPump.h"

class MainFrame : public CBCGPMDIFrameWnd
{
	DECLARE_DYNAMIC(MainFrame)
public:
	MainFrame();
	virtual ~MainFrame();

	CBCGPToolBar *getObjectEditToolBar();
	CBCGPRibbonStatusBar *getStatusBar();

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void createToolBar();
	void createStatusBar();
	void createDockingWindows();

	void initScintilla();
	void initOGRE();

	CBCGPToolBar objectEditToolBar;
	CBCGPRibbonStatusBar statusBar;

	ResourceManager	resourceManager;
	SceneResource sceneResource;
	MiscEditWnd miscEditWnd;
	ResourcePreview	resourcePreview;
	PropertyWnd	propertyWnd;
	CameraWnd cameraWnd;
	OutputWnd outputWnd;

	RenderPump renderPump;
};
