#include "stdafx.h"
#include "Editor.h"
#include "MiscEditWnd.h"

IMPLEMENT_DYNAMIC(MiscEditWnd, CBCGPDockingControlBar)

MiscEditWnd *MiscEditWnd::Current = NULL;
MiscEditWnd::MiscEditWnd()
{
	Current = this;
}

MiscEditWnd::~MiscEditWnd()
{
}

BEGIN_MESSAGE_MAP(MiscEditWnd, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int MiscEditWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	mSheet.Create(CBCGPTabWnd::STYLE_FLAT, rectDummy, this, ID_MISC_EDIT_TAB, CBCGPBaseTabWnd::LOCATION_TOP);
	mSheet.EnableTabSwap(FALSE);
	AdjustLayout();

	return 0;
}


void MiscEditWnd::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);
	AdjustLayout();
}

void MiscEditWnd::AdjustLayout()
{
	if (GetSafeHwnd () == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);
	
	mSheet.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE|SWP_NOZORDER);
}
