#pragma once

#include "ResourceManagerTree.h"

class ResourceManager : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(ResourceManager)
public:
	ResourceManager();
	virtual ~ResourceManager();

	static ResourceManager *current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	void AdjustLayout();

	ResourceManagerTree mResourceManagerTree;
};
