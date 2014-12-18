#include "stdafx.h"
#include "GameView.h"
#include "GameDoc.h"
#include "DeferredShading.h"
#include "OgreRenderWindow.h"

IMPLEMENT_DYNCREATE(GameView, RenderView)

GameView *GameView::current = NULL;
GameView::GameView()
{
	current = this;
}

GameView::~GameView()
{
}

int GameView::getWidth()
{
	return renderWindow->getWidth();
}

int GameView::getHeight()
{
	return renderWindow->getHeight();
}

void GameView::roaming(OIS::Keyboard *Keyboard, OIS::Mouse *Mouse, float Elapsed)
{
	GameDoc* gameDoc = (GameDoc*)GetDocument();
	if(!cameraManager)
	{
		//IDirect3DDevice9 *Device = NULL;
		//mRenderWindow->getCustomAttribute("D3DDEVICE", (void*)&Device);
		setCameraManager(gameDoc->getCameraManager());
	}

	CPoint screenPoint;
	GetCursorPos(&screenPoint);
	ScreenToClient(&screenPoint);

	if(	screenPoint.x < 0 || screenPoint.x > getWidth() || 
		screenPoint.y < 0 || screenPoint.y > getHeight())
	{
		return;
	}

	RenderView::roaming(Keyboard, Mouse, Elapsed);
}

BEGIN_MESSAGE_MAP(GameView, RenderView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

void GameView::OnDestroy()
{
	RenderView::OnDestroy();
	GameView::current = NULL;

	((GameDoc*)GetDocument())->Destroy();
	GameDoc::current = NULL;
}

void GameView::OnSetFocus(CWnd* pOldWnd)
{
	RenderView::OnSetFocus(pOldWnd);
	GameView::current = this;

	// ((GameDoc*)GetDocument())->setActiveView(this);
	GameDoc::current = (GameDoc*)GetDocument();
}

void GameView::setupView()
{
	RenderView::setupView();
	GameDoc *Doc = (GameDoc*)GetDocument();
	Doc->setDeferredShadingSystem(new DeferredShadingSystem(viewport, Doc->getSceneManager(), Doc->getCamera()));
	Doc->getDeferredShadingSystem()->initialize();
}
