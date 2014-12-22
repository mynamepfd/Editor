#pragma once

#include "ScintillaDocView.h"
#include <string>
#include <vector>

class ParticleView : public Scintilla::CScintillaView
{
	DECLARE_DYNCREATE(ParticleView)
public:
	ParticleView();
	virtual ~ParticleView();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void OnInitialUpdate();
	void OnCharAdded(Scintilla::SCNotification* pSCNotification);
	void OnUpdateUI(Scintilla::SCNotification* pSCNotification);

	void parseFile();
	void parseParticleFile();
	void setAStyle(int style, COLORREF fore, COLORREF back=RGB(0xff, 0xff, 0xff), int size=-1, const char *face=0);

	bool timer; int lastChar;
	std::vector<std::string> materials;
};
