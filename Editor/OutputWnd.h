#pragma once

#include <vector>

class OutputWnd : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(OutputWnd)
public:
	OutputWnd();
	virtual ~OutputWnd();

	void Cache(CString Str);
	void Flush();
	void Clear();

	static OutputWnd *Current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CBCGPEdit mMessageList; CFont mFont;
	std::vector<CString> mCache;
};
