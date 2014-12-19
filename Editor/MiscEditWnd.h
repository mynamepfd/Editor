#pragma once

#include "MiscEditSheet.h"

class MiscEditWnd : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(MiscEditWnd)
public:
	MiscEditWnd();
	virtual ~MiscEditWnd();

	static MiscEditWnd *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void AdjustLayout();

	CMiscEditSheet mSheet;
};
