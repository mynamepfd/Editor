#pragma once

class PropertyListener
{
public:
	/** 诸如ObjectEidtHandler等修改了PropertyWnd所引用的资源时，
		应主动使PropertyWnd更新其中的数据。
	*/
	virtual void firePropertyChanged() = 0;

	/** 当修改了PropertyWnd中的属性，将会更新其所引用的资源。
	*/
	virtual void onPropertyChanged(CBCGPProp *prop) = 0;
};

class PropertyWnd : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(PropertyWnd)
public:
	PropertyWnd();
	virtual ~PropertyWnd();

	CBCGPPropList *getPropList() { return &propList; }
	void setListener(PropertyListener *listener) { this->listener = listener; }
	void firePropertyChanged();

	static PropertyWnd *current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam);

	CBCGPPropList propList;
	PropertyListener *listener;
};
