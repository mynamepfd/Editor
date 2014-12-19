#include "stdafx.h"
#include "CameraWnd.h"
#include "CameraManager.h"

#include "OgreCamera.h"
#include "OgreStringConverter.h"

IMPLEMENT_DYNAMIC(CameraWnd, CBCGPDockingControlBar)

CameraWnd *CameraWnd::current = NULL;
CameraWnd::CameraWnd():
	cameraManager(NULL)
{
	current = this;
}

CameraWnd::~CameraWnd()
{
}

void CameraWnd::update(CameraManager *cameraManager)
{
	Ogre::Camera *camera = cameraManager->getCamera();

	const Ogre::Vector3 &position = camera->getDerivedPosition();
	props[XPOS]->SetValue(position.x);
	props[YPOS]->SetValue(position.y);
	props[ZPOS]->SetValue(position.z);

	Ogre::Vector3 direction = camera->getDerivedDirection();
	props[XDIR]->SetValue(direction.x);
	props[YDIR]->SetValue(direction.y);
	props[ZDIR]->SetValue(direction.z);

	this->cameraManager = cameraManager;
}

BEGIN_MESSAGE_MAP(CameraWnd, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, &CameraWnd::OnPropertyChanged )
END_MESSAGE_MAP()

int CameraWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	propList.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER, rectDummy, this, 0);
	propList.EnableHeaderCtrl(FALSE);
	propList.SetVSDotNetLook();

	CBCGPProp *prop = NULL;

	props[POLYGON_MODE] = new CBCGPProp(
		"PolygonMode", (_variant_t)"Solid", "", POLYGON_MODE);
	props[POLYGON_MODE]->AllowEdit(FALSE);
	props[POLYGON_MODE]->AddOption("Points");
	props[POLYGON_MODE]->AddOption("Wireframe");
	props[POLYGON_MODE]->AddOption("Solid");
	propList.AddProperty(props[POLYGON_MODE]);

	prop = new CBCGPProp("Position");
	props[XPOS] = new CBCGPProp("X", (_variant_t)0.0f, "", XPOS);
	props[XPOS]->AllowEdit(FALSE);
	prop->AddSubItem(props[XPOS]);
	props[YPOS] = new CBCGPProp("Y", (_variant_t)0.0f, "", YPOS);
	props[YPOS]->AllowEdit(FALSE);
	prop->AddSubItem(props[YPOS]);
	props[ZPOS] = new CBCGPProp("Z", (_variant_t)0.0f, "", ZPOS);
	props[ZPOS]->AllowEdit(FALSE);
	prop->AddSubItem(props[ZPOS]);
	propList.AddProperty(prop);

	prop = new CBCGPProp("Direction");
	props[XDIR] = new CBCGPProp("X", (_variant_t)0.0f, "", XDIR);
	props[XDIR]->AllowEdit(FALSE);
	prop->AddSubItem(props[XDIR]);
	props[YDIR] = new CBCGPProp("Y", (_variant_t)0.0f, "", YDIR);
	props[YDIR]->AllowEdit(FALSE);
	prop->AddSubItem(props[YDIR]);
	props[ZDIR] = new CBCGPProp("Z", (_variant_t)0.0f, "", ZDIR);
	props[ZDIR]->AllowEdit(FALSE);
	prop->AddSubItem(props[ZDIR]);
	propList.AddProperty(prop);

	propList.ExpandAll();
	AdjustLayout();

	return 0;
}

void CameraWnd::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout();
}

LRESULT CameraWnd::OnPropertyChanged(WPARAM wparam, LPARAM lparam)
{
	if(cameraManager == NULL)
	{
		propList.RemoveAll();
		return 0;
	}

	CBCGPProp *Prop = (CBCGPProp*)lparam;
	switch(Prop->GetData())
	{
	case POLYGON_MODE:
		CString strPolygonMode = props[POLYGON_MODE]->GetValue();
		if(strPolygonMode == "Solid")
		{
			cameraManager->getCamera()->setPolygonMode(Ogre::PM_SOLID);
		} else 
		if(strPolygonMode == "Wireframe")
		{
			cameraManager->getCamera()->setPolygonMode(Ogre::PM_WIREFRAME);
		} else 
		if(strPolygonMode == "Points")
		{
			cameraManager->getCamera()->setPolygonMode(Ogre::PM_POINTS);
		}
	}

	return 0;
}

void CameraWnd::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	propList.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
}
