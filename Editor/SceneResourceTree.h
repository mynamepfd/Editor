#pragma once

#include "PropertyWnd.h"

class SceneObject;

namespace Ogre
{
	class SceneManager;
}

	/*enum
	{
		// Scene
		SCENE_NAME,
		SCENE_AMBIENT_LIGHT,
		SCENE_SHADOW_TECHNIQUE,
		SCENE_FOG_MODE,
		SCENE_FOG_COLOR,
		SCENE_FOG_DENSITY,
		SCENE_FOG_START,
		SCENE_FOG_END,
		SCENE_SKY_TYPE,
		// Light
		LIGHT_TYPE,
		LIGHT_DIFFUSE_COLOUR,
		LIGHT_SPECULAR_COLOUR,
		LIGHT_POSITION_X,
		LIGHT_POSITION_Y,
		LIGHT_POSITION_Z,
		LIGHT_DIRECTION_X,
		LIGHT_DIRECTION_Y,
		LIGHT_DIRECTION_Z,
		LIGHT_ATTENUATION_RANGE,
		LIGHT_ATTENUATION_CONSTANT,
		LIGHT_ATTENUATION_LINEAR,
		LIGHT_ATTENUATION_QUADRATIC,
		LIGHT_SPOTLIGHT_RANGE_INNER_ANGLE,
		LIGHT_SPOTLIGHT_RANGE_OUTER_ANGLE,
		LIGHT_SPOTLIGHT_RANGE_FALLOFF,
		// Model & Mesh
		MESH_SHADOW,
		MESH_POSITION_X,
		MESH_POSITION_Y,
		MESH_POSITION_Z,
		MESH_SCALE_X,
		MESH_SCALE_Y,
		MESH_SCALE_Z,
		MESH_DIRECTION_X,
		MESH_DIRECTION_Y,
		MESH_DIRECTION_Z,
		// Liquid
		LIQUID_POSITION_X,
		LIQUID_POSITION_Y,
		LIQUID_POSITION_Z,
	};*/

class SceneResourceTree : public CTreeCtrl, public PropertyListener
{
	DECLARE_DYNAMIC(SceneResourceTree)
public:
	SceneResourceTree();
	virtual ~SceneResourceTree();

	HTREEITEM getTreeItem(int type) { return treeItems[type]; }

	void firePropertyChanged();
	void onPropertyChanged(CBCGPProp *prop);

	void resetImageList();
	void rebuildTree();

	enum
	{
		SCENE,
		SKY,
		TERRAIN_SET,
		TERRAIN,
		LIGHT_SET,
		LIGHT,
		MODEL_SET,
		MODEL,
		MESH_SET,
		MESH,
		LIQUID_SET,
		LIQUID,
		PARTICLE_SET,
		PARTICLE,
		_COUNT
	};

	static SceneResourceTree *current;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);

	void afterSelectTreeItem(HTREEITEM treeItem);
	void onSelectScene();
	void onSelectSky();
	void onSelectTerrainSet();
	void onSelectTerrain(SceneObject *sceneObject);
	void onSelectLightSet();
	void onSelectLight(SceneObject *sceneObject);
	void onSelectModelSet();
	void onSelectModel(SceneObject *sceneObject);
	void onSelectMeshSet();
	void onSelectMesh(SceneObject *sceneObject);
	void onSelectLiquidSet();
	void onSelectLiquid(SceneObject *sceneObject);
	void onSelectParticleSet();
	void onSelectParticle(SceneObject *sceneObject);

	void onUpdateScene(CBCGPProp *prop);
	void onUpdateSky(CBCGPProp *prop);
	void onUpdateTerrainSet(CBCGPProp *prop);
	void onUpdateTerrain(CBCGPProp *prop, SceneObject *sceneObject);
	void onUpdateLightSet(CBCGPProp *prop);
	void onUpdateLight(CBCGPProp *prop, SceneObject *sceneObject);
	void onUpdateModelSet(CBCGPProp *prop);
	void onUpdateModel(CBCGPProp *prop, SceneObject *sceneObject);
	void onUpdateMeshSet(CBCGPProp *prop);
	void onUpdateMesh(CBCGPProp *prop, SceneObject *sceneObject);
	void onUpdateLiquidSet(CBCGPProp *prop);
	void onUpdateLiquid(CBCGPProp *prop, SceneObject *sceneObject);
	void onUpdateParticleSet(CBCGPProp *prop);
	void onUpdateParticle(CBCGPProp *prop, SceneObject *sceneObject);

	CImageList imageList;
	HTREEITEM treeItems[_COUNT];
};
