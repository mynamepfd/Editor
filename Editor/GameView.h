#pragma once

#include "RenderView.h"

class GameView : public RenderView
{
	DECLARE_DYNCREATE(GameView)
public:
	GameView();
	~GameView();

	int getWidth();
	int getHeight();

	void roaming(OIS::Keyboard *Keyboard, OIS::Mouse *Mouse, float Elapsed);

	static GameView *current;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	void setupView();
};
