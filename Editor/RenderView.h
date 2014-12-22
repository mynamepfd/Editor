#pragma once

class CameraManager;

namespace Ogre
{
	class RenderWindow;
	class Viewport;
	class Camera;
}

namespace OIS
{
	class Keyboard;
	class Mouse;
}

class RenderView : public CView
{
	DECLARE_DYNCREATE(RenderView)
public:
	RenderView();
	virtual ~RenderView();

	Ogre::RenderWindow *getRenderWindow() { return renderWindow; }
	Ogre::Viewport *getViewport() { return viewport; }

	void setCameraManager(CameraManager *cameraManager);
	void _setCamera(Ogre::Camera *camera);

	virtual void roaming(OIS::Keyboard *keyboard, OIS::Mouse *mouse, float elapsed);

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnCustomAfterCreate(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	virtual void OnDraw(CDC* pDC);
	virtual void setupView();

	Ogre::RenderWindow *renderWindow;
	Ogre::Viewport *viewport;

	Ogre::Camera *camera; CameraManager *cameraManager;
};
