#include "stdafx.h"
#include "Editor.h"
#include "SceneDoc.h"
#include "PropertyWnd.h"
#include "SceneResourceTree.h"
#include "SceneObject.h"

IMPLEMENT_DYNAMIC(PropertyWnd, CBCGPDockingControlBar)

PropertyWnd *PropertyWnd::current = NULL;
PropertyWnd::PropertyWnd()
{
	current = this;
	listener = NULL;
}

PropertyWnd::~PropertyWnd()
{
}

void PropertyWnd::firePropertyChanged()
{
	if(listener != NULL)
		listener->firePropertyChanged();
}

BEGIN_MESSAGE_MAP(PropertyWnd, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

int PropertyWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	propList.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER, rectDummy, this, 0);
	propList.EnableHeaderCtrl(FALSE);
	propList.SetVSDotNetLook();

	return 0;
}

void PropertyWnd::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	
	CRect rectClient;
	GetClientRect(rectClient);
	propList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}

LRESULT PropertyWnd::OnPropertyChanged(WPARAM wparam, LPARAM lparam)
{
	if(listener != NULL)
		listener->onPropertyChanged((CBCGPProp*)lparam);
	return 0;
}
