#pragma once

#include "SceneResourceTree.h"

class SceneResource : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(SceneResource)
public:
	SceneResource();
	virtual ~SceneResource();

	static SceneResource *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void AdjustLayout();
	SceneResourceTree mSceneResourceTree;
};
