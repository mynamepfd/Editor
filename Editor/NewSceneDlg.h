#pragma once

#include "resource.h"

class NewSceneDlg : public CDialog
{
	DECLARE_DYNAMIC(NewSceneDlg)
public:
	NewSceneDlg(CWnd* pParent = NULL);
	virtual ~NewSceneDlg();

	_variant_t getProperty(int type);

	enum
	{
		SCENE_NAME,
		AMBIENT_LIGHT,
		//SHADOW_TECHNIQUE,
		// Sky
		SKY_TYPE,
		SKY_MATERIAL,
		// Fog
		FOG_MODE,
		FOG_COLOUR,
		FOG_DENSITY,
		FOG_START,
		FOG_END,
		// Terrain global options
		LIGHT_MAP_DIRECTION_X,
		LIGHT_MAP_DIRECTION_Y,
		LIGHT_MAP_DIRECTION_Z,
		LIGHT_MAP_SIZE,
		COMPOSITE_MAP_DIFFUSE,
		COMPOSITE_MAP_DISTANCE,
		COMPOSITE_MAP_SIZE,
		// Default import settings
		FILENAME_PREFIX,
		FILENAME_EXTENSION,
		TERRAIN_SIZE,
		WORLD_SIZE,
		INPUT_SCALE,
		MIN_BATCH_SIZE,
		MAX_BATCH_SIZE,
		TEXTURE_WORLD_SIZE,
		DIFFUSE_SPECULAR,
		NORMAL_HEIGHT,
		HEIGHT_MAP,
		_COUNT
	};

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnPropertyChanged(WPARAM wparam, LPARAM lparam);

	BOOL OnInitDialog();
	void DoDataExchange(CDataExchange* pDX);

	CStatic propListFrame; CBCGPPropList propList;
	CBCGPProp *props[_COUNT]; COleVariant values[_COUNT];
};
