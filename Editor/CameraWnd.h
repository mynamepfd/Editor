#pragma once

class CameraManager;

class CameraWnd : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CameraWnd)
public:
	CameraWnd();
	virtual ~CameraWnd();

	void update(CameraManager *CameraManager);

	enum KCameraWnd
	{
		POLYGON_MODE,
		XPOS,
		YPOS,
		ZPOS,
		XDIR,
		YDIR,
		ZDIR,
		COUNT
	};

	static CameraWnd *current;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam);

	void AdjustLayout();

	CBCGPPropList propList; CBCGPProp *props[COUNT];
	CameraManager *cameraManager;
};
