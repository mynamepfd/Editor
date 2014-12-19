#pragma once

#include "CameraManager.h"

class TiXmlElement;
class RenderView;
class SceneObject;

namespace Ogre
{
	class SceneManager;
	class Camera;
	class ManualObject;
}

class ResourcePreviewToolBar : public CBCGPToolBar
{
public:    
    virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)    
    {        
        CBCGPToolBar::OnUpdateCmdUI ((CFrameWnd*) GetOwner (), bDisableIfNoHndler);
    }

    virtual BOOL AllowShowOnList () const{ return FALSE; }
};

class ResourcePreview : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(ResourcePreview)
public:
	ResourcePreview();
	virtual ~ResourcePreview();

	void AfterSelectResource(TiXmlElement *Elmt);
	void update(float Elapsed);

	static ResourcePreview *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowGrid();
	afx_msg void OnUpdateShowGrid(CCmdUI *pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void ClearSceneObject();
	void AdjustCamera();

	void ShowGrid();

	RenderView *mView;
	ResourcePreviewToolBar mToolbar;
	bool mShowGrid;

	Ogre::SceneManager *mSceneManager;
	Ogre::Camera *mCamera;
	CameraManager mCameraManager;
	Ogre::ManualObject *mGrid;

	SceneObject *mSceneObject;
};
