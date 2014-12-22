#pragma once

class CMiscEditSheet : public CBCGPTabWnd
{
	DECLARE_DYNAMIC(CMiscEditSheet)
public:
	CMiscEditSheet();
	virtual ~CMiscEditSheet();

	enum
	{
		TEXTURE_PREVIEW,
		MATERIAL_PREVIEW,
		PARTICLE_PREVIEW,
		TERRAIN_EDIT,
		_COUNT
	};

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CBCGPDialog *tabs[_COUNT];
};
